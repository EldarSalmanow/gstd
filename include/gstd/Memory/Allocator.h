#ifndef GSTD_ALLOCATOR_H
#define GSTD_ALLOCATOR_H

#include <gstd/Type/Types.h>

namespace gstd {

    template<typename AllocatorT>
    struct AllocatorTraits {

        using AllocatorType = AllocatorT;

        using PointerType = typename AllocatorType::PointerType;

        using SizeType = typename AllocatorType::SizeType;

        using AlignmentType = typename AllocatorType::AlignmentType;

        GSTD_INLINE static GSTD_CONSTEXPR auto Allocate(SizeType size,
                                                        AlignmentType alignment) -> PointerType {
            return AllocatorType::Allocate(size,
                                           alignment);
        }

        GSTD_INLINE static GSTD_CONSTEXPR auto Deallocate(PointerType buffer) -> void {
            return AllocatorType::Deallocate(buffer);
        }
    };

    class Allocator {
    public:

        /* using PointerType = void *; */

        /* using SizeType = std::uint64_t; */

        /* using AlignmentType = std::uint64_t; */

    public:

        /* GSTD_INLINE static GSTD_CONSTEXPR auto Allocate(SizeType size,
                                                           AlignmentType alignment) -> PointerType; */

        /* GSTD_INLINE static GSTD_CONSTEXPR auto Deallocate(PointerType pointer) -> void; */
    };

}

#endif //GSTD_ALLOCATOR_H
