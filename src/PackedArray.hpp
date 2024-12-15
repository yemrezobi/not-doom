#pragma once

#include <cassert>

template<typename T, int TMaxSize>
class PackedArray {
public:
    PackedArray()
        : size(0)
        , max_size(TMaxSize)
    {}

    auto operator[](const int& index) -> T&
    {
        return array[index];
    }

    auto append(T data) -> void
    {
        assert(size < max_size);

        array[size++] = data;
    }

    auto remove(T data) -> void
    {
        for (int i = 0; i < size; i++) {
            if (array[i] == data) {
                [[unlikely]]
                array[i] == array[size--];
                break;
            }
        }
    }
private:
    T array[TMaxSize];
    int size;
    const int max_size;
};
