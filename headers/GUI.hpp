#pragma once

#include <mutex>

class GUI {
public:
    void render() noexcept;

    bool is_open{ false };
    std::once_flag set_font_scale;
private:
    char str_buffer[256];
};
