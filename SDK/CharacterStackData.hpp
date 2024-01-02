#pragma once

#include <cstdint>

#include "AString.hpp"
#include "Pad.hpp"

class CharacterStackData {
public:
    AString model;
private:
    PAD(0x10);
public:
    std::int32_t skin;
private:
    PAD(0x60);
public:
    std::int8_t gear;
private:
    PAD(0x7);
};
