/*
    personality.cpp — World Champion personalities + exchange sacrifice evaluator.
    Copyright (C) 2026 Puzzle Champions fork of Texel.

    Personality scales are derived from the "World Champions Playing Style Report"
    (Wikipedia + Exeter Chess Club + Chess.com style research). Each scale is
    per128 (128 = neutral/default behaviour).
*/

#include "personality.hpp"
#include <map>

// Neutral scale
static const PersonalityScale NEUTRAL = {128,128,128,128,128,128,128,128};

// Function-local static (Meyers singleton) to avoid static-initialization-order
// fiasco: UciParams::personality is constructed at startup and calls
// personalityNames(), which must not touch a not-yet-constructed global map.
static const std::map<std::string, PersonalityScale>& scales() {
    static const std::map<std::string, PersonalityScale> SCALES = {
    // kingSafe threat mobility endgame pawnStr exchSac passP drawTol
    {"Capablanca",  { 90,  70,  80, 160, 150,  50, 120,  90}},
    {"Alekhine",    {180, 170, 150,  90,  80, 140, 140,  70}},
    {"Botvinnik",   {128, 128, 128, 128, 130, 128, 128, 128}},
    {"Smyslov",     {100, 110, 100, 160, 150, 110, 130, 110}},
    {"Tal",         {210, 210, 180,  70,  60, 190, 160,  40}},
    {"Petrosian",   { 50,  40,  60, 110, 170, 210,  80, 200}},
    {"Spassky",     {140, 150, 140, 120, 110, 130, 120, 100}},
    {"Fischer",     {150, 150, 150, 140, 130, 120, 140,  60}},
    {"Karpov",      { 60,  50,  70, 180, 180,  40, 140, 120}},
    {"Kasparov",    {190, 200, 170, 110, 120, 160, 160,  50}},
    {"Kramnik",     { 70,  60,  80, 190, 160,  60, 120, 160}},
    {"Carlsen",     {120, 120, 130, 180, 160, 110, 150, 100}},
    {"Topalov",     {190, 190, 170,  90,  90, 170, 150,  50}},
    };
    return SCALES;
}

PersonalityScale getPersonalityScale(const std::string& name) {
    const auto& SCALES = scales();
    auto it = SCALES.find(name);
    if (it == SCALES.end())
        return NEUTRAL;
    return it->second;
}

std::vector<std::string> personalityNames() {
    const auto& SCALES = scales();
    std::vector<std::string> names;
    names.push_back("None");
    for (const auto& p : SCALES)
        names.push_back(p.first);
    return names;
}
