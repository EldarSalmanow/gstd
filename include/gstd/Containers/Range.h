#ifndef GSTD_RANGE_H
#define GSTD_RANGE_H

namespace gstd {

    template<typename ValueT>
    class RangeIterator {
    public:

        constexpr RangeIterator(ValueT value)
                : _value(value) {}

    public:

        constexpr auto operator==(RangeIterator<ValueT> &rangeIterator) -> bool {
            return _value == rangeIterator._value;
        }

        constexpr auto operator*() -> ValueT {
            return _value;
        }

        constexpr auto operator++() -> void {
            ++_value;
        }

    private:

        ValueT _value;
    };

    template<typename ValueT>
    class Range {
    public:

        using ValueType = ValueT;

        using Iterator = RangeIterator<ValueType>;

        using ConstIterator = const Iterator;

    public:

        GSTD_CONSTEXPR Range(ValueT *start,
                             ValueT *end)
                : _start(start),
                  _end(end) {}

    public:

        constexpr auto begin() -> Iterator {
            return RangeIterator(_start);
        }

        constexpr auto end() -> Iterator {
            return RangeIterator(_end);
        }

        constexpr auto begin() const -> ConstIterator {
            return _start;
        }

        constexpr auto end() const -> ConstIterator {
            return _end;
        }

        constexpr auto cbegin() const -> ConstIterator {
            return _start;
        }

        constexpr auto cend() const -> ConstIterator {
            return _end;
        }

    private:

        ValueT *_start;

        ValueT *_end;
    };

    template<typename T>
    auto PrintLn(T &&value) -> void;

    template<typename ContainerT,
             typename FunctionT>
    constexpr auto ForEach(const ContainerT &container,
                           FunctionT function) -> void {
        auto begin = container.begin();
        auto end = container.end();

        for (auto iterator = begin; iterator != end; ++iterator) {
            function(*iterator);
        }
    }

}

void g() {
//    gstd::ForEach(gstd::Range(1, 5),
//                  [] (const auto &value) -> void {
//        gstd::PrintLn(value);
//    });
//
//    for (auto value : gstd::Range(1, 20)) {
//        gstd::PrintLn(value);
//    }
}

#endif //GSTD_RANGE_H
