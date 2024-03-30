#ifndef GSTD_TRAIT_H
#define GSTD_TRAIT_H

#include <gstd/Type/Types.h>

namespace gstd {

    template<bool Test,
             typename T = void>
    struct EnableIf {

    };

    template<typename T>
    struct EnableIf<true, T> {

        using Type = T;

    };

    template<bool Test,
             typename T = void>
    using EnableIfT = typename EnableIf<Test,
                                        T>::Type;

    template<typename T,
             T V>
    struct IntegralConstant {

        using Type = IntegralConstant<T,
                                      V>;

        using ValueType = T;

        static GSTD_CONSTEXPR ValueType Value = V;

        constexpr auto operator()() const GSTD_NOEXCEPT -> ValueType {
            return Value;
        }

        constexpr operator ValueType() const GSTD_NOEXCEPT {
            return Value;
        }

    };

    template<bool V>
    using BoolConstant = IntegralConstant<bool,
                                          V>;

    using TrueType = BoolConstant<true>;

    using FalseType = BoolConstant<false>;

    template<typename... Types>
    using VoidT = void;

    template<typename T,
             typename R>
    struct IsSame : FalseType {

    };

    template<typename T>
    struct IsSame<T, T> : TrueType {

    };

    template<typename T,
             typename R>
    GSTD_INLINE GSTD_CONSTEXPR auto IsSameV = IsSame<T,
                                                     R>::Value;

    template<typename T>
    GSTD_INLINE GSTD_CONSTEXPR bool IsLValueReferenceV = false;

    template<typename T>
    GSTD_INLINE GSTD_CONSTEXPR bool IsLValueReferenceV<T &> = true;

    template<typename T,
             typename R>
    GSTD_INLINE GSTD_CONSTEXPR bool IsConvertibleV = std::is_convertible_v<T,
                                                                           R>;

    template<typename T,
             typename = void>
    struct AddReference {

        using Type = T;

        using LValue = Type;

        using RValue = Type;

    };

    template<typename T>
    struct AddReference<T, VoidT<T &>> {

        using Type = T;

        using LValue = Type &;

        using RValue = Type &&;

    };

    template<typename T>
    struct AddLValueReference {

        using Type = typename AddReference<T>::LValue;

    };

    template<typename T>
    using AddLValueReferenceT = typename AddLValueReference<T>::Type;

    template<typename T>
    struct AddRValueReference {

        using Type = typename AddReference<T>::RValue;

    };

    template<typename T>
    using AddRValueReferenceT = typename AddRValueReference<T>::Type;

    template<typename T>
    struct RemoveReference {

        using Type = T;

    };

    template<typename T>
    struct RemoveReference<T &> {

        using Type = T;

    };

    template<typename T>
    struct RemoveReference<T &&> {

        using Type = T;

    };

    template<typename T>
    using RemoveReferenceT = typename RemoveReference<T>::Type;

    template<typename T>
    AddRValueReferenceT<T> DeclVal() GSTD_NOEXCEPT;

    template<typename T>
    struct InstantiateType {

        using Type = typename T::Type;

    };

    template<typename T>
    using InstantiateTypeT = typename InstantiateType<T>::Type;

    template<typename T>
    struct InstantiateValue {

        using Type = decltype(T::Value);

        GSTD_INLINE static GSTD_CONSTEXPR Type Value = T::Value;

    };

    template<typename T>
    using InstantiateValueT = typename InstantiateValue<T>::Type;

    template<typename T>
    GSTD_INLINE GSTD_CONSTEXPR InstantiateValueT<T> InstantiateValueV = InstantiateValue<T>::Value;

    template<typename T>
    struct LazyType {

        using Type = T;

    };

    template<auto V>
    struct LazyValue {

        using Type = decltype(V);

        GSTD_INLINE static GSTD_CONSTEXPR Type Value = V;

    };

    namespace detail {

        template<bool C,
                 typename T,
                 typename R>
        struct InstantiateIfImpl {

            using Type = InstantiateTypeT<T>;

        };

        template<typename T,
                 typename R>
        struct InstantiateIfImpl<false,
                                 T,
                                 R> {

            using Type = InstantiateTypeT<R>;

        };

    }

    template<typename C,
             typename T,
             typename R>
    struct InstantiateIf {

        using Type = typename detail::InstantiateIfImpl<InstantiateValueV<C>,
                                                        T,
                                                        R>::Type;

    };

    struct NoInstantiate {

    };

    template<typename C,
             typename T,
             typename R = NoInstantiate>
    using LazyInstantiateIfT = typename InstantiateIf<C,
                                                      T,
                                                      R>::Type;

    template<bool C,
             typename T,
             typename R = NoInstantiate>
    using InstantiateIfT = LazyInstantiateIfT<IntegralConstant<bool,
                                                               C>,
                                              T,
                                              R>;

}

#endif //GSTD_TRAIT_H
