#ifndef GSTD_NODE_H
#define GSTD_NODE_H

#include <gstd/Memory/RawPtr.h>

namespace gstd {

    template<typename ValueT>
    class BaseNode {
    public:

        using ValueType = ValueT;

    public:

        GSTD_CONSTEXPR GSTD_EXPLICIT BaseNode(ValueType value) GSTD_NOEXCEPT
                : _value(std::move(value)) {}

    public:

        static GSTD_CONSTEXPR auto New(ValueType value) GSTD_NOEXCEPT -> BaseNode {
            return BaseNode {
                std::move(value)
            };
        }

    public:

        GSTD_CONSTEXPR auto Value() GSTD_NOEXCEPT -> Ref<ValueType> {
            return MakeRef(_value);
        }

        GSTD_CONSTEXPR auto Value() const GSTD_NOEXCEPT -> Ref<const ValueType> {
            return MakeCRef(_value);
        }

    private:

        ValueType _value;
    };

    template<typename ValueT>
    class SinglyLinkedNode : public BaseNode<ValueT> {
    public:

        using ValueType = ValueT;

    public:

        GSTD_CONSTEXPR GSTD_EXPLICIT SinglyLinkedNode(ValueType value,
                                                      RawPtr<SinglyLinkedNode> next) GSTD_NOEXCEPT
                : BaseNode<ValueType>(std::move(value)),
                  _next(next) {}

    public:

        static GSTD_CONSTEXPR auto New(ValueType value,
                                       RawPtr<SinglyLinkedNode> next) GSTD_NOEXCEPT -> SinglyLinkedNode {
            return SinglyLinkedNode {
                std::move(value),
                next
            };
        }

        static GSTD_CONSTEXPR auto New(ValueType value) GSTD_NOEXCEPT -> SinglyLinkedNode {
            return SinglyLinkedNode::New(std::move(value),
                                         nullptr);
        }

    public:

        GSTD_CONSTEXPR auto Next() GSTD_NOEXCEPT -> RawPtr<SinglyLinkedNode> {
            return _next;
        }

        GSTD_CONSTEXPR auto Next() const GSTD_NOEXCEPT -> RawPtr<const SinglyLinkedNode> {
            return _next;
        }

    private:

        RawPtr<SinglyLinkedNode> _next;
    };

    template<typename ValueT>
    class DoublyLinkedNode : public BaseNode<ValueT> {
    public:

        using ValueType = ValueT;

    public:

        GSTD_CONSTEXPR GSTD_EXPLICIT DoublyLinkedNode(ValueType value,
                                                      RawPtr<DoublyLinkedNode> prev,
                                                      RawPtr<DoublyLinkedNode> next) GSTD_NOEXCEPT
                : BaseNode<ValueType>(std::move(value)),
                  _next(next),
                  _prev(prev) {}

    public:

        static GSTD_CONSTEXPR auto New(ValueType value,
                                       DoublyLinkedNode *prev,
                                       DoublyLinkedNode *next) GSTD_NOEXCEPT -> DoublyLinkedNode {
            return DoublyLinkedNode {
                std::move(value),
                prev,
                next
            };
        }

        static GSTD_CONSTEXPR auto NewWithPrev(ValueType value,
                                               DoublyLinkedNode *prev) GSTD_NOEXCEPT -> DoublyLinkedNode {
            return DoublyLinkedNode::New(std::move(value),
                                         prev);
        }

        static GSTD_CONSTEXPR auto NewWithNext(ValueType value,
                                               DoublyLinkedNode *next) GSTD_NOEXCEPT -> DoublyLinkedNode {
            return DoublyLinkedNode::New(std::move(value),
                                         next);
        }

        static GSTD_CONSTEXPR auto New(ValueType value) GSTD_NOEXCEPT -> DoublyLinkedNode {
            return DoublyLinkedNode::New(std::move(value),
                                         nullptr,
                                         nullptr);
        }

    public:

        GSTD_CONSTEXPR auto Next() GSTD_NOEXCEPT -> DoublyLinkedNode * {
            return _next;
        }

        GSTD_CONSTEXPR auto Next() const GSTD_NOEXCEPT -> const DoublyLinkedNode * {
            return _next;
        }

        GSTD_CONSTEXPR auto Prev() GSTD_NOEXCEPT -> DoublyLinkedNode * {
            return _prev;
        }

        GSTD_CONSTEXPR auto Prev() const GSTD_NOEXCEPT -> const DoublyLinkedNode * {
            return _prev;
        }

    private:

        DoublyLinkedNode *_next;

        DoublyLinkedNode *_prev;
    };

}

#endif //GSTD_NODE_H
