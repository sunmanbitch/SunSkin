#pragma once

#include <cstdint>

#include "AString.hpp"
#include "Pad.hpp"

class Skin {
public:
    std::int32_t skin_id;
private:
    PAD(0x4);
public:
    AString skin_name;
};
