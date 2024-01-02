#pragma once

#include "GameState.hpp"
#include "Pad.hpp"

class GameClient {
private:
    PAD(0xC);
public:
    GGameState_s game_state;
};
