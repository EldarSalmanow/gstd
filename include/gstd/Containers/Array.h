#ifndef GSTD_ARRAY_H
#define GSTD_ARRAY_H

#include <gstd/Containers/Slice.h>
#include <gstd/Containers/Span.h>
#include <gstd/Type/Type.h>

namespace gstd {

    template<typename ValueT,
             std::uint64_t SizeV>
    class Array {
    public:

        using ValueType = ValueT;

        using SizeType = std::uint64_t;

        static constexpr SizeType SizeValue = SizeV;

        using Iterator = ValueType *;

        using ConstIterator = const ValueType *;

        using IndexType = std::uint64_t;

    public:

        GSTD_CONSTEXPR Array();

        GSTD_CONSTEXPR Array(InitializerList<ValueType> initializerList);

        GSTD_CONSTEXPR Array(const Array<ValueType,
                                         SizeValue> &array);

        GSTD_CONSTEXPR Array(Array<ValueType,
                                   SizeValue> &&array) GSTD_NOEXCEPT;

    public:

        GSTD_CONSTEXPR auto At(const IndexType &index) -> Optional<Ref<ValueType>>;

        GSTD_CONSTEXPR auto At(const IndexType &index) const -> Optional<Ref<ValueType>>;

    public:

        GSTD_CONSTEXPR auto Iter() -> Iterator;

        GSTD_CONSTEXPR auto Iter() const -> ConstIterator;

        GSTD_CONSTEXPR auto begin() -> Iterator;

        GSTD_CONSTEXPR auto end() -> Iterator;

        GSTD_CONSTEXPR auto begin() const -> ConstIterator;

        GSTD_CONSTEXPR auto end() const -> ConstIterator;

        GSTD_CONSTEXPR auto cbegin() const -> ConstIterator;

        GSTD_CONSTEXPR auto cend() const -> ConstIterator;

    public:

        GSTD_CONSTEXPR auto operator=(const Array &array) -> Array &;

        GSTD_CONSTEXPR auto operator=(Array &&array) noexcept -> Array &;

        GSTD_CONSTEXPR auto operator[](const IndexType &index) -> ValueType &;

        GSTD_CONSTEXPR auto operator[](const IndexType &index) const -> const ValueType &;

        GSTD_CONSTEXPR auto operator[](const Slice &slice) const -> Span<ValueType>; // TODO

    private:

        ValueType _values[SizeValue];
    };

}

#endif //GSTD_ARRAY_H
