
#include "es.hpp"

void siu_init(siu_t *siu)
{
    siu->is_known = false;

    for (int i = 0; i < 7; ++i)
        siu->units[i] = 0;
}

// unit format example: [m] [kg m2 s-2] [s-1] [m s2]
void siu_init(siu_t *siu, std::string unit)
{
    siu->is_known = true;

    for (int i = 0; i < 7; ++i)
        siu->units[i] = 0;
    
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

        int unit_index = siu_str_to_unit(unit_name);
        if (unit_index == -1)
            std::cerr << "Unknown unit: '" << unit_name << "'" << std::endl, exit(1);
        
        siu->units[unit_index] += unit_power;
    }
}

int siu_str_to_unit(std::string unit)
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

std::string siu_unit_to_str(int unit)
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

bool siu_compare(siu_t a, siu_t b)
{
    // if (!this->is_known || !rhs.is_known)
        // return false;
    
    for (int i = 0; i < 7; ++i)
        if (a.units[i] != b.units[i])
            return false;
    
    return true;
}

// std::ostream &operator<<(std::ostream &os, const SIUnit &sys)
// {
//     if (!sys.is_known)
//     {
//         os << "?";
//         return os;
//     }

//     std::string unit_str = "";

//     for (int i = 0; i < 7; ++i)
//         if (sys.units[i] != 0)
//             unit_str += sys.unit_to_str(i) + (sys.units[i] == 1 ? "" : std::to_string(sys.units[i])) + " ";
    
//     if (unit_str.size() == 0)
//     {
//         os << "\\";
//         return os;
//     }
        
//     unit_str.erase(unit_str.size() - 1);
//     os << unit_str;
//     return os;
// }

// [kg.m.s^{-1}]
std::string siu_to_latex(siu_t siu)
{
    if (!siu.is_known)
        return "{[?]}";

    std::string unit_str = "{[";

    for (int i = 0; i < 7; ++i)
        if (siu.units[i] != 0)
            unit_str += siu_unit_to_str(i) + (siu.units[i] == 1 ? "." : "^{" + std::to_string(siu.units[i])) + "}.";
    
    if (unit_str.size() == 2)
        return "";
        
    return unit_str.erase(unit_str.size() - 1) + "]}";
}

siu_t siu_multiply(siu_t a, siu_t b)
{
    siu_t res;
    siu_init(&res);

    if (!a.is_known || !b.is_known)
        return res;
    
    res.is_known = true;
    for (int i = 0; i < 7; ++i)
        res.units[i] = a.units[i] + b.units[i];
    
    return res;
}

siu_t siu_divide(siu_t a, siu_t b)
{
    siu_t res;
    siu_init(&res);

    if (!a.is_known || !b.is_known)
        return res;
    
    res.is_known = true;
    for (int i = 0; i < 7; ++i)
        res.units[i] = a.units[i] - b.units[i];
    
    return res;
}
