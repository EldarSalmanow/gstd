#ifndef GSTD_PAIR_H
#define GSTD_PAIR_H

#include <tuple>

namespace gstd {

    template<typename FirstT,
             typename SecondT>
    class Pair {
    public:

        using FirstType = FirstT;

        using SecondType = SecondT;

    public:

        GSTD_CONSTEXPR Pair(FirstType &&first,
                            SecondType &&second)
                : _first(std::move(first)),
                  _second(std::move(second)) {}

        GSTD_CONSTEXPR Pair(const Pair<FirstType,
                                       SecondType> &pair)
                : _first(pair._first),
                  _second(pair._second) {}

        GSTD_CONSTEXPR Pair(Pair<FirstType,
                                 SecondType> &&pair) GSTD_NOEXCEPT
                : _first(std::move(pair._first)),
                  _second(std::move(pair._second)) {}

    public:

        static GSTD_CONSTEXPR auto New(FirstType &&first,
                                       SecondType &&second) -> Pair<FirstType,
                                                                    SecondType> {
            return Pair<FirstType,
                        SecondType> {
                std::forward<FirstType>(first),
                std::forward<SecondType>(second)
            };
        }

    public:

        GSTD_CONSTEXPR auto First() & -> FirstType & {
            return _first;
        }

        GSTD_CONSTEXPR auto First() const & -> const FirstType & {
            return _first;
        }

        GSTD_CONSTEXPR auto First() && -> FirstType && {
            return _first;
        }

        GSTD_CONSTEXPR auto First() const && -> const FirstType && {
            return _first;
        }

        GSTD_CONSTEXPR auto Second() & -> SecondType {
            return _second;
        }

        GSTD_CONSTEXPR auto Second() const & -> const SecondType {
            return _second;
        }

        GSTD_CONSTEXPR auto Second() && -> SecondType && {
            return _second;
        }

        GSTD_CONSTEXPR auto Second() const && -> const SecondType && {
            return _second;
        }

    public:

        template <size_t Index>
        GSTD_CONSTEXPR auto get() & -> std::tuple_element_t<Index,
                                                            gstd::Pair<FirstType,
                                                                       SecondType>> & {
            static_assert(Index >= 0 && Index < 2,
                          "`Index` must be in [0, 1] range for `gstd::Pair<FirstType, SecondType>`!");

            if GSTD_CONSTEXPR (Index == 0) {
                return _first;
            } else if GSTD_CONSTEXPR (Index == 1) {
                return _second;
            }
        }

        template <size_t Index>
        GSTD_CONSTEXPR auto get() const & -> const std::tuple_element_t<Index,
                                                                        gstd::Pair<FirstType,
                                                                                   SecondType>> & {
            static_assert(Index >= 0 && Index < 2,
                          "`Index` must be in [0, 1] range for `gstd::Pair<FirstType, SecondType>`!");

            if GSTD_CONSTEXPR (Index == 0) {
                return _first;
            } else if GSTD_CONSTEXPR (Index == 1) {
                return _second;
            }
        }

        template <size_t Index>
        GSTD_CONSTEXPR auto get() && -> std::tuple_element_t<Index,
                                                             gstd::Pair<FirstType,
                                                                        SecondType>> && {
            static_assert(Index >= 0 && Index < 2,
                          "`Index` must be in [0, 1] range for `gstd::Pair<FirstType, SecondType>`!");

            if GSTD_CONSTEXPR (Index == 0) {
                return std::move(_first);
            } else if GSTD_CONSTEXPR (Index == 1) {
                return std::move(_second);
            }
        }

        template <size_t Index>
        GSTD_CONSTEXPR auto get() const && -> const std::tuple_element_t<Index,
                                                                         gstd::Pair<FirstType,
                                                                                    SecondType>> && {
            static_assert(Index >= 0 && Index < 2,
                          "`Index` must be in [0, 1] range for `gstd::Pair<FirstType, SecondType>`!");

            if GSTD_CONSTEXPR (Index == 0) {
                return std::move(_first);
            } else if GSTD_CONSTEXPR (Index == 1) {
                return std::move(_second);
            }
        }

    public:

        GSTD_CONSTEXPR auto operator=(const gstd::Pair<FirstType,
                                                       SecondType> &pair) -> gstd::Pair<FirstType,
                                                                                        SecondType> & {
            if (this == &pair) {
                return *this;
            }

            _first = pair._first;
            _second = pair._second;

            return *this;
        }

        GSTD_CONSTEXPR auto operator=(gstd::Pair<FirstType,
                                                 SecondType> &&pair) GSTD_NOEXCEPT -> gstd::Pair<FirstType,
                                                                                                 SecondType> & {
            if (this == &pair) {
                return *this;
            }

            _first = std::move(pair._first);
            _second = std::move(pair._second);

            return *this;
        }

        GSTD_CONSTEXPR auto operator==(const gstd::Pair<FirstType,
                                                        SecondType> &pair) const -> bool {
            return _first == pair._first
                && _second == pair._second;
        }

    private:

        FirstType _first;

        SecondType _second;
    };

    template<typename FirstT,
             typename SecondT>
    Pair(FirstT,
         SecondT) -> Pair<FirstT,
                          SecondT>;

    template<typename FirstT,
             typename SecondT>
    GSTD_CONSTEXPR auto MakePair(FirstT &&first,
                                 SecondT &&second) -> Pair<FirstT,
                                                           SecondT> {
        return Pair<FirstT,
                    SecondT>::New(std::forward<FirstT>(first),
                                  std::forward<SecondT>(second));
    }

}

namespace std {

    template<typename FirstT,
             typename SecondT>
    struct tuple_size<gstd::Pair<FirstT,
                                 SecondT>> : integral_constant<size_t,
                                                               2> {};

    template<typename FirstT,
             typename SecondT>
    struct tuple_element<0,
                         gstd::Pair<FirstT,
                                    SecondT>> {

        using type = FirstT;

    };

    template<typename FirstT,
             typename SecondT>
    struct tuple_element<1,
                         gstd::Pair<FirstT,
                                    SecondT>> {

        using type = SecondT;

    };

    template<size_t Index,
            typename FirstT,
            typename SecondT>
    GSTD_CONSTEXPR auto get(gstd::Pair<FirstT,
                                       SecondT> &pair) -> std::tuple_element_t<Index,
                                                                               gstd::Pair<FirstT,
                                                                                          SecondT>> & {
        return pair.template get<Index>();
    }

    template<size_t Index,
            typename FirstT,
            typename SecondT>
    GSTD_CONSTEXPR auto get(const gstd::Pair<FirstT,
                                             SecondT> &pair) -> const std::tuple_element_t<Index,
                                                                                           gstd::Pair<FirstT,
                                                                                                      SecondT>> & {
        return pair.template get<Index>();
    }

    template<size_t Index,
            typename FirstT,
            typename SecondT>
    GSTD_CONSTEXPR auto get(gstd::Pair<FirstT,
                                       SecondT> &&pair) -> std::tuple_element_t<Index,
                                                                                gstd::Pair<FirstT,
                                                                                           SecondT>> && {
        return pair.template get<Index>();
    }

    template<size_t Index,
            typename FirstT,
            typename SecondT>
    GSTD_CONSTEXPR auto get(const gstd::Pair<FirstT,
                                             SecondT> &&pair) -> const std::tuple_element_t<Index,
                                                                                            gstd::Pair<FirstT,
                                                                                                       SecondT>> && {
        return pair.template get<Index>();
    }

}

#endif //GSTD_PAIR_H
