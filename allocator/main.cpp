#include "allocator.h"
#include <assert.h>
#include <iostream>

int main() {
    {
        Allocator<int> allocator(4, 16);
        auto begin = allocator.alloc(4);
        assert(allocator.ToString() == "|1111|0000|00000000|");
        allocator.alloc(4);
        assert(allocator.ToString() == "|1111|1111|00000000|");
        allocator.dealloc(begin);
        assert(allocator.ToString() == "|0000|1111|00000000|");
    }

    {
        Allocator<int> allocator(4, 16);
        auto begin = allocator.alloc(16);
        assert(allocator.ToString() == "|1111111111111111|");
        bool exception_caught = false;
        try {
            allocator.alloc(4);
        } catch (std::exception ex) {
            exception_caught = true;
        }
        assert(exception_caught);
    }

    {
        Allocator<int> allocator(4, 16);
        auto first = allocator.alloc(4);
        assert(allocator.ToString() == "|1111|0000|00000000|");
        auto second = allocator.alloc(8);
        assert(allocator.ToString() == "|1111|0000|11111111|");
        auto third = allocator.alloc(4);
        assert(allocator.ToString() == "|1111|1111|11111111|");
        allocator.dealloc(first);
        assert(allocator.ToString() == "|0000|1111|11111111|");
        allocator.dealloc(second);
        assert(allocator.ToString() == "|0000|1111|00000000|");
        allocator.dealloc(third);
        assert(allocator.ToString() == "|0000000000000000|");
    }
}
