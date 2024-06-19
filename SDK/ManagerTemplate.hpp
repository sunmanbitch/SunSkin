#pragma once

#include <cstdint>

#include "RiotArray.hpp"
#include "Pad.hpp"

template <class T>
class ManagerTemplate {
private:
    PAD(0x8);
public:
    RiotArray<T*> riotArray;
};
