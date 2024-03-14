#ifndef GSTD_ARENA_H
#define GSTD_ARENA_H

#include <gstd/Memory/Allocator.h>

namespace gstd {

    template<std::uint64_t SizeV>
    class Arena {
    public:

        using PointerType = void *;

        using SizeType = std::uint64_t;

        GSTD_INLINE static GSTD_CONSTEXPR SizeType SizeValue = SizeV;

    public:

        GSTD_CONSTEXPR auto Buffer() -> PointerType {
            return _buffer;
        }

        GSTD_CONSTEXPR auto UnusedMemory(SizeType size) -> PointerType {
            return _buffer;
        }

    private:

        std::uint8_t _buffer[SizeValue];

        std::unordered_map<std::uint8_t *, std::uint64_t> _used;
    };

    template<std::uint64_t ArenaSizeV>
    class ArenaAllocator {
    public:

        using ArenaSizeType = decltype(ArenaSizeV);

        GSTD_INLINE static GSTD_CONSTEXPR ArenaSizeType ArenaSizeValue = ArenaSizeV;

        using PointerType = void *;

        using SizeType = std::uint64_t;

        using AlignmentType = std::uint64_t;

    public:

        explicit ArenaAllocator(Arena<ArenaSizeValue> arena)
                : _arena(arena) {}

    public:

        GSTD_INLINE GSTD_CONSTEXPR auto Allocate(SizeType size,
                                                 AlignmentType alignment /* unused now*/) -> PointerType {
            auto freeMemory = _arena.UnusedMemory(size);

            auto ptr = new (freeMemory) std::uint8_t[size];

            return (PointerType) ptr;
        }

        GSTD_INLINE GSTD_CONSTEXPR auto Deallocate(PointerType pointer) -> void {
            auto ptr = (std::uint8_t *) pointer;

            delete[] ptr;
        }

    private:

        Arena<ArenaSizeValue> _arena;
    };

    template<typename AllocT>
    class MyVec {
    public:

        MyVec(AllocT allocator);

    public:

        void t() {
            auto ptr = (int *) _alloc.Allocate(sizeof(int), alignof(int));
            *ptr = 10;
        }

    private:

        AllocT _alloc;
    };

    void k() {
        Arena<64 * 1024> arena;
        ArenaAllocator<64 * 1024> allocator(arena);
        MyVec<ArenaAllocator<64 * 1024>> v(allocator);
        v.t();
    }

}

#endif //GSTD_ARENA_H
