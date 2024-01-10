#include "CheatManager.hpp"

CheatManager& CheatManager::getInstance() noexcept
{
    static CheatManager instance;
    return instance;
}
