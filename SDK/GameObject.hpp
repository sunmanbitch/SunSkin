#pragma once

#include <cstdint>
#include <string>

#include "Pad.hpp"

class GameObject {
public:
	CLASS_GETTER(std::int32_t, get_team, 0x3C)
	CLASS_GETTER_P(std::string, get_name, 0x60)
};
