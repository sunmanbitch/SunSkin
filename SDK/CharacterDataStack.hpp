#pragma once

#include <vector>

#include "CharacterStackData.hpp"

class CharacterDataStack {
public:
    std::vector<CharacterStackData> stack;
    CharacterStackData base_skin;
public:
    void update(bool = true) noexcept;
    void push(const char*, int, int=0, bool = false, bool = false, bool = false, bool = false, bool = true, bool = false, int = -1, const char* = "\x00", int = 0, const char* = "\x00", int = 0, bool = false, int = 1) noexcept;
};
