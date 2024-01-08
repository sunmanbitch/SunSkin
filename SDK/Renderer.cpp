#include "Renderer.hpp"
#include "CheatManager.hpp"

bool Renderer::wroldToScreen(Position* vector3, Position* vector2) noexcept
{
    return CheatManager::getInstance().memory->WorldToScreen(this, vector3, vector2);
}