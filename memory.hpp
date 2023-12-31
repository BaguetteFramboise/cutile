#ifndef CUTILE_MEMORY_HPP
#define CUTILE_MEMORY_HPP

#include "./memory.h"

template <typename T>
struct memory_view
{
    const T* data;
    u32      count;
};

template <typename DestT, typename SrcT>
force_inline memory_view<DestT> to_memory_view(memory_view<SrcT> src)
{
    constexpr b8 mul = sizeof(DestT) <= sizeof(SrcT);
    if constexpr (mul)
    {
        return memory_view<DestT>
        {
            .data = (const DestT*)src.data,
            .count = src.count * sizeof(SrcT) / sizeof(DestT)
        };
    }
    else
    {
        return memory_view<DestT>
        {
            .data = (const DestT*)src.data,
            .count = src.count / sizeof(DestT) / sizeof(SrcT)
        };
    }
}

template <typename DestT, typename SrcT, u32 ArraySize>
force_inline memory_view<DestT> to_memory_view(SrcT(&array)[ArraySize])
{
    return to_memory_view<DestT>(memory_view<SrcT>{.data = array, .count = ArraySize});
}

template <typename T>
force_inline void fill_memory(T* data, u32 count, T value);

template <typename T>
force_inline void copy_memory(T* data, const T* in, u32 count);

template <typename T>
force_inline void reverse_memory(T* data, u32 count);

template <typename T>
force_inline bool8 memory_equals(const T* lhs, const T* rhs, u32 count);

template <typename T>
force_inline T*      allocate(allocator* allocator, u64 count);
template <typename T>
force_inline void    deallocate(allocator* allocator, T* ptr);

template <typename T>
force_inline void fill_memory(T* data, u32 count, T value)
{
    for (u32 i = 0; i < count; ++i) data[i] = value;
}

template <typename T>
force_inline void copy_memory(T* data, const T* in, u32 count)
{
    for (u32 i = 0; i < count; ++i) data[i] = in[i];
}

template <typename T>
force_inline void reverse_memory(T* data, u32 count)
{
    u32 end = count * 0.5;
    for (u32 i = 0; i < end; ++i) 
    {
        u8 dump = data[i];
        u32 rhs_i = count - 1 - i;
        data[i] = data[rhs_i];
        data[rhs_i] = dump;
    }
}

template <typename T>
force_inline bool8 memory_equals(const T* lhs, const T* rhs, u32 count)
{
    for (u32 i = 0; i < count; ++i) if (lhs[i] != rhs[i]) return bool8_false;
    return bool8_true;
}

template <typename T>
force_inline T* allocate(allocator* allocator, u64 count)
{
    return (T*)(allocate(allocator, sizeof(T) * count));
}
template <typename T>
force_inline void deallocate(allocator* allocator, T* ptr)
{
    deallocate(allocator, (void*)ptr);
}

#endif // !CUTILE_MEMORY_HPP
