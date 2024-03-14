#ifndef GSTD_VECTOR_H
#define GSTD_VECTOR_H

#include <gstd/Containers/Slice.h>
#include <gstd/Type/Types.h>

#include <sstream>

namespace gstd {

    template<typename ValueT,
             typename AllocatorT = Allocator<ValueT>>
    class Vector {
    public:

        using ValueType = ValueT;

        using AllocatorType = AllocatorT;

        using SizeType = std::uint64_t;

        using Iterator = ValueType *;

        using ConstIterator = const ValueType *;

        /*
         * TODO
         */
        using ReverseIterator = int;

        /*
         * TODO
         */
        using ConstReverseIterator = int;

        using IndexType = std::uint64_t;

    public:

        static_assert(!std::is_reference_v<ValueType>,
                      "`ValueType` can`t be reference type!");

        static_assert(std::is_unsigned_v<SizeType>,
                      "`SizeType` must be unsigned type!");

        static_assert(std::is_unsigned_v<IndexType>,
                      "`IndexType` must be unsigned type!");

    public:

        GSTD_CONSTEXPR Vector()
                : _buffer(nullptr),
                  _size(0),
                  _capacity(0) {}

        GSTD_CONSTEXPR Vector(ValueType *buffer,
                              const SizeType &size)
                : _buffer(buffer),
                  _size(size),
                  _capacity(size) {}

        GSTD_CONSTEXPR Vector(InitializerList<ValueType> initializerList);

        GSTD_CONSTEXPR Vector(const Vector<ValueType> &vector);

        GSTD_CONSTEXPR Vector(Vector<ValueType> &&vector) GSTD_NOEXCEPT;

    public:

        GSTD_CONSTEXPR ~Vector() GSTD_NOEXCEPT {
            if (_buffer != nullptr) {
                for (IndexType index = 0; index < _size; ++index) {
                    _buffer[index].~ValueType();
                }

                delete[] _buffer;
            }

            delete[] _buffer;

            _buffer = nullptr;
            _size = 0;
            _capacity = 0;
        }

    public:

        static GSTD_CONSTEXPR auto New() -> Vector<ValueType> {
            return Vector<ValueType> {};
        }

        static GSTD_CONSTEXPR auto New(ValueType *buffer,
                                       const SizeType &size) -> Vector<ValueType> {
            return Vector<ValueType> {
                buffer,
                size
            };
        }

        static GSTD_CONSTEXPR auto New(const Vector<ValueType> &vector) -> Vector<ValueType> {
            return Vector<ValueType> {
                vector
            };
        }

        static GSTD_CONSTEXPR auto New(Vector<ValueType> &&vector) GSTD_NOEXCEPT -> Vector<ValueType> {
            return Vector<ValueType> {
                std::move(vector)
            };
        }

    public:

        GSTD_CONSTEXPR auto Append(const ValueType &value) -> void {
            if (_size >= _capacity) {
                // ...
            }

            new (_buffer + _size) ValueType(value);
            ++_size;
        }

        GSTD_CONSTEXPR auto Append(ValueType &&value) -> void {
            if (_size >= _capacity) {
                // ...
            }

            new (_buffer + _size) ValueType(std::move(value));
            ++_size;
        }

        template<typename... ArgumentsT>
        GSTD_CONSTEXPR auto Append(ArgumentsT &&...arguments) -> void {
            if (_size >= _capacity) {
                // ...
            }

            new (_buffer + _size) ValueType(std::forward<ArgumentsT>(arguments)...);
            ++_size;
        }

        GSTD_CONSTEXPR auto At(const IndexType &index) -> Optional<Ref<ValueType>>;

        GSTD_CONSTEXPR auto At(const IndexType &index) const -> Optional<Ref<ValueType>>;

        GSTD_CONSTEXPR auto Insert(const IndexType &index,
                                   const ValueType &value) -> void;

        GSTD_CONSTEXPR auto Insert(const IndexType &index,
                                   ValueType &&value) -> void;

        template<typename... ArgumentsT>
        GSTD_CONSTEXPR auto Insert(const IndexType &index,
                                   ArgumentsT &&...value) -> void;

        GSTD_CONSTEXPR auto Remove(const IndexType &index) -> void {
            if (_size <= 0) {
                // ...
            }

            if (index >= _size) {
                // ...
            }

            // ?
        }

        GSTD_CONSTEXPR auto Size() const -> SizeType {
            return _size;
        }

        GSTD_CONSTEXPR auto Capacity() const -> SizeType {
            return _capacity;
        }

        GSTD_CONSTEXPR auto Reserve(const SizeType &size) -> void;

        GSTD_CONSTEXPR auto Fit() -> void;

    public:

        GSTD_CONSTEXPR auto Iter() -> Iterator;

        GSTD_CONSTEXPR auto Iter() const -> ConstIterator;

        GSTD_CONSTEXPR auto begin() -> Iterator;

        GSTD_CONSTEXPR auto end() -> Iterator;

        GSTD_CONSTEXPR auto begin() const -> ConstIterator;

        GSTD_CONSTEXPR auto end() const -> ConstIterator;

        GSTD_CONSTEXPR auto cbegin() const -> ConstIterator;

        GSTD_CONSTEXPR auto cend() const -> ConstIterator;

    public:

        GSTD_CONSTEXPR auto operator=(const Vector<ValueType> &vector) -> Vector<ValueType> & {
            if (&vector == this) {
                return *this;
            }

            // ...

            return *this;
        }

        GSTD_CONSTEXPR auto operator=(Vector<ValueType> &&vector) GSTD_NOEXCEPT -> Vector<ValueType> & {
            if (&vector == this) {
                return *this;
            }

            // ...

            return *this;
        }

        GSTD_CONSTEXPR auto operator==(const Vector<ValueType> &vector) const -> bool;

        GSTD_CONSTEXPR auto operator[](const IndexType &index) -> ValueType &;

        GSTD_CONSTEXPR auto operator[](const IndexType &index) const -> const ValueType &;

        GSTD_CONSTEXPR auto operator[](const Slice &slice) const -> Vector<ValueType>;

    private:

        ValueType *_buffer;

        SizeType _size;

        SizeType _capacity;
    };

    struct A {
        A(int, int);

        int a;
        int b;
    };

    void h() {
        auto v = Vector<A>::New();
        auto u = v;
        u.Append(1, 10);
        v.Append(10, 20);
        std::vector<int> a;
        a.emplace_back(1);
    }

}

#endif //GSTD_VECTOR_H
