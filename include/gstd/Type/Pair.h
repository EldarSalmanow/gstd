#ifndef GSTD_PAIR_H
#define GSTD_PAIR_H

#include <gstd/Macro/Macro.h>

namespace gstd {

    /**
     *
     * @tparam FirstT
     * @tparam SecondT
     */
    template<typename FirstT,
             typename SecondT>
    class Pair {
    public:

        /*
         *
         * Pair PUBLIC TYPES
         *
         */

        /**
         *
         */
        using FirstType = FirstT;

        /**
         *
         */
        using SecondType = SecondT;

    private:

        /*
         *
         * Pair PUBLIC TYPES
         *
         */

        /**
         *
         */
        using IndexType = std::uint64_t;

    private:

        /*
         *
         * Pair PUBLIC ASSERTION
         *
         */

        static_assert(std::is_unsigned_v<IndexType>,
                      "`IndexType` must be unsigned type!");

    public:

        /*
         *
         * Pair PUBLIC CONSTRUCTORS
         *
         */

        GSTD_CONSTEXPR Pair()
        GSTD_NOEXCEPT(std::conjunction_v<std::is_nothrow_default_constructible<FirstType>,
                                         std::is_nothrow_default_constructible<SecondType>>)
                : _first(),
                  _second() {}

        GSTD_CONSTEXPR Pair(FirstType &&first,
                            SecondType &&second) GSTD_NOEXCEPT
                : _first(std::move(first)),
                  _second(std::move(second)) {}

        GSTD_CONSTEXPR Pair(const Pair &pair)
        GSTD_NOEXCEPT(std::conjunction_v<std::is_nothrow_copy_constructible<FirstType>,
                                         std::is_nothrow_copy_constructible<SecondType>>)
                : _first(pair._first),
                  _second(pair._second) {}

        GSTD_CONSTEXPR Pair(Pair &&pair) GSTD_NOEXCEPT
                : _first(std::move(pair._first)),
                  _second(std::move(pair._second)) {}

    public:

        /*
         *
         * Pair PUBLIC STATIC CREATE METHODS
         *
         */

        static GSTD_CONSTEXPR auto New() GSTD_NOEXCEPT(GSTD_NOEXCEPT(Pair {})) -> Pair {
            return Pair {};
        }

        static GSTD_CONSTEXPR auto New(FirstType &&first,
                                       SecondType &&second)
        GSTD_NOEXCEPT(GSTD_NOEXCEPT(Pair {
            std::declval<FirstType>(),
            std::declval<SecondType>()
        })) -> Pair {
            return Pair {
                std::forward<FirstType>(first),
                std::forward<SecondType>(second)
            };
        }

    public:

        /*
         *
         * Pair PUBLIC METHODS
         *
         */

        template <IndexType Index>
        GSTD_CONSTEXPR auto get() & GSTD_NOEXCEPT -> std::tuple_element_t<Index,
                                                                          Pair> & {
            static_assert(Index < 2,
                          "`Index` must be in [0, 1] range!");

            if GSTD_CONSTEXPR (Index == 0) {
                return _first;
            } else if GSTD_CONSTEXPR (Index == 1) {
                return _second;
            }
        }

        template <IndexType Index>
        GSTD_CONSTEXPR auto get() const & GSTD_NOEXCEPT -> const std::tuple_element_t<Index,
                                                                                      Pair> & {
            static_assert(Index < 2,
                          "`Index` must be in [0, 1] range!");

            if GSTD_CONSTEXPR (Index == 0) {
                return _first;
            } else if GSTD_CONSTEXPR (Index == 1) {
                return _second;
            }
        }

        template <IndexType Index>
        GSTD_CONSTEXPR auto get() && GSTD_NOEXCEPT -> std::tuple_element_t<Index,
                                                                           Pair> && {
            static_assert(Index < 2,
                          "`Index` must be in [0, 1] range!");

            if GSTD_CONSTEXPR (Index == 0) {
                return std::move(_first);
            } else if GSTD_CONSTEXPR (Index == 1) {
                return std::move(_second);
            }
        }

        template <IndexType Index>
        GSTD_CONSTEXPR auto get() const && GSTD_NOEXCEPT -> const std::tuple_element_t<Index,
                                                                                       Pair> && {
            static_assert(Index < 2,
                          "`Index` must be in [0, 1] range!");

            if GSTD_CONSTEXPR (Index == 0) {
                return std::move(_first);
            } else if GSTD_CONSTEXPR (Index == 1) {
                return std::move(_second);
            }
        }

    public:

        /*
         *
         * Pair PUBLIC GETTER METHODS
         *
         */

        GSTD_CONSTEXPR auto First() & GSTD_NOEXCEPT -> FirstType & {
            return _first;
        }

        GSTD_CONSTEXPR auto First() const & GSTD_NOEXCEPT -> const FirstType & {
            return _first;
        }

        GSTD_CONSTEXPR auto First() && GSTD_NOEXCEPT -> FirstType && {
            return _first;
        }

        GSTD_CONSTEXPR auto First() const && GSTD_NOEXCEPT -> const FirstType && {
            return _first;
        }

        GSTD_CONSTEXPR auto Second() & GSTD_NOEXCEPT -> SecondType {
            return _second;
        }

        GSTD_CONSTEXPR auto Second() const & GSTD_NOEXCEPT -> const SecondType {
            return _second;
        }

        GSTD_CONSTEXPR auto Second() && GSTD_NOEXCEPT -> SecondType && {
            return _second;
        }

        GSTD_CONSTEXPR auto Second() const && GSTD_NOEXCEPT -> const SecondType && {
            return _second;
        }

    public:

        /*
         *
         * Pair PUBLIC OPERATOR METHODS
         *
         */

        GSTD_CONSTEXPR auto operator=(const Pair &pair)
        GSTD_NOEXCEPT(std::conjunction_v<std::is_nothrow_copy_assignable<FirstType>,
                                         std::is_nothrow_copy_assignable<SecondType>>) -> Pair & {
            if (this == &pair) {
                return *this;
            }

            _first = pair._first;
            _second = pair._second;

            return *this;
        }

        GSTD_CONSTEXPR auto operator=(Pair &&pair) GSTD_NOEXCEPT -> Pair & {
            if (this == &pair) {
                return *this;
            }

            _first = std::move(pair._first);
            _second = std::move(pair._second);

            return *this;
        }

        GSTD_CONSTEXPR auto operator==(const Pair &pair) const GSTD_NOEXCEPT -> bool {
            return _first == pair._first
                && _second == pair._second;
        }

    private:

        /*
         *
         * Pair PRIVATE FIELDS
         *
         */

        FirstType _first;

        SecondType _second;
    };

    /**
     * @tparam FirstT
     * @tparam SecondT
     */
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
                                       SecondT> &pair) GSTD_NOEXCEPT -> std::tuple_element_t<Index,
                                                                                             gstd::Pair<FirstT,
                                                                                                        SecondT>> & {
        return pair.template get<Index>();
    }

    template<size_t Index,
            typename FirstT,
            typename SecondT>
    GSTD_CONSTEXPR auto get(const gstd::Pair<FirstT,
                                             SecondT> &pair) GSTD_NOEXCEPT -> const std::tuple_element_t<Index,
                                                                                                         gstd::Pair<FirstT,
                                                                                                                    SecondT>> & {
        return pair.template get<Index>();
    }

    template<size_t Index,
            typename FirstT,
            typename SecondT>
    GSTD_CONSTEXPR auto get(gstd::Pair<FirstT,
                                       SecondT> &&pair) GSTD_NOEXCEPT -> std::tuple_element_t<Index,
                                                                                              gstd::Pair<FirstT,
                                                                                                         SecondT>> && {
        return pair.template get<Index>();
    }

    template<size_t Index,
            typename FirstT,
            typename SecondT>
    GSTD_CONSTEXPR auto get(const gstd::Pair<FirstT,
                                             SecondT> &&pair) GSTD_NOEXCEPT -> const std::tuple_element_t<Index,
                                                                                                          gstd::Pair<FirstT,
                                                                                                                     SecondT>> && {
        return pair.template get<Index>();
    }

}

#endif //GSTD_PAIR_H
