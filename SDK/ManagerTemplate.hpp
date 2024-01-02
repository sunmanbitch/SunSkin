#pragma once

#include <cstdint>

#include "Pad.hpp"

template <class T>
class ManagerTemplate {
private:
    PAD(0x8);
public:
    T** list;
    std::int32_t length;
    std::int32_t capacity;
};
