#pragma once

#include <cstddef>
#include <stdexcept>
#include <string>
#include <iostream>

template <class T>
class Allocator {
public:
    Allocator(size_t min_size, size_t max_size): min_size_(min_size), max_size_(max_size),
        blocks_(new Block[max_size_]), memory_(new T[max_size_]) {
        blocks_[0] = Block{blocks_, blocks_ + max_size_, false};
    } 

    Allocator() {
        Allocator(kDefaultMinSize, kDefaultMaxSize);
    }

    T* alloc(size_t size) {
        size = std::max(size, min_size_);
        for (Block* ptr = blocks_; ptr != blocks_ + max_size_; ptr = ptr->end) {
            if (!ptr->used && ptr->Size() >= size) {
                while (ptr->Size() / 2 >= size) {
                    Block* mid = ptr + ptr->Size() / 2;
                    *mid = Block{mid, ptr->end, false};
                    ptr->end = mid;
                }

                ptr->used = true;
                return memory_ + (ptr - blocks_);
            }
        }
        throw std::runtime_error("No free space");
    }

    void dealloc(T* ptr) {
        Block* block = blocks_ + (ptr - memory_);
        block->used = false;
        while (block->Size() != max_size_) {
            size_t size = block->Size();
            size_t shift = block - blocks_;
            Block* first = blocks_ + (shift / (2 * size)) * 2 * size;
            Block* second = first + size;
            if (first->used || second->used || first->Size() != second->Size()) {
                break;
            }
            *first = Block{first, second->end, false};
            block = first;
        }
    }

    std::string ToString() {
        std::string result = "|";
        Block* ptr = blocks_;
        while (ptr != blocks_ + max_size_) {
            for (int j = 0; j < ptr->Size(); ++j) {
                result += ptr->used + '0';
            }
            result += "|";
            ptr = ptr->end;
        }
        return result;
    }

    ~Allocator() {
        delete blocks_;
        delete memory_;
    }

private:
    struct Block {
    public:
        Block* begin;
        Block* end;
        bool used;

    public:
        size_t Size() {
            return end - begin;
        }
    };

private:
    size_t min_size_;
    size_t max_size_;
    Block* blocks_;
    T* memory_;

    static const int kDefaultMinSize;
    static const int kDefaultMaxSize;
};

template<class T>
const int Allocator<T>::kDefaultMinSize = 4;

template<class T>
const int Allocator<T>::kDefaultMaxSize = 1 << 20;
