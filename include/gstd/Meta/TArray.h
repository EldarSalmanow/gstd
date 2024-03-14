#ifndef GSTD_TARRAY_H
#define GSTD_TARRAY_H

#include <gstd/Meta/TIndexer.h>

namespace gstd {

    template<typename T,
             typename... Types>
    struct TArray : TArray<Types...> {

        using Type = T;

    };

    template<typename T>
    struct TArray<T> {

        using Type = T;

    };

    template<std::uint64_t IndexV,
             typename ArrayT>
    struct TArrayElement {

    };

    template<std::uint64_t IndexV,
            typename... Types>
    struct TArrayElement<IndexV,
                         TArray<Types...>> {

        GSTD_INLINE static GSTD_CONSTEXPR auto IndexValue = IndexV;

        using Type = typename TIndexer<IndexValue,
                                       Types...>::Type;

    };

}

#endif //GSTD_TARRAY_H
