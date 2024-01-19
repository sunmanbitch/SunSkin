#pragma once

#include "vmt_smart_hook.hpp"

class Hooks {
private:
    std::unique_ptr<::vmt_smart_hook> device_vmt{ nullptr };
public:
    void install() noexcept;
    void uninstall() noexcept;
};
