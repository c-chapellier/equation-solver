
#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <array>

class SIUnit
{
public:
    bool is_known;
    enum UnitType
    {
        S, M, KG, A, K, MOL, CD
    };
    int units[7];

private:
    static SIUnit str_to_unit(std::string unit);
    static std::string unit_to_str(int unit);

    SIUnit(const int *units);

public:
    SIUnit();
    SIUnit(std::string unit);
    std::string to_latex() const;
    SIUnit multiply(SIUnit unit);
    SIUnit divide(SIUnit unit);

    bool operator==(const SIUnit &rhs) const;

    friend std::ostream &operator<<(std::ostream &os, const SIUnit &sys);
};
