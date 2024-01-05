#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"

#include "CheatManager.hpp"
#include "Utils.hpp"

void ImGui::textUnformattedCentered(const char* text) noexcept
{
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(text).x) / 2.0f);
    ImGui::TextUnformatted(text);
}

void ImGui::hoverInfo(const char* desc) noexcept
{
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary | ImGuiHoveredFlags_DelayNormal | ImGuiHoveredFlags_NoSharedDelay))
    {
        ImGui::SetTooltip(desc);
    }
}

void ImGui::rainbowText() noexcept
{
    static float r{ 1.0f };
    static float g{ 0.f };
    static float b{ 0.f };

    if (CheatManager::getInstance().config->rainbowText) {
        if (r == 1.f && g >= 0.f && b <= 0.f) {
            g += 0.005f;
            b = 0.f;
        }
        if (r <= 1.f && g >= 1.f && b == 0.f) {
            g = 1.f;
            r -= 0.005f;
        }
        if (r <= 0.f && g == 1.f && b >= 0.f) {
            r = 0.f;
            b += 0.005f;
        }
        if (r == 0.f && g <= 1.f && b >= 1.f) {
            b = 1.f;
            g -= 0.005f;
        }
        if (r >= 0.f && g <= 0.f && b == 1.f) {
            g = 0.f;
            r += 0.005f;
        }
        if (r >= 1.f && g >= 0.f && b <= 1.f) {
            r = 1.f;
            b -= 0.005f;
        }
        ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(r, g, b, 1.00f);
    }
    else {
        if (auto& clr{ ImGui::GetStyle().Colors[ImGuiCol_Text] }; clr.x != 0.92f && clr.y != 0.92f && clr.z != 0.92f)
            clr = ImVec4(0.92f, 0.92f, 0.92f, 0.92f);
    }
}

struct InputTextCallback_UserData {
    std::string* Str;
    ImGuiInputTextCallback ChainCallback;
    void* ChainCallbackUserData;
};

static int InputTextCallback(ImGuiInputTextCallbackData* data) noexcept
{
    const auto user_data{ static_cast<InputTextCallback_UserData*>(data->UserData) };
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
        const auto str{ user_data->Str };
        IM_ASSERT(data->Buf == str->c_str());
        str->resize(data->BufTextLen);
        data->Buf = const_cast<char*>(str->c_str());
    }
    else if (user_data->ChainCallback) {
        data->UserData = user_data->ChainCallbackUserData;
        return user_data->ChainCallback(data);
    }
    return 0;
}

bool ImGui::InputText(const char* label, std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* userData) noexcept
{
    IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
    flags |= ImGuiInputTextFlags_CallbackResize;
    auto cbUserData{ InputTextCallback_UserData(str, callback, userData) };
    return InputText(label, const_cast<char*>(str->c_str()), str->capacity() + 1, flags, InputTextCallback, &cbUserData);
}

void ImGui::ButtonEnableBind(const char* desc, KeyBind& key_bind, const char* tips) noexcept
{
    ImGui::Text(desc);
    ImGui::SameLine();
    ImGui::Button(ImGui::GetKeyName(key_bind.imGuiKeyCode));
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary | ImGuiHoveredFlags_DelayNormal | ImGuiHoveredFlags_NoSharedDelay))
    {
        key_bind.setToPressedKey();
        ImGui::SetTooltip(tips);
    }
}
