#ifndef GSTD_ARRAY_H
#define GSTD_ARRAY_H

#include <gstd/Containers/Slice.h>
#include <gstd/Containers/Span.h>
#include <gstd/Type/Type.h>
#include <gstd/Utils/Match.h>

namespace gstd {

    template<typename ValueT,
             std::uint64_t SizeV>
    class Array {
    public:

        using ValueType = ValueT;

        using SizeType = std::uint64_t;

        static constexpr const SizeType SizeValue = SizeV;

        using Iterator = ValueType *;

        using ConstIterator = const ValueType *;

        using IndexType = std::uint64_t;

    public:

        GSTD_CONSTEXPR Array() = default;

        GSTD_CONSTEXPR Array(InitializerList<ValueType> initializerList) {
            if (initializerList.Size() != SizeValue) {
                Panic("Initializer list size and array size must be equals!");
            }

            for (IndexType index = 0; index < SizeValue; ++index) {
                _buffer[index] = initializerList[index];
            }
        }

        GSTD_CONSTEXPR Array(const Array<ValueType,
                                         SizeValue> &array) {
            for (IndexType index = 0; index < SizeValue; ++index) {
                _buffer[index] = array[index];
            }
        }

        GSTD_CONSTEXPR Array(Array<ValueType,
                                   SizeValue> &&array) GSTD_NOEXCEPT {
            for (IndexType index = 0; index < SizeValue; ++index) {
                _buffer[index] = std::move(array[index]);
            }
        }

    public:

        GSTD_CONSTEXPR auto At(const IndexType &index) -> Optional<Ref<ValueType>> {
            if (index < 0 and index >= Size()) {
                return MakeNone();
            }

            return MakeSome(MakeRef(_buffer[index]));
        }

        GSTD_CONSTEXPR auto At(const IndexType &index) const -> Optional<Ref<const ValueType>> {
            if (index < 0 and index >= Size()) {
                return MakeNone();
            }

            return MakeSome(MakeCRef(_buffer[index]));
        }

        GSTD_CONSTEXPR auto Size() GSTD_NOEXCEPT -> SizeType {
            return SizeValue;
        }

    public:

        GSTD_CONSTEXPR auto Iter() -> Iterator {
            return _buffer;
        }

        GSTD_CONSTEXPR auto Iter() const -> ConstIterator {
            return _buffer;
        }

        GSTD_CONSTEXPR auto begin() -> Iterator {
            return _buffer;
        }

        GSTD_CONSTEXPR auto end() -> Iterator {
            return _buffer + Size();
        }

        GSTD_CONSTEXPR auto begin() const -> ConstIterator {
            return _buffer;
        }

        GSTD_CONSTEXPR auto end() const -> ConstIterator {
            return _buffer + Size();
        }

        GSTD_CONSTEXPR auto cbegin() const -> ConstIterator {
            return _buffer;
        }

        GSTD_CONSTEXPR auto cend() const -> ConstIterator {
            return _buffer + Size();
        }

    public:

        // TODO: check
        GSTD_CONSTEXPR auto operator=(const Array &array) -> Array & {
            if (this == &array) {
                return *this;
            }

            _buffer = array._buffer;

            return *this;
        }

        // TODO: check
        GSTD_CONSTEXPR auto operator=(Array &&array) noexcept -> Array & {
            if (this == &array) {
                return *this;
            }

            _buffer = std::move(array._buffer);

            return *this;
        }

        GSTD_CONSTEXPR auto operator[](const IndexType &index) -> ValueType & {
            return At(index).template UnwrapOrElse([] () {
                Panic("Index out of range!");
            });
        }

        GSTD_CONSTEXPR auto operator[](const IndexType &index) const -> const ValueType & {
            return At(index).template UnwrapOrElse([] () {
                Panic("Index out of range!");
            });
        }

        GSTD_CONSTEXPR auto operator[](const Slice &slice) const -> Span<ValueType> {
            auto start = slice.GetStart();
            auto stop = slice.GetStop();
            auto step = slice.GetStep();

            if (step != 1) {
                Panic("Step in slice operator in `Array` must be 1!");
            }

            auto absoluteStart = gstd::Match<IndexType>(start)
                    .Case([] (const auto &value) -> bool {
                        return value == NoPos;
                    }, 0)
                    .Case([] (const auto &value) -> bool {
                        return value >= 0;
                    }, start)
                    .Default(Size() - start)
                    .Result()
                    .Unwrap();

            auto absoluteStop = gstd::Match<IndexType>(stop)
                    .Case([] (const auto &value) -> bool {
                        return value == NoPos;
                    }, Size())
                    .Case([] (const auto &value) -> bool {
                        return value >= 0;
                    }, stop)
                    .Default(Size() - stop)
                    .Result()
                    .Unwrap();

            if (absoluteStart > absoluteStop) {
                Panic("Invalid slice!");
            }

            return Span<ValueType> {
                    _buffer + absoluteStart,
                    absoluteStart - absoluteStop
            };
        }

    private:

        ValueType _buffer[SizeValue];
    };

}

#endif //GSTD_ARRAY_H
