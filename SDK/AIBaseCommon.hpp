#pragma once

#include <cstdint>

#include "CharacterDataStack.hpp"
#include "GameObject.hpp"
#include "Cryption.hpp"

class AIBaseCommon : public GameObject {
public:
    CLASS_GETTER_P(CharacterDataStack, get_character_data_stack, offsets::AIBaseCommon::CharacterDataStack)
    CLASS_GETTER_P(Cryption<std::int32_t>, get_cryption, offsets::AIBaseCommon::SkinId)

    void change_skin(const char* model, const std::int32_t skin) noexcept;
private:
    bool checkSpecialSkins(const char* model, const std::int32_t skin) noexcept;
};