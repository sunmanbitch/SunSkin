#pragma once

#include <memory>

#include "Config.hpp"
#include "GUI.hpp"
#include "Hooks.hpp"
#include "Logger.hpp"
#include "Memory.hpp"
#include "SkinDatabase.hpp"
#include "Holdon.hpp"

class CheatManager {
public:
    static CheatManager& getInstance() noexcept;

    bool cheatState{ true };
    std::unique_ptr<Hooks> hooks;
    std::unique_ptr<Config> config;
    std::unique_ptr<GUI> gui;
    std::unique_ptr<Memory> memory;
    std::unique_ptr<SkinDatabase> database;
    std::unique_ptr<ImGuiAppLog> logger;
    std::unique_ptr<Holdon> holdon;

private:
    CheatManager();
    void start() noexcept;
};
