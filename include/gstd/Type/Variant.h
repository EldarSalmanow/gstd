#ifndef GSTD_VARIANT_H
#define GSTD_VARIANT_H

#include <gstd/Meta/Meta.h>

#include <variant>

namespace gstd {

    template<typename... Types>
    class Variant;

    template<size_t IndexV,
             typename T>
    struct VariantElement {

    };

    template<size_t IndexV,
             typename... Types>
    struct VariantElement<IndexV,
                          Variant<Types...>> {

        using Type = TIndexerT<IndexV,
                               Types...>;

    };

    template<size_t IndexV,
             typename T>
    using VariantElementT = typename VariantElement<IndexV,
                                                    T>::Type;

    template<typename... Types>
    struct VariantStorage;

    template<typename T,
             typename... Types>
    struct VariantStorage<T,
                          Types...> {

        template<size_t IndexV>
        GSTD_CONSTEXPR auto Get() & -> VariantElementT<IndexV,
                                                       Variant<T,
                                                               Types...>> & {
            if constexpr (IndexV == 0) {
                return value;
            } else {
                return tail.template Get<IndexV - 1>();
            }
        }

        template<size_t IndexV>
        GSTD_CONSTEXPR auto Get() const & -> const VariantElementT<IndexV,
                                                                   Variant<T,
                                                                           Types...>> & {
            if constexpr (IndexV == 0) {
                return value;
            } else {
                return tail.template Get<IndexV - 1>();
            }
        }

        template<size_t IndexV>
        GSTD_CONSTEXPR auto Get() && -> VariantElementT<IndexV,
                                                        Variant<T,
                                                                Types...>> && {
            if constexpr (IndexV == 0) {
                return value;
            } else {
                return tail.template Get<IndexV - 1>();
            }
        }

        template<size_t IndexV>
        GSTD_CONSTEXPR auto Get() const && -> const VariantElementT<IndexV,
                                                                    Variant<T,
                                                                            Types...>> && {
            if constexpr (IndexV == 0) {
                return value;
            } else {
                return tail.template Get<IndexV - 1>();
            }
        }

        union {
            T value;

            VariantStorage<Types...> tail;
        };

    };

    template<typename T>
    struct VariantStorage<T> {

        template<size_t IndexV>
        GSTD_CONSTEXPR auto Get() & -> T & {
            static_assert(IndexV == 0);

            return value;
        }

        template<size_t IndexV>
        GSTD_CONSTEXPR auto Get() const & -> const T &{
            static_assert(IndexV == 0);

            return value;
        }

        template<size_t IndexV>
        GSTD_CONSTEXPR auto Get() && -> T && {
            static_assert(IndexV == 0);

            return value;
        }

        template<size_t IndexV>
        GSTD_CONSTEXPR auto Get() const && -> const T && {
            static_assert(IndexV == 0);

            return value;
        }

        union {
            T value;
        };

    };

    template<typename... Types>
    class Variant;

    template<typename... Types>
    class Variant : private VariantStorage<Types...> {
    public:

        using Storage = VariantStorage<Types...>;

    public:

        GSTD_CONSTEXPR GSTD_EXPLICIT Variant()
                : _index(-1) {}

        template<typename T>
        GSTD_CONSTEXPR GSTD_EXPLICIT Variant(T &&value) {
            GSTD_CONSTEXPR auto Index = IndexOfV<T, Types...>;

            Storage::template Get<Index>() = std::forward<T>(value);

            _index = Index;
        }

    public:

        template<typename T>
        GSTD_CONSTEXPR auto Get() & -> T & {
            if (!HasValue<T>()) {
                //
            }

            return Storage::template Get<TIndexOfV<T,
                                                   Types...>>();
        }

        template<typename T>
        GSTD_CONSTEXPR auto Get() const & -> const T & {
            if (!HasValue<T>()) {
                //
            }

            return Storage::template Get<TIndexOfV<T,
                                                   Types...>>();
        }

        template<typename T>
        GSTD_CONSTEXPR auto Get() && -> T && {
            if (!HasValue<T>()) {
                //
            }

            return Storage::template Get<TIndexOfV<T,
                                                   Types...>>();
        }

        template<typename T>
        GSTD_CONSTEXPR auto Get() && -> const T && {
            if (!HasValue<T>()) {
                //
            }

            return Storage::template Get<TIndexOfV<T,
                                                   Types...>>();
        }

        GSTD_CONSTEXPR auto Index() -> size_t {
            return _index;
        }

        template<typename T>
        GSTD_CONSTEXPR auto HasValue() const -> bool {
            return TIndexOfV<T, Types...> == _index;
        }

    private:

        size_t _index;
    };

    void t() {
        Variant<int, float, double> y;
        y.Get<int>() = 10;

        std::variant<float, int> v;
        v.index();
    }

}

#endif //GSTD_VARIANT_H
