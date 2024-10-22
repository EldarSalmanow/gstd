#ifndef BITSET_H
#define BITSET_H

namespace gstd {

    template<std::uint64_t SizeV>
    class Bitset {
    public:

        using SizeType = std::uint64_t;

        using IndexType = std::uint64_t;

        static GSTD_CONSTEXPR SizeType SizeValue = SizeV;

    private:

        static GSTD_CONSTEXPR SizeType SizeInBytes = (SizeValue % 8 == 0) ? SizeValue / 8 : (SizeValue - (SizeValue % 8)) / 8 + 1;

    public:

        Bitset() = default;

    public:

        // GSTD_CONSTEXPR auto operator[](std::uint64_t index) ->  {

        // }

    private:

        bool bits[SizeInBytes];
    };

    class DynamicBitset {

    };

}


#endif //BITSET_H
