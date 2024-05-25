
#include "SIUnit.hpp"

static const std::vector<std::pair<std::string, std::array<int, 7>>> UNITS_DB {
    // SI Units
    //         cd mol   K  kg   m   s   A
    { "cd",  {  1,  0,  0,  0,  0,  0,  0 } },
    { "mol", {  0,  1,  0,  0,  0,  0,  0 } },
    { "K",   {  0,  0,  1,  0,  0,  0,  0 } },
    { "kg",  {  0,  0,  0,  1,  0,  0,  0 } },
    { "m",   {  0,  0,  0,  0,  1,  0,  0 } },
    { "s",   {  0,  0,  0,  0,  0,  1,  0 } },
    { "A",   {  0,  0,  0,  0,  0,  0,  1 } },
    // Derived Units
    //         cd mol   K  kg   m   s   A
    // { "rad", {  0,  0,  0,  0,  0,  0,  0 } }, // ""
    // { "sr",  {  0,  0,  0,  0,  0,  0,  0 } }, // ""
    // { "Hz",  {  0,  0,  0,  0,  0, -1,  0 } }, // "s-1"
    { "N",   {  0,  0,  0,  1,  1, -2,  0 } }, // "kg⋅m⋅s-2"
    { "Pa",  {  0,  0,  0,  1, -1, -2,  0 } }, // "kg⋅m-1⋅s-2"
    { "Nm",   {  0,  0,  0,  1,  2, -2,  0 } }, // "kg⋅m2⋅s-2"
    { "W",   {  0,  0,  0,  1,  2, -3,  0 } }, // "kg⋅m2⋅s-3"
    { "C",   {  0,  0,  0,  0,  0,  1,  1 } }, // "s⋅A"
    { "V",   {  0,  0,  0,  1,  2, -3, -1 } }, // "kg⋅m2⋅s-3⋅A-1"
    { "F",   {  0,  0,  0, -1, -2,  4,  2 } }, // "kg-1⋅m-2⋅s4⋅A2"
    { "ohm", {  0,  0,  0,  1,  2, -3, -2 } }, // "kg⋅m2⋅s-3⋅A-2"
    { "S",   {  0,  0,  0, -1, -2,  3,  2 } }, // "kg-1⋅m-2⋅s3⋅A2"
    { "Wb",  {  0,  0,  0,  1,  2, -2, -1 } }, // "kg⋅m2⋅s-2⋅A-1"
    { "T",   {  0,  0,  0,  1, -2, -2,  1 } }, // "kg⋅s-2⋅A-1"
    { "H",   {  0,  0,  0,  1,  2, -2, -2 } }, // "kg⋅m2⋅s-2⋅A-2"
    { "°C",  {  0,  0,  1,  0,  0,  0,  0 } }, // "K"
    // { "lm",  {  1,  0,  0,  0,  0,  0,  0 } }, // "cd"
    { "lx",  {  1,  0,  0,  0, -2,  0,  0 } }, // "cd/m2"
    // { "Bq",  {  0,  0,  0,  0,  0, -1,  0 } }, // "s-1"
    { "Gy",  {  0,  0,  0,  0,  0, -2,  0 } }, // "m2⋅s-2"
    { "Sv",  {  0,  0,  0,  0,  0, -2,  0 } }, // "m2⋅s-2"
    { "kat", {  0,  1,  0,  0,  0, -1,  0 } }, // "mol⋅s-1"
};

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

        SIUnit current_unit = SIUnit::str_to_unit(unit_name);

        for (int j = 0; j < 7; ++j)
            this->units[j] += current_unit.units[j] * unit_power;
    }
}

SIUnit::SIUnit(const int *units)
    : is_known(true)
{
    for (int i = 0; i < 7; ++i)
        this->units[i] = units[i];
}

SIUnit SIUnit::str_to_unit(std::string unit)
{
    for (auto &unit_str : UNITS_DB)
        if (unit == unit_str.first)
            return SIUnit(unit_str.second.data());

    std::cerr << "Unknown unit: '" << unit << "'" << std::endl, exit(1);
}

std::string SIUnit::unit_to_str(int unit)
{
    for (auto &unit_str : UNITS_DB)
        if (unit_str.second[unit] != 0)
            return unit_str.first;

    std::cerr << "Unknown unit index: '" << unit << "'" << std::endl, exit(1);
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
            unit_str += SIUnit::unit_to_str(i) + (sys.units[i] == 1 ? "" : std::to_string(sys.units[i])) + " ";
    
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
        return "\\,{[?]}";

    std::string unit_str = "\\,{[";

    int current_unit[7];
    
    for (int i = 0; i < 7; ++i)
        current_unit[i] = this->units[i];

    // search for derived units by exact match
    for (auto &unit_str_pair : UNITS_DB)
    {
        bool match = true;
        for (int i = 0; i < 7; ++i)
        {
            if (current_unit[i] != unit_str_pair.second[i])
            {
                match = false;
                break;
            }
        }
        if (match)
        {
            unit_str += unit_str_pair.first + ".";
            for (int i = 0; i < 7; ++i)
                current_unit[i] = 0;
            break;
        }
    }

    for (int i = 0; i < 7; ++i)
        if (current_unit[i] != 0)
            unit_str += SIUnit::unit_to_str(i) + (current_unit[i] == 1 ? "." : "^{" + std::to_string(this->units[i]) + "}.");
    
    if (unit_str.size() == 4)
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
