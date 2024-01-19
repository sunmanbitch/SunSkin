#pragma once

#include <d3d9.h>
#include <d3d11.h>

#include "GUI.hpp"

class Holdon
{
public:
    void create_render_target() noexcept;
    void release_render_target() noexcept;
    void init_imgui() noexcept;
    void implDxInit(IDXGISwapChain*) noexcept;
    void implDxInit(IDirect3DDevice9*) noexcept;
    void render() noexcept;
    void render(IDirect3DDevice9*) noexcept;
    void keyEvent() noexcept;
    void gameStatus() noexcept;
    void initHeroSkin() noexcept;

private:
    GUI gui;
    ID3D11Device* d3d11_device{ nullptr };
    ID3D11DeviceContext* d3d11_device_context{ nullptr };
    ID3D11RenderTargetView* main_render_target_view{ nullptr };
    IDXGISwapChain* p_swap_chain{ nullptr };
};
