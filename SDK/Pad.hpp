#pragma once

#include <cstddef>

#define CONCAT(a, b) a##b
#define PAD_NAME(n) CONCAT(pad, n)
#define PAD(size) std::byte PAD_NAME(__LINE__)[size]

#define CLASS_GETTER(returnType, name, offset)                            \
[[nodiscard]] inline returnType name() const noexcept                     \
{                                                                         \
    return *reinterpret_cast<returnType*>(std::uintptr_t(this) + offset); \
}

#define CLASS_GETTER_P(returnType, name, offset)                         \
[[nodiscard]] inline returnType* name() const noexcept                   \
{                                                                        \
    return reinterpret_cast<returnType*>(std::uintptr_t(this) + offset); \
}

#define DEFINE_MEMBER_0(x) x

#define DEFINE_MEMBER_N(x,offset) \
struct                            \
{                                 \
    PAD(offset);                  \
    DEFINE_MEMBER_0(x);           \
}

#define arr2vec(obj, ptr) \
std::vector<obj*>{ ptr->list, &ptr->list[ptr->length] }
