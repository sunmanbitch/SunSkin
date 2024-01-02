#pragma once

#include <cstdlib>
#include <cstdint>

namespace detail {
    class fnv_hash {
    private:
        constexpr static auto k_offset_basis{ 0xcbf29ce484222325ull };
        constexpr static auto k_prime{ 1099511628211ull };

    public:
        template <std::size_t N>
        static __forceinline constexpr auto hash_constexpr(const char(&str)[N], const std::size_t size = N) -> std::uint64_t {
            return static_cast<std::uint64_t>(1ull * (size == 1 ? (k_offset_basis ^ str[0]) : (hash_constexpr(str, size - 1) ^ str[size - 1])) * k_prime);
        }

        static auto __forceinline hash_runtime(const char* str) -> std::uint64_t {
            auto result{ k_offset_basis };
            do {
                result ^= *str++;
                result *= k_prime;
            } while (*(str - 1) != '\0');

            return result;
        }
    };
}

using fnv = ::detail::fnv_hash;

#define FNV(str) (std::integral_constant<std::uint64_t, fnv::hash_constexpr(str)>::value)
