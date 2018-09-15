#ifndef COMBAT_REPORT_HPP
#define COMBAT_REPORT_HPP

#include <cstdint>

struct CombatReport
{
    uint16_t event; // 
    uint16_t x1;
    uint16_t y1;
    uint16_t terrain1;
    uint16_t unit_icon1;
    uint16_t x2;
    uint16_t y2;
    uint16_t terrain2;
    uint16_t unit_icon2;
};

#endif // COMBAT_REPORT_HPP
