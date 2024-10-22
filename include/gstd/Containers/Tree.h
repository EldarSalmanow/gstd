#ifndef GSTD_TREE_H
#define GSTD_TREE_H

#include <gstd/Containers/Node.h>

#include <gstd/Memory/Memory.h>

namespace gstd {

    template<typename ValueT>
    class Tree {

    };

    template<typename ValueT>
    class BinarySearchTreeNode : public BaseNode<ValueT> {
    public:

        using ValueType = ValueT;

    public:

        GSTD_CONSTEXPR GSTD_EXPLICIT BinarySearchTreeNode(ValueType value)
                : BaseNode<ValueType>(std::move(value)) {}

    public:

        GSTD_CONSTEXPR auto Left() -> BinarySearchTreeNode * {
            return _left;
        }

        GSTD_CONSTEXPR auto Right() -> BinarySearchTreeNode * {
            return _right;
        }

    private:

        BinarySearchTreeNode *_left;

        BinarySearchTreeNode *_right;
    };

    template<typename ValueT>
    class BinarySearchTree : public Tree<ValueT> {
    public:

    using ValueType = ValueT;

    using NodeType = BinarySearchTreeNode<ValueType>;

    public:

    GSTD_CONSTEXPR BinarySearchTree() = default;

    GSTD_CONSTEXPR BinarySearchTree(Allocator<NodeType> *allocator)
    : _allocator(allocator) {}

    public:

    GSTD_CONSTEXPR auto Insert(const ValueType &value,
                               const NodeType *node) -> NodeType * {
        if (!node) {
            node = _allocator(NodeType::New(value,
nullptr,
    nullptr));
        } else if (value < node->Value()) {
            node->Left() = Insert(value,
            node->Left());
        } else {
            node->Right() = Insert(value,
                                   node->Right());
        }

    return node;
    }

    private:

        RawPtr<NodeType> _root;

        RawPtr<Allocator<NodeType>> _allocator;
    };

    template<typename ValueT>
    class RedBlackTree : public Tree<ValueT> {

    };

}

#endif //GSTD_TREE_H
