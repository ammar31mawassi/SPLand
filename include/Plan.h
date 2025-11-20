#pragma once
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
using std::vector;

enum class PlanStatus {
    AVALIABLE,
    BUSY,
};

class Plan {
public:
    Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);
    const int getlifeQualityScore() const;
    const int getEconomyScore() const;
    const int getEnvironmentScore() const;
    void setSelectionPolicy(SelectionPolicy *selectionPolicy);
    void step();
    void printStatus();
    const vector<Facility*> &getFacilities() const;
    void addFacility(Facility* facility);
    const string toString() const;
    const string& settlementName() const;
    SelectionPolicy* getSelectionPolicy() const;
    string planStatusString() const;
    const int getPlanID() const;
    const vector<FacilityType> &getFacilityOptions() const;
    const vector<Facility*> getUnderConstructions() const;
    vector<Facility*>& getUnderConstructions1();
    vector<Facility*>& getFacilities1();
    PlanStatus getStatus() const ;

    Settlement getSettlement() const;
    void clear();
    //rule 5
    Plan(const Plan& other);
    Plan& operator=(const Plan& other);
    ~Plan() ;

    Plan(Plan&& other);
    Plan& operator=(Plan&& other);


private:
    int plan_id;
    const Settlement &settlement;
    SelectionPolicy *selectionPolicy; //What happens if we change this to a reference?
    PlanStatus status;
    vector<Facility*> facilities;
    vector<Facility*> underConstruction;
    const vector<FacilityType> &facilityOptions;
    int life_quality_score, economy_score, environment_score;
    size_t buldingMax;
};