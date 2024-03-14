#ifndef GSTD_STRING_H
#define GSTD_STRING_H

#include "Vector.h"

#include <fstream>

namespace gstd {

    template<typename ValueT>
    class Iterator {
    public:

        using ValueType = ValueT;

    public:

        GSTD_CONSTEXPR GSTD_EXPLICIT Iterator(ValueT *pointer)
                : _pointer(pointer) {}

    public:

        static GSTD_CONSTEXPR auto New(ValueT *pointer) -> Iterator<ValueType> {
            return Iterator<ValueType> {
                pointer
            };
        }

    public:

        GSTD_CONSTEXPR auto Current() & -> ValueType & {
            return *_pointer;
        }

        GSTD_CONSTEXPR auto Current() const & -> const ValueType & {
            return *_pointer;
        }

        GSTD_CONSTEXPR auto HasNext() -> bool {
            return true;
        }

        GSTD_CONSTEXPR auto Next(/* size_t count = 1 */) -> void {
            if (!HasNext()) {
                throw std::runtime_error("Error!");
            }

            ++_pointer;
        }

        GSTD_CONSTEXPR auto HasPrev() -> bool {
            return true;
        }

        GSTD_CONSTEXPR auto Prev() -> void {
            if (!HasPrev()) {
                throw std::runtime_error("Error!");
            }

            --_pointer;
        }

    public:

        GSTD_CONSTEXPR auto operator==(const Iterator<ValueType> &iterator) const -> bool {
            return _pointer == iterator._pointer;
        }

        GSTD_CONSTEXPR auto operator++() -> Iterator<ValueType> {
            Next();

            return *this;
        }

        GSTD_CONSTEXPR auto operator--() -> Iterator<ValueType> {
            Prev();

            return *this;
        }

        GSTD_CONSTEXPR auto operator*() & -> ValueType & {
            return Current();
        }

        GSTD_CONSTEXPR auto operator*() const & -> const ValueType & {
            return Current();
        }

    private:

        ValueType *_pointer;
    };

    template<typename ValueT>
    GSTD_CONSTEXPR auto MakeIterator(ValueT *pointer) -> Iterator<ValueT> {
        return Iterator<ValueT>::New(pointer);
    }

    template<typename ClassT>
    struct IterableTraits {};

    template<typename ClassT,
             typename TraitsT = IterableTraits<ClassT>>
    class Iterable {
    private:

        using ClassType = ClassT;

        using TraitsType = TraitsT;

    public:

        using IteratorType = typename TraitsType::IteratorType;

        using ConstIteratorType = typename TraitsType::ConstIteratorType;

    public:

        GSTD_CONSTEXPR auto Iter() -> IteratorType {
            return ClassInstance()->Iter();
        }

        GSTD_CONSTEXPR auto Iter() const -> ConstIteratorType {
            return ClassInstance()->Iter();
        }

        GSTD_CONSTEXPR auto begin() -> IteratorType {
            return ClassInstance()->begin();
        }

        GSTD_CONSTEXPR auto end() -> IteratorType {
            return ClassInstance()->end();
        }


        GSTD_CONSTEXPR auto begin() const -> ConstIteratorType {
            return ClassInstance()->begin();
        }

        GSTD_CONSTEXPR auto end() const -> ConstIteratorType {
            return ClassInstance()->end();
        }

        GSTD_CONSTEXPR auto cbegin() const -> ConstIteratorType {
            return ClassInstance()->begin();
        }

        GSTD_CONSTEXPR auto cend() const -> ConstIteratorType {
            return ClassInstance()->end();
        }

    private:

        GSTD_CONSTEXPR auto ClassInstance() -> ClassType * {
            return static_cast<ClassType *>(this);
        }

        GSTD_CONSTEXPR auto ClassInstance() const -> const ClassType * {
            return static_cast<const ClassType *>(this);
        }
    };

#define GSTD_GEN_ITERABLE_METHODS(begin_iterator, end_iterator)     \
                                                                    \
    GSTD_CONSTEXPR auto Iter() {                                    \
        return begin_iterator;                                      \
    }                                                               \
                                                                    \
    GSTD_CONSTEXPR auto Iter() const {                              \
        return begin_iterator;                                      \
    }                                                               \
                                                                    \
    GSTD_CONSTEXPR auto begin() {                                   \
        return begin_iterator;                                      \
    }                                                               \
                                                                    \
    GSTD_CONSTEXPR auto end() {                                     \
        return end_iterator;                                        \
    }                                                               \
                                                                    \
    GSTD_CONSTEXPR auto begin() const {                             \
        return begin_iterator;                                      \
    }                                                               \
                                                                    \
    GSTD_CONSTEXPR auto end() const {                               \
        return end_iterator;                                        \
    }                                                               \
                                                                    \
    GSTD_CONSTEXPR auto cbegin() const {                            \
        return begin_iterator;                                      \
    }                                                               \
                                                                    \
    GSTD_CONSTEXPR auto cend() const {                              \
        return end_iterator;                                        \
    }

    template<typename ClassT>
    struct IndexOpTraits {};

    template<typename ClassT,
             typename TraitsT = IndexOpTraits<ClassT>>
    class IndexOp {
    private:

        using ClassType = ClassT;

        using TraitsType = TraitsT;

    public:

        using IndexType = typename TraitsType::IndexType;

        using ElementType = typename TraitsType::ElementType;

        using ConstElementType = typename TraitsType::ConstElementType;

    public:

        GSTD_CONSTEXPR auto operator[](const IndexType &index) -> ElementType {
            return ClassInstance()->operator[](index);
        }

        GSTD_CONSTEXPR auto operator[](const IndexType &index) const -> ConstElementType {
            return ClassInstance()->operator[](index);
        }

    private:

        GSTD_CONSTEXPR auto ClassInstance() -> ClassType * {
            return static_cast<ClassType *>(this);
        }

        GSTD_CONSTEXPR auto ClassInstance() const -> const ClassType * {
            return static_cast<const ClassType *>(this);
        }
    };

#define GSTD_GEN_INDEX_OP_METHODS(element)                                             \
    GSTD_CONSTEXPR auto operator[](const IndexType &index) -> ElementType {            \
        return element;                                                                \
    }                                                                                  \
                                                                                       \
    GSTD_CONSTEXPR auto operator[](const IndexType &index) const -> ConstElementType { \
        return element;                                                                \
    }

    template<typename ValueT>
    class Vec;

    template<typename ValueT>
    struct IterableTraits<Vec<ValueT>> {

        using IteratorType = Iterator<ValueT>;

        using ConstIteratorType = const Iterator<ValueT>;

    };

    template<typename ValueT>
    struct IndexOpTraits<Vec<ValueT>> {

        using IndexType = std::uint64_t;

        using ElementType = ValueT &;

        using ConstElementType = const ValueT &;
    };

    template<typename ValueT>
    class Vec : public Iterable<Vec<ValueT>>, public IndexOp<Vec<ValueT>> {
    private:

        using SelfType = Vec<ValueT>;

    public:

        using ValueType = ValueT;

        using IteratorType = typename Iterable<SelfType>::IteratorType;

        using ConstIteratorType = typename Iterable<SelfType>::ConstIteratorType;

        using IndexType = typename IndexOp<SelfType>::IndexType;

        using ElementType = typename IndexOp<SelfType>::ElementType;

        using ConstElementType = typename IndexOp<SelfType>::ConstElementType;

    public:

        constexpr Vec(ValueT *ptr)
                : _buffer(ptr),
                  _size(0) {}

    public:

        GSTD_GEN_ITERABLE_METHODS(MakeIterator(_buffer),
                                  MakeIterator(_buffer + _size))

    public:

        GSTD_GEN_INDEX_OP_METHODS(_buffer[index])

    private:

        ValueT *_buffer;

        size_t _size;
    };

    template<typename ClassT>
    void Print(const Iterable<ClassT> &iterable) {
        for (auto &element : iterable) {
            std::cout << element;
        }
    }

    void s() {
        static constexpr int ps[10] = {10, 20, 1, 2, 3, 5};
        constexpr Vec<int> v((int *) ps);
        static_assert(v[0] == 10);

        Print(v);
    }

    using CodePoint = std::uint32_t;

    class Symbol {
    public:

        Symbol(CodePoint codePoint);

    public:

        static Symbol New(CodePoint codePoint);

    private:

        CodePoint _codePoint;
    };

    constexpr auto Size(const char *string) -> size_t {
        size_t size = 0;

        for (size_t index = 0; string[index] != '\0'; ++index, ++size) {}

        return size;
    }

    class String;
    class StringIterator;

    template<>
    struct IterableTraits<String> {

        using IteratorType = StringIterator;

        using ConstIteratorType = const StringIterator;

    };

    class String : public Iterable<String> {
    public:

        using ValueType = std::uint8_t;

        using Iterator = typename Iterable<String>::IteratorType;

        using ConstIterator = typename Iterable<String>::ConstIteratorType;

        using value_type = ValueType;

        using pointer = ValueType *;

        using const_pointer = const ValueType *;

        using reference = ValueType &;

        using const_reference = const ValueType &;

        using iterator = Iterator;

        using const_iterator = ConstIterator;

    public:

        GSTD_CONSTEXPR GSTD_EXPLICIT String(const char *string)
                : _buffer(const_cast<char *>(string)),
                  _size(Size(_buffer)) {}

    public:

        static GSTD_CONSTEXPR auto New(const char *string) -> String {
            return String {
                string
            };
        }

        static GSTD_CONSTEXPR auto New(const std::string &string) -> String {
            return String::New(string.data());
        }

        static GSTD_CONSTEXPR auto New(const char8_t *string) -> String;

//        static GSTD_CONSTEXPR auto New(const std::u8string &string) -> String {
//            return String::New(string.data());
//        } TODO

        static GSTD_CONSTEXPR auto New(const char16_t *string) -> String;

        static GSTD_CONSTEXPR auto New(const std::u16string &string) -> String {
            return String::New(string.data());
        }

        static GSTD_CONSTEXPR auto New(const char32_t *string) -> String;

        static GSTD_CONSTEXPR auto New(const std::u32string &string) -> String {
            return String::New(string.data());
        }

        static GSTD_CONSTEXPR auto New(const wchar_t *string) -> String;

        static GSTD_CONSTEXPR auto New(const std::wstring &string) -> String {
            return String::New(string.data());
        }

    public:

        Optional<Ref<char>> At(const size_t &index) {
            if (index < 0 || index >= _size) {
                return MakeNone();
            }

            return MakeSome(Ref<char>(_buffer[index]));
        }

    public:

        GSTD_GEN_ITERABLE_METHODS(_buffer, _buffer + _size)

    public:

        GSTD_CONSTEXPR auto operator[](const size_t &index) -> char {
            return _buffer[index];
        }

        GSTD_CONSTEXPR auto operator[](const size_t &index) const -> const char {
            return _buffer[index];
        }

    private:

        char *_buffer;

        size_t _size;
    };

    class StringIterator {
    public:

        StringIterator(char *pointer);

    public:

        bool has_next() const;

        void next();

        Symbol current() const;
    };

    class StringRef {
    public:


    };

    class SmallString {
    public:


    };

    class StaticString {
    public:


    };

    inline std::istream &operator>>(std::istream &stream,
                                    const String &string);

    inline std::ostream &operator<<(std::ostream &stream,
                                    const String &string);

}

#endif //GSTD_STRING_H
