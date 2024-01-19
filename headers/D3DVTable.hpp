#pragma once

#include "CheatManager.hpp"
#include "imgui_impl_dx9.h"

namespace d3d_vtable {
    std::once_flag init_device;

    struct dxgi_present {
        static long WINAPI hooked(IDXGISwapChain* p_swap_chain, UINT sync_interval, UINT flags) noexcept
        {
            std::call_once(init_device, [&p_swap_chain]()
                {
                    const auto& holdon{ CheatManager::getInstance().holdon };
                    holdon->implDxInit(p_swap_chain);
                    holdon->initHeroSkin();
                }
            );
            CheatManager::getInstance().holdon->render();
            return m_original(p_swap_chain, sync_interval, flags);
        }
        static decltype(&hooked) m_original;
    };
    decltype(dxgi_present::m_original) dxgi_present::m_original;

    struct dxgi_resize_buffers {
        static long WINAPI hooked(IDXGISwapChain* p_swap_chain, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swap_chain_flags) noexcept
        {
            const auto& holdon{ CheatManager::getInstance().holdon };
            holdon->release_render_target();
            const auto hr{ m_original(p_swap_chain, buffer_count, width, height, new_format, swap_chain_flags) };
            holdon->create_render_target();
            return hr;
        }
        static decltype(&hooked) m_original;
    };
    decltype(dxgi_resize_buffers::m_original) dxgi_resize_buffers::m_original;

    struct end_scene {
        static long WINAPI hooked(IDirect3DDevice9* p_device) noexcept
        {
            std::call_once(init_device, [&p_device]()
                {
                    const auto& holdon{ CheatManager::getInstance().holdon };
                    holdon->implDxInit(p_device);
                    holdon->initHeroSkin();
                }
            );
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
