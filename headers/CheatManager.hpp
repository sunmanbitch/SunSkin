#pragma once

#include <memory>

#include "Config.hpp"
#include "Hooks.hpp"
#include "Logger.hpp"
#include "Memory.hpp"
#include "SkinDatabase.hpp"
#include "Holdon.hpp"

class CheatManager {
public:
    static CheatManager& getInstance() noexcept;

    bool cheatState{ true };
    std::unique_ptr<Hooks> hooks{ std::make_unique<Hooks>() };
    std::unique_ptr<Config> config{ std::make_unique<Config>() };
    std::unique_ptr<Memory> memory{ std::make_unique<Memory>() };
    std::unique_ptr<SkinDatabase> database{ std::make_unique<SkinDatabase>() };
    std::unique_ptr<ImGuiAppLog> logger{ std::make_unique<ImGuiAppLog>() };
    std::unique_ptr<Holdon> holdon{ std::make_unique<Holdon>() };

private:
    CheatManager() = default;
};
