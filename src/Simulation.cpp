
#include "../include/Simulation.h"

#include <fstream>

#include "../include/Action.h"
#include "../include/Plan.h"

#include "../include/Auxiliary.h"

Simulation::Simulation(const string &configFilePath) : isRunning(false), planCounter(0), actionsLog(),
plans(), settlements(), facilitiesOptions(){
    string lineCommand ;
    std::ifstream in(configFilePath, std::ios_base::in);

   while (std::getline(in,lineCommand)) {
    vector<string> commands = Auxiliary::parseArguments(lineCommand);
        if(commands[0] != "#"){
            if (commands[0] == "settlement") {
                SettlementType type;
                if (commands[2] == "0") {
                    type = SettlementType::VILLAGE;
                }
                else if (commands[2] == "1") {
                    type = SettlementType::CITY;
                }
                else {
                    type = SettlementType::METROPOLIS;
                }
                Settlement* settlement = new Settlement(commands.at(1), type);
                addSettlement(settlement);
            }
            else if (commands[0] == "plan") {
                if(isSettlementExists(commands[1])){
                    if (commands[2] == "nve")  {
                        addPlan(this->getSettlement(commands[1]),new NaiveSelection()); 
                    }
                
                    if (commands[2] == "bal")  {
                        addPlan(this->getSettlement(commands[1]),new BalancedSelection(0,0,0));                    
                    }
                    if (commands[2] == "eco")  {
                    
                        addPlan(this->getSettlement(commands[1]),new EconomySelection()); 
                    
                    }
                    if (commands[2] == "env")  {
                        addPlan(this->getSettlement(commands[1]),new SustainabilitySelection()); 
                    
                    }
                }
            }
            else if (commands.at(0) == "facility") {
                FacilityCategory category = FacilityCategory(std::stoi(commands[2]));
                int price =  std::stoi(commands.at(3));
                int lifeScore =  std::stoi(commands.at(4));
                int ecoScore =  std::stoi(commands.at(5));
                int envScore =  std::stoi(commands.at(6));
                FacilityType facility = FacilityType(commands.at(1),category,price,lifeScore,ecoScore,envScore);
               addFacility(facility);
            }
        }
    }
    in.close();
}

void Simulation::start() {
    isRunning = true;
    std::cout<<"The simulation has started"<<std::endl;
    string input;
    vector<string> command;
    BaseAction* action = nullptr;
    while(isRunning){
        std::getline(std::cin,input);
        command = Auxiliary::parseArguments(input);
        if(command.size() == 0) continue;
        if(command[0] == "step") 
            action = new SimulateStep(std::stoi(command[1]));
        else if(command[0] =="plan")
            action = new AddPlan(command[1],command[2]);
        else if(command[0] =="settlement"){
            SettlementType type;
            if (command[2] == "0")
                type = SettlementType::VILLAGE;
            else if (command[2] == "1")
                type = SettlementType::CITY;
            else
                type = SettlementType::METROPOLIS;
            action = new AddSettlement(command[1],type);
        }
        else if(command[0] =="facility"){
            FacilityCategory type;
            if (command[2] == "0")
                type = FacilityCategory::LIFE_QUALITY;
            else if (command[2] == "1")
                type = FacilityCategory::ECONOMY;
            else
                type = FacilityCategory::ENVIRONMENT;
            action = new AddFacility(command[1],type,std::stoi(command[3]),
                std::stoi(command[4]),std::stoi(command[5]),std::stoi(command[6]));
        }
        else if(command[0] =="planStatus"){
                action = new PrintPlanStatus(std::stoi(command[1]));
            }
        else if(command[0] =="changePolicy")
            action = new ChangePlanPolicy(std::stoi(command[1]),command[2]);
        else if(command[0] =="log")
            action = new PrintActionsLog();
        else if(command[0] =="close")
            action = new Close();
        else if(command[0] =="backup")
            action = new BackupSimulation();
        else if(command[0] =="restore")
            action = new RestoreSimulation();
        if(action != nullptr){
            action->act(*this);
            actionsLog.push_back(action);
        }
        action = nullptr;

    }

}

void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy) {
    Plan toAdd(this->planCounter, settlement, selectionPolicy, facilitiesOptions);
    this->plans.push_back(toAdd);
    this->planCounter++;
}

void Simulation::addAction(BaseAction *action) {
    actionsLog.push_back(action);
}

bool Simulation::addSettlement(Settlement *settlement) {
    if (!isSettlementExists(settlement->getName())) {
        settlements.push_back(settlement);
        return true;
    }
    return false;
}

bool Simulation::addFacility(FacilityType facility) {
    bool found = false;
    for (FacilityType & facilitiesOption : facilitiesOptions) {
        if (facilitiesOption.getName() == facility.getName()) found = true;
    }
    if (!found) {
        facilitiesOptions.push_back(facility);
        return true;
    }
    return false;
}

bool Simulation::isSettlementExists(const string &settlementName) {
    for (std::size_t i = 0; i < this->settlements.size(); i++) {
        if (this->settlements.at(i)->getName()
         == settlementName) {
            return true;
        }
    }
    return false;
}

Settlement & Simulation::getSettlement(const string &settlementName) {
    for (std::size_t i = 0; i < this->settlements.size(); i++) {
        if (this->settlements.at(i)->getName() == settlementName) {
            return *this->settlements.at(i);
        }
    }
    Settlement *ER = nullptr;
    return *ER;
}

Plan & Simulation::getPlan(const int planID) {
    if (this->isPlanExist(planID))
        return plans.at(planID);
    Plan* ERROR = nullptr;
    return *ERROR;
}

void Simulation::step() {
    for (std::size_t i = 0; i < this->plans.size(); i++) {
        plans.at(i).step();
    }
}

void Simulation::close() {
    isRunning = false;
    planCounter = 0;
    for(std::size_t i = 0; i < this->plans.size(); i++){
        std::cout<<plans.at(i).planStatusString()<< std::endl;
    }
}

void Simulation::open() {
    isRunning = true;
}

vector<BaseAction *> Simulation::getActionLog() const {
    return actionsLog;
}

vector<Plan> Simulation::getPlans() const {
    return plans;
}

bool Simulation::isPlanExist(int plan_id) const{
    if(plan_id < planCounter && plan_id >= 0)
        return true;
    return false;
}

void Simulation::clear() {
    
    facilitiesOptions.clear();
    plans.clear();
    settlements.clear();
    actionsLog.clear();
}


Simulation::Simulation(const Simulation &other):isRunning(other.isRunning),planCounter(other.planCounter), actionsLog(),
plans(), settlements(), facilitiesOptions(){
    for (BaseAction *action : other.actionsLog) {
        this->actionsLog.push_back(action->clone());
    }
    for (auto st : other.settlements) {
        this->settlements.push_back(new Settlement(*st));
    }
    for (const FacilityType& ft : other.facilitiesOptions) {
        this->facilitiesOptions.push_back(ft);
    }
    for (auto& plan : other.plans) {
        Plan p(plan.getPlanID(), getSettlement(plan.getSettlement().getName()), plan.getSelectionPolicy()->clone(), facilitiesOptions);
        for(auto& facility : plan.getFacilities()){
            p.getFacilities1().push_back(new Facility(*facility));
        }
        for(auto& facility : plan.getUnderConstructions()){
            p.getFacilities1().push_back(new Facility(*facility));
        }
        plans.push_back(p);
    }
}

Simulation & Simulation::operator=(const Simulation &other) {
    if (this != &other) {
        for(auto& action : this->actionsLog){
            delete action;
        }
        for (auto& st : this->settlements) {
            delete st;
        }
        clear();
        isRunning = other.isRunning;
        planCounter = other.planCounter;
        for (auto& action : other.actionsLog) {
            this->actionsLog.push_back(action->clone());
        }
        for (auto& st : other.settlements) {
            
            this->settlements.push_back(new Settlement(*st));
        }
        for (auto& ft : other.facilitiesOptions) {
            this->facilitiesOptions.push_back(ft);
        }
        for (auto& plan : other.plans) {
            Plan p(plan.getPlanID(), getSettlement(plan.getSettlement().getName()), plan.getSelectionPolicy()->clone(), facilitiesOptions);
            for(auto& facility : plan.getFacilities()){
                p.getFacilities1().push_back(new Facility(*facility));
            }
            for(auto& facility : plan.getUnderConstructions()){
                p.getFacilities1().push_back(new Facility(*facility));
            }
            plans.push_back(p);
        }
    }
    else {
        std::cout<<"h1"<<std::endl;
    }
    return *this;
}

Simulation::~Simulation() {
    for(size_t i = 0; i < actionsLog.size(); i++){
        delete actionsLog[i];
        actionsLog[i] = nullptr;
    }
    for(size_t i = 0; i < settlements.size(); i++){
        delete settlements[i];
        settlements[i] = nullptr;
    }
    this->clear();
}

Simulation::Simulation(Simulation &&other):isRunning(other.isRunning),planCounter(other.planCounter), actionsLog(),
plans(), settlements(), facilitiesOptions() {

    for (BaseAction *action : other.actionsLog) {
        this->actionsLog.push_back(action);
    }
    for (Settlement *st : other.settlements) {
        this->settlements.push_back(st);
    }
    for (FacilityType& ft : other.facilitiesOptions) {
        this->facilitiesOptions.push_back(ft);
    }
    for (const Plan& plan : other.plans) {
        this->plans.push_back(plan);
    }
    other.clear();
}

Simulation & Simulation::operator=(Simulation &&other) {
    if (this != &other) {
        for(BaseAction *action : this->actionsLog){
            delete action;
            action = nullptr;
        }
        for (Settlement *st : this->settlements) {
            delete st;
            st = nullptr;
        }
        clear();
        isRunning = other.isRunning;
        planCounter = other.planCounter;
        for (BaseAction *action : other.actionsLog) {
            this->actionsLog.push_back(action);
        }
        for (const Plan& plan : other.plans) {
            this->plans.push_back(plan);
        }
        for (Settlement *st : other.settlements) {
            this->settlements.push_back(st);
        }
        for (const FacilityType& ft : other.facilitiesOptions) {
            this->facilitiesOptions.push_back(ft);
        }
        other.clear();
    }
    else {
        std::cout<<"h2"<<std::endl;
    }
    return *this;
}
