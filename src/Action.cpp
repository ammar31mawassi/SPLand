
#include <utility>

#include "../include/Action.h"

#include <iostream>
#include <ostream>

extern Simulation* backup;

BaseAction::BaseAction() : errorMsg("") ,status(ActionStatus::ERROR) {
}

ActionStatus BaseAction::getStatus() const {
    return this->status;
}

string BaseAction::getStatusString() const {
    if (status == ActionStatus::ERROR)
        return "ERROR";
    return "COMPLETE";
}
void BaseAction::setStatus(ActionStatus as){
    this->status = as;
}
void BaseAction::setErrorMsg(string msg){
    this->errorMsg = msg;
}

void BaseAction::complete() {
    this->status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg) {
    this->errorMsg = std::move(errorMsg);
    status = ActionStatus::ERROR;
}

const string & BaseAction::getErrorMsg() const {
    return this->errorMsg;
}

SimulateStep::SimulateStep(const int numOfSteps): numOfSteps(numOfSteps) {
}

void SimulateStep::act(Simulation &simulation) {
    for (int i = 0; i < numOfSteps; i++) {
        simulation.step();
    }
    this->complete();
}

const string SimulateStep::toString() const {
    return "step " + std::to_string(this->numOfSteps) + " " + this->getStatusString();
}

SimulateStep * SimulateStep::clone() const {
    auto *clone = new SimulateStep(numOfSteps);
    clone->setErrorMsg(this->getErrorMsg());
    clone->setStatus(this->getStatus());
    return clone;
}

AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy):
settlementName(settlementName),selectionPolicy(selectionPolicy) {
}

void AddPlan::act(Simulation &simulation) {
    if (!simulation.isSettlementExists(settlementName)) {
        this->error("Error: Cannot create this plan");
        std::cout << this->getErrorMsg() << std::endl;
        return;
    }
    SelectionPolicy *selection;
    if(selectionPolicy == "eco"){
        selection = new EconomySelection();
    }else if(selectionPolicy == "nve"){
        selection = new NaiveSelection();
    }else if(selectionPolicy == "bal"){
        selection = new BalancedSelection(0,0,0);
    }else if(selectionPolicy == "env"){
        selection = new SustainabilitySelection();
    } else {
        this->error("Error: Cannot create this plan");
        std::cout << this->getErrorMsg() << std::endl;
        return;
    }
    simulation.addPlan(simulation.getSettlement(settlementName),selection);
    this->complete();
}

const string AddPlan::toString() const {
    return "AddPlan " + settlementName + " " + selectionPolicy + " " + this->getStatusString();
}

AddPlan * AddPlan::clone() const {
    auto *clone = new AddPlan(this->settlementName,this->selectionPolicy);
    clone->setErrorMsg(this->getErrorMsg());
    clone->setStatus(this->getStatus());
    return clone;
}

AddSettlement::AddSettlement(const string &settlementName, SettlementType settlementType):
settlementName(settlementName), settlementType(settlementType) {
}

void AddSettlement::act(Simulation &simulation) {
    auto *toAdd = new Settlement(settlementName,settlementType);
    if (simulation.addSettlement(toAdd)) {
        this->complete();
    }
    else{
        error("Error: Settlement already exists");
        std::cout << this->getErrorMsg() << std::endl;
        delete toAdd;
    }
}

AddSettlement * AddSettlement::clone() const {
    auto *clone = new AddSettlement(this->settlementName,this->settlementType);
    clone->setErrorMsg(this->getErrorMsg());
    clone->setStatus(this->getStatus());
    return clone;
}

const string AddSettlement::toString() const {
    string type;
    if (settlementType == SettlementType::VILLAGE)
        type = "0";
    else if (settlementType == SettlementType::CITY)
        type = "1";
    else
        type = "2";
    return "settlement " + settlementName + " " + type + " " + this->getStatusString();
}

AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price,
    const int lifeQualityScore, const int economyScore, const int environmentScore):
facilityName(facilityName), facilityCategory(facilityCategory), price(price), lifeQualityScore(lifeQualityScore),
economyScore(economyScore), environmentScore(environmentScore){
}

void AddFacility::act(Simulation &simulation) {
    FacilityType toAdd(facilityName,facilityCategory,price,lifeQualityScore,economyScore,environmentScore);
    if (!simulation.addFacility(toAdd)){
        error("Error: Facility already exists");
        std::cout << this->getErrorMsg() << std::endl;
    }
    this->complete();
}

AddFacility * AddFacility::clone() const {
    auto *clone = new AddFacility(facilityName,facilityCategory,price,lifeQualityScore,economyScore,environmentScore);
    clone->setErrorMsg(this->getErrorMsg());
    clone->setStatus(this->getStatus());
    return clone;
}

const string AddFacility::toString() const {
    string category;
    if (facilityCategory == FacilityCategory::LIFE_QUALITY)
        category = "0";
    if (facilityCategory == FacilityCategory::ECONOMY)
        category = "1";
    if (facilityCategory == FacilityCategory::ENVIRONMENT)
        category = "2";
    return "facility " + category + " " + std::to_string(price) + " " + std::to_string(lifeQualityScore) + " " +
        std::to_string(economyScore) + " " + std::to_string(environmentScore) + " " + this->getStatusString();
}

PrintPlanStatus::PrintPlanStatus(int planId): planId(planId) {
}

void PrintPlanStatus::act(Simulation &simulation) {
    if (simulation.isPlanExist(planId)) {
        std::cout<<simulation.getPlan(planId).toString()<<std::endl;
        this->complete();
    }
    else {
        error("Error: Plan doesn't exist");
        std::cout << this->getErrorMsg() << std::endl;
    }
}

PrintPlanStatus * PrintPlanStatus::clone() const {
    auto *clone = new PrintPlanStatus(planId);
    clone->setErrorMsg(this->getErrorMsg());
    clone->setStatus(this->getStatus());
    return clone;
}

const string PrintPlanStatus::toString() const {
    return "planStatus " + std::to_string(planId) + " " + this->getStatusString();
}

ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy): planId(planId), newPolicy(newPolicy) {
}

void ChangePlanPolicy::act(Simulation &simulation) {
    if(!simulation.isPlanExist(planId)){
        error("Error: Cannot change selection policy");
        std::cout << this->getErrorMsg() << std::endl;
        return;
    }
    Plan toChange = simulation.getPlan(planId);
    SelectionPolicy *selection ;
    if(newPolicy == "eco"){
        selection = new EconomySelection();
    }else if(newPolicy == "nve"){
        selection = new NaiveSelection();
    }else if(newPolicy == "bal"){
        selection = new BalancedSelection(toChange.getlifeQualityScore(),toChange.getEconomyScore(),toChange.getEnvironmentScore());
    }else if(newPolicy == "env"){
        selection = new SustainabilitySelection();
    }
    if (simulation.isSettlementExists(toChange.settlementName()) ) {
        simulation.getPlan(planId).setSelectionPolicy(selection);
        complete();
    }
    else {
        error("Error: Cannot change selection policy");
        std::cout << this->getErrorMsg() << std::endl;
        delete selection;
    }
    
}

ChangePlanPolicy * ChangePlanPolicy::clone() const {
    auto *clone = new ChangePlanPolicy(planId,newPolicy);
    clone->setErrorMsg(this->getErrorMsg());
    clone->setStatus(this->getStatus());
    return clone;
}

const string ChangePlanPolicy::toString() const {
    return "changePolicy " + std::to_string(planId) + " " + newPolicy + " " +this->getStatusString();
}

PrintActionsLog::PrintActionsLog() = default;

void PrintActionsLog::act(Simulation &simulation) {
    vector<BaseAction*> actions = simulation.getActionLog();
    for (const BaseAction *action : actions) {
        std::cout<<action->toString()<<std::endl;
    }
    complete();
}

PrintActionsLog * PrintActionsLog::clone() const {
    PrintActionsLog* clone = new PrintActionsLog();
    clone->setErrorMsg(this->getErrorMsg());
    clone->setStatus(this->getStatus());
    return clone;
}

const string PrintActionsLog::toString() const {
    return "log " + this->getStatusString();
}

Close::Close() = default;

void Close::act(Simulation &simulation) {
    simulation.close();
    complete();
}

Close * Close::clone() const {
    Close* clone = new Close();
    clone->setErrorMsg(this->getErrorMsg());
    clone->setStatus(this->getStatus());
    return clone;
}

const string Close::toString() const {
    return "Close " + this->getStatusString();
}

BackupSimulation::BackupSimulation() = default;

void BackupSimulation::act(Simulation &simulation) {
    if(backup!=nullptr){
    	delete backup;
    }
    backup = new Simulation(simulation);
    complete();
}

BackupSimulation * BackupSimulation::clone() const {
    BackupSimulation* clone = new BackupSimulation();
    clone->setErrorMsg(this->getErrorMsg());
    clone->setStatus(this->getStatus());
    return clone;
}

const string BackupSimulation::toString() const {
    return "backup " + this->getStatusString();
}

RestoreSimulation::RestoreSimulation() = default;

void RestoreSimulation::act(Simulation &simulation) {
    if (backup == nullptr) {
        error("Error: No backup available");
        std::cout << this->getErrorMsg() << std::endl;
    }
    else {
        simulation = *backup;
        complete();
    }
}

RestoreSimulation * RestoreSimulation::clone() const {
    RestoreSimulation* clone = new RestoreSimulation();
    clone->setErrorMsg(this->getErrorMsg());
    clone->setStatus(this->getStatus());
    return clone;
}

const string RestoreSimulation::toString() const {
    return "restore " + this->getStatusString();
}
