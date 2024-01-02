#pragma once

#define ENUS 1

#ifdef ENUS
#include "TENUS.hpp"
#elif ZHCN
#include "TZHCN.hpp"
#else
#include "TENUS.hpp"
#endif