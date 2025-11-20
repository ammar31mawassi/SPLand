
#include "Facility.h"

FacilityType::FacilityType(const string &name, const FacilityCategory category, const int price,
    const int lifeQuality_score, const int economy_score, const int environment_score): name(name),
    category(category), price(price), lifeQuality_score(lifeQuality_score), economy_score(economy_score),
    environment_score(environment_score){

    }

const string& FacilityType::getName() const {
    return this->name;
}

int FacilityType::getCost() const {
    return this->price;
}

int FacilityType::getLifeQualityScore() const {
    return this->lifeQuality_score;
}

int FacilityType::getEnvironmentScore() const {
    return this->environment_score;
}

int FacilityType::getEconomyScore() const {
    return this->economy_score;
}

FacilityCategory FacilityType::getCategory() const {
    return this->category;
}
FacilityType::FacilityType( const FacilityType& other) : name(other.name),
    category(other.category), price(other.price), lifeQuality_score(other.lifeQuality_score), economy_score(other.economy_score),
    environment_score(other.environment_score){

}



Facility::Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price,
    const int lifeQuality_score, const int economy_score, const int environment_score): FacilityType(name,category,price,lifeQuality_score,economy_score,environment_score)
    ,settlementName(settlementName),status(FacilityStatus::UNDER_CONSTRUCTIONS),timeLeft(price) {
}

Facility::Facility(const FacilityType &type, const string &settlementName): FacilityType(type),settlementName(settlementName)
,status(FacilityStatus::UNDER_CONSTRUCTIONS),timeLeft(type.getCost()) {
}

const string & Facility::getSettlementName() const {
    return this->settlementName;
}

const int Facility::getTimeLeft() const {
    return this->timeLeft;
}

FacilityStatus Facility::step() {
    this->timeLeft -= 1;
    if (this->timeLeft == 0) {
        this->status = FacilityStatus::OPERATIONAL;
    }
    return status;
}

void Facility::setStatus(FacilityStatus status) {
    this->status = status;
}

const FacilityStatus & Facility::getStatus() const {
    return this->status;
}

const string Facility::toString() const {
    if (this->status == FacilityStatus::UNDER_CONSTRUCTIONS)
        return "FacilityName: " + name + "\nfacilityStatus: UNDER_CONSTRUCTION\n" ;
    else
        return "FacilityName: " + name + "\nfacilityStatus: OPERATIONAL\n";
}