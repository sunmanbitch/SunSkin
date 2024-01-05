#include <ShlObj.h>

#include "Holdon.hpp"
#include "CheatManager.hpp"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

void Holdon::create_render_target() noexcept
{
    ID3D11Texture2D* back_buffer{ nullptr };
    this->p_swap_chain->GetBuffer(0u, IID_PPV_ARGS(&back_buffer));

    if (back_buffer) {
        d3d11_device->CreateRenderTargetView(back_buffer, nullptr, &main_render_target_view);
        back_buffer->Release();
    }
}

void Holdon::release_render_target() noexcept
{
    if (main_render_target_view)
        main_render_target_view->Release(); main_render_target_view = nullptr;
}

void Holdon::init_imgui() noexcept
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto& style{ ImGui::GetStyle() };

    style.WindowPadding = ImVec2(6.0f, 6.0f);
    style.FramePadding = ImVec2(6.0f, 4.0f);
    style.ItemSpacing = ImVec2(6.0f, 4.0f);
    style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

    style.ScrollbarSize = 12.0f;

    style.WindowBorderSize = 0.5f;
    style.ChildBorderSize = 0.5f;
    style.PopupBorderSize = 0.5f;
    style.FrameBorderSize = 0;

    style.WindowRounding = 0.0f;
    style.ChildRounding = 0.0f;
    style.FrameRounding = 0.0f;
    style.ScrollbarRounding = 0.0f;
    style.GrabRounding = 0.0f;
    style.TabRounding = 0.0f;
    style.PopupRounding = 0.0f;

    style.AntiAliasedFill = true;
    style.AntiAliasedLines = true;

    const auto colors{ style.Colors };

    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.44f, 0.44f, 0.44f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.81f, 0.83f, 0.81f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.93f, 0.65f, 0.14f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

    auto& io{ ImGui::GetIO() };
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

    const auto& cheatManager{ CheatManager::getInstance() };

    if (PWSTR pathToFonts; SUCCEEDED(::SHGetKnownFolderPath(FOLDERID_Fonts, 0, nullptr, &pathToFonts))) {
        static const ImWchar tahomaRanges[] = {
            0x0020, 0x00FF, // Basic Latin + Latin Supplement
            0x0100, 0x024F, // Latin Extended-A + Latin Extended-B
            0x0250, 0x02FF, // IPA Extensions + Spacing Modifier Letters
            0x0300, 0x03FF, // Combining Diacritical Marks + Greek/Coptic
            0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
            0x0530, 0x06FF, // Armenian + Hebrew + Arabic
            0x0E00, 0x0E7F, // Thai
            0x1E00, 0x1FFF, // Latin Extended Additional + Greek Extended
            0x2000, 0x20CF, // General Punctuation + Superscripts and Subscripts + Currency Symbols
            0x2100, 0x218F, // Letterlike Symbols + Number Forms
            0,
        };
        const std::filesystem::path path{ pathToFonts };
        ::CoTaskMemFree(pathToFonts);
        ImFontConfig cfg;
        cfg.SizePixels = 15.0f;
        io.Fonts->AddFontFromFileTTF((path / "tahoma.ttf").string().c_str(), cfg.SizePixels, &cfg, tahomaRanges);
        cfg.MergeMode = true;
        io.Fonts->AddFontFromFileTTF((path / "malgun.ttf").string().c_str(), cfg.SizePixels, &cfg, io.Fonts->GetGlyphRangesKorean());
        io.Fonts->AddFontFromFileTTF((path / "msyh.ttc").string().c_str(), cfg.SizePixels, &cfg, io.Fonts->GetGlyphRangesChineseFull());
        cfg.MergeMode = false;
        cheatManager.logger->addLog("Fonts loaded!\n");
    }

    ::ImGui_ImplWin32_Init(cheatManager.memory->window);
}

void Holdon::implDxInit(IDXGISwapChain* device) noexcept
{
    this->p_swap_chain = device;
    this->p_swap_chain->GetDevice(IID_PPV_ARGS(&this->d3d11_device));
    this->d3d11_device->GetImmediateContext(&this->d3d11_device_context);
    create_render_target();
    this->init_imgui();
    ::ImGui_ImplDX11_Init(this->d3d11_device, this->d3d11_device_context);
}

void Holdon::implDxInit(IDirect3DDevice9* device) noexcept
{
    this->init_imgui();
    ::ImGui_ImplDX9_Init(device);
}

void Holdon::render() noexcept
{
    const auto& cheatManager{ CheatManager::getInstance() };
    if (cheatManager.memory->checkRunning()) {
        ::ImGui_ImplDX11_NewFrame();
        ::ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        if (cheatManager.gui->is_open) { cheatManager.gui->render(); }
        this->keyEvent();
        this->gameStatus();

        ImGui::EndFrame();
        ImGui::Render();
        d3d11_device_context->OMSetRenderTargets(1, &main_render_target_view, nullptr);
        ::ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
}

void Holdon::render(IDirect3DDevice9* device) noexcept
{
    const auto& cheatManager{ CheatManager::getInstance() };
    if (cheatManager.memory->checkRunning()) {
        ::ImGui_ImplDX9_NewFrame();
        ::ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        if (cheatManager.gui->is_open) { cheatManager.gui->render(); }
        this->keyEvent();
        this->gameStatus();

        ImGui::EndFrame();
        ImGui::Render();
        unsigned long colorwrite, srgbwrite;
        device->GetRenderState(D3DRS_COLORWRITEENABLE, &colorwrite);
        device->GetRenderState(D3DRS_SRGBWRITEENABLE, &srgbwrite);
        device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
        device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
        ::ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
        device->SetRenderState(D3DRS_COLORWRITEENABLE, colorwrite);
        device->SetRenderState(D3DRS_SRGBWRITEENABLE, srgbwrite);
    }
}

void Holdon::keyEvent() noexcept
{
    const auto& cheatManager{ CheatManager::getInstance() };

    if (ImGui::IsKeyPressed(cheatManager.config->menuKey.imGuiKeyCode))
    {
        cheatManager.gui->is_open = !cheatManager.gui->is_open;
        if (!cheatManager.gui->is_open) { cheatManager.config->save(); }
    }

    if (cheatManager.config->quickSkinChange && ImGui::IsKeyPressed(cheatManager.config->nextSkinKey.imGuiKeyCode))
    {
        if (const auto& player{ cheatManager.memory->localPlayer }; player) {
            const auto& values{ cheatManager.database->champions_skins[fnv::hash_runtime(player->get_character_data_stack()->base_skin.model.str)] };

            auto& skinIndex{ ++cheatManager.config->current_combo_skin_index };
            if (skinIndex < 0 || skinIndex >= values.size())
                skinIndex = 0;
            player->change_skin(values[skinIndex].model_name, values[skinIndex].skin_id);

            cheatManager.config->save();
        }
    }

    if (cheatManager.config->quickSkinChange && ImGui::IsKeyPressed(cheatManager.config->previousSkinKey.imGuiKeyCode))
    {
        if (const auto& player{ cheatManager.memory->localPlayer }; player) {
            const auto& values{ cheatManager.database->champions_skins[fnv::hash_runtime(player->get_character_data_stack()->base_skin.model.str)] };

            auto& skinIndex{ --cheatManager.config->current_combo_skin_index };
            if (skinIndex < 0 || skinIndex >= values.size())
                skinIndex = values.size() - 1;
            player->change_skin(values[skinIndex].model_name, values[skinIndex].skin_id);

            cheatManager.config->save();
        }
    }

    if (ImGui::IsKeyPressed(ImGuiKey_5) && ImGui::IsKeyPressed(ImGuiKey_LeftCtrl)) {
        if (const auto& player{ cheatManager.memory->localPlayer }; player) {
            const auto playerHash{ fnv::hash_runtime(player->get_character_data_stack()->base_skin.model.str) };
            if (const auto it{ std::ranges::find_if(cheatManager.database->specialSkins,
                [&skin = player->get_character_data_stack()->base_skin.skin, &ph = playerHash](const SkinDatabase::specialSkin& x) noexcept -> bool
                {
                    return x.champHash == ph && (x.skinIdStart <= skin && x.skinIdEnd >= skin);
                }) }; it != cheatManager.database->specialSkins.end())
            {
                const auto stack{ player->get_character_data_stack() };
                if (stack->base_skin.gear < static_cast<std::int8_t>(it->gears.size()) - 1)
                    ++stack->base_skin.gear;
                else
                    stack->base_skin.gear = static_cast<std::int8_t>(0);

                stack->update();

            }
        }
    }

    if (ImGui::IsKeyPressed(ImGuiKey_F7))
    {
        // The codes you write here are executed when you press the F7 key in the game.
        // const auto minions{ cheatManager.memory->minionList };
        // for (auto i{ 0u }; i < minions->length; ++i) {
        //     const auto& minion{ minions->list[i] };

        //     if ((!minion->isMinion()) && (!minion->isJungle()))
        //     {
        //         const auto& owner{ minion->redirectTarget() };
        //         if (owner) {}
        //         else
        //             cheatManager.logger->addLog("Minion: %s\tModelName: %s\n", minion->get_name()->c_str(), minion->get_character_data_stack()->base_skin.model.str);
        //         // continue;
        //         // if (owner)
        //         //     cheatManager.logger->addLog("\tOwnerName: %s\tModelName: %s\n", owner->get_name()->c_str(), owner->get_character_data_stack()->base_skin.model.str);
        //     }
        // }

        // const auto& turrets{ cheatManager.memory->turretList };
        // for (auto i{ 0u }; i < turrets->length; ++i) {
        //     const auto& turret{ turrets->list[i] };
        //     cheatManager.logger->addLog("Turret: %s\tModelName: %s\n", turret->get_name()->c_str(), turret->get_character_data_stack()->base_skin.model.str);
        // }
    }

}

static void changeModelForObject(const AIBaseCommon* obj, const char* model, const std::int32_t skin) noexcept
{
    if (skin == -1)
        return;

    if (const auto stack{ obj->get_character_data_stack() }; stack->base_skin.skin != skin) {
        stack->base_skin.skin = skin;
        stack->stack.clear();
        stack->push(model, skin);
    }
}

static void changeSkinForObject(const AIBaseCommon* obj, const std::int32_t skin) noexcept
{
    if (skin == -1)
        return;

    if (const auto stack{ obj->get_character_data_stack() }; stack->base_skin.skin != skin) {
        stack->base_skin.skin = skin;
        stack->update();
    }
}

void Holdon::gameStatus() noexcept
{
    const auto& cheatManager{ CheatManager::getInstance() };
    const auto& heroes{ cheatManager.memory->heroList };

    for (auto i{ 0u }; i < heroes->length; ++i) {
        const auto& dataStack{ heroes->list[i]->get_character_data_stack() };

        if (dataStack->stack.empty())
            continue;

        // Viego transforms into another champion as 2nd form, our own skin's id may not match for every champion. (same problem exists in sylas) 
        if (const auto& championName{ cheatManager.database->heroHash[dataStack->base_skin.model.str] }; championName == FNV("Viego") || championName == FNV("Sylas"))
            continue;

        if (auto& stack{ dataStack->stack.front() }; stack.skin != dataStack->base_skin.skin) {
            stack.skin = dataStack->base_skin.skin;
            dataStack->update();
        }
    }

    const auto& player{ cheatManager.memory->localPlayer };
    const auto& minions{ cheatManager.memory->minionList };

    for (auto i{ 0u }; i < minions->length; ++i) {
        const auto& minion{ minions->list[i] };

        // todo: only update in screen
        if (const auto& it{ record_minions.find(minion) }; it != record_minions.end() && it->second >= std::chrono::steady_clock::now())
            continue;

        if (minion->isOther())
        {
            for (const auto& it : record_minions)
            {
                if (std::chrono::steady_clock::now() > it.second)
                    record_minions.erase(it.first);
            }
            const auto& expirationTime = std::chrono::steady_clock::now() + std::chrono::seconds(60 * 3);
            record_minions[minion] = expirationTime;
            continue;
        }
        else
        {
            const auto& expirationTime = std::chrono::steady_clock::now() + std::chrono::seconds(5);
            record_minions[minion] = expirationTime;
        }

        if (minion->isMinion()) {
            if (player && player->get_team() == 200)
                changeSkinForObject(minion, cheatManager.config->current_minion_skin_index * 2 + 1);
            else
                changeSkinForObject(minion, cheatManager.config->current_minion_skin_index * 2);
            continue;
        }

        if (minion->isJungle())
        {
            const auto& wildName{ cheatManager.database->wild.find(minion->getModelHash())->second };
            const auto& [origin, isAdd] { cheatManager.config->current_combo_jungle_mob_skin_index.insert({ wildName, 0 }) };
            if (!isAdd)
                changeSkinForObject(minion, cheatManager.config->current_combo_jungle_mob_skin_index[wildName]);
            continue;
        }

        if (const auto& owner{ minion->redirectTarget() }; owner) {
            if (minion->isVision())
            {
                if (player && owner == player && cheatManager.config->current_ward_skin_id != 0)
                    changeSkinForObject(minion, cheatManager.config->current_ward_skin_id);
            }
            else if (minion->isTestCube())
            {
                if (cheatManager.database->heroHash[owner->get_character_data_stack()->base_skin.model.str] == FNV("Yone"))
                    changeModelForObject(minion, "Yone", owner->get_character_data_stack()->base_skin.skin);
            }
            else
                changeSkinForObject(minion, owner->get_character_data_stack()->base_skin.skin);
            continue;
        }

    }

}
