#ifndef GSTD_MATCH_H
#define GSTD_MATCH_H

#include <gstd/Type/Optional.h>

namespace gstd {

    /**
     * Class for emulation pattern matching functionality from other programming languages
     * @tparam ResultT Result value type after matching
     * @tparam ValueT Input value type for matching
     */
    template<typename ResultT,
             typename ValueT>
    class Matcher {
    public:

        /**
         * Result value type after matching
         */
        using ResultType = ResultT;

        /**
         * Input value type for matching
         */
        using ValueType = ValueT;

    public:

        /**
         * Constructor for 'Matcher' from input value
         * @param matchValue Value for matching
         */
        GSTD_CONSTEXPR GSTD_EXPLICIT Matcher(ValueType &&matchValue)
                : _matchValue(std::forward<ValueType>(matchValue)),
                  _result(MakeNone()) {}

    public:

        /**
         *
         */
        static GSTD_CONSTEXPR auto New(ValueType &&value) -> Matcher {
            return Matcher {
                std::forward<ValueType>(value)
            };
        }

    public:

        GSTD_CONSTEXPR auto Case(ValueType &&matchingValue,
                                 ResultType &&resultValue) -> Matcher & {
            if (_matchValue == std::forward<ValueType>(matchingValue)) {
                _result.Insert(std::forward<ResultType>(resultValue));
            }

            return *this;
        }

        template<typename FunctionT>
        GSTD_CONSTEXPR auto Case(FunctionT &&matchingFunction,
                                 ResultType &&resultValue) -> Matcher & {
            static_assert(IsSameV<std::invoke_result_t<FunctionT &&,
                                                       ValueType &&>,
                                  bool>,
                          "`FunctionT` must return value of `bool` type!");

            if (std::invoke(std::forward<FunctionT>(matchingFunction),
                            std::forward<ValueType>(_matchValue))) {
                _result.Insert(std::forward<ResultType>(resultValue));
            }

            return *this;
        }

        template<typename TypeT>
        GSTD_CONSTEXPR auto Case(ResultType &&resultValue) -> Matcher & {
            if GSTD_CONSTEXPR (std::is_same_v<TypeT, ValueType>) {
                _result.Insert(std::forward<ResultType>(resultValue));
            }

            return *this;
        }

        /**
         * Set default value if all cases ??
         */
        GSTD_CONSTEXPR auto Default(ResultType &&resultValue) -> Matcher & {
            if (_result.IsNone()) {
                _result.Insert(std::forward<ResultType>(resultValue));
            }

            return *this;
        }

        /**
         * Getting result of pattern matching
         */
        GSTD_CONSTEXPR auto Result() -> Optional<ResultType> {
            return _result;
        }

    private:

        /**
         * Value for pattern matching
         */
        ValueType _matchValue;

        /**
         * Result of pattern matching
         */
        Optional<ResultType> _result;
    };

    template<typename ResultT,
             typename ValueT>
    GSTD_CONSTEXPR auto Match(ValueT &&value) -> Matcher<ResultT,
                                                         ValueT> {
        return Matcher<ResultT,
                       ValueT>::New(std::forward<ValueT>(value));
    }

}

#endif //GSTD_MATCH_H
