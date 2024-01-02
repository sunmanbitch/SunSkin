#pragma once

#include "AString.hpp"
#include "Pad.hpp"
#include "Skin.hpp"
#include "RiotArray.hpp"

class Champion {
private:
    PAD(0x8);
public:
    AString champion_name;
private:
    PAD(0xD0);
public:
    RiotArray<Skin> skins;
};
