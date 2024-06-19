#pragma once

#include <cstdint>

template <class T>
class RiotArray {
public:
    class iterator {
    private:
        T* ptr_;
    public:
        iterator() noexcept = default;
        iterator(T* ptr) noexcept : ptr_(ptr) {}
        T& operator*() const { return *this->ptr_; }
        iterator& operator++() noexcept {
            this->ptr_++;
            return *this;
        }
        friend bool operator!=(const iterator& a, const iterator& b) noexcept {
            return a.ptr_ != b.ptr_;
        }
    };
    T* data;
private:
    std::uint32_t size;
    std::uint32_t capacity;
public:
    iterator begin() noexcept {
        return iterator(this->data);
    }

    iterator end() noexcept {
        return iterator(this->data + this->size);
    }

};
