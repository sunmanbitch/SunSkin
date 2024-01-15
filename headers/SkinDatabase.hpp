#pragma once

#include <cstdint>
#include <map>
#include <utility>
#include <vector>
#include <compare>
#include <unordered_map>
#include <unordered_set>

#include "fnv_hash.hpp"

class SkinDatabase {
public:
    class skin_info {
    public:
        const char* model_name;
        std::string skin_name;
        std::int32_t skin_id;
    };

    class jungle_mob_skin_info {
    public:
        std::uint64_t name_hash;
        std::vector<const char*> skins;
    };

    class specialSkin {
    public:
        std::int32_t skinIdStart;
        std::int32_t skinIdEnd;
        std::vector<const char*> gears;
    };

    void load() noexcept;


    std::vector<const char*> minions_skins{
        "Minion", "Summer Minion",
        "Project Minion", "Snowdown Minion",
        "Draven Minion", "Star Guardian Minion",
        "Arcade Minion", "Snowdown 2 Minion",
        "Odyssey Minion", "Mouse Minion", "Arcane Minion"
    };

    std::vector<const char*> turret_skins{
        "Default Order Turret", "Default Chaos Turret",
        "Snow Order Turret", "Snow Chaos Turret",
        "Twisted Treeline Order Turret", "Twisted Treeline Chaos Turret",
        "URF Order Turret", "URF Chaos Turret",
        "Arcade Turret",
        "Temple of Lily and Lotus Turret",
        "Arcane Order Turret", "Arcane Chaos Turret",
        "Butcher's Bridge Order Turret", "Butcher's Bridge Chaos Turret",
        "Howling Abyss Order Turret", "Howling Abyss Chaos Turret"
    };

    const std::vector<jungle_mob_skin_info> jungle_mobs_skins{
        {
            FNV("Baron"),
            { "Baron", "Snowdown Baron", "Championship Baron", "Lunar Revel Baron", "MSI Baron", "Odyssey Baron", "Championship Birthday Baron", "Ruined King Baron" }
        },
        {
            FNV("Blue"),
            { "Blue", "Dark Blue", "Pool Party Blue", "Ruined King Blue" }
        },
        {
            FNV("Red"),
            { "Red", "Pool Party Red", "Ruined King Red" }
        },
        {
            FNV("Crab"),
            { "Crab", "Halloween Light Scuttle", "Halloween Dark Scuttle", "Ruined King Scuttle" }
        },
        {
            FNV("Krug"),
            { "Krug", "Dark Krug" }
        },
        {
            FNV("Razorbeak"),
            { "Razorbeak", "Chicken Razorbeak" }
        }
    };

    const std::unordered_map<std::uint64_t, specialSkin> specialSkins{
        { FNV("Katarina"), { 29, 36, { "Dagger 1", "Dagger 2", "Dagger 3", "Dagger 4", "Dagger 5", "Dagger 6" } } },
        { FNV("Renekton"), { 26, 32, { "Head off", "Head on", "Fins", "Ultimate" } } },
        { FNV("MissFortune"), { 16, 16, { "Scarlet fair", "Zero hour", "Royal arms", "Starswarm" } } },
        { FNV("Ezreal"), { 5, 5, { "Level 1", "Level 2", "Level 3" } } }
    };

    const std::unordered_set<std::uint64_t> minionNames{
        FNV("SRU_OrderMinionRanged"), FNV("SRU_OrderMinionMelee"), FNV("SRU_OrderMinionSiege"), FNV("SRU_OrderMinionSuper"),
        FNV("SRU_ChaosMinionRanged"), FNV("SRU_ChaosMinionMelee"), FNV("SRU_ChaosMinionSiege"), FNV("SRU_ChaosMinionSuper")
    };
    const std::unordered_set<std::uint64_t> plants{ FNV("SRU_Plant_Health"), FNV("SRU_Plant_Satchel"), FNV("SRU_Plant_Vision"), FNV("SRU_PlantRespawnMarker") };
    const std::unordered_set<std::uint64_t> vision{ FNV("JammerDevice"), FNV("YellowTrinket"), FNV("BlueTrinket"), FNV("SightWard"), FNV("VisionWard") };
    const std::unordered_map<std::uint64_t, std::uint64_t> wild{
        { FNV("SRU_Red"), FNV("Red") },
        { FNV("SRU_Blue"), FNV("Blue") },
        { FNV("SRU_Razorbeak"), FNV("Razorbeak"), },
        { FNV("SRU_RazorbeakMini"), FNV("Razorbeak") },
        { FNV("SRU_Krug"), FNV("Krug"), },
        { FNV("SRU_KrugMini"), FNV("Krug"), },
        { FNV("SRU_KrugMiniMini"), FNV("Krug") },
        { FNV("SRU_Murkwolf"), FNV("Murkwolf"), },
        { FNV("SRU_MurkwolfMini"), FNV("Murkwolf") },
        { FNV("SRU_Gromp"), FNV("Gromp") },
        { FNV("Sru_Crab"), FNV("Crab") },
        { FNV("SRU_Baron"), FNV("Baron") }
    };
    const std::unordered_map<std::uint64_t, std::uint64_t> targetMap{
        { FNV("NunuSnowball"), FNV("Nunu") },
        { FNV("KindredWolf"), FNV("Kindred") },
        { FNV("QuinnValor"), FNV("Quinn") },
    };
    const std::uint64_t testCube{ FNV("TestCubeRender10Vision") };
    const std::unordered_set<std::uint64_t> dragon{
        FNV("SRU_Dragon_Air"),
        FNV("SRU_Dragon_Earth"),
        FNV("SRU_Dragon_Fire"),
        FNV("SRU_Dragon_Water"),
        FNV("SRU_Dragon_Hextech"),
        FNV("SRU_Dragon_Chemtech"),
        FNV("SRU_Dragon_Elder")
    };
    const std::unordered_set<std::uint64_t> other{
        FNV("SRU_PlantRespawnMarker"),
        FNV("SRU_CampRespawnMarker"),
        FNV("SRU_RiftHerald"),
        FNV("SRU_Jungle_Companions"),
        FNV("PreSeason_Turret_Shield"),
        FNV("DominationScout")
    };

    std::unordered_map<std::string, std::uint64_t> heroHash;
    std::unordered_map<std::uint64_t, std::vector<skin_info>> champions_skins;
    std::vector<const char*> wards_skins;

private:
    void loadHeroHash() noexcept;
    void loadChampionsSkins() noexcept;
    void loadWardsSkins() noexcept;

};
