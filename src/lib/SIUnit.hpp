
#pragma once

#include <string>
#include <iostream>
#include <map>

class SIUnit
{
public:
    bool is_known;
    std::map<std::string, int> units = std::map<std::string, int>({
        { "s", 0 },
        { "m", 0 },
        { "kg", 0 },
        { "A", 0 },
        { "K", 0 },
        { "mol", 0 },
        { "cd", 0 }
    });

public:
    SIUnit();
    SIUnit(std::string unit);
    std::string to_string() const;
    SIUnit multiply(SIUnit unit);
    SIUnit divide(SIUnit unit);
};
