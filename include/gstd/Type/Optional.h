#ifndef GSTD_OPTIONAL_H
#define GSTD_OPTIONAL_H

#include <gstd/Containers/String.h>
#include <gstd/Diagnostic/Diagnostic.h>
#include <gstd/Type/Pair.h>
#include <gstd/Type/Ref.h>
#include <gstd/Type/Result.h>

#include <fmt/format.h>

/*
 *
 * TODOL (New Project ???)
 *
 * TODO: {level:
 *              '1',
 *        location:
 *              [Optional::SomeFn],
 *        tags:
 *              'Optional',
 *        text:
 *              'Check the function type 'T''
 *        }
 *
 */

namespace gstd {

    /**
     * Wrapper class for some value
     * @tparam ValueT Value type
     */
    template<typename ValueT>
    class Some {
    public:

        /*
         *
         * Some PUBLIC TYPES
         *
         */

        /**
         * Value type
         */
        using ValueType = ValueT;

    public:

        /*
         *
         * Some PUBLIC CONSTRUCTORS
         *
         */

        /**
         * Main constructor for `Some` from any value
         * @param value Value
         */
        GSTD_CONSTEXPR GSTD_EXPLICIT Some(ValueType &&value) GSTD_NOEXCEPT
                : _value(std::move(value)) {}

    public:

        /*
         *
         * Some PUBLIC STATIC METHODS
         *
         */

        /**
         * Creating `Some` value
         * @param value Input value
         * @return `Some` value
         */
        static GSTD_CONSTEXPR auto New(ValueType &&value) GSTD_NOEXCEPT -> Some<ValueType> {
            return Some<ValueType> {
                std::forward<ValueType>(value)
            };
        }

    public:

        /*
         *
         * Some PUBLIC METHODS
         *
         */

        /**
         * Copying value and return copy of it
         * @return Copy of value
         */
        GSTD_CONSTEXPR auto Copy()
        const GSTD_NOEXCEPT(std::is_nothrow_copy_constructible_v<ValueType>) -> ValueType {
            return _value;
        }

        /**
         * Getting lvalue reference of value
         * @return Lvalue reference of value
         */
        GSTD_CONSTEXPR auto Ref() GSTD_NOEXCEPT -> ValueType & {
            return _value;
        }

        /**
         * Getting constant lvalue reference of value
         * @return Constant lvalue reference of value
         */
        GSTD_CONSTEXPR auto CRef() const GSTD_NOEXCEPT -> const ValueType & {
            return _value;
        }

        /**
         * Getting rvalue reference of value for moving
         * @return Rvalue reference of value
         */
        GSTD_CONSTEXPR auto Move() GSTD_NOEXCEPT -> ValueType && {
            return std::move(_value);
        }

        /**
         * Getting constant rvalue reference of value for moving
         * @return Constant rvalue reference of value
         */
        GSTD_CONSTEXPR auto CMove() const GSTD_NOEXCEPT -> const ValueType && {
            return std::move(_value);
        }

    public:

        /*
         *
         * Some PUBLIC OPERATORS
         *
         */

        /**
         * Equality compare operator for other `Some`
         * @tparam InputValueT Value type of input `Some`
         * @param some Input `Some` value
         * @return Is equal values in this and input `Some`s
         */
        template<typename InputValueT>
        GSTD_CONSTEXPR auto operator==(const Some<InputValueT> &some) const GSTD_NOEXCEPT {
            static_assert(std::equality_comparable_with<ValueType,
                                                        InputValueT>,
                          "`ValueType` and `InputValueT` must be comparable with '==' operator!");

            return CRef() == some.CRef();
        }

    private:

        /*
         *
         * Some PRIVATE FIELDS
         *
         */

        /**
         * Value
         */
        ValueType _value;
    };

    /**
     * Smart creating a `Some` value
     * @tparam ValueT Value type
     * @param value Value
     * @return `Some` value
     */
    template<typename ValueT>
    GSTD_CONSTEXPR auto MakeSome(ValueT &&value) GSTD_NOEXCEPT -> Some<ValueT> {
        return Some<ValueT>::New(std::forward<ValueT>(value));
    }

    /**
     * Wrapper class for none value
     */
    class None {
    public:

        /*
         *
         * None PUBLIC STATIC METHODS
         *
         */

        /**
         * Creating `None` value
         * @return `None` value
         */
        static GSTD_CONSTEXPR None New() GSTD_NOEXCEPT {
            return None {};
        }
    };

    /**
     * Smart creating a `None` value
     * @return `None` value
     */
    GSTD_CONSTEXPR auto MakeNone() GSTD_NOEXCEPT -> None {
        return None::New();
    }

    template<typename ValueT>
    class Optional;

    namespace detail {

        enum class OptionalState : bool {
            Some = true,
            None = false
        };

        template<typename ValueT,
                 bool IsTrivialV = std::is_trivial_v<ValueT>>
        class OptionalStorage {
        public:

            friend class Optional<ValueT>;

        public:

            using ValueType = ValueT;

        public:

            GSTD_CONSTEXPR GSTD_EXPLICIT OptionalStorage(Some<ValueType> &&some) GSTD_NOEXCEPT
                    : _some(std::move(some)),
                      _state(OptionalState::Some) {}

            GSTD_CONSTEXPR GSTD_EXPLICIT OptionalStorage(None &&none) GSTD_NOEXCEPT
                    : _none(std::move(none)),
                      _state(OptionalState::None) {}

            GSTD_CONSTEXPR OptionalStorage(const OptionalStorage &storage)
            GSTD_NOEXCEPT(std::is_nothrow_copy_constructible_v<ValueType>) {
                if (storage._state == OptionalState::Some) {
                    Init(MakeSome(storage._some.Copy()));
                } else {
                    Init(MakeNone());
                }
            }

            GSTD_CONSTEXPR OptionalStorage(OptionalStorage &&storage) GSTD_NOEXCEPT {
                if (storage._state == OptionalState::Some) {
                    Init(std::move(storage._some));
                } else {
                    Init(MakeNone());
                }
            }

        public:

            GSTD_CONSTEXPR ~OptionalStorage()
            GSTD_NOEXCEPT(std::is_nothrow_destructible_v<ValueType>) {
                if (_state != OptionalState::None) {
                    std::destroy_at(&_some.Ref());
                }
            }

        public:

            GSTD_CONSTEXPR auto Init(Some<ValueType> &&some) {
                std::construct_at(&_some,
                                  std::move(some));
                _state = OptionalState::Some;
            }

            GSTD_CONSTEXPR auto Init(None &&none) {
                _none = std::move(none);
                _state = OptionalState::None;
            }

            GSTD_CONSTEXPR auto Assign(Some<ValueType> &&some) {
                if (_state == OptionalState::Some) {
                    _some = std::move(some);
                } else {
                    Init(std::move(some));
                }
            }

            GSTD_CONSTEXPR auto Assign(None &&none) {
                if (_state == OptionalState::Some) {
                    std::destroy_at(&_some.Ref());
                }

                Init(std::move(none));
            }

        public:

            GSTD_CONSTEXPR auto operator=(const OptionalStorage &storage) -> OptionalStorage & {
                if (this == &storage) {
                    return *this;
                }

                if (storage._state == OptionalState::Some) {
                    Assign(MakeSome(storage._some.Copy()));
                } else {
                    Assign(MakeNone());
                }

                return *this;
            }

            GSTD_CONSTEXPR auto operator=(OptionalStorage &&storage) GSTD_NOEXCEPT -> OptionalStorage & {
                if (this == &storage) {
                    return *this;
                }

                if (storage._state == OptionalState::Some) {
                    Assign(std::move(storage._some));
                } else {
                    Assign(MakeNone());
                }

                return *this;
            }

        public:

            union {
                Some<ValueType> _some;

                None _none;
            };

            OptionalState _state;
        };

        template<typename ValueT>
        class OptionalStorage<ValueT,
                              true> {
        public:

            friend class Optional<ValueT>;

        public:

            using ValueType = ValueT;

        public:

            GSTD_CONSTEXPR GSTD_EXPLICIT OptionalStorage(Some<ValueType> &&some) GSTD_NOEXCEPT
                    : _some(std::move(some)),
                      _state(OptionalState::Some) {}

            GSTD_CONSTEXPR GSTD_EXPLICIT OptionalStorage(None &&none)
            GSTD_NOEXCEPT(std::is_nothrow_default_constructible_v<ValueType>)
                    : _some(MakeSome(ValueType {})),
                      _state(OptionalState::None) {}

            GSTD_CONSTEXPR OptionalStorage(const OptionalStorage &storage)
            GSTD_NOEXCEPT(std::is_nothrow_copy_constructible_v<ValueType>) = default;

            GSTD_CONSTEXPR OptionalStorage(OptionalStorage &&storage) GSTD_NOEXCEPT = default;

        public:

            GSTD_CONSTEXPR auto Init(Some<ValueType> &&some) GSTD_NOEXCEPT {
                _some = std::move(some);
                _state = OptionalState::Some;
            }

            GSTD_CONSTEXPR auto Init(None &&none) GSTD_NOEXCEPT {
                _none = std::move(none);
                _state = OptionalState::None;
            }

            GSTD_CONSTEXPR auto Assign(Some<ValueType> &&some) GSTD_NOEXCEPT {
                Init(std::move(some));
            }

            GSTD_CONSTEXPR auto Assign(None &&none) GSTD_NOEXCEPT {
                Init(std::move(none));
            }

        public:

            GSTD_CONSTEXPR auto operator=(const OptionalStorage &storage) -> OptionalStorage & = default;

            GSTD_CONSTEXPR auto operator=(OptionalStorage &&storage) GSTD_NOEXCEPT -> OptionalStorage & = default;

        public:

            union {
                Some<ValueType> _some;

                None _none;
            };

            OptionalState _state;
        };

        template<typename T>
        struct IsOptional {

            GSTD_INLINE static GSTD_CONSTEXPR bool Value = false;

        };

        template<typename T>
        struct IsOptional<Optional<T>> {

            GSTD_INLINE static GSTD_CONSTEXPR bool Value = true;

        };

        template<typename T>
        GSTD_INLINE GSTD_CONSTEXPR auto IsOptionalV = IsOptional<T>::Value;

    }

    /**
     * Class for type safe containing `Some` value or `None` value in compile-time and run-time
     * @tparam ValueT Value type
     */
    template<typename ValueT>
    class Optional : private detail::OptionalStorage<ValueT> {
    public:

        /*
         *
         * Optional PUBLIC TYPES
         *
         */

        /**
         * Value type
         */
        using ValueType = ValueT;

    private:

        /*
         *
         * Optional PRIVATE TYPES
         *
         */

        /**
         * Storage type
         */
        using Storage = detail::OptionalStorage<ValueT>;

    public:

        /*
         *
         * Optional PUBLIC CONSTRUCTORS
         *
         */

        /**
         * Default constructor for `Optional`
         * After constructing contains `None` value
         */
        GSTD_CONSTEXPR Optional() GSTD_NOEXCEPT
                : Optional(MakeNone()) {}

        /**
         * Constructor for `Optional` from `Some` value
         * After constructing contains `Some` value
         * @param some `Some` value
         */
        GSTD_CONSTEXPR Optional(Some<ValueType> &&some) GSTD_NOEXCEPT
                : Storage(std::forward<Some<ValueType>>(some)) {}

        /**
         * Constructor for `Optional` from `None` value
         * After constructing contains `None` value
         * @param none `None` value
         */
        GSTD_CONSTEXPR Optional(None &&none) GSTD_NOEXCEPT
                : Storage(std::forward<None>(none)) {}

        /**
         * Copy constructor for `Optional` from other `Optional`
         * @param optional Other `Optional`
         */
        GSTD_CONSTEXPR Optional(const Optional<ValueType> &optional)
        GSTD_NOEXCEPT(GSTD_NOEXCEPT(Storage(optional))) = default;

        /**
         * Move constructor for `Optional` from other `Optional`
         * @param optional Other `Optional`
         */
        GSTD_CONSTEXPR Optional(Optional<ValueType> &&optional) GSTD_NOEXCEPT = default;

    public:

        /*
         *
         * Optional PUBLIC METHODS
         *
         */

        /**
         * Check containing `Some` value
         * @return Is `Optional` contains `Some` value
         */
        GSTD_CONSTEXPR auto IsSome() const GSTD_NOEXCEPT -> bool {
            return Storage::_state == detail::OptionalState::Some;
        }

        /**
         * Check contained value in `Some` value
         * @tparam FunctionT Function type
         * @param function Predicate for check contained value
         * @return Is `Optional` contains `Some` value and function on value returns `true`
         */
        template<typename FunctionT>
        GSTD_CONSTEXPR auto IsSomeAnd(FunctionT &&function)
        const GSTD_NOEXCEPT(std::is_nothrow_invocable_v<FunctionT &&,
                                                        const ValueType &>) -> bool {
            static_assert(std::is_invocable_v<FunctionT &&,
                                              const ValueType &>,
                          "`FunctionT` must be invocable with `ValueType` argument!");
            static_assert(std::is_same_v<std::invoke_result_t<FunctionT &&,
                                                              const ValueType &>,
                                         bool>,
                          "`FunctionT` must return `bool`!");

            if (IsNone()) {
                return false;
            }

            return std::invoke(std::forward<FunctionT>(function),
                               Storage::_some.CRef());
        }

        /**
         * Check containing `None` value
         * @return Is `Optional` contains `None` value
         */
        GSTD_CONSTEXPR auto IsNone() const GSTD_NOEXCEPT -> bool {
            return !IsSome();
        }

        // TODO: comment all

        GSTD_CONSTEXPR auto Copy()
        const & GSTD_NOEXCEPT(std::is_nothrow_copy_constructible_v<ValueType>) -> Optional<ValueType> {
            return *this;
        }

        GSTD_CONSTEXPR auto Copy()
        const && GSTD_NOEXCEPT(std::is_nothrow_copy_constructible_v<ValueType>) -> Optional<ValueType> = delete;

        GSTD_CONSTEXPR auto AsRef() & GSTD_NOEXCEPT -> Optional<Ref<ValueType>> {
            if (IsNone()) {
                return MakeNone();
            }

            return MakeSome(MakeRef(Storage::_some.Ref()));
        }

        GSTD_CONSTEXPR auto AsRef() const & GSTD_NOEXCEPT -> Optional<Ref<const ValueType>> {
            if (IsNone()) {
                return MakeNone();
            }

            return MakeSome(MakeCRef(Storage::_some.CRef()));
        }

        GSTD_CONSTEXPR auto AsRef() && GSTD_NOEXCEPT -> Optional<Ref<ValueType>>  = delete;

        GSTD_CONSTEXPR auto AsRef() const && GSTD_NOEXCEPT -> Optional<Ref<const ValueType>> = delete;

        GSTD_CONSTEXPR auto AsCRef() const & GSTD_NOEXCEPT -> Optional<Ref<const ValueType>> {
            if (IsNone()) {
                return MakeNone();
            }

            return MakeSome(MakeCRef(Storage::_some.CRef()));
        }

        GSTD_CONSTEXPR auto AsCRef() const && GSTD_NOEXCEPT -> Optional<Ref<const ValueType>> = delete;

        GSTD_CONSTEXPR auto Move() & GSTD_NOEXCEPT -> Optional<ValueType> && {
            return std::move(*this);
        }

        GSTD_CONSTEXPR auto Move() && GSTD_NOEXCEPT -> Optional<ValueType> && = delete;

        /**
         * Check contained value in `Some` value
         * Panic if `Optional` contains `None` value
         * @tparam InputValueT Input value type
         * @param inputValue Input value
         * @return Is equal value in `Some` value and input value
         */
        template<typename InputValueT>
        GSTD_CONSTEXPR auto Contains(const InputValueT &inputValue) const GSTD_NOEXCEPT -> bool {
            static_assert(std::equality_comparable_with<ValueType,
                                                        InputValueT>,
                          "`ValueType` and `InputValueT` must be comparable with '==' operator!");

            if (IsNone()) {
                return false;
            }

            return Storage::_some.CRef() == inputValue;
        }

        /**
         * Check contained value in `Some` value
         * @tparam FunctionT Function type
         * @param function Predicate for check contained value
         * @return Is `Optional` contains `Some` value and function on value returns `true`
         */
        template<typename FunctionT>
        GSTD_CONSTEXPR auto Exists(FunctionT &&function)
        const GSTD_NOEXCEPT(std::is_nothrow_invocable_v<FunctionT &&,
                                                        const ValueType &>) -> bool {
            static_assert(std::is_invocable_v<FunctionT &&,
                                              const ValueType &>,
                          "`FunctionT` must be invocable with `ValueType` argument!");
            static_assert(std::is_same_v<std::invoke_result_t<FunctionT &&,
                                                              const ValueType &>,
                                         bool>,
                          "`FunctionT` must return `bool`!");

            if (IsNone()) {
                return false;
            }

            return std::invoke(std::forward<FunctionT>(function),
                               Storage::_some.CRef());
        }

        /**
         * Check containing `Some` value and return value from it
         * Panic if `Optional` contains `None` value
         * @param message Message for failed expect
         * @return Value from `Some` value if `Optional` contains it
         */
        GSTD_CONSTEXPR auto Expect(/* TODO: replace to gstd`s String */ std::string_view message) && -> ValueType {
            if (IsNone()) {
                Panic(fmt::format("`Optional` failed expect `Some` with message '{}'!", message).c_str());
            }

            return Storage::_some.Move();
        }

        /**
         * Check containing `None` value
         * Panic if `Optional` contains `Some` value
         * @param message Message for failed expect
         * @return No return
         */
        GSTD_CONSTEXPR auto ExpectNone(/* TODO: replace to gstd`s String */ std::string_view message) && -> void {
            if (IsSome()) {
                Panic(fmt::format("`Optional` failed expect `None` with message '{}'!", message).c_str());
            }
        }

        /**
         * Unwrap `Optional` and return value from it if `Optional` contains `Some`
         * Panic if `Optional` contains `None`
         * @return Value from `Some` value if `Optional` contains it
         */
        GSTD_CONSTEXPR auto Unwrap() && -> ValueType {
            if (IsNone()) {
                Panic("Unwrap `Optional` failed, it contains `None`!");
            }

            return Storage::_some.Move();
        }

        /**
         * Unwrap `Optional` and return value from it if `Optional` contains `Some` or return alternative input value otherwise
         * @param alternative Alternative input value
         * @return Value from `Some` value if `Optional` contains it or alternative input value otherwise
         */
        GSTD_CONSTEXPR auto UnwrapOr(ValueType &&alternative) && GSTD_NOEXCEPT -> ValueType {
            if (IsNone()) {
                return std::move(alternative);
            }

            return Storage::_some.Move();
        }

        /**
         * Unwrap `Optional` and return value from it if `Optional` contains `Some` value or return result of call `FunctionT` otherwise
         * @tparam FunctionT Function type
         * @param function Function for handle `None` case
         * @return Value if `Optional` contains `Some` value or result of `FunctionT` otherwise
         */
        template<typename FunctionT>
        GSTD_CONSTEXPR auto UnwrapOrElse(FunctionT &&function)
        && GSTD_NOEXCEPT(std::is_nothrow_invocable_v<FunctionT &&>) -> ValueType {
            static_assert(std::is_invocable_v<FunctionT &&>,
                          "`FunctionT` must be invocable!");
            static_assert(std::is_same_v<std::invoke_result_t<FunctionT &&>,
                                         ValueType>,
                          "`FunctionT` must return `ValueType`!");

            if (IsNone()) {
                return std::invoke(std::forward<FunctionT>(function));
            }

            return Storage::_some.Move();
        }

        /**
         * Unwrap `Optional` and return value from it if `Optional` contains `Some` value or return default value otherwise
         * @return Value if `Optional` contains `Some` value or default value otherwise
         */
        GSTD_CONSTEXPR auto UnwrapOrDefault()
        && GSTD_NOEXCEPT(std::is_nothrow_default_constructible_v<ValueType>) -> ValueType {
            static_assert(std::is_default_constructible_v<ValueType>,
                          "`ValueType` must be default constructible!");

            if (IsNone()) {
                return ValueType {};
            }

            return Storage::_some.Move();
        }

        /**
         * Unwrap `Optional` and no return if `Optional` contains `None` value
         * Panic if `Optional` contains `Some` value
         * @return No return
         */
        GSTD_CONSTEXPR auto UnwrapNone() && -> void {
            if (IsSome()) {
                Panic("Unwrap `Optional` failed, it contains `Some`!");
            }
        }

        /**
         * Unwrap `Optional` and no return if `Optional` contains `None` value or call `FunctionT` otherwise
         * @tparam FunctionT Function type
         * @param function Function for handle `Some` case
         * @return No return
         */
        template<typename FunctionT>
        GSTD_CONSTEXPR auto UnwrapNoneOrElse(FunctionT &&function)
        && GSTD_NOEXCEPT(std::is_nothrow_invocable_v<FunctionT &&>) -> void {
            static_assert(std::is_invocable_v<FunctionT &&>,
                          "`FunctionT` must be invocable!");
            static_assert(std::is_same_v<std::invoke_result_t<FunctionT &&>,
                                         void>,
                          "`FunctionT` must return `void`!");

            if (IsSome()) {
                std::invoke(std::forward<FunctionT>(function));
            }
        }

        /**
         * Convert `Optional<ValueType>` to `Optional<(function return type)>`
         * If `Optional` contains `Some` value apply input function on value from it and put result in new `Optional` or put `None` otherwise
         * @tparam FunctionT Function type
         * @param function Function for handle `Some` case and return value for new `Optional`
         * @return `Optional<(function return type)>` with `Some` value with value from function if previous `Optional` contained `Some` value or with `None` value otherwise
         */
        template<typename FunctionT>
        GSTD_CONSTEXPR auto Map(FunctionT &&function)
        && GSTD_NOEXCEPT(std::is_nothrow_invocable_v<FunctionT &&,
                                                     ValueType &&>) -> Optional<std::invoke_result_t<FunctionT &&,
                                                                                                     ValueType &&>> {
            static_assert(std::is_invocable_v<FunctionT &&,
                                              ValueType &&>,
                          "`FunctionT` must be invocable with `ValueType` argument!");

            if (IsNone()) {
                return MakeNone();
            }

            return MakeSome(std::invoke(std::forward<FunctionT>(function),
                                        Storage::_some.Move()));
        }

        /**
         * Inspect contained value
         * If `Optional` contains `Some` value invoke input function
         * Always return this `Optional`
         * @tparam FunctionT Function type
         * @param function Function for inspecting value
         * @return This `Optional`
         */
        template<typename FunctionT>
        GSTD_CONSTEXPR auto Inspect(FunctionT &&function)
        const GSTD_NOEXCEPT(std::conjunction_v<std::is_nothrow_copy_constructible<ValueType>,
                                               std::is_nothrow_invocable<FunctionT &&,
                                                                         const ValueType &>>) -> Optional<ValueType> {
            static_assert(std::is_invocable_v<FunctionT &&,
                                              const ValueType &>,
                          "`FunctionT` must be invocable with `ValueType` argument!");
            static_assert(std::is_same_v<std::invoke_result_t<FunctionT &&,
                                                              const ValueType &>,
                                         void>,
                          "`FunctionT` must return `void`!");

            if (IsSome()) {
                std::invoke(std::forward<FunctionT>(function),
                            Storage::_some.CRef());
            }

            return *this;
        }

        /**
         * If `Optional` contains `Some` value apply value on input function and return result of it or return alternative input value otherwise
         * @tparam FunctionT Function type
         * @tparam InputValueT Input value type
         * @param function
         * @param alternative
         * @return Result of function if `Optional` contains `Some` value or alternative value otherwise
         */
        template<typename FunctionT,
                 typename InputValueT>
        GSTD_CONSTEXPR auto MapOr(FunctionT &&function,
                                  InputValueT &&alternative)
        && GSTD_NOEXCEPT(std::is_nothrow_invocable_v<FunctionT &&,
                                                     ValueType &&>) -> std::invoke_result_t<FunctionT &&,
                                                                                            ValueType &&> {
            static_assert(std::is_invocable_v<FunctionT &&,
                                              ValueType &&>,
                          "`FunctionT` must be invocable with `ValueType` argument!");
            static_assert(std::is_same_v<std::invoke_result_t<FunctionT &&,
                                                              ValueType &&>,
                                         InputValueT>,
                          "`FunctionT` must return `InputValueT`!");

            if (IsNone()) {
                return std::forward<InputValueT>(alternative);
            }

            return std::invoke(std::forward<FunctionT>(function),
                               Storage::_some.Move());
        }

        /**
         * If `Optional` contains `Some` value apply value on input function and return result of it or invoke alternative input function otherwise
         * @tparam FunctionT Function type
         * @tparam AlternativeFunctionT Alternative function type
         * @param function Function for `Some` value case
         * @param alternativeFunction Alternative function for `None` value case
         * @return Result of function if `Optional` contains `Some` value or result of alternative function otherwise
         */
        template<typename FunctionT,
                 typename AlternativeFunctionT>
        GSTD_CONSTEXPR auto MapOrElse(FunctionT &&function,
                                      AlternativeFunctionT &&alternativeFunction)
        && GSTD_NOEXCEPT(std::conjunction_v<std::is_nothrow_invocable<FunctionT &&,
                                                                      ValueType &&>,
                                            std::is_nothrow_invocable<AlternativeFunctionT &&>>) -> std::invoke_result_t<FunctionT &&,
                                                                                                                         ValueType &&> {
            static_assert(std::is_invocable_v<FunctionT &&,
                                              ValueType &&>,
                          "`FunctionT` must be invocable with `ValueType` argument!");
            static_assert(std::is_invocable_v<AlternativeFunctionT &&>,
                          "`AlternativeFunctionT` must be invocable!");
            static_assert(std::is_same_v<std::invoke_result_t<FunctionT &&, ValueType &&>,
                                         std::invoke_result_t<AlternativeFunctionT &&>>,
                          "`FunctionT` and `AlternativeFunctionT` return types must be equal!");

            if (IsNone()) {
                return std::invoke(std::forward<AlternativeFunctionT>(alternativeFunction));
            }

            return std::invoke(std::forward<FunctionT>(function),
                               Storage::_some.Move());
        }

        /**
         * Matching `Some` value and `None` value in `Optional`
         * If `Optional` contains `Some` value call function for handling `Some` value
         * Call function for handling `None` value otherwise
         * @tparam SomeFunctionT Some function type
         * @tparam NoneFunctionT None function type
         * @param someFunction Function for handling `Some` value
         * @param noneFunction Function for handling `None` value
         * @return Result of matching
         */
        template<typename SomeFunctionT,
                 typename NoneFunctionT>
        GSTD_CONSTEXPR auto Match(SomeFunctionT &&someFunction,
                                  NoneFunctionT &&noneFunction)
        & GSTD_NOEXCEPT(std::conjunction_v<std::is_nothrow_invocable<SomeFunctionT &&,
                                                                     ValueType &>,
                                           std::is_nothrow_invocable<NoneFunctionT &&>>) -> std::invoke_result_t<SomeFunctionT &&,
                                                                                                                 ValueType &> {
            static_assert(std::is_invocable_v<SomeFunctionT &&,
                                              ValueType &>,
                          "`SomeFunctionT` must be invocable with `ValueType` argument!");
            static_assert(std::is_invocable_v<NoneFunctionT &&>,
                          "`NoneFunctionT` must be invocable!");
            static_assert(std::is_same_v<std::invoke_result_t<SomeFunctionT &&,
                                                              ValueType &>,
                                         std::invoke_result_t<NoneFunctionT &&>>,
                          "`SomeFunctionT` and `NoneFunctionT` return types must be equal!");

            if (IsNone()) {
                return std::invoke(std::forward<NoneFunctionT>(noneFunction));
            }

            return std::invoke(std::forward<SomeFunctionT>(someFunction),
                               Storage::_some.Ref());
        }

        /**
         * Matching `Some` value and `None` value in `Optional`
         * If `Optional` contains `Some` value call function for handling `Some` value
         * Call function for handling `None` value otherwise
         * @tparam SomeFunctionT Some function type
         * @tparam NoneFunctionT None function type
         * @param someFunction Function for handling `Some` value
         * @param noneFunction Function for handling `None` value
         * @return Result of matching
         */
        template<typename SomeFunctionT,
                 typename NoneFunctionT>
        GSTD_CONSTEXPR auto Match(SomeFunctionT &&someFunction,
                                  NoneFunctionT &&noneFunction)
        const & GSTD_NOEXCEPT(std::conjunction_v<std::is_nothrow_invocable<SomeFunctionT &&,
                                                                           const ValueType &>,
                                                 std::is_nothrow_invocable<NoneFunctionT &&>>) -> std::invoke_result_t<SomeFunctionT &&,
                                                                                                                       const ValueType &> {
            static_assert(std::is_invocable_v<SomeFunctionT &&,
                                              const ValueType &>,
                          "`SomeFunctionT` must be invocable with `ValueType` argument!");
            static_assert(std::is_invocable_v<NoneFunctionT &&>,
                          "`NoneFunctionT` must be invocable!");
            static_assert(std::is_same_v<std::invoke_result_t<SomeFunctionT &&,
                                                              const ValueType &>,
                                         std::invoke_result_t<NoneFunctionT &&>>,
                          "`SomeFunctionT` and `NoneFunctionT` return types must be equal!");

            if (IsNone()) {
                return std::invoke(std::forward<NoneFunctionT>(noneFunction));
            }

            return std::invoke(std::forward<SomeFunctionT>(someFunction),
                               Storage::_some.CRef());
        }

        /**
         * Matching `Some` value and `None` value in `Optional`
         * If `Optional` contains `Some` value call function for handling `Some` value
         * Call function for handling `None` value otherwise
         * @tparam SomeFunctionT Some function type
         * @tparam NoneFunctionT None function type
         * @param someFunction Function for handling `Some` value
         * @param noneFunction Function for handling `None` value
         * @return Result of matching
         */
        template<typename SomeFunctionT,
                 typename NoneFunctionT>
        GSTD_CONSTEXPR auto Match(SomeFunctionT &&someFunction,
                                  NoneFunctionT &&noneFunction)
        && GSTD_NOEXCEPT(std::conjunction_v<std::is_nothrow_invocable<SomeFunctionT &&,
                                                                      ValueType &&>,
                                            std::is_nothrow_invocable<NoneFunctionT &&>>) -> std::invoke_result_t<SomeFunctionT &&,
                                                                                                                  ValueType &&> {
            static_assert(std::is_invocable_v<SomeFunctionT &&,
                                              ValueType &&>,
                          "`SomeFunctionT` must be invocable with `ValueType` argument!");
            static_assert(std::is_invocable_v<NoneFunctionT &&>,
                          "`NoneFunctionT` must be invocable!");
            static_assert(std::is_same_v<std::invoke_result_t<SomeFunctionT &&,
                                                              ValueType &&>,
                                         std::invoke_result_t<NoneFunctionT &&>>,
                          "`SomeFunctionT` and `NoneFunctionT` return types must be equal!");

            if (IsNone()) {
                return std::invoke(std::forward<NoneFunctionT>(noneFunction));
            }

            return std::invoke(std::forward<SomeFunctionT>(someFunction),
                               Storage::_some.Move());
        }

        /**
         * Matching `Some` value and `None` value in `Optional`
         * If `Optional` contains `Some` value call function for handling `Some` value
         * Call function for handling `None` value otherwise
         * @tparam SomeFunctionT Some function type
         * @tparam NoneFunctionT None function type
         * @param someFunction Function for handling `Some` value
         * @param noneFunction Function for handling `None` value
         * @return Result of matching
         */
        template<typename SomeFunctionT,
                 typename NoneFunctionT>
        GSTD_CONSTEXPR auto Match(SomeFunctionT &&someFunction,
                                  NoneFunctionT &&noneFunction)
        const && GSTD_NOEXCEPT(std::conjunction_v<std::is_nothrow_invocable<SomeFunctionT &&,
                                                                            ValueType &&>,
                                                  std::is_nothrow_invocable<NoneFunctionT &&>>) -> std::invoke_result_t<SomeFunctionT &&,
                                                                                                                        const ValueType &&> {
            static_assert(std::is_invocable_v<SomeFunctionT &&,
                                              const ValueType &>,
                          "`SomeFunctionT` must be invocable with `ValueType` argument!");
            static_assert(std::is_invocable_v<NoneFunctionT &&>,
                          "`NoneFunctionT` must be invocable!");
            static_assert(std::is_same_v<std::invoke_result_t<SomeFunctionT &&,
                                                              const ValueType &>,
                                         std::invoke_result_t<NoneFunctionT &&>>,
                          "`SomeFunctionT` and `NoneFunctionT` return types must be equal!");

            if (IsNone()) {
                return std::invoke(std::forward<NoneFunctionT>(noneFunction));
            }

            return std::invoke(std::forward<SomeFunctionT>(someFunction),
                               Storage::_some.CMove());
        }

        /**
         * Convert `Optional` to `Result`
         * If `Optional` contains `Some` create `Result` with `Ok` value with value from `Some` value
         * Create `Result` with `Err` value with input error value otherwise
         * @tparam ErrorT Error type
         * @param error Input error value
         * @return `Result` object
         */
        template<typename ErrorT>
        GSTD_CONSTEXPR auto OkOr(ErrorT &&error) && GSTD_NOEXCEPT-> Result<ValueType,
                                                                           ErrorT> {
            if (IsNone()) {
                return MakeErr(std::forward<ErrorT>(error));
            }

            return MakeOk(Storage::_some.Move());
        }

        /**
         * Convert `Optional` to `Result`
         * If `Optional` contains `Some` create `Result` with `Ok` value with value from `Some` value
         * Create `Result` with `Err` value with function calling result value otherwise
         * @tparam FunctionT Function type
         * @param function Function for create error value
         * @return `Result` object
         */
        template<typename FunctionT>
        GSTD_CONSTEXPR auto OkOrElse(FunctionT &&function)
        && GSTD_NOEXCEPT(std::is_nothrow_invocable_v<FunctionT &&>) -> Result<ValueType,
                                                                              std::invoke_result_t<FunctionT &&>> {
            static_assert(std::is_invocable_v<FunctionT &&>,
                          "`FunctionT` must be invocable!");

            if (IsNone()) {
                return MakeErr(std::invoke(std::forward<FunctionT>(function)));
            }

            return MakeOk(Storage::_some.Move());
        }

        /**
         * Boolean 'and' with other `Optional`
         * If this `Optional` contains `Some` value return input `Optional`
         * Return `None` value otherwise
         * @tparam InputValueT Value type of input `Optional`
         * @param inputOptional Input `Optional`
         * @return Result of 'and' boolean operation
         */
        template<typename InputValueT>
        GSTD_CONSTEXPR auto And(Optional<InputValueT> &&inputOptional) && GSTD_NOEXCEPT -> Optional<InputValueT> {
            if (IsNone()) {
                return MakeNone();
            }

            return std::move(inputOptional);
        }

        /**
         * Applying function on this `Optional`
         * If `Optional` contains `Some` value apply input function on value from this `Optional` and return new `Optional` with result
         * Return `None` value otherwise
         * @tparam FunctionT Function type
         * @param function Input function
         * @return Result of applying function
         */
        template<typename FunctionT>
        GSTD_CONSTEXPR auto AndThen(FunctionT &&function)
        && GSTD_NOEXCEPT(std::is_nothrow_invocable_v<FunctionT &&,
                                                     ValueType &&>) -> std::invoke_result_t<FunctionT &&,
                                                                                            ValueType &&> {
            static_assert(std::is_invocable_v<FunctionT &&,
                                              ValueType &&>,
                          "`FunctionT` must be invocable with `ValueType` argument!");
            static_assert(detail::IsOptionalV<std::invoke_result_t<FunctionT &&,
                                                                   ValueType &&>>,
                          "`FunctionT` must return `Optional`!");

            if (IsNone()) {
                return MakeNone();
            }

            return std::invoke(std::forward<FunctionT>(function),
                               Storage::_some.Move());
        }

        /**
         * Filtering value in `Optional`
         * If `Optional` contains `Some` value call input function (predicate) by value in `Some` value
         * If function returns `true` return `Optional` with `Some` value
         * Otherwise return `Optional` with `None` value
         * @tparam FunctionT Function type
         * @param function Input function (predicate) for filtering
         * @return `Optional` after filtering value
         */
        template<typename FunctionT>
        GSTD_CONSTEXPR auto Filter(FunctionT &&function)
        && GSTD_NOEXCEPT(std::is_nothrow_invocable_v<FunctionT &&,
                                                     const ValueType &>) -> Optional<ValueType> {
            static_assert(std::is_invocable_v<FunctionT &&,
                                              const ValueType &>,
                          "`FunctionT` must be invocable with `ValueType` argument!");
            static_assert(std::is_same_v<std::invoke_result_t<FunctionT &&,
                                                              const ValueType &>,
                                         bool>,
                          "`FunctionT` must return `bool`!");

            if (IsNone()) {
                return MakeNone();
            }

            if (!function(Storage::_some.CRef())) {
                return MakeNone();
            }

            return MakeSome(Storage::_some.Move());
        }

        /**
         * Boolean 'or' operation with other `Optional`
         * If this `Optional` contains `Some` value return this `Optional`
         * Return input `Optional` otherwise
         * @param inputOptional Input `Optional`
         * @return Result of 'or' boolean operation
         */
        GSTD_CONSTEXPR auto Or(Optional<ValueType> &&inputOptional) && GSTD_NOEXCEPT -> Optional<ValueType> {
            if (IsNone()) {
                return std::move(inputOptional);
            }

            return MakeSome(Storage::_some.Move());
        }

        /**
         * Boolean 'or' operation with function that generates other `Optional`
         * If this `Optional` contains `Some` value return this `Optional`
         * Call input function and return result of it otherwise
         * @tparam FunctionT Function type
         * @param function Input function that returns other `Optional`
         * @return Result of 'or' boolean operation
         */
        template<typename FunctionT>
        GSTD_CONSTEXPR auto OrElse(FunctionT &&function)
        && GSTD_NOEXCEPT(std::is_nothrow_invocable_v<FunctionT &&>) -> Optional<ValueType> {
            static_assert(std::is_invocable_v<FunctionT &&>,
                          "`FunctionT` must be invocable!");
            static_assert(detail::IsOptionalV<std::invoke_result_t<FunctionT &&>>,
                          "`FunctionT` must return `Optional`!");
            static_assert(std::is_same_v<typename std::invoke_result_t<FunctionT &&>::ValueType,
                                         ValueType>,
                          "`FunctionT` must return `Optional` with `ValueType` that equal to self `ValueType`!");

            if (IsNone()) {
                return std::invoke(std::forward<FunctionT>(function));
            }

            return MakeSome(Storage::_ok.Move());
        }

        /**
         * Boolean 'xor' operation with other `Optional`
         * If exactly one of this or input `Optional`s contains `Some` value return `Optional` with `Some` value
         * Return `Optional` with `None` value otherwise
         * @param inputOptional Input `Optional`
         * @return Result of 'xor' boolean operation
         */
        GSTD_CONSTEXPR auto Xor(Optional<ValueType> &&inputOptional) && GSTD_NOEXCEPT-> Optional<ValueType> {
            if (IsSome() && inputOptional.IsSome()) {
                return MakeNone();
            }

            if (IsSome()) {
                return MakeSome(Storage::_some.Move());
            }

            if (inputOptional.IsSome()) {
                return std::move(inputOptional);
            }

            return MakeNone();
        }

        /**
         * Insert value into `Optional`
         * @param inputValue New value
         * @return Reference to value in `Optional`
         */
        GSTD_CONSTEXPR auto Insert(ValueType &&inputValue) GSTD_NOEXCEPT -> ValueType & {
            Storage::Assign(MakeSome(std::move(inputValue)));

            return Storage::_some.Ref();
        }

        /**
         * Get value of `Optional`
         * If `Optional` contains `Some` value return reference to value
         * Insert input value into `Optional` and return reference to value otherwise
         * @param inputValue New value
         * @return Reference to value in `Optional`
         */
        GSTD_CONSTEXPR auto GetOrInsert(ValueType &&inputValue) GSTD_NOEXCEPT -> ValueType & {
            if (IsNone()) {
                Storage::Assign(MakeSome(std::forward<ValueType>(inputValue)));
            }

            return Storage::_some.Ref();
        }

        /**
         * Get value of `Optional`
         * If `Optional` contains `Some` value return reference to value
         * Call default constructor of value, insert result into `Optional` and return reference to value otherwise
         * @return Reference to value in `Optional`
         */
        GSTD_CONSTEXPR auto GetOrInsertDefault()
        GSTD_NOEXCEPT(std::is_nothrow_default_constructible_v<ValueType>) -> ValueType & {
            static_assert(std::is_default_constructible_v<ValueType>,
                          "`ValueType` must be default constructible!");

            if (IsNone()) {
                Storage::Assign(MakeSome(ValueType {}));
            }

            return Storage::_some.Ref();
        }

        /**
         * Get value of `Optional`
         * If `Optional` contains `Some` value return reference to value
         * Call input function, insert result of it into `Optional` and return reference to value otherwise
         * @tparam FunctionT Function type
         * @param function Function for get new value
         * @return Reference to value in `Optional`
         */
        template<typename FunctionT>
        GSTD_CONSTEXPR auto GetOrInsertWith(FunctionT &&function)
        GSTD_NOEXCEPT(std::is_nothrow_invocable_v<FunctionT &&>) -> ValueType & {
            static_assert(std::is_invocable_v<FunctionT &&>,
                          "`FunctionT` must be invocable!");
            static_assert(std::is_same_v<std::invoke_result_t<FunctionT &&>,
                                         ValueType>,
                          "`FunctionT` must return `ValueType`!");

            if (IsNone()) {
                Storage::Assign(MakeSome(std::invoke(std::forward<FunctionT>(function))));
            }

            return Storage::_some.Ref();
        }

        /**
         * Make copy of this `Optional`, insert into this `Optional` to `None` value and return copy
         * @return Copy of this `Optional`
         */
        GSTD_CONSTEXPR auto Take() GSTD_NOEXCEPT -> Optional<ValueType> {
            if (IsNone()) {
                return MakeNone();
            }

            auto copy = MakeSome(Storage::_some.Move());

            Storage::Assign(MakeNone());

            return copy;
        }

        /**
         * Make copy of this `Optional`, insert into this `Optional` `None` value and return copy
         * If applyed function on value in `Some` value returns `true` will be executed `Take`
         * Return `Optional` with `None` value otherwise
         * @tparam FunctionT Function type
         * @param function Function for check
         * @return Copy of this `Optional`
         */
        template<typename FunctionT>
        GSTD_CONSTEXPR auto TakeIf(FunctionT &&function)
        GSTD_NOEXCEPT(std::is_nothrow_invocable_v<FunctionT &&,
                                                  ValueType &>) -> Optional<ValueType> {
            static_assert(std::is_invocable_v<FunctionT &&,
                                              ValueType &>,
                          "`FunctionT` must be invocable with 'ValueType` argument!");
            static_assert(std::is_same_v<std::invoke_result_t<FunctionT &&,
                                                              ValueType &>,
                                         bool>,
                          "`FunctionT` must return `bool`!");

            if (IsNone()) {
                return MakeNone();
            }

            if (!predicate(Storage::_some.Ref())) {
                return MakeNone();
            }

            auto copy = MakeSome(Storage::_some.Move());

            Storage::Assign(MakeNone());

            return copy;
        }

        /**
         * Make copy of this `Optional`, insert input value into `Optional` and return copy
         * @param value Value for inserting
         * @return Copy of this `Optional` before inserting
         */
        GSTD_CONSTEXPR auto Replace(ValueType &&value) GSTD_NOEXCEPT -> Optional<ValueType> {
            auto copy = MakeSome(Storage::_some.Move());

            Storage::Assign(MakeSome(std::forward<ValueType>(value)));

            return copy;
        }

        /**
         * Zip this and input `Optional`s
         * If this and input `Optional`s have `Some` value return `Optional` with pair of values
         * Return `None` otherwise
         * @tparam InputValueT Value type of input `Optional`
         * @param inputOptional Input `Optional`
         * @return `Optional` with pair of values
         */
        template<typename InputValueT>
        GSTD_CONSTEXPR auto Zip(Optional<InputValueT> &&inputOptional) && GSTD_NOEXCEPT -> Optional<Pair<ValueType,
                                                                                                         InputValueT>> {
            if (IsNone() || inputOptional.IsNone()) {
                return MakeNone();
            }

            return MakeSome(MakePair(Storage::_some.Move(),
                                     inputOptional._some.Move()));
        }

        /**
         * Zip this and input `Optional`s by input function
         * If this and input `Optional`s have `Some` value invoke input function and return `Optional` with result of it
         * Return `None` otherwise
         * @tparam InputValueT Value type of input `Optional`
         * @tparam FunctionT Function type
         * @param inputOptional Input `Optional`
         * @param function Function for zip this and input `Optional`s
         * @return `Optional` with function result
         */
        template<typename InputValueT,
                 typename FunctionT>
        GSTD_CONSTEXPR auto ZipWith(Optional<InputValueT> &&inputOptional,
                                    FunctionT &&function)
        && GSTD_NOEXCEPT(std::is_nothrow_invocable_v<FunctionT &&,
                                                     ValueType &&,
                                                     InputValueT &&>) -> Optional<std::invoke_result_t<FunctionT &&,
                                                                                                       ValueType &&,
                                                                                                       InputValueT &&>> {
            if (IsNone() || inputOptional.IsNone()) {
                return MakeNone();
            }

            return MakeSome(std::invoke(std::forward<FunctionT>(function),
                                        Storage::_some.Move(),
                                        inputOptional._some.Move()));
        }

    public:

        /*
         *
         * Optional PUBLIC OPERATORS
         *
         */

        /**
         * Copy assignment operator for other `Optional`
         * @param optional Other `Optional`
         * @return This `Optional` after assignment
         */
        GSTD_CONSTEXPR auto operator=(const Optional<ValueType> &optional) -> Optional<ValueType> & = default;

        /**
         * Move assignment operator for other `Optional`
         * @param optional Other `Optional`
         * @return This `Optional` after assigmment
         */
        GSTD_CONSTEXPR auto operator=(Optional<ValueType> &&optional) GSTD_NOEXCEPT -> Optional<ValueType> & = default;

        /**
         * Assignment operator for `Some` value
         * @param some `Some` value
         * @return This `Optional` after assignment
         */
        GSTD_CONSTEXPR auto operator=(Some<ValueType> &&some) -> Optional<ValueType> & {
            Storage::Assign(std::forward<Some<ValueType>>(some));

            return *this;
        }

        /**
         * Assignment operator for `None` value
         * @param none `None` value
         * @return This `Optional` after assignment
         */
        GSTD_CONSTEXPR auto operator=(None &&none) -> Optional<ValueType> & {
            Storage::Assign(std::forward<None>(none));

            return *this;
        }

        /**
         * Implicit conversion operator to `bool` type
         * @return Is `Optional` contains `Some` value
         */
        GSTD_CONSTEXPR operator bool() const GSTD_NOEXCEPT {
            return IsSome();
        }
    };

    /**
     * Smart creating of `Optional` with `Some` value
     * @tparam ValueT Value type
     * @param value Value
     * @return `Optional` type with `Some` value
     */
    template<typename ValueT>
    GSTD_CONSTEXPR auto MakeSomeOptional(ValueT &&value) GSTD_NOEXCEPT -> Optional<ValueT> {
        return Optional<ValueT>(MakeSome(std::forward<ValueT>(value)));
    }

    /**
     * Smart creating of `Optional` with `None` value
     * @tparam ValueT Value type
     * @return `Optional` type with `None` value
     */
    template<typename ValueT>
    GSTD_CONSTEXPR auto MakeNoneOptional() -> Optional<ValueT> {
        return Optional<ValueT>(MakeNone());
    }

}

#endif //GSTD_OPTIONAL_H
