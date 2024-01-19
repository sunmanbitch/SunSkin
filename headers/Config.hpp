#pragma once

#include <cstdint>
#include <filesystem>
#include <map>

#include <nlohmann/json.hpp>

#include "KeyBind.hpp"

using json = nlohmann::json;

class Config {
private:
    struct Pos
    {
        float x;
        float y;
    };
public:
    Config() noexcept;
    void load() noexcept;
    void save() noexcept;

    KeyBind menuKey{ KeyBind(ImGuiKey_Insert) };
    KeyBind nextSkinKey{ KeyBind(ImGuiKey_PageUp) };
    KeyBind previousSkinKey{ KeyBind(ImGuiKey_PageDown) };
    bool rainbowText{ false };
    bool defaultDisplay{ true };
    bool noSkin{ false };
    float fontScale{ 1.0f };
    bool heroName{ true };
    bool quickSkinChange{ false };

    // window position
    Pos window_position{ 0.f, 0.f };

    // player
    std::int32_t current_combo_skin_index{ 0 };

    // minion
    std::int32_t current_combo_minion_index{ 0 };

    // ward
    std::int32_t current_ward_skin_id{ 0 };

    // turrets, don't save them in config
    std::int32_t current_combo_order_turret_index{ 0 };
    std::int32_t current_combo_chaos_turret_index{ 0 };

    // other champions
    std::map<std::uint64_t, std::int32_t> current_combo_ally_skin_index;
    std::map<std::uint64_t, std::int32_t> current_combo_enemy_skin_index;

    // jungle mobs
    std::map<std::uint64_t, std::int32_t> current_combo_jungle_mob_skin_index;
private:
    std::filesystem::path path;
    json config_json{ json() };
};
