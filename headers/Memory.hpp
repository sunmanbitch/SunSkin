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

enum class Approach : std::int8_t {
    only_read_x,
    mov_s_x,
    call_x,
    origin,
    enum_end,
    enum_start = only_read_x
};

class offset_signature {
public:
    std::string pattern;
    Approach approach;
    std::int32_t additional;
    std::uintptr_t* offset;
};

class Memory {
public:
    void Search(bool gameClient = true);
    inline bool checkRunning() noexcept
    {
        return (this->client && this->client->game_state == GGameState_s::Running);
    }

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

private:
    using translateString_t = const char* (__fastcall*)(const char*);
    using characterDataStackUpdate_t = const void(__fastcall*)(const CharacterDataStack*, bool);
    using characterDataStackPush_t = std::uintptr_t(__fastcall*)(const CharacterDataStack*, const char*, std::int32_t, std::int32_t, bool, bool, bool, bool, bool, bool, std::int8_t, const char*, std::int32_t, const char*, std::int32_t, bool, std::int32_t);
    using getGoldRedirectTarget_t = const AIBaseCommon* (__fastcall*)(const AIMinionClient*);
    using WorldToScreen_t = const bool(__fastcall*)(const Renderer*, const Position*, Position*);
    using materialRegistry_t = std::uintptr_t(__fastcall*)();

public:
    translateString_t translateString;
    characterDataStackUpdate_t characterDataStackUpdate;
    // push(self, model, skin, 0, false, false, false, false, true, false, -1, "\x00", 0, "\x00", 0, false, 1);
    characterDataStackPush_t characterDataStackPush;
    getGoldRedirectTarget_t getGoldRedirectTarget;
    WorldToScreen_t WorldToScreen;
private:
    void update(bool gameClient = true) noexcept;

    const std::vector<offset_signature> gameClientSig
    {
        {
            "48 8B 05 ? ? ? ? 4C 8B FA 83 78 0C 02",
            Approach::mov_s_x, 0, &offsets::global::GameClient
        }
    };

    const std::vector<offset_signature> sigs
    {
        {
            "48 8B 3D ? ? ? ? 48 3B CF",
            Approach::mov_s_x, 0, &offsets::global::Player
        },
        {
            "48 8B 05 ? ? ? ? 48 8B 78 08 8B 40 10 4C 8D 3C C7 49 3B FF",
            Approach::mov_s_x, 0, &offsets::global::ManagerTemplate_AIHero_
        },
        {
            "48 8B 0D ? ? ? ? 48 69 D0 ? ? 00 00 48 8B 05",
            Approach::mov_s_x, 0, &offsets::global::ChampionManager
        },
        {
            "48 8B 0D ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? 48 8B C8",
            Approach::mov_s_x, 0, &offsets::global::ManagerTemplate_AIMinionClient_
        },
        {
            "48 8B 1D ? ? ? ? 48 8B 5B 28",
            Approach::mov_s_x, 0, &offsets::global::ManagerTemplate_AITurret_
        },
        {
            "48 8B 0D ? ? ? ? FF 15 ? ? ? ? 48 8B 05 ? ? ? ?",
            Approach::mov_s_x, 0, &offsets::global::Riot__g_window
        },
        {
            "48 8B 1D ? ? ? ? 49 8D 95 ? ? ? ? 4C 8D",
            Approach::mov_s_x, 0, &offsets::global::ViewProjMatrix
        },
        {
            "48 8D 8B ? ? ? ? E8 ?? ?? ?? ?? 49 8D 97 ?? ?? ?? ?? 4C 8D 45 D8",
            Approach::only_read_x, 0, &offsets::ViewProjMatrix::Renderer
        },
        {
            "48 8D 8B ? ? ? ? 48 89 44 24 ? C7 44 24",
            Approach::only_read_x, 0, &offsets::AIBaseCommon::CharacterDataStack
        },
        {
            "40 38 BB ? ? 00 00 0F 85 ? ? 00 00 66 C7 83 ? ? ? ? ? ? 0F 31 48 C1 E2 20 4C 8D 83 ? ? 00 00 48 0B C2 44 8B CF 48 89 44 24 ? 8B D7 48 63 C2",
            Approach::only_read_x, 0, &offsets::AIBaseCommon::SkinId
        },
        {
            "48 8D 8D ? ? ? ? E8 ? ? ? ? 48 8B 54 24 78",
            Approach::only_read_x, 0, &offsets::AIBaseCommon::Position
        },
        {
            "48 8B 8F ? ? 00 00 45 33 C0 8B D3 48 8B 01 FF 90 ? ? 00 00",
            Approach::only_read_x, 0, &offsets::MaterialRegistry::D3DDevice
        },
        {
            "48 8D BB ? ? ? ? C6 83 ? ? ? ? ? 0F 84",
            Approach::only_read_x, 0, &offsets::MaterialRegistry::SwapChain
        },
        {
            "48 89 5C 24 08 48 89 74 24 10 48 89 7C 24 18 55 41 56 41 57 48 8D 6C 24 F9",
            Approach::origin, 0, &offsets::functions::CharacterDataStack__Push
        },
        {
            "88 54 24 10 53 55 56 57 41 54 41 55 41 56 41",
            Approach::origin, 0, &offsets::functions::CharacterDataStack__Update
        },
        {
            "E8 ? ? ? ? 8B 57 44",
            Approach::call_x, 0, &offsets::functions::Riot__Renderer__MaterialRegistry__GetSingletonPtr
        },
        {
            "E8 ? ? ? ? 0F 57 DB 4C 8B C0 F3 0F 5A DE",
            Approach::call_x, 0, &offsets::functions::translateString_UNSAFE_DONOTUSE
        },
        {
            "E8 ? ? ? ? 4C 3B F8 0F 94 C0",
            Approach::call_x, 0, &offsets::functions::GetGoldRedirectTarget
        },
        {
            "E8 ?? ?? ?? ?? 49 8D 97 ?? ?? ?? ?? 4C 8D 45 D8",
            Approach::call_x, 0, &offsets::functions::WorldToScreen
        }
    };

};
