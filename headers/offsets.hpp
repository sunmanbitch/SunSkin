#pragma once

#include <cstdint>

namespace offsets {

    namespace global {
        inline std::uint64_t Player{ 0 };
        inline std::uint64_t ChampionManager{ 0 };
        inline std::uint64_t Riot__g_window{ 0 };
        inline std::uint64_t ManagerTemplate_AIMinionClient_{ 0 };
        inline std::uint64_t ManagerTemplate_AIHero_{ 0 };
        inline std::uint64_t ManagerTemplate_AITurret_{ 0 };
        inline std::uint64_t GameClient{ 0 };
        inline std::uint64_t ViewProjMatrix{ 0 };
    };

    namespace ViewProjMatrix {
        inline std::uint64_t Renderer{ 0 };
    }

    namespace AIBaseCommon {
        inline std::uint64_t CharacterDataStack{ 0 };
        inline std::uint64_t SkinId{ 0 };
        inline std::uint64_t Position{ 0 };
    };

    namespace MaterialRegistry {
        inline std::uint64_t D3DDevice{ 0 };
        inline std::uint64_t SwapChain{ 0 };
    };

    namespace functions {
        inline std::uint64_t Riot__Renderer__MaterialRegistry__GetSingletonPtr{ 0 };
        inline std::uint64_t translateString_UNSAFE_DONOTUSE{ 0 };
        inline std::uint64_t CharacterDataStack__Push{ 0 };
        inline std::uint64_t CharacterDataStack__Update{ 0 };
        inline std::uint64_t GetGoldRedirectTarget{ 0 };
        inline std::uint64_t WorldToScreen{ 0 };
    };
};
