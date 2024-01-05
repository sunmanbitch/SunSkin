#include <algorithm>
#include <cstdint>
#include <map>
#include <ranges>
#include <string>
#include <utility>

#include "CheatManager.hpp"
#include "SkinDatabase.hpp"
#include "fnv_hash.hpp"

void SkinDatabase::load() noexcept
{
    this->loadHeroHash();
    this->loadHeroSkinId();

    this->loadChampionsSkins();
    this->loadHeroSkinIndex();

    this->loadWardsSkins();
}

void SkinDatabase::loadChampionsSkins() noexcept
{
    const auto& cheatManager{ CheatManager::getInstance() };

    for (const auto& champion : cheatManager.memory->championManager->champions) {
        const auto champ_name{ champion->champion_name.str };
        const auto champ_name_hash{ fnv::hash_runtime(champ_name) };

        if (this->heroSkinIds.find(champ_name_hash) == this->heroSkinIds.end())
            continue;

        std::map<std::string, std::int32_t> temp_skin_list;
        for (auto x{ 0u }; x < champion->skins.size; ++x) {
            const auto skin_id{ champion->skins.list[x].skin_id };
            const auto skin_display_name{ std::string("game_character_skin_displayname_") + champ_name + "_" + std::to_string(skin_id) };
            auto skin_display_name_translated{ skin_id > 0 ? std::string(cheatManager.memory->translateString(skin_display_name.c_str())) : std::string(champ_name) };

            if (skin_display_name_translated == skin_display_name)
                continue;

            if (const auto it{ temp_skin_list.find(skin_display_name_translated) }; it == temp_skin_list.end())
                temp_skin_list[skin_display_name_translated] = 1;
            else
            {
                skin_display_name_translated.append(" Chroma " + std::to_string(it->second));
                it->second = it->second + 1;
            }

            this->champions_skins[champ_name_hash].emplace_back(champ_name, skin_display_name_translated, skin_id);
            if (skin_id == 7 && champ_name_hash == FNV("Lux"))
            {
                this->champions_skins[FNV("Lux")].emplace_back("LuxAir", skin_display_name_translated + " " + "Air", skin_id);
                this->champions_skins[FNV("Lux")].emplace_back("LuxDark", skin_display_name_translated + " " + "Dark", skin_id);
                this->champions_skins[FNV("Lux")].emplace_back("LuxFire", skin_display_name_translated + " " + "Fire", skin_id);
                this->champions_skins[FNV("Lux")].emplace_back("LuxIce", skin_display_name_translated + " " + "Ice", skin_id);
                this->champions_skins[FNV("Lux")].emplace_back("LuxMagma", skin_display_name_translated + " " + "Magma", skin_id);
                this->champions_skins[FNV("Lux")].emplace_back("LuxMystic", skin_display_name_translated + " " + "Mystic", skin_id);
                this->champions_skins[FNV("Lux")].emplace_back("LuxNature", skin_display_name_translated + " " + "Nature", skin_id);
                this->champions_skins[FNV("Lux")].emplace_back("LuxStorm", skin_display_name_translated + " " + "Storm", skin_id);
                this->champions_skins[FNV("Lux")].emplace_back("LuxWater", skin_display_name_translated + " " + "Water", skin_id);

            }
            else if (skin_id == 6 && champ_name_hash == FNV("Sona"))
            {
                this->champions_skins[FNV("Sona")].emplace_back("SonaDJGenre02", skin_display_name_translated + " " + "2", skin_id);
                this->champions_skins[FNV("Sona")].emplace_back("SonaDJGenre03", skin_display_name_translated + " " + "3", skin_id);
            }
        }
    }

}

void SkinDatabase::loadHeroHash() noexcept
{
    const auto& cheatManager{ CheatManager::getInstance() };

    for (auto i{ 0u }; i < cheatManager.memory->heroList->length; ++i)
    {
        const auto& hero{ cheatManager.memory->heroList->list[i] };
        const auto& heroName{ hero->get_character_data_stack()->base_skin.model.str };
        const auto& heroNameHash{ fnv::hash_runtime(heroName) };
        this->heroHash[heroName] = heroNameHash;
    }
}

void SkinDatabase::loadHeroSkinId() noexcept
{
    const auto& cheatManager{ CheatManager::getInstance() };

    for (auto i{ 0u }; i < cheatManager.memory->heroList->length; ++i)
    {
        const auto& hero{ cheatManager.memory->heroList->list[i] };
        const auto& heroNameHash{ this->heroHash[hero->get_character_data_stack()->base_skin.model.str] };
        if (heroNameHash == FNV("PracticeTool_TargetDummy")) { continue; }
        const auto& heroSkinId{ hero->get_character_data_stack()->base_skin.skin };
        this->heroSkinIds[heroNameHash] = heroSkinId;
    }
}

void SkinDatabase::loadWardsSkins() noexcept
{
    const auto& cheatManager{ CheatManager::getInstance() };
    const auto& ward_pre{ "game_character_skin_displayname_SightWard_" };

    this->wards_skins.push_back("default");

    for (auto ward_skin_id{ 1u };; ++ward_skin_id) {
        const auto ward_display_name{ ward_pre + std::to_string(ward_skin_id) };
        const auto ward_display_name_translated{ cheatManager.memory->translateString(ward_display_name.c_str()) };

        if (ward_display_name == ward_display_name_translated)
            break;

        this->wards_skins.push_back(ward_display_name_translated);
    }

}

void SkinDatabase::loadHeroSkinIndex() noexcept
{
    for (const auto& [champ_name_hash, skin_list] : this->champions_skins)
        for (auto i{ 0u }; i < skin_list.size(); ++i)
            if (skin_list[i].skin_id == this->heroSkinIds[champ_name_hash])
                this->heroSkinIndex[champ_name_hash] = i;
}
