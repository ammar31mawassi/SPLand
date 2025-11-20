

#include "../include/Plan.h"

#include <iostream>

Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy,
           const vector<FacilityType> &facilityOptions): plan_id(planId), settlement(settlement), 
           selectionPolicy(selectionPolicy),status(PlanStatus::AVALIABLE),facilities(),
           underConstruction(), facilityOptions(facilityOptions),
           life_quality_score(0), economy_score(0), environment_score(0),buldingMax(0){
    if(settlement.getType() == SettlementType::VILLAGE){
        this->buldingMax = 1;
    } else if(settlement.getType() == SettlementType::CITY){
        this->buldingMax = 2;
    } else {
        this->buldingMax = 3;
    }
}

const int Plan::getlifeQualityScore() const {
    return this->life_quality_score;
}

const int Plan::getEconomyScore() const {
    return this->economy_score;
}

const int Plan::getEnvironmentScore() const {
    return this->environment_score;
}

void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy) {
    if(this->selectionPolicy != nullptr){
        delete this->selectionPolicy;
    }
    this->selectionPolicy = selectionPolicy;
}

void Plan::step() {
    
    if (status == PlanStatus::AVALIABLE) {//Steps 1 and 2
        while (underConstruction.size() < buldingMax) {
            Facility* toBuild = new Facility(this->selectionPolicy->selectFacility(this->facilityOptions),this->settlement.getName());
            this->addFacility(toBuild);
        }
    }

    for (std::size_t i = 0; i < this->underConstruction.size(); i++) {//step 2
        underConstruction.at(i)->step();
        if (underConstruction.at(i)->getStatus() == FacilityStatus::OPERATIONAL) {
            addFacility(underConstruction.at(i));
            i--;
        }
    }
    if (underConstruction.size() < buldingMax) {//Step 4
        status = PlanStatus::AVALIABLE;
    }else {
        status = PlanStatus::BUSY;
    }

}

void Plan::printStatus() {
    if (status == PlanStatus::BUSY)std::cout << " Busy" << std::endl;
    else std::cout << "Available" << std::endl;
}

const vector<Facility *> & Plan::getFacilities() const {
    return this->facilities;
}

void Plan::addFacility(Facility *facility) {
    if (facility->getStatus() == FacilityStatus::OPERATIONAL) {
        this->life_quality_score += facility->getLifeQualityScore();
        this->economy_score += facility->getEconomyScore();
        this->environment_score += facility->getEnvironmentScore();
        this->facilities.push_back(facility);
        for (size_t i = 0; i < this->underConstruction.size() ; i++) {
            if (this->underConstruction.at(i)->getName() == facility->getName()) {
                this->underConstruction.erase(this->underConstruction.begin() + i);
                break;
            }
        }
    }
    else {
        underConstruction.push_back(facility);
    }
}

const string Plan::toString() const {
    string stat = "planID: " + std::to_string(plan_id) +"\nSettlementName: " + settlement.getName() + "\nplanStatus: " ;
    if (status == PlanStatus::AVALIABLE)
        stat += "AVAILABLE \n";
    else
        stat += "BUSY \n";
    stat +="selectionPolicy: " + selectionPolicy->toString() + "\nLifeQualityScore: " +
        std::to_string(life_quality_score) + "\nEconomyScore: " + std::to_string(economy_score) + "\nEnvironmentScore: "
        + std::to_string(environment_score) + "\n";
    for(const Facility *facility : underConstruction){
        stat += facility->toString();
    }
    for (const Facility *facility : facilities) {
        stat += facility->toString();
    }
    return stat;
}

const string& Plan::settlementName() const {
    return settlement.getName();
}

SelectionPolicy* Plan::getSelectionPolicy() const{
    return this->selectionPolicy;
}

string Plan::planStatusString() const {
    return "planID: " + std::to_string(plan_id) +"\nSettlementName: " + this->settlementName() + "\nLifeQualityScore: " +
        std::to_string(life_quality_score) + "\nEconomyScore: " + std::to_string(economy_score) + "\nEnvironmentScore: "
        + std::to_string(environment_score);;
}

const int Plan::getPlanID() const {
    return plan_id;
}
const vector<FacilityType>& Plan::getFacilityOptions() const {
    return facilityOptions;
}
const vector<Facility*> Plan::getUnderConstructions() const{
    return underConstruction;
}

PlanStatus Plan::getStatus() const {
    return status;
}

Settlement Plan::getSettlement() const {
    return settlement;
}

void Plan::clear() {

    this->facilities.clear();
    this->underConstruction.clear();
    delete selectionPolicy;
    selectionPolicy = nullptr;
}

// rule of 5
Plan::Plan(const Plan& other): plan_id(other.plan_id),
      settlement(other.settlement),
      selectionPolicy(other.selectionPolicy->clone()),
      status(other.status),
      facilities(),
      underConstruction(),
      facilityOptions(other.facilityOptions),
      life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score),buldingMax(other.buldingMax) {
    for(Facility* facility : other.getFacilities()){
            Facility* toInsert1 = new Facility(*facility);
            this->facilities.push_back(toInsert1);
        }
    for(Facility *facility : other.getUnderConstructions()){
            Facility* toInsert2 = new Facility(*facility);
            this->underConstruction.push_back(toInsert2);
    }

}

Plan& Plan::operator=(const Plan& other){
    if(this != &other){
        for(Facility *facility1 : this->facilities){
            delete facility1;
        }

        for(Facility *facility2 : this->underConstruction){
            delete facility2;
        }
        clear();
        this->plan_id = other.getPlanID();
        this->status = other.status;
        this->life_quality_score = other.getlifeQualityScore();
        this->economy_score = other.getEconomyScore();
        this->environment_score = other.getEnvironmentScore();
        SettlementType t = settlement.getType();
        if(t == SettlementType::VILLAGE){
           buldingMax = 1;
           }else if(t == SettlementType::CITY){
            buldingMax = 2;
           }else{
            buldingMax = 3;
        }
        selectionPolicy = other.getSelectionPolicy()->clone();
        for(Facility* facility : other.getFacilities()){
            Facility* toInsert1 = new Facility(*facility);
            this->facilities.push_back(toInsert1);
        }
        for(Facility *facility : other.getUnderConstructions()){
            Facility* toInsert2 = new Facility(*facility);
            this->underConstruction.push_back(toInsert2);
        }
    }
    return *this;
}

Plan::~Plan(){
    for(Facility *facility1 : this->facilities){
        delete facility1;
    }

    for(Facility *facility2 : this->underConstruction){
        delete facility2;
    }
    clear();
}

Plan::Plan(Plan&& other):  plan_id(other.getPlanID()),settlement(other.getSettlement()),
selectionPolicy(other.getSelectionPolicy()),status(other.getStatus()),facilities(),underConstruction(),facilityOptions(other.getFacilityOptions()),
life_quality_score(other.getlifeQualityScore()),economy_score(other.getEconomyScore()),environment_score(other.getEnvironmentScore()),buldingMax(0){


    for(Facility *facility : other.getFacilities()){
        this->facilities.push_back(facility);
    }
    for(Facility *facility : other.getUnderConstructions()){
        this->underConstruction.push_back(facility);
    }
    other.setSelectionPolicy(nullptr);
    other.clear();
}

Plan& Plan::operator=(Plan&& other){
    if(this != &other){
        for(Facility *facility1 : this->facilities){
            delete facility1;
        }

        for(Facility *facility2 : this->underConstruction){
            delete facility2;
        }
        clear();
        this->plan_id = other.getPlanID();
        this->life_quality_score = other.getlifeQualityScore();
        this->economy_score = other.getEconomyScore();
        this->environment_score = other.getEnvironmentScore();
        this->status = other.getStatus();
        SettlementType t = other.getSettlement().getType();
        if(t == SettlementType::VILLAGE)
            buldingMax = 1;
        else if (t == SettlementType::CITY)
            buldingMax = 2;
        else
            buldingMax = 3;
        selectionPolicy = other.getSelectionPolicy();
        other.setSelectionPolicy(nullptr);
        for(Facility *facility : other.getFacilities()){
            this->facilities.push_back(facility);
        }
        for(Facility *facility : other.getUnderConstructions()) {
            this->underConstruction.push_back(facility);
        }
        other.clear();
    }
    return *this;

}


vector<Facility*>& Plan::getUnderConstructions1(){
    return underConstruction;
}
vector<Facility*>& Plan::getFacilities1(){
    return facilities;
}
    