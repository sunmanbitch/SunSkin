#pragma once

#include <cstdint>
#include <string>

#include "Pad.hpp"
#include "offsets.hpp"

class GameObject {
public:
    union {
        DEFINE_MEMBER_N(std::int32_t team, offsets::GameObject::Team);
        DEFINE_MEMBER_N(std::string name, offsets::GameObject::Name);
    };
};
