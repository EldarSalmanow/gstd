#ifndef GSTD_TMAP_H
#define GSTD_TMAP_H

namespace gstd {

    // TypeMap<Pair<Key1, Val1>, Pair<Key2, Val2>>

    template<typename FirstT,
             typename SecondT>
    struct TPair {

        using FirstType = FirstT;

        using SecondType = SecondT;

    };

    template<std::uint64_t IndexV,
             typename PairT>
    struct TPairElement {

    };

    template<std::uint64_t IndexV,
             typename FirstT,
             typename SecondT>
    struct TPairElement<IndexV,
                        TPair<FirstT,
                              SecondT>> {

        GSTD_INLINE static GSTD_CONSTEXPR auto IndexValue = IndexV;

        using Type = typename TIndexer<IndexValue,
                                       FirstT,
                                       SecondT>::Type;

    };

//    template<typename FirstT,
//             typename PairT>
//    struct TPairSearch {
//
//    };
//
//    template<typename FirstT,
//             typename FirstPairT,
//             typename SecondPairT>
//    struct TPairSearch<FirstT,
//                       TypePair<FirstPairT,
//                                SecondPairT>> {
//
//    };
//
//    template<typename FirstT,
//             typename SecondPairT>
//    struct TPairSearch<FirstT,
//                       TypePair<FirstT,
//                                SecondPairT>> {
//
//        using FirstType = FirstT;
//
//        using SecondType = SecondPairT;
//
//        using Type = SecondType;
//
//    };

    template<typename PairT,
             typename... Pairs>
    struct TMap : TMap<Pairs...> {

        using PairType = PairT;

    };

    template<typename PairT>
    struct TMap<PairT> {

        using PairType = PairT;

    };

    template<typename FirstT,
             typename PairT,
             typename... Pairs>
    struct TMapSearch {

        using FirstType = FirstT;

        using SecondType = std::conditional_t<std::is_same_v<FirstT,
                                                             typename PairT::FirstType>,
                                              typename PairT::SecondType,
                                              typename TMapSearch<FirstT,
                                                                  Pairs...>::Type>;

        using Type = SecondType;

    };

    template<typename FirstT,
             typename PairT>
    struct TMapSearch<FirstT,
                      PairT> {

        using FirstType = FirstT;

        using SecondType = std::enable_if_t<std::is_same_v<FirstT,
                                                           typename PairT::FirstType>,
                                            typename PairT::SecondType>;

        using Type = SecondType;

    };

    template<typename T,
             typename MapT>
    struct TMapElement {

    };

    template<typename FirstT,
             typename... Pairs>
    struct TMapElement<FirstT,
                       TMap<Pairs...>> {

        using FirstType = FirstT;

        using SecondType = typename TMapSearch<FirstT,
                                               Pairs...>::Type;

        using Type = SecondType;

    };

    void e() {
        TMapSearch<double, TPair<void, float>, TPair<double, int>, TPair<int, double>>::Type t = 0.31;
        static_assert(std::is_same_v<double, decltype(t)>);

        TMapElement<double, TMap<TPair<int, void>, TPair<double, const float>, TPair<const unsigned long *, std::string *>>>::Type p = 0.0;
        static_assert(std::is_same_v<decltype(p), const float>);
    }

}

#endif //GSTD_TMAP_H
