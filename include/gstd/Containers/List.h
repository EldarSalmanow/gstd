#ifndef GSTD_LIST_H
#define GSTD_LIST_H

namespace gstd {

    namespace detail {

        template<typename ValueT>
        class Node {
        public:

            using ValueType = ValueT;

        private:

            ValueType _value;

            Optional<Node<ValueType>> _next;
        };

    }

    template<typename ValueT,
             typename AllocatorT>
    class List {
    public:

        using ValueType = ValueT;

        using AllocatorType = AllocatorT;

    public:

        constexpr auto operator[](size_t index) {
            auto current = _begin;

            for (size_t i = 0; i < index && current._next.IsSome(); ++i) {
                current = current._next;
            }

            return current._value;
        }

    private:

        Optional<detail::Node<ValueT>> _begin;
    };

}

#endif //GSTD_LIST_H
