#include "CharacterDataStack.hpp"
#include "CheatManager.hpp"

void CharacterDataStack::update(bool change) noexcept
{
    CheatManager::getInstance().memory->characterDataStackUpdate(this, change);
}

void CharacterDataStack::push(const char* model, int skin, int gear, bool unknown1, bool unknown2, bool unknown3, bool unknown4, bool unknown5, bool unknown6, int unknown7, const char* unknown8, int unknown9, const char* unknown10, int unknown11, bool unknown12, int unknown13) noexcept
{
    CheatManager::getInstance().memory->characterDataStackPush(this, model, skin, gear, unknown1, unknown2, unknown3, unknown4, unknown5, unknown6, unknown7, unknown8, unknown9, unknown10, unknown11, unknown12, unknown13);
}