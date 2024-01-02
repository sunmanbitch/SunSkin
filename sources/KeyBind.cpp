#include "KeyBind.hpp"

KeyBind::KeyBind(const ImGuiKey keyCode) noexcept : imGuiKeyCode{ keyCode } {  }

bool KeyBind::setToPressedKey() noexcept
{
    for (ImGuiMouseButton imguimouse{ ImGuiMouseButton_Left }; imguimouse < ImGuiMouseButton_COUNT; imguimouse = (ImGuiMouseButton)(imguimouse + 1))
    {
        if (ImGui::IsMouseClicked(imguimouse))
            return false;
    }

    for (ImGuiKey imguikey{ ImGuiKey_NamedKey_BEGIN }; imguikey < ImGuiKey_NamedKey_END; imguikey = (ImGuiKey)(imguikey + 1))
    {
        if (!ImGui::IsKeyPressed(imguikey))
            continue;
        imGuiKeyCode = imguikey;
        return true;
    }

    return false;
}