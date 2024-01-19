#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>
#include <ranges>

#include "GUI.hpp"
#include "CheatManager.hpp"
#include "Translate.hpp"
#include "Utils.hpp"
#include "RandomGenerator.hpp"
#include "AITurret.hpp"
#include "AIHero.hpp"

inline void GUI::footer() noexcept
{
    using namespace std::string_literals;
    static const auto buildText{ "Last Build: "s + __DATE__ + " - " + __TIME__ };
    ImGui::Separator();
    ImGui::textUnformattedCentered(buildText.c_str());
    ImGui::textUnformattedCentered("Copyright (C) 2024-2026 Sun");
}

inline void GUI::changeTurretSkin(const std::int32_t skinId, const std::int32_t team) noexcept
{
    if (skinId == -1)
        return;

    const auto& memory{ CheatManager::getInstance().memory };
    const auto& playerTeam{ memory->localPlayer ? memory->localPlayer->team : 100 };

    for (const auto& turret : arr2vec(AITurret, memory->turretList)) {
        if (turret->team != team)
            continue;

        const auto& dataStack{ turret->get_character_data_stack() };
        if (playerTeam == team)
            dataStack->base_skin.skin = skinId * 2;
        else
            dataStack->base_skin.skin = skinId * 2 + 1;
        dataStack->update();
    }
}

void GUI::render() noexcept
{
    std::call_once(set_font_scale, [&]
        {
            const auto& config{ CheatManager::getInstance().config };
            ImGui::GetIO().FontGlobalScale = config->fontScale;
            const auto& [x, y] = config->window_position;
            if (x != 0.f || y != 0.f) ImGui::SetNextWindowPos({ x, y }, ImGuiCond_FirstUseEver);
            this->is_open = config->defaultDisplay;
        }
    );

    if (!this->is_open)
        return;

    if (!ImGui::Begin(LOGO, nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::End();
        return;
    }
    ImGui::rainbowText();

    if (!ImGui::BeginTabBar("TabBar", ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyScroll | ImGuiTabBarFlags_NoTooltip))
    {
        ImGui::EndTabBar();
        return;
    }
    this->playerTabItem();
    this->heroesTabItem();
    this->globalTabItem();
    this->extrasTabItem();
    // if (ImGui::BeginTabItem("Logger")) {
    //     const auto& cheatManager{ CheatManager::getInstance() };
    //     cheatManager.logger->draw();
    //     ImGui::EndTabItem();
    // }
    ImGui::EndTabBar();

    const auto& [x, y] { ImGui::GetWindowPos() };
    auto& window_position{ CheatManager::getInstance().config->window_position };
    if (window_position.x != x || window_position.y != y) window_position = { x, y };

    ImGui::End();
}

inline void GUI::playerTabItem() noexcept
{
    const auto& cheatManager{ CheatManager::getInstance() };
    const auto& player{ cheatManager.memory->localPlayer };
    if (!player) return;
    if (!ImGui::BeginTabItem(PLAYER)) return;

    ImGui::Text(PLAYERSKINSET);

    const auto& champions_skins{ cheatManager.database->champions_skins };
    const auto& heroHashes{ cheatManager.database->heroHash };
    const auto& heroModelName{ player->get_character_data_stack()->base_skin.model.str };
    const auto& values{ champions_skins.at(heroHashes.at(heroModelName)) };

    if (auto& skinIndex{ cheatManager.config->current_combo_skin_index };ImGui::BeginCombo(CURRSKIN, values[skinIndex].skin_name.c_str()))
    {
        for (auto i{ 0u }; i < values.size(); ++i)
        {
            const auto& isSelected{ i == skinIndex };
            if (ImGui::Selectable(values[i].skin_name.c_str(), isSelected))
            {
                player->change_skin(values[i].model_name, values[i].skin_id);
                skinIndex = i;
            }
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    const auto& playerHash{ heroHashes.at(heroModelName) };
    const auto& skin{ player->get_character_data_stack()->base_skin.skin };
    const auto& it{ cheatManager.database->specialSkins.find(playerHash) };
    const auto& stack{ player->get_character_data_stack() };
    if (it != cheatManager.database->specialSkins.end() && it->second.skinIdStart <= skin && it->second.skinIdEnd >= skin && ImGui::BeginCombo(CURRGEAR, it->second.gears[stack->base_skin.gear]))
    {
        for (auto i{ 0u }; i < it->second.gears.size(); ++i)
        {
            const auto& isSelected{ i == stack->base_skin.gear };
            if (ImGui::Selectable(it->second.gears[i], isSelected))
            {
                stack->base_skin.gear = i;
                stack->update();
            }
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::Separator();

    if (auto& skinIndex{ cheatManager.config->current_ward_skin_id };ImGui::BeginCombo(CURRWARDSKIN, cheatManager.database->wards_skins[skinIndex]))
    {
        for (auto i{ 0u }; i < cheatManager.database->wards_skins.size(); ++i)
        {
            const auto& isSelected{ i == skinIndex };
            if (ImGui::Selectable(cheatManager.database->wards_skins[i], isSelected))
                skinIndex = i;
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    this->footer();
    ImGui::EndTabItem();

}

inline void GUI::heroesTabItem() noexcept
{
    const auto& cheatManager{ CheatManager::getInstance() };

    const auto& heroes{ arr2vec(AIHero, cheatManager.memory->heroList) };
    const auto& findFunc{
        [&cheatManager](const auto& hero)
        {
            return hero != cheatManager.memory->localPlayer && cheatManager.database->heroHash[hero->get_character_data_stack()->base_skin.model.str] != FNV("PracticeTool_TargetDummy");
        }
    };
    const auto& heroSize{ std::ranges::count_if(heroes, findFunc) };

    if (heroSize <= 0) return;

    if (!ImGui::BeginTabItem("Other Champs")) return;

    ImGui::Text("Other Champs Skins Settings:");

    const auto& player{ cheatManager.memory->localPlayer };
    const auto& my_team{ player ? player->team : 100 };
    std::int32_t last_team{ 0 };

    for (const auto& hero : heroes) {
        if (hero == player)
            continue;

        const auto& heroModelName{ hero->get_character_data_stack()->base_skin.model.str };
        const auto& heroHashName{ fnv::hash_runtime(heroModelName) };
        if (heroHashName == FNV("PracticeTool_TargetDummy"))
            continue;

        const auto& is_enemy{ hero->team != my_team };

        if (last_team == 0 || hero->team != last_team) {
            if (last_team != 0)
                ImGui::Separator();
            if (is_enemy)
                ImGui::Text(" Enemy champions");
            else
                ImGui::Text(" Ally champions");
            last_team = hero->team;
        }

        auto& config_array{ is_enemy ? cheatManager.config->current_combo_enemy_skin_index : cheatManager.config->current_combo_ally_skin_index };
        if (cheatManager.config->heroName)
            std::snprintf(this->str_buffer, sizeof(this->str_buffer), "HeroName: [ %s ]##%X", heroModelName, reinterpret_cast<std::uintptr_t>(hero));
        else
            std::snprintf(this->str_buffer, sizeof(this->str_buffer), "PlayerName: [ %s ]##%X", hero->name.c_str(), reinterpret_cast<std::uintptr_t>(hero));
        const auto& values{ cheatManager.database->champions_skins[heroHashName] };

        if (auto& skinIndex{ config_array.at(heroHashName) };ImGui::BeginCombo(this->str_buffer, values[skinIndex].skin_name.c_str()))
        {
            for (auto i{ 0u }; i < values.size(); ++i)
            {
                const auto& isSelected{ i == skinIndex };
                if (ImGui::Selectable(values[i].skin_name.c_str(), isSelected))
                {
                    hero->change_skin(values[i].model_name, values[i].skin_id);
                    skinIndex = i;
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }
    footer();
    ImGui::EndTabItem();
}

inline void GUI::globalTabItem() noexcept
{
    const auto& cheatManager{ CheatManager::getInstance() };

    if (!ImGui::BeginTabItem("Global Skins"))
        return;

    ImGui::Text("Global Skins Settings:");
    if (auto& skinIndex{ cheatManager.config->current_combo_minion_index };ImGui::BeginCombo("Minion Skins:", cheatManager.database->minions_skins[skinIndex]))
    {
        for (auto i{ 0u }; i < cheatManager.database->minions_skins.size(); ++i)
        {
            const auto& isSelected{ i == skinIndex };
            if (ImGui::Selectable(cheatManager.database->minions_skins[i], isSelected))
                skinIndex = i;
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::Separator();

    if (auto& skinIndex{ cheatManager.config->current_combo_order_turret_index };ImGui::BeginCombo("Order Turret Skins:", cheatManager.database->turret_skins[skinIndex]))
    {
        for (auto i{ 0u }; i < cheatManager.database->turret_skins.size(); ++i)
        {
            const auto& isSelected{ i == skinIndex };
            if (ImGui::Selectable(cheatManager.database->turret_skins[i], isSelected))
            {
                this->changeTurretSkin(i, 100);
                skinIndex = i;
            }
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    if (auto& skinIndex{ cheatManager.config->current_combo_chaos_turret_index };ImGui::BeginCombo("Chaos Turret Skins:", cheatManager.database->turret_skins[skinIndex]))
    {
        for (auto i{ 0u }; i < cheatManager.database->turret_skins.size(); ++i)
        {
            const auto& isSelected{ i == skinIndex };
            if (ImGui::Selectable(cheatManager.database->turret_skins[i], isSelected))
            {
                this->changeTurretSkin(i, 200);
                skinIndex = i;
            }
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::Separator();

    ImGui::Text("Jungle Mobs Skins Settings:");
    for (const auto& [name_hash, skins] : cheatManager.database->jungle_mobs_skins)
    {
        std::snprintf(this->str_buffer, 256, "Current %s skin", skins[0]);
        const auto& [ele, _] { cheatManager.config->current_combo_jungle_mob_skin_index.insert({ name_hash, 0 }) };
        if (auto& skinIndex{ ele->second };ImGui::BeginCombo(this->str_buffer, skins[skinIndex]))
        {
            for (auto i{ 0u }; i < skins.size(); ++i)
            {
                const auto& isSelected{ i == skinIndex };
                if (ImGui::Selectable(skins[i], isSelected))
                    skinIndex = i;
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }

    footer();
    ImGui::EndTabItem();
}

inline void GUI::extrasTabItem() noexcept
{
    const auto& cheatManager{ CheatManager::getInstance() };
    const auto& player{ cheatManager.memory->localPlayer };
    if (!ImGui::BeginTabItem("Extras")) return;

    ImGui::ButtonEnableBind("Menu Key", cheatManager.config->menuKey, "Hovering the mouse over this button allows you to press other keys to rebind it.");

    ImGui::Checkbox(cheatManager.config->heroName ? "HeroName based" : "PlayerName based", &cheatManager.config->heroName);
    ImGui::Checkbox("Rainbow Text", &cheatManager.config->rainbowText);
    ImGui::Checkbox("Default Display", &cheatManager.config->defaultDisplay);
    ImGui::hoverInfo("Effective next time.");
    ImGui::Checkbox("Default No Skin", &cheatManager.config->noSkin);
    ImGui::hoverInfo("Effective next time.");
    ImGui::Checkbox("Quick Skin Change", &cheatManager.config->quickSkinChange);
    ImGui::hoverInfo("It allows you to change skin without opening the menu with the key you assign from the keyboard.");

    if (cheatManager.config->quickSkinChange)
    {
        ImGui::Separator();
        ImGui::ButtonEnableBind("Previous Skin Key", cheatManager.config->previousSkinKey, "Hovering the mouse over this button allows you to press other keys to rebind it.");
        ImGui::ButtonEnableBind("Next Skin Key", cheatManager.config->nextSkinKey, "Hovering the mouse over this button allows you to press other keys to rebind it.");
        ImGui::Separator();
    }

    if (player)
        ImGui::InputText("Change Nick", &player->name);

    if (ImGui::Button("No skins except local player"))
    {
        for (auto& val : cheatManager.config->current_combo_enemy_skin_index | std::views::values)
            val = 0;

        for (auto& val : cheatManager.config->current_combo_ally_skin_index | std::views::values)
            val = 0;

        for (const auto& hero : arr2vec(AIHero, cheatManager.memory->heroList))
        {
            if (hero == player)
                continue;
            hero->change_skin(hero->get_character_data_stack()->base_skin.model.str, 0);
        }
    } ImGui::hoverInfo("Sets the skins of all champions except the local player to the default skin.");

    if (ImGui::Button("Random Skins")) {
        for (const auto& hero : arr2vec(AIHero, cheatManager.memory->heroList))
        {
            const auto championHash{ fnv::hash_runtime(hero->get_character_data_stack()->base_skin.model.str) };

            if (championHash == FNV("PracticeTool_TargetDummy"))
                continue;

            const auto& skinCount{ cheatManager.database->champions_skins[championHash].size() };
            const auto& skinDatabase{ cheatManager.database->champions_skins[championHash] };

            if (hero == player) {
                auto& skinIndex{ cheatManager.config->current_combo_skin_index };
                skinIndex = random(1ull, skinCount - 1);
                hero->change_skin(skinDatabase[skinIndex].model_name, skinDatabase[skinIndex].skin_id);
            }
            else {
                const auto& my_team{ player ? player->team : 100 };
                auto& config{ hero->team != my_team ? cheatManager.config->current_combo_enemy_skin_index : cheatManager.config->current_combo_ally_skin_index };
                auto& data{ config[championHash] };
                data = random(1ull, skinCount - 1);
                hero->change_skin(skinDatabase[data].model_name, skinDatabase[data].skin_id);
            }
        }
    } ImGui::hoverInfo("Randomly changes the skin of all champions.");

    ImGui::SliderFloat("Font Scale", &cheatManager.config->fontScale, 1.0f, 2.0f, "%.3f");
    if (ImGui::GetIO().FontGlobalScale != cheatManager.config->fontScale)
    {
        ImGui::GetIO().FontGlobalScale = cheatManager.config->fontScale;
    } ImGui::hoverInfo("Changes the menu font scale.");

    if (ImGui::Button("Force Close"))
        cheatManager.hooks->uninstall();
    ImGui::hoverInfo("You will be returned to the reconnect screen.");
    ImGui::Text("FPS: %.0f FPS", ImGui::GetIO().Framerate);
    footer();
    ImGui::EndTabItem();
}
