#pragma once

#include <vector>

#include "Champion.hpp"
#include "Pad.hpp"

class ChampionManager {
private:
    PAD(0x18);
public:
    std::vector<Champion*> champions;
};
