#pragma warning(disable: 28182 6011)

#include <Windows.h>
#include <chrono>
#include <thread>

#include "Memory.hpp"

Memory::bytes_t Memory::pattern_to_byte(const char* pattern) noexcept
{
    bytes_t bytes{};
    bytes.reserve(strlen(pattern) / 2u);
    const auto start{ const_cast<char*>(pattern) };
    const auto end{ const_cast<char*>(pattern) + strlen(pattern) };

    for (auto current{ start }; current < end; ++current) {
        if (*current == '?')
        {
            ++current;
            if (*current == '?') ++current;
            bytes.push_back(-1);
        }
        else
        {
            bytes.push_back(strtoul(current, &current, 16));
        }
    }
    return bytes;
};

std::uint8_t* Memory::find_signature(const wchar_t* szModule, const char* szSignature) noexcept
{
    const auto module{ ::GetModuleHandleW(szModule) };

    const auto dosHeader{ reinterpret_cast<PIMAGE_DOS_HEADER>(module) };
    const auto ntHeaders{ reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(module) + dosHeader->e_lfanew) };
    const auto textSection{ IMAGE_FIRST_SECTION(ntHeaders) };

    const auto sizeOfImage{ textSection->SizeOfRawData };
    const auto patternBytes{ pattern_to_byte(szSignature) };
    const auto scanBytes{ reinterpret_cast<std::uint8_t*>(module) + textSection->VirtualAddress };

    const auto s{ patternBytes.size() };
    const auto d{ patternBytes.data() };

    MEMORY_BASIC_INFORMATION mbi{ nullptr };
    const std::uint8_t* next_check_address{ nullptr };

    for (auto i{ 0ul }; i < sizeOfImage - s; ++i) {
        bool found{ true };
        for (auto j{ 0ul }; j < s; ++j) {
            const auto current_address{ scanBytes + i + j };
            if (current_address >= next_check_address) {
                if (!::VirtualQuery(current_address, &mbi, sizeof(mbi)))
                    break;

                if (mbi.Protect == PAGE_NOACCESS) {
                    i += reinterpret_cast<std::uintptr_t>(mbi.BaseAddress) + mbi.RegionSize - (reinterpret_cast<std::uintptr_t>(scanBytes) + i);
                    i--;
                    found = false;
                    break;
                }
                else {
                    next_check_address = static_cast<std::uint8_t*>(mbi.BaseAddress) + mbi.RegionSize;
                }
            }

            if (scanBytes[i + j] != d[j] && d[j] != -1) {
                found = false;
                break;
            }
        }

        if (found)
            return &scanBytes[i];
    }

    return nullptr;

}

void Memory::update(bool gameClient) noexcept
{
    if (gameClient) {
        this->client = *reinterpret_cast<GameClient**>(offsets::global::GameClient);
    }
    else {
        this->localPlayer = *reinterpret_cast<AIBaseCommon**>(offsets::global::Player);
        this->heroList = *reinterpret_cast<ManagerTemplate<AIHero>**>(offsets::global::ManagerTemplate_AIHero_);
        this->minionList = *reinterpret_cast<ManagerTemplate<AIMinionClient>**>(offsets::global::ManagerTemplate_AIMinionClient_);
        this->turretList = *reinterpret_cast<ManagerTemplate<AITurret>**>(offsets::global::ManagerTemplate_AITurret_);
        this->championManager = *reinterpret_cast<ChampionManager**>(offsets::global::ChampionManager);
        this->viewProjMatrix = *reinterpret_cast<ViewProjMatrix**>(offsets::global::ViewProjMatrix);
        this->materialRegistry = reinterpret_cast<materialRegistry_t>(offsets::functions::Riot__Renderer__MaterialRegistry__GetSingletonPtr)();
        this->swapChain = *reinterpret_cast<IDXGISwapChain**>(this->materialRegistry + offsets::MaterialRegistry::SwapChain);
        this->window = *reinterpret_cast<HWND*>(offsets::global::Riot__g_window);
        this->translateString = reinterpret_cast<translateString_t>(offsets::functions::translateString_UNSAFE_DONOTUSE);
        this->characterDataStackUpdate = reinterpret_cast<characterDataStackUpdate_t>(offsets::functions::CharacterDataStack__Update);
        this->characterDataStackPush = reinterpret_cast<characterDataStackPush_t>(offsets::functions::CharacterDataStack__Push);
        this->getGoldRedirectTarget = reinterpret_cast<getGoldRedirectTarget_t>(offsets::functions::GetGoldRedirectTarget);
        this->WorldToScreen = reinterpret_cast<WorldToScreen_t>(offsets::functions::WorldToScreen);
    }
}

void Memory::Search(bool gameClient)
{
    try {
        const auto& signatureToSearch{ (gameClient ? this->gameClientSig : this->sigs) };

        for (const auto& sig : signatureToSearch)
            *sig.offset = 0;

        while (true) {
            bool missing_offset{ false };
            for (auto& sig : signatureToSearch) {

                if (*sig.offset != 0)
                    continue;

                const auto& address{ find_signature(nullptr, sig.pattern.c_str()) };

                if (!address) {
                    ::MessageBoxA(nullptr, ("Failed to find pattern: " + sig.pattern).c_str(), "SunSkin", MB_OK | MB_ICONWARNING);
                    missing_offset = true;
                    continue;
                }

                if (sig.approach == Approach::only_read_x)
                    *sig.offset = *reinterpret_cast<std::uint32_t*>(address + (sig.pattern.find_first_of('?') / 3) + sig.additional);
                else if (sig.approach == Approach::mov_s_x)
                    *sig.offset = reinterpret_cast<std::uintptr_t>(address + *reinterpret_cast<std::uint32_t*>(address + 3) + 7 + sig.additional);
                else if (sig.approach == Approach::call_x)
                    *sig.offset = reinterpret_cast<std::uintptr_t>(address + *reinterpret_cast<std::uint32_t*>(address + 1) + 5 + sig.additional);
                else if (sig.approach == Approach::origin)
                    *sig.offset = reinterpret_cast<std::uintptr_t>(address + sig.additional);

                // cheatManager.logger->addLog("Found: %s\n\tAddress: 0x%X\n", sig.pattern.c_str(), *sig.offset);

                if (!*sig.offset) {
                    missing_offset = true;
                    break;
                }
            }

            if (!missing_offset)
                break;

            using namespace std::chrono_literals;
            std::this_thread::sleep_for(2s);
        }
        this->update(gameClient);
    }
    catch (const std::exception& e) {
        ::MessageBoxA(nullptr, e.what(), "SunSkin", MB_OK | MB_ICONWARNING);
    }
}
