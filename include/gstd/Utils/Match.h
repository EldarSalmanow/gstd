#ifndef GSTD_MATCH_H
#define GSTD_MATCH_H

#include <gstd/Type/Optional.h>

namespace gstd {

    template<typename ResultT,
             typename ValueT>
    class Matcher {
    public:

        using ResultType = ResultT;

        using ValueType = ValueT;

    public:

        GSTD_CONSTEXPR GSTD_EXPLICIT Matcher(ValueType &&matchValue)
                : _matchValue(std::forward<ValueType>(matchValue)),
                  _result(MakeNone()) {}

    public:

        static GSTD_CONSTEXPR auto New(ValueType &&value) -> Matcher<ResultType,
                                                                     ValueType> {
            return Matcher {
                std::forward<ValueType>(value)
            };
        }

    public:

        GSTD_CONSTEXPR auto Case(ValueType &&matchingValue,
                                     ResultType &&resultValue) -> Matcher<ResultType,
                                                                          ValueType> & {
            if (_matchValue == std::forward<ValueType>(matchingValue)) {
                _result.Insert(std::forward<ResultType>(resultValue));
            }

            return *this;
        }

        template<typename FunctionT>
        GSTD_CONSTEXPR auto Case(FunctionT &&matchingFunction,
                                     ResultType &&resultValue) -> Matcher<ResultType,
                                                                          ValueType> & {
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

        GSTD_CONSTEXPR auto Default(ResultType &&resultValue) -> Matcher<ResultType,
                                                                             ValueType> & {
            if (_result.IsNone()) {
                _result.Insert(std::forward<ResultType>(resultValue));
            }

            return *this;
        }

        GSTD_CONSTEXPR auto Result() -> Optional<ResultType> {
            return _result;
        }

    private:

        ValueType _matchValue;

        Optional<ResultType> _result;
    };

    template<typename ResultT,
             typename ValueT>
    GSTD_CONSTEXPR auto Match(ValueT &&value) -> Matcher<ResultT,
                                                         ValueT> {
        return Matcher<ResultT,
                       ValueT>::New(std::forward<ValueT>(value));
    }

    enum class NodeKind {
        Declaration,
        Statement,
        Expression
    };

    GSTD_CONSTEXPR auto ToString(NodeKind kind) -> std::string_view {
        return Match<std::string_view>(std::move(kind))
                .Case(NodeKind::Declaration, "Declaration")
                .Case(NodeKind::Statement,   "Statement")
                .Case(NodeKind::Expression,  "Expression")
                .Default("<unknown>")
                .Result()
                .Unwrap();
    }

    void l() {
        static_assert(ToString(NodeKind::Expression) == "Expression");
        static_assert(ToString(NodeKind::Declaration) == "Declaration");
    }

}

#endif //GSTD_MATCH_H
