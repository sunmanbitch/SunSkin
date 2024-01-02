#pragma once

#include "imgui.h"

class KeyBind {
public:
    KeyBind() = default;
    explicit KeyBind(ImGuiKey keyCode) noexcept;

    bool setToPressedKey() noexcept;
    ImGuiKey imGuiKeyCode{ ImGuiKey_None };
};