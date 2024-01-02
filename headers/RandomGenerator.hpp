#pragma once

#include <mutex>
#include <random>

class RandomGenerator {
public:
    template <std::integral T>
    [[nodiscard]] static T random(T min, T max) noexcept
    {
        std::scoped_lock lock{ mutex };
        return std::uniform_int_distribution{ min, max }(gen);
    }

    template <std::floating_point T>
    [[nodiscard]] static T random(T min, T max) noexcept
    {
        std::scoped_lock lock{ mutex };
        return std::uniform_real_distribution{ min, max }(gen);
    }

    template <typename T>
    [[nodiscard]] static std::enable_if_t<std::is_enum_v<T>, T> random(T min, T max) noexcept
    {
        return static_cast<T>(random(static_cast<std::underlying_type_t<T>>(min), static_cast<std::underlying_type_t<T>>(max)));
    }
private:
    inline static std::mt19937 gen{ std::random_device{}() };
    inline static std::mutex mutex;
};

template <typename T>
[[nodiscard]] T random(T min, T max) noexcept
{
    return RandomGenerator::random(min, max);
}
