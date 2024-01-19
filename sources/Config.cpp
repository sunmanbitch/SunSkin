#include <fstream>
#include <string>
#include <system_error>
#include <ranges>

#include <Windows.h>
#include <ShlObj.h>

#include "Config.hpp"
#include "CheatManager.hpp"
#include "KeyBind.hpp"

Config::Config() noexcept
{
    if (PWSTR pathToDocuments; SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &pathToDocuments))) {
        this->path = pathToDocuments;
        CoTaskMemFree(pathToDocuments);
    }

    this->path /= "SunSkin";
}

void Config::load() noexcept
{
    const auto& cheatManager{ CheatManager::getInstance() };
    const auto& player{ cheatManager.memory->localPlayer };
    auto in{ std::ifstream(this->path / u8"SunSkin64") };

    if (in.good())
    {
        if (json j{ json::parse(in, nullptr, false, true) }; !j.is_discarded())
            config_json = j;
        in.close();
    }
    else
    {
        config_json = {
            {"heroName", true}
        };
    }

    if (player)
    {
        const auto& player_name{ player->get_character_data_stack()->base_skin.model.str };
        const auto& player_name_hash{ cheatManager.database->heroHash.at(player_name) };
        const auto& current_combo_skin_index_key = std::to_string(player_name_hash) + ".current_combo_skin_index";
        const auto& skins{ cheatManager.database->champions_skins.at(player_name_hash) };

        const auto& skinID{ player->get_character_data_stack()->base_skin.skin };
        const auto& it{ std::ranges::find_if(skins, [&skinID](const auto& skin) { return skin.skin_id == skinID; }) };
        this->current_combo_skin_index = config_json.value(current_combo_skin_index_key, it != skins.end() ? std::distance(skins.begin(), it) : 0);
    }

    this->window_position = { config_json.value("window_position_x", 0.f), config_json.value("window_position_y", 0.f) };

    this->menuKey = KeyBind(config_json.value("menuKey", ImGuiKey_Insert));
    this->nextSkinKey = KeyBind(config_json.value("nextSkinKey", ImGuiKey_PageUp));
    this->previousSkinKey = KeyBind(config_json.value("previousSkinKey", ImGuiKey_PageDown));
    this->heroName = config_json.value("heroName", true);
    this->rainbowText = config_json.value("raibowText", false);
    this->defaultDisplay = config_json.value("defaultDisplay", true);
    this->noSkin = config_json.value("noSkin", false);
    this->quickSkinChange = config_json.value("quickSkinChange", false);
    this->fontScale = config_json.value("fontScale", 1.0f);
    this->current_ward_skin_id = config_json.value("current_ward_skin_id", 0);

    const auto& ally_skins_object{ this->config_json.find("current_combo_ally_skin_index") };
    const auto& enemy_skins_object{ this->config_json.find("current_combo_enemy_skin_index") };
    const auto& my_team{ player ? player->team : 100 };

    for (const auto& hero : arr2vec(AIHero, cheatManager.memory->heroList))
    {
        if (player == hero) continue;

        const auto& heroHash{ cheatManager.database->heroHash[hero->get_character_data_stack()->base_skin.model.str] };

        const auto& skins{ cheatManager.database->champions_skins[heroHash] };
        const auto& skinID{ hero->get_character_data_stack()->base_skin.skin };
        const auto& it{ std::ranges::find_if(skins, [&skinID](const auto& skin) { return skin.skin_id == skinID; }) };
        const auto& heroSkinIndex{ it != skins.end() ? std::distance(skins.begin(), it) : 0 };

        const auto& skins_object{ hero->team == my_team ? ally_skins_object : enemy_skins_object };
        auto& current_combo_skin_index{ hero->team == my_team ? this->current_combo_ally_skin_index : this->current_combo_enemy_skin_index };

        if (heroSkinIndex != 0 || skins_object == this->config_json.end())
            current_combo_skin_index[heroHash] = heroSkinIndex;
        else if (const auto& it{ skins_object.value().find(std::to_string(heroHash)) }; it == skins_object.value().end())
            current_combo_skin_index[heroHash] = heroSkinIndex;
        else
            current_combo_skin_index[heroHash] = it.value().get<std::int32_t>();
    }

    const auto jungle_mobs_skins{ config_json.find("current_combo_jungle_mob_skin_index") };
    if (jungle_mobs_skins != config_json.end())
        for (const auto& it : jungle_mobs_skins.value().items())
            this->current_combo_jungle_mob_skin_index[std::stoull(it.key())] = it.value().get<std::int32_t>();

}

void Config::save() noexcept
{
    const auto& cheatManager{ CheatManager::getInstance() };
    const auto player{ cheatManager.memory->localPlayer };

    if (player)
        config_json[std::to_string(cheatManager.database->heroHash[player->get_character_data_stack()->base_skin.model.str]) + ".current_combo_skin_index"] = this->current_combo_skin_index;

    config_json["window_position_x"] = this->window_position.x;
    config_json["window_position_y"] = this->window_position.y;
    config_json["menuKey"] = this->menuKey.imGuiKeyCode;
    config_json["nextSkinKey"] = this->nextSkinKey.imGuiKeyCode;
    config_json["previousSkinKey"] = this->previousSkinKey.imGuiKeyCode;
    config_json["heroName"] = this->heroName;
    config_json["raibowText"] = this->rainbowText;
    config_json["defaultDisplay"] = this->defaultDisplay;
    config_json["noSkin"] = this->noSkin;
    config_json["quickSkinChange"] = this->quickSkinChange;
    config_json["fontScale"] = this->fontScale;
    config_json["current_ward_skin_id"] = this->current_ward_skin_id;

    for (const auto& [fst, snd] : this->current_combo_ally_skin_index)
        config_json["current_combo_ally_skin_index"][std::to_string(fst)] = snd;

    for (const auto& [fst, snd] : this->current_combo_enemy_skin_index)
        config_json["current_combo_enemy_skin_index"][std::to_string(fst)] = snd;

    for (const auto& [fst, snd] : this->current_combo_jungle_mob_skin_index)
        config_json["current_combo_jungle_mob_skin_index"][std::to_string(fst)] = snd;

    std::error_code ec;
    std::filesystem::create_directory(this->path, ec);
    auto out{ std::ofstream(this->path / u8"SunSkin64") };

    if (!out.good())
        return;

    out << config_json.dump(4);
    out.close();
}
