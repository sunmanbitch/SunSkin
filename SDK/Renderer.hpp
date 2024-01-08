#pragma once

#include "Position.hpp"

class Renderer {
public:
    bool wroldToScreen(Position*, Position*) noexcept;
};
