
#pragma once

#include <string>
#include <iostream>
#include <vector>

class SIUnit
{
public:
    bool is_known;
    enum UnitType
    {
        S, M, KG, A, K, MOL, CD
    };
    int units[7];

public:
    SIUnit();
    SIUnit(std::string unit);
    std::string to_latex() const;
    SIUnit multiply(SIUnit unit);
    SIUnit divide(SIUnit unit);
    int str_to_unit(std::string unit) const;
    std::string unit_to_str(int unit) const;

    bool operator==(const SIUnit &rhs) const;

    friend std::ostream &operator<<(std::ostream &os, const SIUnit &sys);
};
