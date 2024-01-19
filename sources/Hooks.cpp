#pragma warning(disable : 6011)

#include <Windows.h>
#include <ShlObj.h>

#include "imgui_impl_win32.h"

#include "CheatManager.hpp"
#include "D3DVTable.hpp"
#include "Hooks.hpp"

inline WNDPROC originalWndProc;
LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static LRESULT WINAPI wndProc(const HWND window, const UINT msg, const WPARAM wParam, const LPARAM lParam) noexcept
{
    if (ImGui_ImplWin32_WndProcHandler(window, msg, wParam, lParam))
        return true;
    return ::CallWindowProc(originalWndProc, window, msg, wParam, lParam);
}

void Hooks::install() noexcept
{
    const auto& cheatManager{ CheatManager::getInstance() };
    if (cheatManager.memory->swapChain) {
        device_vmt = std::make_unique<::vmt_smart_hook>(cheatManager.memory->swapChain);
        device_vmt->apply_hook<d3d_vtable::dxgi_present>(8);
        device_vmt->apply_hook<d3d_vtable::dxgi_resize_buffers>(13);
        device_vmt->rehook();
        cheatManager.logger->addLog("DX11 Hooked!\n");
    }
    else if (cheatManager.memory->d3dDevice) {
        device_vmt = std::make_unique<::vmt_smart_hook>(cheatManager.memory->d3dDevice);
        device_vmt->apply_hook<d3d_vtable::end_scene>(42);
        device_vmt->apply_hook<d3d_vtable::reset>(16);
        device_vmt->rehook();
        cheatManager.logger->addLog("DX9 Hooked!\n");
    }
    originalWndProc = WNDPROC(::SetWindowLongPtr(cheatManager.memory->window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&wndProc)));
    cheatManager.logger->addLog("WndProc hooked!\n\tOriginal: 0x%X\n\tNew: 0x%X\n", &originalWndProc, &wndProc);
}

void Hooks::uninstall() noexcept
{
    const auto& cheatManager{ CheatManager::getInstance() };
    ::SetWindowLongW(cheatManager.memory->window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(originalWndProc));

    if (device_vmt)
        device_vmt->unhook();

    CheatManager::getInstance().cheatState = false;
}
