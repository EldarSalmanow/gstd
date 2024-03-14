#ifndef GSTD_TCOUNTER_H
#define GSTD_TCOUNTER_H

namespace gstd {

    template<typename... Types>
    struct TCounter;

    template<typename T,
             typename... Types>
    struct TCounter<T,
                    Types...> {

        GSTD_INLINE static GSTD_CONSTEXPR std::uint64_t Value = TCounter<Types...>::Value + 1;

    };

    template<>
    struct TCounter<> {

        GSTD_INLINE static GSTD_CONSTEXPR std::uint64_t Value = 0;

    };

    template<typename... Types>
    GSTD_INLINE GSTD_CONSTEXPR auto TCounterV = TCounter<Types...>::Value;

}

#endif //GSTD_TCOUNTER_H
