#pragma once

#include <Windows.h>
#include <cstdint>
#include <d3d9.h>
#include <d3d11.h>

#include "Offsets.hpp"

#include "AIBaseCommon.hpp"
#include "AIHero.hpp"
#include "AITurret.hpp"
#include "AIMinionClient.hpp"
#include "ChampionManager.hpp"
#include "GameClient.hpp"
#include "ManagerTemplate.hpp"
#include "ViewProjMatrix.hpp"
#include "Renderer.hpp"
#include "Position.hpp"

class offset_signature {
public:
    std::string pattern;
    bool sub_base;
    bool read;
    bool relative;
    std::int32_t additional;
    std::uint64_t* offset;
};

class Memory {
public:
    void Search(bool gameClient = true);
    bool checkRunning() noexcept;

    std::uintptr_t base;
    HWND window;

    GameClient* client;
    AIBaseCommon* localPlayer;
    ManagerTemplate<AIHero>* heroList;
    ManagerTemplate<AIMinionClient>* minionList;
    ManagerTemplate<AITurret>* turretList;
    ChampionManager* championManager;
    ViewProjMatrix* viewProjMatrix;

    std::uintptr_t materialRegistry;
    IDirect3DDevice9* d3dDevice;
    IDXGISwapChain* swapChain;

    using translateString_t = const char* (__fastcall*)(const char*);
    using characterDataStackUpdate_t = const void(__fastcall*)(const CharacterDataStack*, bool);
    using characterDataStackPush_t = const void(__fastcall*)(const CharacterDataStack*, const char*, int, int, bool, bool, bool, bool, bool, bool, int, const char*, int, const char*, int, bool, int);
    using getGoldRedirectTarget_t = const AIBaseCommon* (__fastcall*)(const AIMinionClient*);
    using WorldToScreen_t = const bool(__fastcall*)(const Renderer*, const Position*, Position*);

    translateString_t translateString;
    characterDataStackUpdate_t characterDataStackUpdate;
    // push(self, model, skin, 0, false, false, false, false, true, false, -1, "\x00", 0, "\x00", 0, false, 1);
    characterDataStackPush_t characterDataStackPush;
    getGoldRedirectTarget_t getGoldRedirectTarget;
    WorldToScreen_t WorldToScreen;
private:
    void update(bool gameClient = true) noexcept;

    std::vector<offset_signature> gameClientSig
    {
        {
            "48 8B 05 ? ? ? ? 4C 8B FA 83 78 0C 02",
            true, false, true, 0, &offsets::global::GameClient
        }
    };

    std::vector<offset_signature> sigs
    {
        {
            "48 8B 3D ? ? ? ? 48 3B CF",
            true, false, true, 0, &offsets::global::Player
        },
        {
            "48 8B 05 ? ? ? ? 48 8B 78 08 8B 40 10 4C 8D 3C C7 49 3B FF",
            true, false, true, 0, &offsets::global::ManagerTemplate_AIHero_
        },
        {
            "48 8B 0D ? ? ? ? 48 69 D0 ? ? 00 00 48 8B 05",
            true, false, true, 0, &offsets::global::ChampionManager
        },
        {
            "48 89 0D ? ? ? 02 48 8D 05 ? ? ? 01 33 D2",
            true, false, true, 0, &offsets::global::ManagerTemplate_AIMinionClient_
        },
        {
            "48 89 0D ? ? ? 01 48 8D 05 ? ? ? 01 33 D2",
            true, false, true, 0, &offsets::global::ManagerTemplate_AITurret_
        },
        {
            "48 8B 0D ? ? ? ? FF 15 ? ? ? ? 48 8B 05 ? ? ? ?",
            true, false, true, 0, &offsets::global::Riot__g_window
        },
        {
            "48 8B 1D ? ? ? ? 49 8D 95 ? ? ? ? 4C 8D",
            true, false, true, 0, &offsets::global::ViewProjMatrix
        },
        {
            "48 8D 8B ? ? ? ? E8 ?? ?? ?? ?? 49 8D 97 ?? ?? ?? ?? 4C 8D 45 D8",
            false, true, false, 0, &offsets::ViewProjMatrix::Renderer
        },
        {
            "48 8D 8B ? ? ? ? 48 89 44 24 ? C7 44 24",
            false, true, false, 0, &offsets::AIBaseCommon::CharacterDataStack
        },
        {
            "40 38 BB ? ? ? ? 0F 85 ? ? ? ? 66 C7 83 ? ? ? ? ? ? 0F 31 48 C1 E2 20 4C 8D 83 ? ? ? ? 48 0B C2 44 8B CF 48 89 44 24 ? 8B D7",
            false, true, false, 0, &offsets::AIBaseCommon::SkinId
        },
        {
            "48 8D 8D ? ? ? ? E8 ? ? ? ? 48 8B 54 24 78",
            false, true, false, 0, &offsets::AIBaseCommon::Position
        },
        {
            "48 8B 8F ? ? 00 00 45 33 C0 8B D3 48 8B 01 FF 90 ? ? 00 00",
            false, true, false, 0, &offsets::MaterialRegistry::D3DDevice
        },
        {
            "48 8D BB ? ? ? ? C6 83 ? ? ? ? ? 0F 84",
            false, true, false, 0, &offsets::MaterialRegistry::SwapChain
        },
        {
            "E8 ? ? ? ? 41 8B 97 ? ? ? ? 48 8B 0D ? ? ? ?",
            true, false, false, 0, &offsets::functions::CharacterDataStack__Push
        },
        {
            "88 54 24 10 53 55 56 57 41 54 41 55 41 56 41",
            true, false, false, 0, &offsets::functions::CharacterDataStack__Update
        },
        {
            "E8 ? ? ? ? 8B 57 44",
            true, false, false, 0, &offsets::functions::Riot__Renderer__MaterialRegistry__GetSingletonPtr
        },
        {
            "E8 ? ? ? ? 0F 57 DB 4C 8B C0 F3 0F 5A DE",
            true, false, false, 0, &offsets::functions::translateString_UNSAFE_DONOTUSE
        },
        {
            "E8 ? ? ? ? 4C 3B F8 0F 94 C0",
            true, false, false, 0, &offsets::functions::GetGoldRedirectTarget
        },
        {
            "E8 ?? ?? ?? ?? 49 8D 97 ?? ?? ?? ?? 4C 8D 45 D8",
            true, false, false, 0, &offsets::functions::WorldToScreen
        }
    };

};
