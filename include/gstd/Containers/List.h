#ifndef GSTD_LIST_H
#define GSTD_LIST_H

#include <gstd/Containers/Node.h>

namespace gstd {

    template<typename ValueT>
    class SinglyLinkedList;

    template<typename ValueT>
    class SinglyLinkedListIterator {
    public:

        friend class SinglyLinkedList<ValueT>;

    public:

        using ValueType = ValueT;

        using ContainerType = SinglyLinkedList<ValueType>;

        using NodeType = SinglyLinkedNode<ValueType>;

    public:

        SinglyLinkedListIterator(NodeType *current)
                : _current(current) {}

    public:

        GSTD_CONSTEXPR auto operator++() -> void {
            if (!_current->Next()) {
                Panic("!!!");
            }

            _current = _current->Next();
        }

        GSTD_CONSTEXPR auto operator*() const -> ValueType & {
            return _current->Value();
        }

    private:

        NodeType *_current;
    };

    template<typename ValueT>
    class SinglyLinkedList {
    public:

        using ValueType = ValueT;

        using SizeType = std::uint64_t;

        using IndexType = std::uint64_t;

        using NodeType = SinglyLinkedNode<ValueType>;

        using IteratorType = SinglyLinkedListIterator<ValueType>;

        using ConstIteratorType = const SinglyLinkedListIterator<const ValueType>;

    public:

        GSTD_CONSTEXPR auto At(IndexType index) GSTD_NOEXCEPT -> Optional<Ref<ValueType>> {
            if (index >= Size()) {
                return MakeNone();
            }

            auto current = _begin;

            for (IndexType i = 0; i < index; ++i) {
                current = current._next;
            }

            return MakeSome(MakeRef(current._value));
        }

        void Add(ValueType value) {
            if (!_begin) {
                _begin = new NodeType(std::move(value),
                                      nullptr);

                return;
            }

            auto current = _begin;

            for (IndexType index = 0; index < Size(); ++index) {
                current = _begin->Next();
            }

            current->Next() = new NodeType(std::move(value), nullptr);
        }

        GSTD_CONSTEXPR auto Remove(const ValueType &value) -> void {
            auto iterator = Find(value);

            if (iterator != end()) {
                auto next = iterator._current._next;


            }
        }

        GSTD_CONSTEXPR auto Find(const ValueType &value) -> IteratorType {
            auto current = begin();

            while (current != end()
                && current->Value() != value) {
                ++current;
            }

            return current;
        }

        GSTD_CONSTEXPR auto Find(const ValueType &value) const -> ConstIteratorType {
            auto current = begin();

            while (current != end()
                   && current->Value() != value) {
                ++current;
            }

            return current;
        }

        GSTD_CONSTEXPR auto Size() const GSTD_NOEXCEPT {
            return _size;
        }

    public:

        GSTD_CONSTEXPR auto begin() -> IteratorType {
            return IteratorType(_begin);
        }

        GSTD_CONSTEXPR auto end() -> IteratorType {
            return IteratorType(_begin);
        }

        GSTD_CONSTEXPR auto begin() const -> ConstIteratorType {
            return ConstIteratorType(_begin);
        }

        GSTD_CONSTEXPR auto end() const -> ConstIteratorType {
            return ConstIteratorType(_begin);
        }

    public:

        constexpr auto operator[](IndexType index) {
            return At(index).UnwrapOrElse([] () -> void {
                Panic("Index out of range!");
            });
        }

    private:

        NodeType *_begin;

        SizeType _size;
    };

}

#endif //GSTD_LIST_H
