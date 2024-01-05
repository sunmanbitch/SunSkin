#pragma once

#include "AIBaseCommon.hpp"

class AIMinionClient : public AIBaseCommon {
public:
    const AIBaseCommon* redirectTarget() noexcept;
    std::uint64_t getModelHash() noexcept;
    bool isMinion() noexcept;
    bool isVision() noexcept;
    bool isTestCube() noexcept;
    bool isJungle() noexcept;
    bool isOther() noexcept;
};
