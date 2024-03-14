#ifndef GSTD_TINDEXER_H
#define GSTD_TINDEXER_H

namespace gstd {

    template<std::uint64_t IndexV,
             typename T,
             typename... Types>
    struct TIndexer {

        using Type = typename TIndexer<IndexV - 1,
                                       Types...>::Type;

    };

    template<typename T,
             typename... Types>
    struct TIndexer<0,
                    T,
                    Types...> {

        using Type = T;

    };

    template<std::uint64_t IndexV,
             typename... Types>
    using TIndexerT = typename TIndexer<IndexV,
                                        Types...>::Type;

}

#endif //GSTD_TINDEXER_H
