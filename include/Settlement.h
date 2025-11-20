#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;

class Facility;

enum class SettlementType {
    VILLAGE,
    CITY,
    METROPOLIS,
};

class Settlement {
public:
    Settlement(const string &name, SettlementType type);
    const string &getName() const;
    SettlementType getType() const;
    const string toString() const;

    // rule of 3
    Settlement(const Settlement& other);
    ~Settlement() = default;

    // rule of 5
    Settlement(Settlement&& other);

private:
    const string name;
    SettlementType type;
};