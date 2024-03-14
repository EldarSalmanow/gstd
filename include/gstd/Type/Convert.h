#ifndef GSTD_CONVERT_H
#define GSTD_CONVERT_H

#include <gstd/Type/Trait.h>
#include <string>

namespace gstd {

    template<typename ToT,
             typename FromT>
    GSTD_CONSTEXPR auto StaticCast(FromT value) GSTD_NOEXCEPT -> ToT {
        return static_cast<ToT>(value);
    }

    template<typename ToT,
             typename FromT>
    GSTD_CONSTEXPR auto ReinterpretCast(FromT value) GSTD_NOEXCEPT -> ToT {
        return reinterpret_cast<ToT>(value);
    }

    template<typename ToT,
             typename FromT>
    GSTD_CONSTEXPR auto ConstCast(FromT value) GSTD_NOEXCEPT -> ToT {
        return const_cast<ToT>(value);
    }

    template<typename ToT,
             typename FromT>
    GSTD_CONSTEXPR auto DynamicCast(FromT value) GSTD_NOEXCEPT -> ToT {
        return dynamic_cast<ToT>(value);
    }

    template<typename T>
    GSTD_CONSTEXPR auto Forward(RemoveReferenceT<T> &value) GSTD_NOEXCEPT -> T && {
        return static_cast<T &&>(value);
    }

    /**
     * @todo Update static_assert failure message
     */
    template<typename T>
    GSTD_CONSTEXPR auto Forward(RemoveReferenceT<T> &&value) GSTD_NOEXCEPT -> T && {
        static_assert(!IsLValueReferenceV<T>, "Bad Forward call!");

        return static_cast<T &&>(value);
    }

    template<typename T>
    GSTD_CONSTEXPR auto Move(T &&value) noexcept -> RemoveReferenceT<T> && {
        return static_cast<RemoveReferenceT<T> &&>(value);
    }

    namespace detail {

        template<typename ToT,
                 typename FromT>
        struct As {

            using ToType = ToT;

            using FromType = FromT;

            static auto Convert(FromType &&value) -> ToType {
                static_assert(std::is_convertible_v<FromType, ToType>,
                              "Can`t convert `FromType` type to `ToType` type!");

                return reinterpret_cast<ToType>(std::forward<FromType>(value));
            }

        };

        template<typename FromT>
        struct As<std::string, FromT> {

            using ToType = std::string;

            using FromType = FromT;

            static auto Convert(FromType &&value) -> ToType {
                return std::to_string(std::forward<FromType>(value));
            }

        };

    }

    template<typename CheckT,
            typename InputT>
    constexpr auto Is(InputT &&value) -> bool {
        // TODO
        return std::is_same_v<CheckT, InputT>
            || std::is_convertible_v<CheckT, InputT>;
    }

    template<typename ToT,
             typename FromT>
    auto As(FromT &&value) {
        return detail::As<ToT,
                          FromT>::Convert(std::forward<FromT>(value));
    }

    template<typename FromT>
    auto AsString(FromT &&value) -> std::string {
        return As<std::string,
                  FromT>(std::forward<FromT>(value));
    }

}

#endif //GSTD_CONVERT_H
