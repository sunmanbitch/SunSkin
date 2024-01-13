#pragma once

#include <vector>

#include "CharacterStackData.hpp"

class CharacterDataStack {
public:
    std::vector<CharacterStackData> stack;
    CharacterStackData base_skin;
public:
    void update(bool = true) noexcept;
    void push(const char*, const std::int32_t) noexcept;
};
