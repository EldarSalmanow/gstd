#ifndef GSTD_REF_H
#define GSTD_REF_H

#include <type_traits>

#include <gstd/Type/Types.h>

namespace gstd {

    namespace detail {

        template<typename ValueT>
        static GSTD_CONSTEXPR ValueT &FUNC(ValueT &value) GSTD_NOEXCEPT {
            return value;
        }

        template<typename ValueT>
        static void FUNC(ValueT &&) = delete;

    }

    template<typename ValueT>
    class Ref {
    public:

        using ValueType = ValueT;

        using ReferenceType = ValueType &;

    public:

        template<typename InputValueT,
                 typename = decltype(detail::FUNC<ValueType>(std::declval<InputValueT>()),
                                     EnableIfT<!IsSameV<Ref,
                                                        std::remove_cvref_t<InputValueT>>>())>
        GSTD_CONSTEXPR Ref(InputValueT &&value)
        noexcept(noexcept(detail::FUNC<ValueType>(Forward<InputValueT>(value))))
                : _value(std::addressof(detail::FUNC<ValueType>(Forward<InputValueT>(value)))) {}

        GSTD_CONSTEXPR Ref(const Ref<ValueType> &reference) GSTD_NOEXCEPT
                : _value(reference._value) {}

    public:

        GSTD_CONSTEXPR auto Get() const GSTD_NOEXCEPT -> ReferenceType {
            return *_value;
        }

    public:

        GSTD_CONSTEXPR auto operator=(const Ref<ValueType> &reference) GSTD_NOEXCEPT -> Ref<ValueType> & {
            if (this == &reference) {
                return *this;
            }

            _value = reference._value;

            return *this;
        }

        GSTD_CONSTEXPR operator ReferenceType() const GSTD_NOEXCEPT {
            return Get();
        }

        template<typename... ArgumentsT>
        GSTD_CONSTEXPR auto operator()(ArgumentsT &&...arguments) const
        noexcept(std::is_nothrow_invocable_v<ReferenceType,
                                             ArgumentsT &&...>) -> std::invoke_result_t<ReferenceType,
                                                                                        ArgumentsT &&...> {
            return std::invoke(Get(),
                               std::forward<ArgumentsT>(arguments)...);
        }

    private:

        ValueType *_value;
    };

    template<typename ValueType>
    Ref(ValueType &) -> Ref<ValueType>;

//    GSTD_CONSTEXPR auto MakeRef() GSTD_NOEXCEPT {
//        return Ref<VType>;
//    }

    class T {
    public:

        T(int f);

        T(const T &) = delete;
        T(T &&t) = delete;

        int k();
    };

    int j(T &&h) {
        return h.k();
    }

    void g() {
        auto r = std::reference_wrapper(j);
        int l = 10;
        T t(1);
        decltype(int, void);
        r(std::move(t));
        auto ref = Ref(j);
        ref(std::move(t));
    }

}

#endif //GSTD_REF_H
