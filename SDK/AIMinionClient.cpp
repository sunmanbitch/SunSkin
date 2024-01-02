#include "AIMinionClient.hpp"
#include "CheatManager.hpp"

const AIBaseCommon* AIMinionClient::redirectTarget() noexcept
{
    return CheatManager::getInstance().memory->getGoldRedirectTarget(this);
}