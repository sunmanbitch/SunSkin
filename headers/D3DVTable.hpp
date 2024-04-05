#pragma once

#include "CheatManager.hpp"

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
};
