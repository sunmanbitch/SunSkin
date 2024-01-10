#include <cstdint>
#include <string>

#include "AIBaseCommon.hpp"
#include "CheatManager.hpp"
#include "fnv_hash.hpp"

bool AIBaseCommon::checkSpecialSkins(const std::int32_t& skin) noexcept
{
    const auto& dataStack{ this->get_character_data_stack() };
    const auto& champ_name{ fnv::hash_runtime(dataStack->base_skin.model.str) };
    const std::int8_t& zero{ 0 };
    const std::int8_t& nonzero{ -1 };

    auto result{ false };

    if (champ_name != FNV("Kayn"))
        dataStack->base_skin.gear = nonzero;

    const auto& cheatManager{ CheatManager::getInstance() };
    const auto& it{ cheatManager.database->specialSkins.find(champ_name) };
    if (it != cheatManager.database->specialSkins.end() && it->second.skinIdStart <= skin && skin <= it->second.skinIdEnd)
        dataStack->base_skin.gear = zero;
    else if (champ_name == FNV("Lux") && skin == 7)
        result = true;
    else if (champ_name == FNV("Sona") && skin == 6)
        result = true;
    else if (champ_name == FNV("TestCubeRender10Vision"))
        result = true;

    return result;
}

void AIBaseCommon::change_skin(const char* model, const std::int32_t skin, const bool isEncrypt) noexcept
{
    if (skin < 0) return;

    const auto& dataStack{ this->get_character_data_stack() };

    if (this->checkSpecialSkins(skin))
    {
        if (isEncrypt) this->get_cryption()->encrypt(skin);
        dataStack->base_skin.skin = skin;
        if (!dataStack->stack.empty()) dataStack->stack.clear();
        dataStack->push(model, skin);
    }
    else if (dataStack->base_skin.skin != skin)
    {
        if (isEncrypt) this->get_cryption()->encrypt(skin);
        dataStack->base_skin.skin = skin;
        dataStack->update();
    }
}
