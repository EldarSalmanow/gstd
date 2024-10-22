#ifndef GSTD_REF_H
#define GSTD_REF_H

#include <type_traits>

namespace gstd {

    namespace detail {

        struct RefImpl {

            template<typename ValueT>
            static GSTD_CONSTEXPR ValueT &FUNC(ValueT &value) GSTD_NOEXCEPT {
                return value;
            }

            template<typename ValueT>
            static void FUNC(ValueT &&) = delete;

        };

    }

    /**
     * Wrapper for reference value
     * @tparam ValueT Value type
     */
    template<typename ValueT>
    class Ref {
    public:

        /*
         *
         * Ref PUBLIC TYPES
         *
         */

        /**
         * Value type
         */
        using ValueType = ValueT;

        /**
         * Reference type
         */
        using ReferenceType = ValueType &;

    public:

        /*
         *
         * Ref PUBLIC CONSTRUCTORS
         *
         */

        /**
         * Constructor for `Ref` from reference to value
         * @tparam InputValueT Input value type that convertible to `ValueType`
         * @param value Input value reference
         */
        template<typename InputValueT,
                 typename = decltype(detail::RefImpl::FUNC<ValueType>(std::declval<InputValueT>()),
                                     std::enable_if_t<!std::is_same_v<Ref,
                                                                      std::remove_cvref_t<InputValueT>>>())>
        GSTD_CONSTEXPR Ref(InputValueT &&value)
        GSTD_NOEXCEPT(GSTD_NOEXCEPT(detail::RefImpl::FUNC<ValueType>(std::declval<InputValueT>())))
                : _value(std::addressof(detail::RefImpl::FUNC<ValueType>(std::forward<InputValueT>(value)))) {}

        /**
         * Copy constructor for `Ref` from other `Ref`
         * @param reference Other `Ref` object
         */
        GSTD_CONSTEXPR Ref(const Ref<ValueType> &reference) GSTD_NOEXCEPT
                : _value(reference._value) {}

    public:

        /*
         *
         * Ref PUBLIC METHODS
         *
         */

        /**
         * Getting contained reference
         * @return Reference
         */
        GSTD_CONSTEXPR auto Get() const GSTD_NOEXCEPT -> ReferenceType {
            return *_value;
        }

    public:

        /*
         *
         * Ref PUBLIC OPERATORS
         *
         */

        /**
         * Copy assignment operator for other `Ref`
         * @param reference Other `Ref`
         * @return This `Ref` after assignment
         */
        GSTD_CONSTEXPR auto operator=(const Ref<ValueType> &reference) GSTD_NOEXCEPT -> Ref<ValueType> & {
            if (this == &reference) {
                return *this;
            }

            _value = reference._value;

            return *this;
        }

        /**
         * Call operator for `Ref` by input arguments
         * @tparam ArgumentsT Arguments type
         * @param arguments Arguments for calling
         * @return Result of calling
         */
        template<typename... ArgumentsT>
        GSTD_CONSTEXPR auto operator()(ArgumentsT &&...arguments)
        const GSTD_NOEXCEPT(std::is_nothrow_invocable_v<ReferenceType,
                                                        ArgumentsT &&...>) -> std::invoke_result_t<ReferenceType,
                                                                                                   ArgumentsT &&...> {
            return std::invoke(Get(),
                               std::forward<ArgumentsT>(arguments)...);
        }

        /**
         * Cast operator to reference type for `Ref`
         * @return Reference to value from wrapper
         */
        GSTD_CONSTEXPR operator ReferenceType() const GSTD_NOEXCEPT {
            return Get();
        }

    private:

        /*
         *
         * Ref PRIVATE FIELDS
         *
         */

        /**
         * Container for reference
         */
        ValueType *_value;
    };

    /**
     * Deduction rule for `Ref`
     */
    template<typename ValueType>
    Ref(ValueType &) -> Ref<ValueType>;

    /**
     * Make reference wrapper by lvalue reference
     * @tparam ValueT Value type
     * @param value Lvalue reference to value
     * @return Reference wrapper with reference
     */
    template<typename ValueT>
    GSTD_CONSTEXPR auto MakeRef(ValueT &value) GSTD_NOEXCEPT -> Ref<ValueT> {
        return Ref<ValueT> {
            value
        };
    }

    /**
     * Make const reference wrapper by const lvalue reference
     * @tparam ValueT Value type
     * @param value Const lvalue reference to value
     * @return Reference wrapper with const reference
     */
    template<typename ValueT>
    GSTD_CONSTEXPR auto MakeRef(const ValueT &value) GSTD_NOEXCEPT -> Ref<const ValueT> {
        return Ref<const ValueT> {
            value
        };
    }

    /**
     * Deleted reference wrapper factory method by rvalue reference
     * @tparam ValueT Value type
     * @param value Rvalue reference to value
     * @return Reference wrapper with reference
     */
    template<typename ValueT>
    GSTD_CONSTEXPR auto MakeRef(ValueT &&value) -> Ref<ValueT> = delete;

    /**
     * Deleted const reference wrapper factory method by const rvalue reference
     * @tparam ValueT Value type
     * @param value Const rvalue reference to value
     * @return Reference wrapper with const reference
     */
    template<typename ValueT>
    GSTD_CONSTEXPR auto MakeRef(const ValueT &&value) -> Ref<const ValueT> = delete;

    /**
     * Make const reference wrapper factory method by lvalue reference
     * @tparam ValueT Value type
     * @param value Lvalue reference to value
     * @return Reference wrapper with const reference
     */
    template<typename ValueT>
    GSTD_CONSTEXPR auto MakeCRef(ValueT &value) GSTD_NOEXCEPT -> Ref<const ValueT> {
        return Ref<const ValueT> {
            value
        };
    }

    /**
     * Make const reference wrapper factory method by const lvalue reference
     * @tparam ValueT Value type
     * @param value Const lvalue reference to value
     * @return Reference wrapper with const reference
     */
    template<typename ValueT>
    GSTD_CONSTEXPR auto MakeCRef(const ValueT &value) GSTD_NOEXCEPT -> Ref<const ValueT> {
        return Ref<const ValueT> {
            value
        };
    }

    /**
     * Deleted maker const reference wrapper factory method by rvalue reference
     * @tparam ValueT Value type
     * @param value Rvalue reference to value
     * @return Reference wrapper with const reference
     */
    template<typename ValueT>
    GSTD_CONSTEXPR auto MakeCRef(ValueT &&value) -> Ref<const ValueT> = delete;

    /**
     * Deleted const reference wrapper factory method by const rvalue reference
     * @tparam ValueT Value type
     * @param value Const rvalue reference to value
     * @return Reference wrapper with const reference
     */
    template<typename ValueT>
    GSTD_CONSTEXPR auto MakeCRef(const ValueT &&value) -> Ref<const ValueT> = delete;

}

#endif //GSTD_REF_H
