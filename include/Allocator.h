#ifndef DW_ALLOCATOR_H
#define DW_ALLOCATOR_H

#include <cstddef>
#include <stdexcept>
#include <climits>
#include <cstring>

namespace dw {

template<size_t BlockSize>
class PoolAllocator {
public:
  PoolAllocator(size_t blocks_count) : 
    data_{nullptr},
    blocks_count_{0},
    bitfields_{nullptr},
    bitfields_count_{0}
  {
    size_t bitfields_count = blocks_count / (sizeof(char) * CHAR_BIT) +  (((blocks_count / sizeof(char) / CHAR_BIT) * sizeof(char) * CHAR_BIT) < blocks_count ? 1 : 0);
    size_t total_allocation = BlockSize * blocks_count + bitfields_count * sizeof(char);
    void* pointer = malloc(total_allocation);
    if (pointer == nullptr) throw std::bad_alloc{};
    data_ = pointer + sizeof(char) * bitfields_count;
    blocks_count_ = blocks_count;
    bitfields_ = reinterpret_cast<char*>(pointer);
    bitfields_count_ = bitfields_count;
    memset(bitfields_, 0, bitfields_count_);
  } 
  PoolAllocator(const PoolAllocator&) = delete;
  PoolAllocator(PoolAllocator&&) noexcept;
  PoolAllocator& operator=(const PoolAllocator&) = delete;
  PoolAllocator& operator=(PoolAllocator&&) noexcept;
  ~PoolAllocator() {
    free(bitfields_);
  }

public:
  [[nodsicard]] void* Allocate(size_t size_bytes) {
    size_t blocks_needed = size_bytes / BlockSize + (((size_bytes / BlockSize) * BlockSize) < size_bytes ? 1 : 0);
    size_t index = static_cast<size_t>(-1);
    for (size_t i = 0; i < bitfields_count_ * sizeof(char) * CHAR_BIT; ++i) {
      if (bitfields_[i / sizeof(char) / CHAR_BIT] & 1 << (sizeof(char) * CHAR_BIT - i % (sizeof(char) * CHAR_BIT) - 1)) continue;
      for (size_t j = i + 1; j < bitfields_count_ * sizeof(char) * CHAR_BIT; ++j) {
        if (bitfields_[j / sizeof(char) / CHAR_BIT] & 1 << (sizeof(char) * CHAR_BIT - j % (sizeof(char) * CHAR_BIT) - 1)) {
          i = j;
          break;
        }
        if (j - i + 1 >= blocks_needed) {
          index = i;
          goto loop_break;
        }
      }
    }
    loop_break:
    if (index == static_cast<size_t>(-1)) return nullptr;
    for (size_t i = 0; i < blocks_needed; ++i) {
      bitfields_[(index + i) / sizeof(char) / CHAR_BIT] |= 1 << (sizeof(char) * CHAR_BIT - ((index + i) % (sizeof(char) * CHAR_BIT)) - 1);
    }
    return data_ + BlockSize * index;
  }
  void Deallocate(void* pointer, size_t size_bytes) {
    if (pointer == nullptr || pointer < data_ || pointer > data_ + blocks_count_ * BlockSize) return;
    size_t blocks = size_bytes / BlockSize + (((size_bytes / BlockSize) * BlockSize) < size_bytes ? 1 : 0);
    size_t index = (reinterpret_cast<std::size_t>(pointer) - reinterpret_cast<std::size_t>(data_)) / BlockSize;
    for (size_t i = 0; i <= blocks && index + i < blocks_count_; ++i) {
      bitfields_[(index + i) / sizeof(char) / CHAR_BIT] &= ~(1 << (sizeof(char) * CHAR_BIT - ((index + i) % (sizeof(char) * CHAR_BIT)) - 1));
    }
  }
  template<typename T, typename... Args>
  [[nodiscard]] T* Allocate(size_t count, Args&&... args) {
    T* pointer = reinterpret_cast<T*>(Allocate(sizeof(T) * count));
    if (pointer == nullptr) return pointer;
    for (size_t i = 0; i < count; ++i) {
      T* ptr = pointer + i;
      ptr = new(ptr) T(std::forward<Args>(args)...);
    }
    return pointer;
  }
  template<typename T>
  void Deallocate(T* pointer, size_t count) {
    if (pointer == nullptr || pointer < data_ || pointer > data_ + blocks_count_ * BlockSize) return;
    for (size_t i = 0; i < count; ++i) {
      (pointer + i)->~T();
    }
    Deallocate(reinterpret_cast<void*>(pointer), sizeof(T) * count);
  }

private:
  void* data_;
  size_t blocks_count_;
  char* bitfields_;
  size_t bitfields_count_;
};
}

#endif