#pragma once

#include <string>

#include "imgui.h"

namespace ImGui
{
    bool InputText(const char* label, std::string* str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void* userData = nullptr) noexcept;
    void textUnformattedCentered(const char* text) noexcept;
    void rainbowText() noexcept;
    void hoverInfo(const char* desc) noexcept;
    void ButtonEnableBind(const char* desc, KeyBind& key_bind, const char* tips) noexcept;
};
