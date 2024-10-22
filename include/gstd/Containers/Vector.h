#ifndef GSTD_VECTOR_H
#define GSTD_VECTOR_H

#include <gstd/Containers/Slice.h>
#include <gstd/Memory/Allocator.h>

namespace gstd {

    template<typename ValueT>
    class Vector {
    public:

        using ValueType = ValueT;

        using AllocatorType = Allocator;

        using SizeType = std::uint64_t;

        using Iterator = RangeIterator<ValueType>;

        using ConstIterator = /* @todo const? */ RangeIterator<const ValueType>;

        using IndexType = std::uint64_t;

    public:

        static_assert(!std::is_reference_v<ValueType>,
                      "`ValueType` can`t be reference type!");

        static_assert(std::is_unsigned_v<SizeType>,
                      "`SizeType` must be unsigned type!");

        static_assert(std::is_unsigned_v<IndexType>,
                      "`IndexType` must be unsigned type!");

    public:

        GSTD_CONSTEXPR Vector(RawPtr<Allocator> allocator = DefaultAllocator())
                : _buffer(nullptr),
                  _size(0),
                  _capacity(0),
                  _allocator(allocator) {}

        GSTD_CONSTEXPR Vector(RawPtr<ValueType> buffer,
                              const SizeType &size,
                              RawPtr<Allocator> allocator = DefaultAllocator())
                : _buffer(buffer),
                  _size(size),
                  _capacity(size),
                  _allocator(allocator) {}

        GSTD_CONSTEXPR Vector(InitializerList<ValueType> initializerList,
                              RawPtr<Allocator> allocator = DefaultAllocator());

        GSTD_CONSTEXPR Vector(const Vector &vector);

        GSTD_CONSTEXPR Vector(Vector &&vector) GSTD_NOEXCEPT;

    public:

        GSTD_CONSTEXPR ~Vector() GSTD_NOEXCEPT {
            if (_buffer.HasValue()) {
                for (IndexType index = 0; index < _size; ++index) {
                    _buffer[index].~ValueType();
                }

                _allocator->Deallocate(_buffer);
            }

            _buffer = nullptr;
            _size = 0;
            _capacity = 0;
        }

    public:

        static GSTD_CONSTEXPR auto New() -> Vector {
            return Vector {};
        }

        static GSTD_CONSTEXPR auto New(ValueType *buffer,
                                       const SizeType &size) -> Vector {
            return Vector {
                buffer,
                size
            };
        }

        static GSTD_CONSTEXPR auto New(const Vector &vector) -> Vector {
            return Vector {
                vector
            };
        }

        static GSTD_CONSTEXPR auto New(Vector &&vector) GSTD_NOEXCEPT -> Vector {
            return Vector {
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

        GSTD_CONSTEXPR auto At(const IndexType &index) -> Optional<Ref<ValueType>> {
            if (!InBounds(index)) {
                return MakeNone();
            }

            return MakeSome(MakeRef(_buffer[index]));
        }

        GSTD_CONSTEXPR auto At(const IndexType &index) const -> Optional<Ref<const ValueType>> {
            if (!InBounds(index)) {
                return MakeNone();
            }

            return MakeSome(MakeCRef(_buffer[index]));
        }

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

    private:

        GSTD_CONSTEXPR auto InBounds(const IndexType &index) const GSTD_NOEXCEPT -> bool {
            return index < Size();
        }

    public:

        GSTD_CONSTEXPR auto Iter() -> Iterator {
            return Iterator {
                _buffer,
                _buffer + _size
            };
        }

        GSTD_CONSTEXPR auto Iter() const -> ConstIterator {
            return ConstIterator {
                _buffer,
                _buffer + _size
            };
        }

        GSTD_CONSTEXPR auto begin() -> Iterator;

        GSTD_CONSTEXPR auto end() -> Iterator;

        GSTD_CONSTEXPR auto begin() const -> ConstIterator;

        GSTD_CONSTEXPR auto end() const -> ConstIterator;

        GSTD_CONSTEXPR auto cbegin() const -> ConstIterator;

        GSTD_CONSTEXPR auto cend() const -> ConstIterator;

    public:

        GSTD_CONSTEXPR auto operator=(const Vector &vector) -> Vector & {
            if (&vector == this) {
                return *this;
            }

            // ...

            return *this;
        }

        GSTD_CONSTEXPR auto operator=(Vector &&vector) GSTD_NOEXCEPT -> Vector & {
            if (&vector == this) {
                return *this;
            }

            // ...

            return *this;
        }

        GSTD_CONSTEXPR auto operator==(const Vector &vector) const -> bool;

        GSTD_CONSTEXPR auto operator[](const IndexType &index) -> ValueType &;

        GSTD_CONSTEXPR auto operator[](const IndexType &index) const -> const ValueType &;

        GSTD_CONSTEXPR auto operator[](const Slice &slice) const -> Vector;

    private:

        RawPtr<ValueType> _buffer;

        SizeType _size;

        SizeType _capacity;

        RawPtr<Allocator> _allocator;
    };

    class StableVector {

    };

    class StaticVector {

    };

    class SmallVector {

    };

    class Devector {

    };

}

#endif //GSTD_VECTOR_H
