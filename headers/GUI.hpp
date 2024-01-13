#pragma once

#include <mutex>

class GUI {
public:
    void render() noexcept;
    bool is_open{ true };
    std::once_flag set_font_scale;
private:
    char str_buffer[256];
    void playerTabItem() noexcept;
    void heroesTabItem() noexcept;
    void globalTabItem() noexcept;
    void extrasTabItem() noexcept;
    void footer() noexcept;
    void changeTurretSkin(const std::int32_t skinId, const std::int32_t team) noexcept;
};
