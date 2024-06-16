#pragma once

#include "Champion.hpp"
#include "Pad.hpp"

class ChampionManager {
private:
    PAD(0x18);
public:
    RiotArray<Champion*> champions;
};
