
#include "SIUnit.hpp"

SIUnit::SIUnit()
    : is_known(false)
{
    for (int i = 0; i < 7; ++i)
        this->units[i] = 0;
}

// unit format example: [m] [kg m2 s-2] [s-1] [m s2]
SIUnit::SIUnit(std::string unit)
    : is_known(true)
{
    for (int i = 0; i < 7; ++i)
        this->units[i] = 0;
    
    unit = unit.substr(1, unit.size() - 2); // remove the brackets
    std::replace(unit.begin(), unit.end(), '\t', ' ');

    if (unit == "\\")
        return;

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

        int unit_index = str_to_unit(unit_name);
        if (unit_index == -1)
            std::cerr << "Unknown unit: '" << unit_name << "'" << std::endl, exit(1);
        
        this->units[unit_index] += unit_power;
    }
}

int SIUnit::str_to_unit(std::string unit) const
{
    if (unit == "m")
        return 0;
    if (unit == "kg")
        return 1;
    if (unit == "s")
        return 2;
    if (unit == "A")
        return 3;
    if (unit == "K")
        return 4;
    if (unit == "mol")
        return 5;
    if (unit == "cd")
        return 6;
    return -1;
}

std::string SIUnit::unit_to_str(int unit) const
{
    if (unit == 0)
        return "m";
    if (unit == 1)
        return "kg";
    if (unit == 2)
        return "s";
    if (unit == 3)
        return "A";
    if (unit == 4)
        return "K";
    if (unit == 5)
        return "mol";
    if (unit == 6)
        return "cd";
    return "Unknown || Error";
}

bool SIUnit::operator==(const SIUnit &rhs) const
{
    // if (!this->is_known || !rhs.is_known)
        // return false;
    
    for (int i = 0; i < 7; ++i)
        if (this->units[i] != rhs.units[i])
            return false;
    
    return true;
}

std::ostream &operator<<(std::ostream &os, const SIUnit &sys)
{
    if (!sys.is_known)
    {
        os << "?";
        return os;
    }

    std::string unit_str = "";

    for (int i = 0; i < 7; ++i)
        if (sys.units[i] != 0)
            unit_str += sys.unit_to_str(i) + (sys.units[i] == 1 ? "" : std::to_string(sys.units[i])) + " ";
    
    if (unit_str.size() == 0)
    {
        os << "\\";
        return os;
    }
        
    unit_str.erase(unit_str.size() - 1);
    os << unit_str;
    return os;
}

// [kg.m.s^{-1}]
std::string SIUnit::to_latex() const
{
    if (!this->is_known)
        return "{[?]}";

    std::string unit_str = "{[";

    for (int i = 0; i < 7; ++i)
        if (this->units[i] != 0)
            unit_str += this->unit_to_str(i) + (this->units[i] == 1 ? "." : "^{" + std::to_string(this->units[i])) + "}.";
    
    if (unit_str.size() == 2)
        return "";
        
    return unit_str.erase(unit_str.size() - 1) + "]}";
}

SIUnit SIUnit::multiply(SIUnit unit)
{
    if (!this->is_known || !unit.is_known)
        return SIUnit();
    
    SIUnit res;
    res.is_known = true;
    for (int i = 0; i < 7; ++i)
        res.units[i] = this->units[i] + unit.units[i];
    
    return res;
}

SIUnit SIUnit::divide(SIUnit unit)
{
    if (!this->is_known || !unit.is_known)
        return SIUnit();
    
    SIUnit res;
    res.is_known = true;
    for (int i = 0; i < 7; ++i)
        res.units[i] = this->units[i] - unit.units[i];
    
    return res;
}
