#include "../include/Settlement.h"

Settlement::Settlement(const string &name, SettlementType type) :name(name),type(type){
}

const string & Settlement::getName() const {
    return this->name;
}

SettlementType Settlement::getType() const {
    return this->type;
}

const string Settlement::toString() const {
    return this->name + "\n";
}

Settlement::Settlement(const Settlement &other) : name(other.getName()),type(other.getType()){
}


Settlement::Settlement(Settlement &&other) : name(other.getName()),type(other.getType()){
}

