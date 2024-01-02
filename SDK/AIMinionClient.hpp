#pragma once

#include "AIBaseCommon.hpp"

class AIMinionClient : public AIBaseCommon {
public:
    const AIBaseCommon* redirectTarget() noexcept;
};
