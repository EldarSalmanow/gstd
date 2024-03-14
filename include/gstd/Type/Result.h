#ifndef GSTD_RESULT_H
#define GSTD_RESULT_H

namespace gstd {

    template<typename ValueT>
    class Optional;

    template<typename ValueT>
    class Ok {
    public:

        using ValueType = ValueT;

    public:

        GSTD_CONSTEXPR GSTD_EXPLICIT Ok(ValueType &&value) GSTD_NOEXCEPT
                : _value(std::move(value)) {}

    public:

        static GSTD_CONSTEXPR auto New(ValueType &&value) GSTD_NOEXCEPT -> Ok<ValueType> {
            return Ok<ValueType> {
                std::forward<ValueType>(value)
            };
        }

    public:

        GSTD_CONSTEXPR auto Copy()
        GSTD_NOEXCEPT(std::is_nothrow_copy_constructible_v<ValueType>) -> ValueType {
            return _value;
        }

        GSTD_CONSTEXPR auto Ref() & GSTD_NOEXCEPT -> ValueType & {
            return _value;
        }

        GSTD_CONSTEXPR auto CRef() const & GSTD_NOEXCEPT -> const ValueType & {
            return _value;
        }

        GSTD_CONSTEXPR auto Move() && GSTD_NOEXCEPT -> ValueType && {
            return std::move(_value);
        }

        GSTD_CONSTEXPR auto CMove() const && GSTD_NOEXCEPT -> const ValueType && {
            return std::move(_value);
        }

    private:

        ValueType _value;
    };

    template<typename ErrorT>
    class Err {
    public:

        using ErrorType = ErrorT;

    public:

        GSTD_CONSTEXPR GSTD_EXPLICIT Err(ErrorType &&error) GSTD_NOEXCEPT
                : _error(std::move(error)) {}

    public:

        static GSTD_CONSTEXPR auto New(ErrorType &&error) GSTD_NOEXCEPT -> Err<ErrorType> {
            return Err<ErrorType> {
                std::forward<ErrorType>(error)
            };
        }

    public:

        GSTD_CONSTEXPR auto Copy()
        const GSTD_NOEXCEPT(std::is_nothrow_copy_constructible_v<ErrorType>) -> ErrorType {
            return _error;
        }

        GSTD_CONSTEXPR auto Ref() & GSTD_NOEXCEPT -> ErrorType & {
            return _error;
        }

        GSTD_CONSTEXPR auto CRef() const & GSTD_NOEXCEPT -> const ErrorType & {
            return _error;
        }

        GSTD_CONSTEXPR auto Move() && GSTD_NOEXCEPT -> ErrorType && {
            return std::move(_error);
        }

        GSTD_CONSTEXPR auto CMove() const && GSTD_NOEXCEPT -> const ErrorType && {
            return std::move(_error);
        }

    private:

        ErrorType _error;
    };

    namespace detail {

        enum class ResultState {
            Ok,
            Err
        };

        template<typename ValueT,
                 typename ErrorT>
        class ResultStorage {
        public:

            GSTD_CONSTEXPR GSTD_EXPLICIT ResultStorage(Ok<ValueT> &&ok) GSTD_NOEXCEPT
                    : _ok(Move(ok)),
                      _isOk(true) {}

            GSTD_CONSTEXPR GSTD_EXPLICIT ResultStorage(Err<ErrorT> &&err) GSTD_NOEXCEPT
                    : _err(Move(err)),
                      _isOk(false) {}

        public:

            GSTD_CONSTEXPR ~ResultStorage() GSTD_NOEXCEPT {

            }

        public:

            GSTD_CONSTEXPR auto GetOk() -> Ok<ValueT> {
                if (!IsOk()) {
                    //
                }

                return _ok;
            }

            GSTD_CONSTEXPR auto GetErr() -> Err<ErrorT> {
                if (!IsErr()) {
                    //
                }

                return _err;
            }

            GSTD_CONSTEXPR auto State() -> ResultState {
                return _state;
            }

        private:

            union {
                Ok<ValueT> _ok;

                Err<ErrorT> _err;
            };

            ResultState _state;
        };

    }

    template<typename ValueT,
             typename ErrorT>
    class Result : private detail::ResultStorage<ValueT,
                                                 ErrorT> {
    public:

        using ValueType = ValueT;

        using ErrorType = ErrorT;

    private:

        using Storage = detail::ResultStorage<ValueType, ErrorType>;

    public:

        GSTD_CONSTEXPR Result() = delete;

        GSTD_CONSTEXPR Result(Ok<ValueT> &&ok);

        GSTD_CONSTEXPR Result(Err<ErrorT> &&err);

        template<typename InputValueT,
                 typename = EnableIfT<IsConvertibleV<InputValueT, ValueType>>>
        GSTD_CONSTEXPR Result(InputValueT &&value);

        GSTD_CONSTEXPR Result(const Result<ValueType, ErrorType> &result) = delete;

        GSTD_CONSTEXPR Result(Result<ValueType, ErrorType> &&result) GSTD_NOEXCEPT = default;

    public:

        GSTD_CONSTEXPR auto IsOk() const GSTD_NOEXCEPT -> bool;

        template<typename UnaryPredicateT>
        GSTD_CONSTEXPR auto IsOkAnd(UnaryPredicateT &&predicate) const -> bool;

        GSTD_CONSTEXPR auto IsErr() const GSTD_NOEXCEPT -> bool;

        template<typename UnaryPredicateT>
        GSTD_CONSTEXPR auto IsErrAnd(UnaryPredicateT &&predicate) const -> bool;

        GSTD_CONSTEXPR auto Ok() const /*GSTD_NOEXCEPT*/ -> Optional<ValueType>;

        GSTD_CONSTEXPR auto Err() const /*GSTD_NOEXCEPT*/ -> Optional<ErrorType>;

        template<typename FunctionT>
        GSTD_CONSTEXPR auto Map(FunctionT &&function) -> Result<std::invoke_result_t<FunctionT &&,
                                                                                     ValueType &&>,
                                                                ErrorType>;

        template<typename InputValueT,
                 typename FunctionT,
                 typename = EnableIfT<IsConvertibleV<std::invoke_result_t<FunctionT &&,
                                                                          ValueType &&>,
                                                     InputValueT>>>
        GSTD_CONSTEXPR auto MapOr(InputValueT &&value,
                                  FunctionT &&function) -> InputValueT;

        // TODO: Rework function?
        template<typename ErrFunctionT,
                 typename OkFunctionT,
                 typename = EnableIfT<IsSameV<std::invoke_result_t<ErrFunctionT &&,
                                                                   ErrorType &&>,
                                              std::invoke_result_t<OkFunctionT &&,
                                                                   ValueType &&>>>>
        GSTD_CONSTEXPR auto MapOrElse(ErrFunctionT &&errFunction,
                                      OkFunctionT &&okFunction) -> std::invoke_result_t<ErrFunctionT &&,
                                                                                        ErrorType &&>;

        template<typename FunctionT>
        GSTD_CONSTEXPR auto MapErr(FunctionT &&function) -> Result<ValueType,
                                                                   std::invoke_result_t<FunctionT &&,
                                                                                        ErrorType &&>>;

        // TODO: Check function type
        template<typename FunctionT>
        GSTD_CONSTEXPR auto Inspect(FunctionT &&function) -> Result<ValueType,
                                                                    ErrorType>;

        // TODO: Check function type
        template<typename FunctionT>
        GSTD_CONSTEXPR auto InspectErr(FunctionT &&function) -> Result<ValueType,
                                                                       ErrorType>;

        GSTD_CONSTEXPR auto Expect(/* TODO: Replace to gstd::String */ std::string_view message) -> ValueType;

        GSTD_CONSTEXPR auto Unwrap() -> ValueType;

        GSTD_CONSTEXPR auto UnwrapOrDefault() -> std::enable_if_t<std::is_default_constructible_v<ValueType>,
                                                                  ValueType>;

        GSTD_CONSTEXPR auto ExpectErr(/* TODO: Replace to gstd::String */ std::string_view message) -> ErrorType;

        GSTD_CONSTEXPR auto UnwrapErr() -> ErrorType;

        template<typename InputValueT>
        GSTD_CONSTEXPR auto And(Result<InputValueT, ErrorType> &&result) -> Result<InputValueT,
                                                                                   ErrorType>;

        template<typename FunctionT>
        GSTD_CONSTEXPR auto AndThen(FunctionT &&function) -> Result<std::invoke_result_t<FunctionT &&,
                                                                                         ValueType &&>,
                                                                    ErrorType>;

        template<typename InputErrorType>
        GSTD_CONSTEXPR auto Or(Result<ValueType, InputErrorType> &&result) -> Result<ValueType,
                                                                                     InputErrorType>;

        template<typename FunctionT>
        GSTD_CONSTEXPR auto OrElse(FunctionT &&function) -> Result<ValueType,
                                                                   std::invoke_result_t<FunctionT &&,
                                                                                        ErrorType &&>>;

        GSTD_CONSTEXPR auto UnwrapOr(ValueType &&value) -> ValueType;

        template<typename FunctionT,
                 typename = EnableIfT<IsSameV<std::invoke_result_t<FunctionT &&,
                                                                   ErrorType &&>,
                                              ValueType>>>
        GSTD_CONSTEXPR auto UnwrapOrElse(FunctionT &&function) -> ValueType;

        GSTD_CONSTEXPR auto Value() & -> ValueType &;

        GSTD_CONSTEXPR auto Value() const & -> const ValueType &;

        GSTD_CONSTEXPR auto Value() && -> ValueType &&;

        GSTD_CONSTEXPR auto Value() const && -> const ValueType &&;

        GSTD_CONSTEXPR auto Error() & -> ErrorType &;

        GSTD_CONSTEXPR auto Error() const & -> const ErrorType &;

        GSTD_CONSTEXPR auto Error() && -> ErrorType &&;

        GSTD_CONSTEXPR auto Error() const && -> const ErrorType &&;

    public:

//        GSTD_CONSTEXPR auto operator=(Ok<ValueType> &&ok) -> Result<ValueType, ErrorType> &;

//        GSTD_CONSTEXPR auto operator=(Err<ErrorType> &&err) -> Result<ValueType, ErrorType> &;

//        GSTD_CONSTEXPR auto operator=(const Result<ValueType, ErrorType> &result) -> Result<ValueType, ErrorType> & = delete;

//        GSTD_CONSTEXPR auto operator=(Result<ValueType, ErrorType> &&result) GSTD_NOEXCEPT -> Result<ValueType, ErrorType> & = default;

//        GSTD_CONSTEXPR operator bool() const {
//            return IsOk();
//        }
    };

    template<typename ValueType>
    GSTD_CONSTEXPR auto MakeOk(ValueType &&value) -> Ok<ValueType> {
        return Ok<ValueType>(Forward<ValueType>(value));
    }

    template<typename ErrorType>
    GSTD_CONSTEXPR auto MakeErr(ErrorType &&error) -> Err<ErrorType> {
        return Err<ErrorType>(Forward<ErrorType>(error));
    }

}

#endif //GSTD_RESULT_H
