#include "AIMinionClient.hpp"
#include "AIHero.hpp"
#include "CheatManager.hpp"
#include "fnv_hash.hpp"

const AIBaseCommon* AIMinionClient::redirectTarget() noexcept
{
    const auto& cheatManager{ CheatManager::getInstance() };
    const auto& target{ cheatManager.memory->getGoldRedirectTarget(this) };
    if (target) return target;

    const auto& modelHash{ this->getModelHash() };
    const auto& targetMap{ cheatManager.database->targetMap };

    if (auto it{ targetMap.find(modelHash) }; it == targetMap.end())
        return target;

    const auto& hero_model_hash{ targetMap.at(modelHash) };
    for (const auto& hero : arr2vec(AIHero, cheatManager.memory->heroList))
    {
        if (hero_model_hash != cheatManager.database->heroHash[hero->get_character_data_stack()->base_skin.model.str])
            continue;
        return hero;
    }

    return target;
}

std::uint64_t AIMinionClient::getModelHash() noexcept
{
    const auto& cheatManager{ CheatManager::getInstance() };
    const auto& model{ this->get_character_data_stack()->base_skin.model.str };
    const auto& modelHash{ fnv::hash_runtime(model) };
    return modelHash;
}

bool AIMinionClient::isMinion() noexcept
{
    const auto& modelHash{ this->getModelHash() };
    const auto& minionNames{ CheatManager::getInstance().database->minionNames };

    return minionNames.find(modelHash) != minionNames.end();
}

bool AIMinionClient::isVision() noexcept
{
    const auto& modelHash{ this->getModelHash() };
    const auto& visionNames{ CheatManager::getInstance().database->vision };

    return visionNames.find(modelHash) != visionNames.end();
}

bool AIMinionClient::isTestCube() noexcept
{
    const auto& modelHash{ this->getModelHash() };
    const auto& testCube{ CheatManager::getInstance().database->testCube };

    return testCube == modelHash;
}

bool AIMinionClient::isJungle() noexcept
{
    const auto& modelHash{ this->getModelHash() };
    const auto& wild{ CheatManager::getInstance().database->wild };

    return wild.find(modelHash) != wild.end();
}

bool AIMinionClient::isOther() noexcept
{
    const auto& modelHash{ this->getModelHash() };
    const auto& cheatManager{ CheatManager::getInstance() };

    const auto& plants{ cheatManager.database->plants };
    if (plants.find(modelHash) != plants.end()) return true;

    const auto& dragon{ cheatManager.database->dragon };
    if (dragon.find(modelHash) != dragon.end()) return true;

    const auto& other{ cheatManager.database->other };
    if (other.find(modelHash) != other.end()) return true;

    return false;
}
