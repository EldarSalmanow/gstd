#ifndef GSTD_TINDEXOF_H
#define GSTD_TINDEXOF_H

namespace gstd {

    template<typename SearchT,
             typename T,
             typename... Types>
    struct TIndexOf {

        GSTD_INLINE static GSTD_CONSTEXPR std::uint64_t Value = TIndexOf<SearchT,
                                                                         Types...>::Value + 1;

    };

    template<typename SearchT,
             typename... Types>
    struct TIndexOf<SearchT,
                    SearchT,
                    Types...> {

        GSTD_INLINE static GSTD_CONSTEXPR std::uint64_t Value = 0;

    };

    template<typename SearchT,
             typename... Types>
    GSTD_INLINE GSTD_CONSTEXPR auto TIndexOfV = TIndexOf<SearchT,
                                                         Types...>::Value;

}

#endif //GSTD_TINDEXOF_H
