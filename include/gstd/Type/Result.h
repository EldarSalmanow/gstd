#ifndef GSTD_RESULT_H
#define GSTD_RESULT_H

#include <fmt/format.h>

namespace gstd {

    /**
     * Wrapper class for any value
     * @tparam ValueT Value type
     */
    template<typename ValueT>
    class Ok {
    public:

        /*
         *
         * Ok PUBLIC TYPES
         *
         */

        /**
         * Value type
         */
        using ValueType = ValueT;

    public:

        /*
         *
         * Ok PUBLIC CONSTRUCTORS
         *
         */

        /**
         * Main constructor for `Ok` from any value
         * @param value Value
         */
        GSTD_CONSTEXPR GSTD_EXPLICIT Ok(ValueType &&value) GSTD_NOEXCEPT
                : _value(std::move(value)) {}

    public:

        /*
         *
         * Ok PUBLIC STATIC METHODS
         *
         */

        /**
         * Creating `Ok` value
         * @param value Input value
         * @return `Ok` value
         */
        static GSTD_CONSTEXPR auto New(ValueType &&value) GSTD_NOEXCEPT -> Ok {
            return Ok {
                std::forward<ValueType>(value)
            };
        }

    public:

        /*
         *
         * Ok PUBLIC METHODS
         *
         */

        /**
         * Copying vale and return copy of it
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
         * Ok PUBLIC OPERATORS
         *
         */

        /**
         * Equality compare operator for other `Ok`
         * @tparam InputValueT Value type of input `Ok`
         * @param ok Input `Ok` value
         * @return Is equal values in this and input `Ok`s
         * @todo Add return type check?
         */
        template<typename InputValueT>
        GSTD_CONSTEXPR auto operator==(const Ok<InputValueT> &ok) const GSTD_NOEXCEPT -> bool {
            static_assert(std::equality_comparable_with<ValueType,
                                                        InputValueT>,
                          "`ValueType` and `InputValueT` must be comparable with '==' operator!");

            return CRef() == ok.CRef();
        }

    private:

        /*
         *
         * Ok PRIVATE FIELDS
         *
         */

        /**
         * Value
         */
        ValueType _value;
    };

    /**
     * Smart creating an `Ok` value
     * @tparam ValueT Value type
     * @param value Value
     * @return `Ok` value
     */
    template<typename ValueT>
    GSTD_CONSTEXPR auto MakeOk(ValueT &&value) GSTD_NOEXCEPT -> Ok<ValueT> {
        return Ok<ValueT>::New(std::forward<ValueT>(value));
    }

    /**
     * Wrapper class for any error
     * @tparam ErrorT Error type
     */
    template<typename ErrorT>
    class Err {
    public:

        /*
         *
         * Err PUBLIC TYPES
         *
         */

        /**
         * Error type
         */
        using ErrorType = ErrorT;

    public:

        /*
         *
         * Err PUBLIC CONSTRUCTORS
         *
         */

        /**
         * Main constructor for `Err` from any error
         * @param error Error
         */
        GSTD_CONSTEXPR GSTD_EXPLICIT Err(ErrorType &&error) GSTD_NOEXCEPT
                : _error(std::move(error)) {}

    public:

        /*
         *
         * Err PUBLIC STATIC METHODS
         *
         */

        /**
         * Creating `Err` value
         * @param error Input error
         * @return `Err` value
         */
        static GSTD_CONSTEXPR auto New(ErrorType &&error) GSTD_NOEXCEPT -> Err {
            return Err {
                std::forward<ErrorType>(error)
            };
        }

    public:

        /*
         *
         * Err PUBLIC METHODS
         *
         */

        /**
         * Copying error and return copy of it
         * @return Copy of error
         */
        GSTD_CONSTEXPR auto Copy()
        const GSTD_NOEXCEPT(std::is_nothrow_copy_constructible_v<ErrorType>) -> ErrorType {
            return _error;
        }

        /**
         * Getting lvalue reference of error
         * @return Lvalue reference of error
         */
        GSTD_CONSTEXPR auto Ref() GSTD_NOEXCEPT -> ErrorType & {
            return _error;
        }

        /**
         * Getting constant lvalue reference of error
         * @return Constant lvalue reference of error
         */
        GSTD_CONSTEXPR auto CRef() const GSTD_NOEXCEPT -> const ErrorType & {
            return _error;
        }

        /**
         * Getting rvalue reference of error for moving
         * @return Rvalue reference of error
         */
        GSTD_CONSTEXPR auto Move() GSTD_NOEXCEPT -> ErrorType && {
            return std::move(_error);
        }

        /**
         * Getting constant rvalue reference of error for moving
         * @return Constant rvalue reference of error
         */
        GSTD_CONSTEXPR auto CMove() const GSTD_NOEXCEPT -> const ErrorType && {
            return std::move(_error);
        }

    public:

        /*
         *
         * Err PUBLIC OPERATORS
         *
         */

        /**
         * Equality compare operator for other `Err`
         * @tparam InputErrorT Error type of input `Err`
         * @param err Input `Err` value
         * @return Is equal errors in this and input `Err`s
         * @todo Add return type check?
         */
        template<typename InputErrorT>
        GSTD_CONSTEXPR auto operator==(const Err<InputErrorT> &err) const GSTD_NOEXCEPT -> bool {
            static_assert(std::equality_comparable_with<ErrorType,
                                                        InputErrorT>,
                          "`ErrorType` and `InputErrorT` must be comparable with '==' operator!");

            return CRef() == err.CRef();
        }

    private:

        /*
         *
         * Err PRIVATE FIELDS
         *
         */

        /**
         * Error
         */
        ErrorType _error;
    };

    /**
     * Smart creating an `Err` value
     * @tparam ErrorT Error type
     * @param error Error
     * @return `Err` value
     */
    template<typename ErrorT>
    GSTD_CONSTEXPR auto MakeErr(ErrorT &&error) GSTD_NOEXCEPT -> Err<ErrorT> {
        return Err<ErrorT>::New(std::forward<ErrorT>(error));
    }

    template<typename ValueT,
             typename ErrorT>
    class Result;

    namespace detail {

        enum class ResultState {
            Ok,
            Err
        };

        template<typename ValueT,
                 typename ErrorT,
                 bool IsTrivialV = std::conjunction_v<std::is_trivial<ValueT>,
                                                      std::is_trivial<ErrorT>>>
        class ResultStorage {
        public:

            friend class Result<ValueT,
                                ErrorT>;

        public:

            using ValueType = ValueT;

            using ErrorType = ErrorT;

        public:

            GSTD_CONSTEXPR GSTD_EXPLICIT ResultStorage(Ok<ValueT> &&ok) GSTD_NOEXCEPT
                    : _ok(std::move(ok)),
                      _state(ResultState::Ok) {}

            GSTD_CONSTEXPR GSTD_EXPLICIT ResultStorage(Err<ErrorT> &&err) GSTD_NOEXCEPT
                    : _err(std::move(err)),
                      _state(ResultState::Err) {}

            GSTD_CONSTEXPR ResultStorage(const ResultStorage &storage)
            GSTD_NOEXCEPT(std::conjunction_v<std::is_nothrow_copy_constructible<ValueType>,
                                             std::is_nothrow_copy_constructible<ErrorType>>) {
                if (storage._state == ResultState::Ok) {
                    Init(MakeOk(storage._ok.Copy()));
                } else if (storage._state == ResultState::Err) {
                    Init(MakeErr(storage._err.Copy()));
                }
            }

            GSTD_CONSTEXPR ResultStorage(ResultStorage &&storage) GSTD_NOEXCEPT {
                if (storage._state == ResultState::Ok) {
                    Init(std::move(storage._ok));
                } else if (storage._state == ResultState::Err) {
                    Init(std::move(storage._err));
                }
            }

        public:

            GSTD_CONSTEXPR ~ResultStorage()
            GSTD_NOEXCEPT(std::conjunction_v<std::is_nothrow_destructible<ValueType>,
                                             std::is_nothrow_destructible<ErrorType>>) {
                if (_state == ResultState::Ok) {
                    std::destroy_at(&_ok.Ref());
                } else if (_state == ResultState::Err) {
                    std::destroy_at(&_err.Ref());
                }
            }

        public:

            /*
             * TODO: Check 'noexcept's
             */

            GSTD_CONSTEXPR auto Init(Ok<ValueType> &&ok) -> void {
                std::construct_at(&_ok,
                                  std::move(ok));
                _state = ResultState::Ok;
            }

            GSTD_CONSTEXPR auto Init(Err<ErrorType> &&err)  -> void {
                std::construct_at(&_err,
                                  std::move(err));
                _state = ResultState::Err;
            }

            GSTD_CONSTEXPR auto Assign(Ok<ValueType> &&ok) -> void {
                if (_state == ResultState::Ok) {
                    _ok = std::move(ok);
                } else if (_state == ResultState::Err) {
                    std::destroy_at(&_err.Ref());

                    Init(std::move(ok));
                }
            }

            GSTD_CONSTEXPR auto Assign(Err<ErrorType> &&err) -> void {
                if (_state == ResultState::Ok) {
                    std::destroy_at(&_ok.Ref());

                    Init(std::move(err));
                } else if (_state == ResultState::Err) {
                    _err = std::move(err);
                }
            }

        public:

            GSTD_CONSTEXPR auto operator=(const ResultStorage &storage)
            GSTD_NOEXCEPT(std::conjunction_v<std::is_nothrow_copy_constructible<ValueType>,
                                             std::is_nothrow_copy_constructible<ErrorType>>) -> ResultStorage & {
                if (this == &storage) {
                    return *this;
                }

                if (storage._state == ResultState::Ok) {
                    Assign(MakeOk(storage._ok.Copy()));
                } else if (storage._state == ResultState::Err) {
                    Assign(MakeErr(storage._err.Copy()));
                }

                return *this;
            }

            GSTD_CONSTEXPR auto operator=(ResultStorage &&storage) GSTD_NOEXCEPT -> ResultStorage & {
                if (this == &storage) {
                    return *this;
                }

                if (storage._state == ResultState::Ok) {
                    Assign(std::move(storage._ok));
                } else if (storage._state == ResultState::Err) {
                    Assign(std::move(storage._err));
                }

                return *this;
            }

        private:

            union {
                Ok<ValueT> _ok;

                Err<ErrorT> _err;
            };

            ResultState _state;
        };

        template<typename ValueT,
                 typename ErrorT>
        class ResultStorage<ValueT,
                            ErrorT,
                            true> {
        public:

            friend class Result<ValueT,
                                ErrorT>;

        public:

            using ValueType = ValueT;

            using ErrorType = ErrorT;

        public:

            GSTD_CONSTEXPR GSTD_EXPLICIT ResultStorage(Ok<ValueT> &&ok) GSTD_NOEXCEPT
                    : _ok(std::move(ok)),
                      _state(ResultState::Ok) {}

            GSTD_CONSTEXPR GSTD_EXPLICIT ResultStorage(Err<ErrorT> &&err) GSTD_NOEXCEPT
                    : _err(std::move(err)),
                      _state(ResultState::Err) {}

            GSTD_CONSTEXPR ResultStorage(const ResultStorage &storage)
            GSTD_NOEXCEPT(std::conjunction_v<std::is_nothrow_copy_constructible<ValueType>,
                                             std::is_nothrow_copy_constructible<ErrorType>>) = default;

            GSTD_CONSTEXPR ResultStorage(ResultStorage &&storage) GSTD_NOEXCEPT = default;

        public:

            GSTD_CONSTEXPR auto Init(Ok<ValueType> &&ok) GSTD_NOEXCEPT -> void {
                _ok = std::move(ok);
                _state = ResultState::Ok;
            }

            GSTD_CONSTEXPR auto Init(Err<ErrorType> &&err) GSTD_NOEXCEPT -> void {
                _err = std::move(err);
                _state = ResultState::Err;
            }

            GSTD_CONSTEXPR auto Assign(Ok<ValueType> &&ok) GSTD_NOEXCEPT -> void {
                Init(std::move(ok));
            }

            GSTD_CONSTEXPR auto Assign(Err<ErrorType> &&err) GSTD_NOEXCEPT -> void {
                Init(std::move(err));
            }

        public:

            GSTD_CONSTEXPR auto operator=(const ResultStorage &storage)
            GSTD_NOEXCEPT(std::conjunction_v<std::is_nothrow_copy_constructible<ValueType>,
                                             std::is_nothrow_copy_constructible<ErrorType>>) -> ResultStorage & = default;

            GSTD_CONSTEXPR auto operator=(ResultStorage &&storage) GSTD_NOEXCEPT -> ResultStorage & = default;

        private:

            union {
                Ok<ValueT> _ok;

                Err<ErrorT> _err;
            };

            ResultState _state;
        };

        template<typename T>
        struct IsResult {

            static GSTD_CONSTEXPR auto Value = false;

        };

        template<typename T,
                 typename E>
        struct IsResult<Result<T,
                               E>> {

            static GSTD_CONSTEXPR auto Value = true;

        };

        template<typename T>
        GSTD_INLINE GSTD_CONSTEXPR auto IsResultV = IsResult<T>::Value;

    }

    template<typename ValueT>
    class Optional;

    /**
     * Class for type safe containing `Ok` value or `Err` value in compile-time and run-rime
     * @tparam ValueT Value type
     * @tparam ErrorT Error type
     */
    template<typename ValueT,
             typename ErrorT>
    class Result : private detail::ResultStorage<ValueT,
                                                 ErrorT> {
    public:

        /*
         *
         * Result PUBLIC TYPES
         *
         */

        /**
         * Value type
         */
        using ValueType = ValueT;

        /**
         * Error type
         */
        using ErrorType = ErrorT;

    private:

        /*
         *
         * Result PRIVATE TYPES
         *
         */

        /**
         * Storage type
         */
        using Storage = detail::ResultStorage<ValueType,
                                              ErrorType>;

    public:

        GSTD_CONSTEXPR Result() = delete;

        GSTD_CONSTEXPR Result(Ok<ValueT> &&ok) GSTD_NOEXCEPT
                : Storage(std::move(ok)) {}

        GSTD_CONSTEXPR Result(Err<ErrorT> &&err) GSTD_NOEXCEPT
                : Storage(std::move(err)) {}

        GSTD_CONSTEXPR Result(const Result &result) = delete;

        GSTD_CONSTEXPR Result(Result &&result) GSTD_NOEXCEPT = default;

    public:

        GSTD_CONSTEXPR auto IsOk() const GSTD_NOEXCEPT -> bool {
            return Storage::_state == detail::ResultState::Ok;
        }

        template<typename FunctionT>
        GSTD_CONSTEXPR auto IsOkAnd(FunctionT &&function)
        const GSTD_NOEXCEPT(std::is_nothrow_invocable_v<FunctionT &&,
                                                        const ValueType &>) -> bool {
            static_assert(std::is_invocable_v<FunctionT &&,
                                              const ValueType &>,
                          "`FunctionT` must be invocable with `ValueType` argument!");
            static_assert(std::is_same_v<std::invoke_result_t<FunctionT &&,
                                                              const ValueType &>,
                                         bool>,
                          "`FunctionT` must return `bool`!");

            if (IsErr()) {
                return false;
            }

            return std::invoke(std::forward<FunctionT>(function),
                               Storage::_ok.CRef());
        }

        GSTD_CONSTEXPR auto IsErr() const GSTD_NOEXCEPT -> bool {
            return !IsOk();
        }

        template<typename FunctionT>
        GSTD_CONSTEXPR auto IsErrAnd(FunctionT &&function)
        const GSTD_NOEXCEPT(std::is_nothrow_invocable_v<FunctionT &&,
                                                        const ErrorType &>) -> bool {
            static_assert(std::is_invocable_v<FunctionT &&,
                                              const ErrorType &>,
                          "`FunctionT` must be invocable with `ErrorType` argument!");
            static_assert(std::is_same_v<std::invoke_result_t<FunctionT &&,
                                                              const ErrorType &>,
                                         bool>,
                          "`FunctionT` must return `bool`!");

            if (IsOk()) {
                return false;
            }

            return std::invoke(std::forward<FunctionT>(function),
                               Storage::_err.CRef());
        }

        // TODO: delete AsOk and AsErr?
        GSTD_CONSTEXPR auto AsOk() && GSTD_NOEXCEPT -> Optional<ValueType>;

        GSTD_CONSTEXPR auto AsErr() && GSTD_NOEXCEPT -> Optional<ErrorType>;

        GSTD_CONSTEXPR auto Copy()
        const & GSTD_NOEXCEPT(std::conjunction_v<std::is_nothrow_copy_constructible<ValueType>,
                                                 std::is_nothrow_copy_constructible<ErrorType>>) -> Result {
            return *this;
        }

        GSTD_CONSTEXPR auto Copy()
        const && GSTD_NOEXCEPT(std::conjunction_v<std::is_nothrow_copy_constructible<ValueType>,
                                                  std::is_nothrow_copy_constructible<ErrorType>>) -> Result = delete;

        GSTD_CONSTEXPR auto AsRef() & GSTD_NOEXCEPT -> Result<Ref<ValueType>,
                                                              Ref<ErrorType>> {
            if (IsOk()) {
                return MakeOk(MakeRef(Storage::_ok.Ref()));
            } else {
                return MakeErr(MakeRef(Storage::_err.Ref()));
            }
        }

        GSTD_CONSTEXPR auto AsRef() const & GSTD_NOEXCEPT -> Result<Ref<const ValueType>,
                                                                    Ref<const ErrorType>> {
            if (IsOk()) {
                return MakeOk(MakeCRef(Storage::_ok.CRef()));
            } else {
                return MakeErr(MakeCRef(Storage::_err.CRef()));
            }
        }

        GSTD_CONSTEXPR auto AsRef() && GSTD_NOEXCEPT -> Result<Ref<ValueType>,
                                                               Ref<ErrorType>>  = delete;

        GSTD_CONSTEXPR auto AsRef() const && GSTD_NOEXCEPT -> Result<Ref<const ValueType>,
                                                                     Ref<const ErrorType>> = delete;

        GSTD_CONSTEXPR auto AsCRef() const & GSTD_NOEXCEPT -> Result<Ref<const ValueType>,
                                                                     Ref<const ErrorType>> {
            if (IsOk()) {
                return MakeOk(MakeCRef(Storage::_ok.CRef()));
            } else {
                return MakeErr(MakeCRef(Storage::_err.CRef()));
            }
        }

        GSTD_CONSTEXPR auto AsCRef() const && GSTD_NOEXCEPT -> Result<Ref<const ValueType>,
                                                                      Ref<const ErrorType>> = delete;

        GSTD_CONSTEXPR auto Move() & GSTD_NOEXCEPT -> Result && {
            return std::move(*this);
        }

        GSTD_CONSTEXPR auto Move() && GSTD_NOEXCEPT -> Result && = delete;

        template<typename InputValueT>
        GSTD_CONSTEXPR auto Contains(const InputValueT &value) const GSTD_NOEXCEPT -> bool {
            static_assert(std::equality_comparable_with<ValueType,
                                                        InputValueT>,
                          "`ValueType` and `InputValueT` must be comparable with '==' operator!");

            if (IsErr()) {
                return false;
            }

            return Storage::_ok.CRef() == value;
        }

        template<typename InputErrorT>
        GSTD_CONSTEXPR auto ContainsErr(const InputErrorT &error) const GSTD_NOEXCEPT -> bool {
            static_assert(std::equality_comparable_with<ErrorType,
                                                        InputErrorT>,
                          "`ErrorType` and `InputErrorT` must be comparable with '==' operator!");

            if (IsOk()) {
                return false;
            }

            return Storage::_err.CRef() == error;
        }

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

            if (IsErr()) {
                return false;
            }

            return std::invoke(std::forward<FunctionT>(function),
                               Storage::_ok.CRef());
        }

        template<typename FunctionT>
        GSTD_CONSTEXPR auto ExistsErr(FunctionT &&function)
        const GSTD_NOEXCEPT(std::is_nothrow_invocable_v<FunctionT &&,
                                                        const ErrorType &>) -> bool {
            static_assert(std::is_invocable_v<FunctionT &&,
                                              const ErrorType &>,
                          "`FunctionT` must be invocable with `ErrorType` argument!");
            static_assert(std::is_same_v<std::invoke_result_t<FunctionT &&,
                                                              const ErrorType &>,
                                         bool>,
                          "`FunctionT` must return `bool`!");

            if (IsOk()) {
                return false;
            }

            return std::invoke(std::forward<FunctionT>(function),
                               Storage::_err.CRef());
        }

        template<typename FunctionT>
        GSTD_CONSTEXPR auto Map(FunctionT &&function)
        && GSTD_NOEXCEPT(std::is_nothrow_invocable_v<FunctionT &&,
                                                     ValueType &&>) -> Result<std::invoke_result_t<FunctionT &&,
                                                                                                   ValueType &&>,
                                                                              ErrorType> {
            static_assert(std::is_invocable_v<FunctionT &&,
                                              ValueType &&>,
                          "`FunctionT` must be invocable with `ValueType` argument!");

            if (IsErr()) {
                return MakeErr(Storage::_err.Move());
            }

            return MakeOk(std::invoke(std::forward<FunctionT>(function),
                                      Storage::_ok.Move()));
        }

        template<typename InputValueT,
                 typename FunctionT>
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

            if (IsErr()) {
                return std::forward<InputValueT>(alternative);
            }

            return std::invoke(std::forward<FunctionT>(function),
                               Storage::_ok.Move());
        }

        template<typename ErrFunctionT,
                 typename OkFunctionT>
        GSTD_CONSTEXPR auto MapOrElse(OkFunctionT &&okFunction,
                                      ErrFunctionT &&errFunction)
        && GSTD_NOEXCEPT(std::conjunction_v<std::is_nothrow_invocable<OkFunctionT &&,
                                                                      ValueType &&>,
                                            std::is_nothrow_invocable<ErrFunctionT &&,
                                                                      ErrorType &&>>) -> std::invoke_result_t<OkFunctionT &&,
                                                                                                              ValueType &&> {
            static_assert(std::is_invocable_v<OkFunctionT &&,
                                              ValueType &&>,
                          "`OkFunctionT` must be invocable with `ValueType` argument!");
            static_assert(std::is_invocable_v<ErrFunctionT &&,
                                              ErrorType &&>,
                          "`ErrFunctionT` must be invocable with `ErrorType` argument!");
            static_assert(std::is_same_v<std::invoke_result_t<OkFunctionT &&,
                                                              ValueType &&>,
                                         std::invoke_result_t<ErrFunctionT &&,
                                                              ErrorType &&>>,
                          "`OkFunctionT` and `ErrFunctionT` return types must be equal!");

            if (IsErr()) {
                return std::invoke(std::forward<ErrFunctionT>(errFunction),
                                   Storage::_err.Move());
            }

            return std::invoke(std::forward<OkFunctionT>(okFunction),
                               Storage::_ok.Move());
        }

        template<typename FunctionT>
        GSTD_CONSTEXPR auto MapErr(FunctionT &&function)
        && GSTD_NOEXCEPT(std::is_nothrow_invocable_v<FunctionT &&,
                                                     ErrorType &&>) -> Result<ValueType,
                                                                              std::invoke_result_t<FunctionT &&,
                                                                                                   ErrorType &&>> {
            static_assert(std::is_invocable_v<FunctionT &&,
                                              ErrorType &&>,
                          "`FunctionT` must be invocable with `ErrorType` argument!");

            if (IsErr()) {
                return MakeErr(std::invoke(std::forward<FunctionT>(function),
                                           Storage::_err.Move()));
            }

            return MakeOk(Storage::_ok.Move());
        }

        template<typename FunctionT>
        GSTD_CONSTEXPR auto Inspect(FunctionT &&function)
        const GSTD_NOEXCEPT(std::conjunction_v<std::is_nothrow_copy_constructible<ValueType>,
                                               std::is_nothrow_invocable<FunctionT &&,
                                                                         const ValueType &>>) -> Result {
            static_assert(std::is_invocable_v<FunctionT &&,
                                              const ValueType &>,
                          "`FunctionT` must be invocable with `ValueType` argument!");
            static_assert(std::is_same_v<std::invoke_result_t<FunctionT &&,
                                                              const ValueType &>,
                                         void>,
                          "`FunctionT` must return `void`!");

            if (IsOk()) {
                std::invoke(std::forward<FunctionT>(function),
                            Storage::_ok.CRef());
            }

            return *this;
        }

        template<typename FunctionT>
        GSTD_CONSTEXPR auto InspectErr(FunctionT &&function)
        const GSTD_NOEXCEPT(std::conjunction_v<std::is_nothrow_copy_constructible<ErrorType>,
                                               std::is_nothrow_invocable<FunctionT &&,
                                                                         const ErrorType &>>) -> Result {
            static_assert(std::is_invocable_v<FunctionT &&,
                                              const ErrorType &>,
                          "`FunctionT` must be invocable with `ErrorType` argument!");
            static_assert(std::is_same_v<std::invoke_result_t<FunctionT &&,
                                                              const ErrorType &>,
                                         void>,
                          "`FunctionT` must return `void`!");

            if (IsErr()) {
                std::invoke(std::forward<FunctionT>(function),
                            Storage::_err.CRef());
            }

            return *this;
        }

        GSTD_CONSTEXPR auto Expect(/* TODO: Replace to gstd::String */ std::string_view message) && -> ValueType {
            if (IsErr()) {
                // TODO: create trait `Debug`
                Panic(fmt::format("`Result` failed expect `Ok` with message '{}'!", message).c_str());
            }

            return Storage::_ok.Move();
        }

        GSTD_CONSTEXPR auto Unwrap() && -> ValueType {
            if (IsErr()) {
                Panic("Unwrap `Result` failed, it contains `Err'!");
            }

            return Storage::_ok.Move();
        }

        GSTD_CONSTEXPR auto UnwrapOrDefault()
        && GSTD_NOEXCEPT(std::is_nothrow_default_constructible_v<ValueType>) -> ValueType {
            static_assert(std::is_default_constructible_v<ValueType>,
                          "`ValueType` must be default constructible!");

            if (IsErr()) {
                return ValueType {};
            }

            return Storage::_ok.Move();
        }

        GSTD_CONSTEXPR auto ExpectErr(/* TODO: Replace to gstd::String */ std::string_view message) && -> ErrorType {
            if (IsOk()) {
                // TODO: `Debug` trait
                Panic(fmt::format("`Result` failed expect `Err` with message '{}'!", message).c_str());
            }

            return Storage::_err.Move();
        }

        GSTD_CONSTEXPR auto UnwrapErr() && -> ErrorType {
            if (IsOk()) {
                Panic("Unwrap `Result` failed, it contains `Ok'!");
            }

            return Storage::_err.Move();
        }

        GSTD_CONSTEXPR auto UnwrapErrOrDefault()
        && GSTD_NOEXCEPT(std::is_nothrow_default_constructible_v<ErrorType>)-> ErrorType {
            static_assert(std::is_default_constructible_v<ErrorType>,
                          "`ErrorType` must be default constructible!");

            if (IsOk()) {
                return ErrorType {};
            }

            return Storage::_err.Move();
        }

        template<typename InputValueT>
        GSTD_CONSTEXPR auto And(Result<InputValueT,
                                       ErrorType> &&result) && GSTD_NOEXCEPT -> Result<InputValueT,
                                                                                       ErrorType> {
            if (IsErr()) {
                return MakeErr(Storage::_err.Move());
            }

            return std::move(result);
        }

        template<typename FunctionT>
        GSTD_CONSTEXPR auto AndThen(FunctionT &&function)
        && GSTD_NOEXCEPT(std::is_nothrow_invocable_v<FunctionT &&,
                                                     ValueType &&>) -> std::invoke_result_t<FunctionT &&,
                                                                                            ValueType &&> {
            static_assert(std::is_invocable_v<FunctionT &&,
                                              ValueType &&>,
                          "`FunctionT` must be invocable with `ValueType` argument!");
            static_assert(detail::IsResultV<std::invoke_result_t<FunctionT &&,
                                                                 ValueType &&>>,
                          "`FunctionT` must return `Result`!");
            static_assert(std::is_same_v<typename std::invoke_result_t<FunctionT &&,
                                                                       ValueType &&>::ErrorType,
                                         ErrorType>,
                          "`FunctionT` must return `Result` with `ErrorType` that equal to self `ErrorType`!");

            if (IsErr()) {
                return MakeErr(Storage::_err.Move());
            }

            return std::invoke(std::forward<FunctionT>(function),
                               Storage::_ok.Move());
        }

        template<typename InputErrorT>
        GSTD_CONSTEXPR auto Or(Result<ValueType,
                                      InputErrorT> &&result) && GSTD_NOEXCEPT -> Result<ValueType,
                                                                                        InputErrorT> {
            if (IsErr()) {
                return std::move(result);
            }

            return MakeOk(Storage::_ok.Move());
        }

        template<typename FunctionT>
        GSTD_CONSTEXPR auto OrElse(FunctionT &&function)
        && GSTD_NOEXCEPT(std::is_nothrow_invocable_v<FunctionT &&,
                                                     ErrorType &&>) -> std::invoke_result_t<FunctionT &&,
                                                                                            ErrorType &&> {
            static_assert(std::is_invocable_v<FunctionT &&,
                                              ErrorType &&>,
                          "`FunctionT` must be invocable with `ErrorType` argument!");
            static_assert(detail::IsResultV<std::invoke_result_t<FunctionT &&,
                                                                 ErrorType &&>>,
                          "`FunctionT` must return `Result`!");
            static_assert(std::is_same_v<typename std::invoke_result_t<FunctionT &&,
                                                                       ErrorType &&>::ValueType,
                                         ValueType>,
                          "`FunctionT` must return `Result` with `ValueType` that equal to self `ValueType`!");

            if (IsErr()) {
                return std::invoke(std::forward<FunctionT>(function),
                                   Storage::_err.Move());
            }

            return MakeOk(Storage::_ok.Move());
        }

        GSTD_CONSTEXPR auto UnwrapOr(ValueType &&value) && GSTD_NOEXCEPT -> ValueType {
            if (IsErr()) {
                return std::move(value);
            }

            return MakeOk(Storage::_ok.Move());
        }

        template<typename FunctionT>
        GSTD_CONSTEXPR auto UnwrapOrElse(FunctionT &&function)
        && GSTD_NOEXCEPT(std::is_nothrow_invocable_v<FunctionT &&,
                                                     ErrorType &&>) -> ValueType {
            static_assert(std::is_invocable_v<FunctionT &&,
                                              ErrorType &&>,
                          "`FunctionT` must be invocable with `ErrorType` argument!");
            static_assert(std::is_same_v<std::invoke_result_t<FunctionT &&,
                                                              ErrorType &&>,
                                         ValueType>,
                          "`FunctionT` must return `ValueType`!");

            if (IsErr()) {
                return std::invoke(std::forward<FunctionT>(function),
                                   Storage::_err.Move());
            }

            return Storage::_some.Move();
        }

    public:

        GSTD_CONSTEXPR auto operator=(const Result &result) -> Result & = delete;

        GSTD_CONSTEXPR auto operator=(Result &&result) GSTD_NOEXCEPT -> Result & = default;


        GSTD_CONSTEXPR auto operator=(Ok<ValueType> &&ok) GSTD_NOEXCEPT -> Result & {
            Storage::Assign(std::forward<Ok<ValueType>>(ok));

            return *this;
        }

        GSTD_CONSTEXPR auto operator=(Err<ErrorType> &&err) GSTD_NOEXCEPT -> Result & {
            Storage::Assign(std::forward<Err<ErrorType>>(err));

            return *this;
        }

        GSTD_CONSTEXPR operator bool() const GSTD_NOEXCEPT {
            return IsOk();
        }
    };

    template<typename ValueType,
             typename ErrorType>
    GSTD_CONSTEXPR auto MakeOkResult(ValueType &&value) -> Result<ValueType,
                                                                  ErrorType> {
        return Result<ValueType, ErrorType> {
            MakeOk<ValueType>(std::forward<ValueType>(value))
        };
    }

    template<typename ValueType,
             typename ErrorType>
    GSTD_CONSTEXPR auto MakeErrResult(ErrorType &&error) -> Result<ValueType,
                                                                   ErrorType> {
        return Result<ValueType,
                      ErrorType> {
            MakeErr<ErrorType>(std::forward<ErrorType>(error))
        };
    }

}

#endif //GSTD_RESULT_H
