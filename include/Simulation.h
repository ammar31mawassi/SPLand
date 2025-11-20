#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
using std::string;
using std::vector;

class BaseAction;
class SelectionPolicy;

class Simulation {
public:
    Simulation(const string &configFilePath);
    void start();
    void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy);
    void addAction(BaseAction *action);
    bool addSettlement(Settlement *settlement);
    bool addFacility(FacilityType facility);
    bool isSettlementExists(const string &settlementName);
    Settlement &getSettlement(const string &settlementName);
    Plan &getPlan(const int planID);
    void step();
    void close();
    void open();
    vector<BaseAction*> getActionLog() const;
    vector<Plan> getPlans() const;
    bool isPlanExist(int plan_id) const;
    void clear() ;
    // rule of 5
    Simulation(const Simulation& other);
    Simulation& operator=(const Simulation& other);
    ~Simulation();

    Simulation(Simulation&& other);
    Simulation& operator=(Simulation&& other);

private:
    bool isRunning;
    int planCounter; //For assigning unique plan IDs
    vector<BaseAction*> actionsLog;
    vector<Plan> plans;
    vector<Settlement*> settlements;
    vector<FacilityType> facilitiesOptions;

};