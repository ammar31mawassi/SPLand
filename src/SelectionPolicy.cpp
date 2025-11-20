
#include "../include/SelectionPolicy.h"
#include <algorithm>

NaiveSelection::NaiveSelection():lastSelectedIndex(-1) {
    
}

const FacilityType & NaiveSelection::selectFacility(const vector<FacilityType> &facilitiesOptions) {
    this->lastSelectedIndex++ ;
    if ((size_t)lastSelectedIndex >= facilitiesOptions.size()) {
        lastSelectedIndex = 0;
    }
    return facilitiesOptions.at(this->lastSelectedIndex);
}

const string NaiveSelection::toString() const {
    return "nve \n";
}

NaiveSelection * NaiveSelection::clone() const {
    auto *clone = new NaiveSelection();
    clone->lastSelectedIndex = lastSelectedIndex;
    return clone;
}

BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore) : LifeQualityScore(LifeQualityScore),
EconomyScore(EconomyScore),EnvironmentScore(EnvironmentScore) {
}

const FacilityType & BalancedSelection::selectFacility(const vector<FacilityType> &facilitiesOptions) {
    int distance = -1;
    int lifeScoreTotal = 0, economyScoreTotal = 0, environmentScoreTotal = 0;
    const FacilityType *BestChoice = nullptr;
    int currDistance = 0;
    for (const auto & facilitiesOption : facilitiesOptions) {// To find the lowest distance
        lifeScoreTotal = facilitiesOption.getLifeQualityScore() + this->LifeQualityScore;
        economyScoreTotal = facilitiesOption.getEconomyScore() + this->EconomyScore;
        environmentScoreTotal = facilitiesOption.getEnvironmentScore() + this->EnvironmentScore;
        currDistance = std::max({lifeScoreTotal,economyScoreTotal,environmentScoreTotal}) - std::min({lifeScoreTotal,economyScoreTotal,environmentScoreTotal});
        if (currDistance < distance || distance == -1) {// better or first one
            distance = currDistance;
            BestChoice = &facilitiesOption;
        }
    }
    this->LifeQualityScore = BestChoice->getLifeQualityScore();// updating the scores
    this->EconomyScore = BestChoice->getEconomyScore();
    this->EnvironmentScore = BestChoice->getEnvironmentScore();
    return *BestChoice;
}

const string BalancedSelection::toString() const {
    return "bal";
}

BalancedSelection * BalancedSelection::clone() const {
    auto *clone = new BalancedSelection(this->LifeQualityScore,this->EconomyScore,this->EnvironmentScore);
    return clone;
}

EconomySelection::EconomySelection():lastSelectedIndex(-1) {

}

const FacilityType & EconomySelection::selectFacility(const vector<FacilityType> &facilitiesOptions) {
    while (true) {
        lastSelectedIndex++;
        if ((size_t)lastSelectedIndex >= facilitiesOptions.size()) {
            lastSelectedIndex = 0;
        }
        if (facilitiesOptions.at(lastSelectedIndex).getCategory() == FacilityCategory::ECONOMY) {
            break;
        }
    }
    return facilitiesOptions.at(lastSelectedIndex);
}

const string EconomySelection::toString() const {
    return "eco";
}

EconomySelection * EconomySelection::clone() const {
    auto *clone = new EconomySelection();
    clone->lastSelectedIndex = lastSelectedIndex;
    return clone;
}

SustainabilitySelection::SustainabilitySelection():lastSelectedIndex(-1) {

}

const FacilityType & SustainabilitySelection::selectFacility(const vector<FacilityType> &facilitiesOptions) {
    while (true) {
        lastSelectedIndex++;
        if ((size_t)lastSelectedIndex >= facilitiesOptions.size()) {
            lastSelectedIndex = 0;
        }
        if (facilitiesOptions.at(lastSelectedIndex).getCategory() == FacilityCategory::ENVIRONMENT) {
            break;
        }
    }
    return facilitiesOptions.at(lastSelectedIndex);
}

const string SustainabilitySelection::toString() const {
    return "env";
}

SustainabilitySelection * SustainabilitySelection::clone() const {
    auto *clone = new SustainabilitySelection();
    clone->lastSelectedIndex = lastSelectedIndex;
    return clone;
}
