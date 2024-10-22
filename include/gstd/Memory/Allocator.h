#ifndef GSTD_ALLOCATOR_H
#define GSTD_ALLOCATOR_H

#include <gstd/Containers/Span.h>

namespace gstd {

    using Byte = std::uint8_t;

    class Allocator {
    public:

        using ValueType = Byte;

        using PointerType = ValueType *;

        using SizeType = std::uint64_t;

        using AlignmentType = std::uint64_t;

    public:

        GSTD_CONSTEXPR GSTD_EXPLICIT Allocator(Span<Byte> region) GSTD_NOEXCEPT
                : _region(region) {}

    public:

        virtual GSTD_CONSTEXPR ~Allocator() GSTD_NOEXCEPT = default;

    public:

        template<typename InputValueT>
        GSTD_CONSTEXPR auto Allocate(const SizeType count = 1,
                                     const AlignmentType align = sizeof(std::max_align_t)) -> InputValueT * {
            if (std::numeric_limits<SizeType>::max() / sizeof(InputValueT) < count) {
                Panic("Allocator::Allocate() called too small");
            }

            return ReinterpretCast<InputValueT *>(DoAllocate(count * sizeof(InputValueT),
                                                             align));
        }

        template<typename InputValueT>
        GSTD_CONSTEXPR auto Deallocate(InputValueT *pointer,
                                       const SizeType count = 1,
                                       const AlignmentType align = sizeof(std::max_align_t)) -> void {
            if (std::numeric_limits<SizeType>::max() / sizeof(InputValueT) < count) {
                Panic("Allocator::Deallocate() called too small");
            }

            return DoDeallocate(ReinterpretCast<PointerType>(pointer),
                                count * sizeof(InputValueT),
                                align);
        }

    protected:

        virtual GSTD_CONSTEXPR auto DoAllocate(SizeType size,
                                               AlignmentType align) -> PointerType = 0;

        virtual GSTD_CONSTEXPR auto DoDeallocate(PointerType pointer,
                                                 SizeType size,
                                                 AlignmentType align) -> void = 0;

        GSTD_CONSTEXPR auto Region() GSTD_NOEXCEPT -> Span<Byte> {
            return _region;
        }

    private:

        Span<Byte> _region;
    };

    class FreeListAllocator : public Allocator {
    public:

        GSTD_CONSTEXPR GSTD_EXPLICIT FreeListAllocator(Span<Byte> region) GSTD_NOEXCEPT
                : Allocator(region) {}

    public:

        static GSTD_CONSTEXPR auto New(Span<Byte> region) GSTD_NOEXCEPT -> FreeListAllocator {
            return FreeListAllocator {
                region
            };
        }

    protected:

        GSTD_CONSTEXPR auto DoAllocate(SizeType size,
                                       AlignmentType align) -> PointerType override;

        GSTD_CONSTEXPR auto DoDeallocate(PointerType pointer,
                                         SizeType size,
                                         AlignmentType align) -> void override;
    };

    static auto DefaultAllocator() -> Allocator *;

}

#endif //GSTD_ALLOCATOR_H
