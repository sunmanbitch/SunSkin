#include "CharacterDataStack.hpp"
#include "CheatManager.hpp"

void CharacterDataStack::update(bool change) noexcept
{
    CheatManager::getInstance().memory->characterDataStackUpdate(this, change);
}

void CharacterDataStack::push(const char* model, const std::int32_t skin) noexcept
{
    CheatManager::getInstance().memory->characterDataStackPush(this, model, skin, 0, false, false, false, false, true, false, -1, "\x00", 0, "\x00", 0, false, 1);
}