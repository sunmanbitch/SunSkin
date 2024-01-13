#pragma warning(disable : 6387 4715)

#include <Windows.h>
#include <clocale>
#include <chrono>
#include <thread>

#include "CheatManager.hpp"

bool WINAPI HideThread(const HANDLE hThread) noexcept
{
    __try {
        using FnSetInformationThread = NTSTATUS(NTAPI*)(HANDLE ThreadHandle, UINT ThreadInformationClass, PVOID ThreadInformation, ULONG ThreadInformationLength);
        const auto NtSetInformationThread{ reinterpret_cast<FnSetInformationThread>(::GetProcAddress(::GetModuleHandleW(L"ntdll.dll"), "NtSetInformationThread")) };

        if (!NtSetInformationThread)
            return false;

        if (const auto status{ NtSetInformationThread(hThread, 0x11u, nullptr, 0ul) }; status == 0x00000000)
            return true;
    }
    __except (TRUE) {
        return false;
    }
    return false;
}

DECLSPEC_SAFEBUFFERS static void WINAPI DllAttach([[maybe_unused]] LPVOID lp) noexcept
{
    using namespace std::chrono_literals;

    const auto& cheatManager{ CheatManager::getInstance() };
    const auto& logger{ cheatManager.logger };
    const auto& memory{ cheatManager.memory };

    if (HideThread(::GetCurrentThread()))
        logger->addLog("Thread Hidden!\n");

    while (true) {
        if (!memory->client)
        {
            std::this_thread::sleep_for(2s);
            memory->Search();
        }
        else if (memory->checkRunning())
        {
            logger->addLog("GameClient found!\n");
            break;
        }
        else
            std::this_thread::sleep_for(1s);
    }

    memory->Search(false);
    logger->addLog("All offsets found!\n");

    cheatManager.database->load();
    logger->addLog("All skins loaded from memory!\n");

    cheatManager.config->load();
    logger->addLog("CFG loaded!\n");

    cheatManager.hooks->install();

    while (cheatManager.cheatState)
        std::this_thread::sleep_for(250ms);

    ::ExitProcess(0u);
}

DECLSPEC_SAFEBUFFERS BOOL APIENTRY DllMain(const HMODULE hModule, const DWORD reason, [[maybe_unused]] LPVOID reserved)
{
    DisableThreadLibraryCalls(hModule);

    if (reason != DLL_PROCESS_ATTACH)
        return FALSE;

    HideThread(hModule);
    std::setlocale(LC_ALL, ".utf8");

    ::_beginthreadex(nullptr, 0u, reinterpret_cast<_beginthreadex_proc_type>(DllAttach), nullptr, 0u, nullptr);
    ::CloseHandle(hModule);
    return TRUE;
}
