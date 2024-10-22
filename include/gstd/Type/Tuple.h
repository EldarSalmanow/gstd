#ifndef GSTD_TUPLE_H
#define GSTD_TUPLE_H

#include <gstd/Meta/Meta.h>

namespace gstd {

    template<typename... Types>
    class Tuple;

    template<typename T,
             typename... Types>
    class Tuple<T,
                Types...> : public Tuple<Types...> {
    public:

        using Type = T;

    public:

        GSTD_CONSTEXPR Tuple(Type &&value,
                             Types &&...values)
                : _value(std::forward<Type>(value)),
                  Tuple<Types...>(std::forward<Types>(values)...) {}

    public:

        GSTD_CONSTEXPR auto Value() & -> Type & {
            return _value;
        }

        GSTD_CONSTEXPR auto Value() const & -> const Type & {
            return _value;
        }

        GSTD_CONSTEXPR auto Value() && -> Type && {
            return std::move(_value);
        }

        GSTD_CONSTEXPR auto Value() const && -> const Type && {
            return std::move(_value);
        }

    private:

        Type _value;
    };

    template<>
    struct Tuple<> {

    };

    template<typename... Types>
    GSTD_CONSTEXPR auto MakeTuple(Types &&...values) -> Tuple<Types...> {
        return Tuple<Types...> {
            std::forward<Types>(values)...
        };
    }

    template<size_t IndexV,
             typename... Types>
    struct TupleType;

    template<size_t IndexV,
             typename T,
             typename... Types>
    struct TupleType<IndexV,
                     T,
                     Types...> {

        using Type = typename TupleType<IndexV - 1,
                                        Types...>::Type;

    };

    template<typename T,
             typename... Types>
    struct TupleType<0,
                     T,
                     Types...> {

        using Type = Tuple<T,
                           Types...>;

    };

    // TODO
    template<>
    struct TupleType<0> {

        using Type = Tuple<>;

    };

    template<size_t IndexV,
             typename... Types>
    using TupleTypeT = typename TupleType<IndexV,
                                          Types...>::Type;

    template<size_t IndexV,
             typename T>
    struct TupleElement {

    };

    template<size_t IndexV,
             typename... Types>
    struct TupleElement<IndexV,
                        Tuple<Types...>> {

        using Type = TIndexerT<IndexV,
                               Types...>;

    };

    template<size_t IndexV,
             typename T>
    using TupleElementT = typename TupleElement<IndexV,
                                                T>::Type;

    template<typename T>
    struct TupleSize {

    };

    template<typename... Types>
    struct TupleSize<Tuple<Types...>> {

        inline static constexpr size_t Value = TCounterV<Types...>;

    };

    template<typename TupleT>
    inline constexpr size_t TupleSizeV = TupleSize<TupleT>::Value;

    template<size_t IndexV,
             typename... Types>
    GSTD_CONSTEXPR auto Get(Tuple<Types...> &tuple) -> TupleElementT<IndexV,
                                                                     Tuple<Types...>> & {
        return static_cast<TupleTypeT<IndexV,
                                      Types...> &>(tuple).Value();
    }

    template<size_t IndexV,
             typename... Types>
    GSTD_CONSTEXPR auto Get(const Tuple<Types...> &tuple) -> const TupleElementT<IndexV,
                                                                                 Tuple<Types...>> & {
        return static_cast<const TupleTypeT<IndexV,
                                            Types...> &>(tuple).Value();
    }

    template<size_t IndexV,
             typename... Types>
    GSTD_CONSTEXPR auto Get(Tuple<Types...> &&tuple) -> TupleElementT<IndexV,
                                                                      Tuple<Types...>> && {
        return static_cast<TupleTypeT<IndexV,
                                      Types...> &&>(tuple).Value();
    }

    template<size_t IndexV,
             typename... Types>
    GSTD_CONSTEXPR auto Get(const Tuple<Types...> &&tuple) -> const TupleElementT<IndexV,
                                                                                  Tuple<Types...>> && {
        return static_cast<const TupleTypeT<IndexV,
                                            Types...> &&>(tuple).Value();
    }

    std::tuple<int, int, float> f;
    void r() {
        constexpr Tuple<int, float, double> t(1, 1.0f, 32.04);
        constexpr auto r = Get<2>(t);
        static_assert(r == 32.04);
        std::tuple<> s;
        auto e = MakeTuple(1, 2, 3, "");
    }

}

#endif //GSTD_TUPLE_H
