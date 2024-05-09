
#include "SIUnit.hpp"

SIUnit::SIUnit()
    : is_known(false), is_unitless(false)
{

}

// unit format example: [m] [kg m2 s-2] [s-1] [m s2]
SIUnit::SIUnit(std::string unit)
    : is_known(true), is_unitless(false)
{
    unit = unit.substr(1, unit.size() - 2); // remove the brackets
    std::replace(unit.begin(), unit.end(), '\t', ' ');

    if (unit == "\\")
    {
        this->is_unitless = true;
        return;
    }

    while (unit.size() > 0)
    {
        while (unit[0] == ' ')
            unit.erase(0, 1);

        int i = 0;
        while (isalpha(unit[i]))
            ++i;
        std::string unit_name = unit.substr(0, i);
        unit.erase(0, i);

        i = 0;
        if (unit[i] == '-')
            ++i;
        while (isdigit(unit[i]))
            ++i;
        int unit_power = i == 0 ? 1 : std::atof(unit.substr(0, i).c_str());
        unit.erase(0, i);

        if (this->units.find(unit_name) == this->units.end())
            std::cerr << "Unknown unit: '" << unit_name << "'" << std::endl, exit(1);
        
        this->units[unit_name] += unit_power;
    }
}

std::string SIUnit::to_string() const
{
    std::string unit_str = "";

    if (this->is_known && this->is_unitless)
        return "\\";

    for (auto &unit : this->units)
        if (unit.second != 0)
            unit_str += unit.first + (unit.second == 1 ? "" : std::to_string(unit.second)) + " ";
    
    if (unit_str.size() == 0)
        return "";
        
    return unit_str.erase(unit_str.size() - 1);
}

SIUnit SIUnit::multiply(SIUnit unit)
{
    if (!this->is_known || !unit.is_known)
        return SIUnit();
    
    SIUnit res;
    res.is_known = true;
    for (auto &u : res.units)
        res.units[u.first] = this->units[u.first] + unit.units[u.first];
    
    return res;
}

SIUnit SIUnit::divide(SIUnit unit)
{
    if (!this->is_known || !unit.is_known)
        return SIUnit();
    
    SIUnit res;
    res.is_known = true;
    for (auto &u : res.units)
        res.units[u.first] = this->units[u.first] - unit.units[u.first];
    
    return res;
}
