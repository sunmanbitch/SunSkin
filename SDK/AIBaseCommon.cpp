#include <cstdint>
#include <string>

#include "AIBaseCommon.hpp"
#include "fnv_hash.hpp"

bool AIBaseCommon::checkSpecialSkins(const char* model, const std::int32_t skin) noexcept
{
    const auto& dataStack{ this->get_character_data_stack() };
    const auto champ_name{ fnv::hash_runtime(dataStack->base_skin.model.str) };
    const std::int8_t& zero{ 0 };
    const std::int8_t& nonzero{ -1 };

    if (champ_name == FNV("Katarina") && (skin >= 29 && skin <= 36))
        dataStack->base_skin.gear = zero;
    else if (champ_name == FNV("Renekton") && (skin >= 26 && skin <= 32))
        dataStack->base_skin.gear = zero;
    else if (champ_name == FNV("MissFortune") && skin == 16)
        dataStack->base_skin.gear = zero;
    else if (champ_name != FNV("Kayn"))
        dataStack->base_skin.gear = nonzero;
    else if (champ_name == FNV("Lux") && skin == 7)
        return true;
    else if (champ_name == FNV("Sona") && skin == 6)
        return true;

    return false;
}

void AIBaseCommon::change_skin(const char* model, const std::int32_t skin) noexcept
{
    this->get_cryption()->encrypt(skin);
    const auto& dataStack{ this->get_character_data_stack() };
    dataStack->base_skin.skin = skin;

    if (this->checkSpecialSkins(model, skin))
    {
        if (!dataStack->stack.empty()) dataStack->stack.clear();
        dataStack->push(model, skin);
    }
    else
        dataStack->update();
}
