#pragma warning(disable : 6011)

#include <Windows.h>
#include <ShlObj.h>
#include <cinttypes>
#include <filesystem>
#include <string>

#include "fnv_hash.hpp"
#include "imgui_impl_dx9.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "vmt_smart_hook.hpp"

#include "CheatManager.hpp"
#include "Hooks.hpp"

LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static LRESULT WINAPI wndProc(const HWND window, const UINT msg, const WPARAM wParam, const LPARAM lParam) noexcept
{
    if (ImGui_ImplWin32_WndProcHandler(window, msg, wParam, lParam))
        return true;
    return ::CallWindowProc(originalWndProc, window, msg, wParam, lParam);
}

std::once_flag init_device;
std::unique_ptr<::vmt_smart_hook> device_vmt{ nullptr };

namespace d3d_vtable {
    struct dxgi_present {
        static long WINAPI hooked(IDXGISwapChain* p_swap_chain, UINT sync_interval, UINT flags) noexcept
        {
            std::call_once(init_device, [&]() {
                const auto& cheatManager{ CheatManager::getInstance() };
                cheatManager.holdon->implDxInit(p_swap_chain);
                cheatManager.holdon->initHeroSkin();
                });
            CheatManager::getInstance().holdon->render();
            return m_original(p_swap_chain, sync_interval, flags);
        }
        static decltype(&hooked) m_original;
    };
    decltype(dxgi_present::m_original) dxgi_present::m_original;

    struct dxgi_resize_buffers {
        static long WINAPI hooked(IDXGISwapChain* p_swap_chain, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swap_chain_flags) noexcept
        {
            CheatManager::getInstance().holdon->release_render_target();
            const auto hr{ m_original(p_swap_chain, buffer_count, width, height, new_format, swap_chain_flags) };
            CheatManager::getInstance().holdon->create_render_target();
            return hr;
        }
        static decltype(&hooked) m_original;
    };
    decltype(dxgi_resize_buffers::m_original) dxgi_resize_buffers::m_original;

    struct end_scene {
        static long WINAPI hooked(IDirect3DDevice9* p_device) noexcept
        {
            std::call_once(init_device, [&]() {
                const auto& cheatManager{ CheatManager::getInstance() };
                cheatManager.holdon->implDxInit(p_device);
                cheatManager.holdon->initHeroSkin();
                });
            CheatManager::getInstance().holdon->render(p_device);
            return m_original(p_device);
        }
        static decltype(&hooked) m_original;
    };
    decltype(end_scene::m_original) end_scene::m_original;

    struct reset {
        static long WINAPI hooked(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* parametrs) noexcept
        {
            ::ImGui_ImplDX9_InvalidateDeviceObjects();
            const auto hr{ m_original(device, parametrs) };
            if (hr >= 0) { ::ImGui_ImplDX9_CreateDeviceObjects(); }
            return hr;
        }
        static decltype(&hooked) m_original;
    };
    decltype(reset::m_original) reset::m_original;
};

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

    cheatManager.holdon->implDXShutdown();

    CheatManager::getInstance().cheatState = false;
}
