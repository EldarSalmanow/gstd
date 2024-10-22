#ifndef GSTD_MEMORYSOURCE_H
#define GSTD_MEMORYSOURCE_H

#include <gstd/Containers/Span.h>

namespace gstd {

    // TODO: replace to Result`s ( Result<void, ErrorT> )

    class MemorySource {
    public:

        virtual auto AllocateRegion(std::uint64_t size) -> Span<Byte> = 0;

        virtual auto DeallocateRegion(const Span<Byte> &region) -> void = 0;
    };

    class VirtualMemorySource : public MemorySource {
    public:

        GSTD_CONSTEXPR VirtualMemorySource() GSTD_NOEXCEPT = default;

    public:

        static GSTD_CONSTEXPR auto New() GSTD_NOEXCEPT -> VirtualMemorySource {
            return VirtualMemorySource {};
        }

    public:

        auto AllocateRegion(std::uint64_t size) -> Span<Byte> override;

        auto DeallocateRegion(const Span<Byte> &region) -> void override;
    };

    template<std::uint64_t SizeV>
    class StackMemorySource : public MemorySource {
    public:

        GSTD_CONSTEXPR StackMemorySource() GSTD_NOEXCEPT = default;

    public:

        static GSTD_CONSTEXPR auto New() GSTD_NOEXCEPT -> StackMemorySource {
            return StackMemorySource {};
        }

    public:

        auto AllocateRegion(std::uint64_t size) -> Span<Byte> override {
            return MakeSpan(_buffer,
                            size);
        }

        auto DeallocateRegion(const Span<Byte> &region) -> void override {

        }

    private:

        std::uint8_t _buffer[SizeV];
    };

}

#endif //GSTD_MEMORYSOURCE_H
