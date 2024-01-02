#include "CheatManager.hpp"

CheatManager& CheatManager::getInstance() noexcept
{
    static CheatManager instance;
    return instance;
}

void CheatManager::start() noexcept
{
    this->hooks = std::make_unique<Hooks>();
    this->config = std::make_unique<Config>();
    this->gui = std::make_unique<GUI>();
    this->memory = std::make_unique<Memory>();
    this->database = std::make_unique<SkinDatabase>();
    this->logger = std::make_unique<ImGuiAppLog>();
    this->holdon = std::make_unique<Holdon>();
}

CheatManager::CheatManager()
{
    this->start();
}