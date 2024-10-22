#ifndef GSTD_SPAN_H
#define GSTD_SPAN_H

#include <gstd/Containers/Slice.h>
#include <gstd/Type/Iterator.h>

namespace gstd {

    template<typename ValueT>
    class Span {
    public:

        using ValueType = ValueT;

        using SizeType = std::uint64_t;

        using Pointer = ValueType *;

        using ConstPointer = const ValueType *;

        using Reference = ValueType &;

        using ConstReference = const ValueType &;

        using Iterator = RangeIterator<ValueType>;

        using ConstIterator = /* @todo const? */ RangeIterator<const ValueType>;

        using IndexType = std::uint64_t;

    public:

        static_assert(std::is_unsigned_v<SizeType>,
                      "`SizeType` must be unsigned type!");

        static_assert(std::is_unsigned_v<IndexType>,
                      "`IndexType` must be unsigned type!");

    public:

        GSTD_CONSTEXPR Span() GSTD_NOEXCEPT = default;

        GSTD_CONSTEXPR Span(Pointer pointer,
                            SizeType size) GSTD_NOEXCEPT
                : _pointer(pointer),
                  _size(size) {}

        template<typename InputValueT,
                 typename = std::enable_if_t<std::is_convertible_v<InputValueT,
                                                                   ValueType>>>
        GSTD_CONSTEXPR Span(Span<InputValueT> span)
        GSTD_NOEXCEPT(std::is_nothrow_convertible_v<InputValueT,
                                                    ValueType>)
                : _pointer(static_cast<Pointer>(span._pointer)),
                  _size(span._size) {}

        template<SizeType SizeV>
        GSTD_CONSTEXPR Span(ValueType (&array)[SizeV]) GSTD_NOEXCEPT
                : _pointer(static_cast<Pointer>(array)),
                  _size(SizeV) {}

        GSTD_CONSTEXPR Span(const Span &span) GSTD_NOEXCEPT
                : _pointer(span._pointer),
                  _size(span._size) {}

        GSTD_CONSTEXPR Span(Span &&span) GSTD_NOEXCEPT = default;

    public:

        GSTD_CONSTEXPR auto Data() const GSTD_NOEXCEPT -> Pointer {
            return _pointer;
        }

        GSTD_CONSTEXPR auto Size() const GSTD_NOEXCEPT -> SizeType {
            return _size;
        }

        GSTD_CONSTEXPR auto SizeInBytes() const GSTD_NOEXCEPT -> SizeType {
            return _size * sizeof(ValueType);
        }

        GSTD_CONSTEXPR auto Empty() const GSTD_NOEXCEPT -> bool {
            return _size == 0;
        }

        GSTD_CONSTEXPR auto At(const SizeType &index) const GSTD_NOEXCEPT -> Optional<Ref<ValueType>> {
            if (index >= _size) {
                return MakeNone();
            }

            return MakeSome(MakeRef(_pointer[index]));
        }

        GSTD_CONSTEXPR auto Front() const GSTD_NOEXCEPT -> Optional<Pointer> {
            if (Empty()) {
                return MakeNone();
            }

            return MakeSome(_pointer);
        }

        GSTD_CONSTEXPR auto Back() const GSTD_NOEXCEPT -> Optional<Pointer> {
            if (Empty()) {
                return None();
            }

            return MakeSome(_pointer + (_size - 1));
        }

        GSTD_CONSTEXPR auto Iter() GSTD_NOEXCEPT -> Iterator {
            return Iterator {
                _pointer,
                _pointer + _size
            };
        }

        GSTD_CONSTEXPR auto Iter() const GSTD_NOEXCEPT -> ConstIterator {
            return ConstIterator {
                _pointer,
                _pointer + _size
            };
        }

        GSTD_CONSTEXPR auto AsConst() const GSTD_NOEXCEPT -> Span<const ValueType> {
            return Span<const ValueType> {
                _pointer,
                _size
            };
        }

        GSTD_CONSTEXPR auto AsVolatile() const GSTD_NOEXCEPT -> Span<volatile ValueType> {
            return Span<volatile ValueType> {
                _pointer,
                _size
            };
        }

        template<typename ToValueT>
        GSTD_CONSTEXPR auto Transmute() const -> Span<ToValueT> {
            return Span<ToValueT> {
                reinterpret_cast<typename Span<ToValueT>::Pointer>(_pointer),
                SizeInBytes() / sizeof(ToValueT)
            };
        }

    public:

        GSTD_CONSTEXPR auto begin() GSTD_NOEXCEPT -> Iterator {
            return Iterator {
                _pointer,
                _pointer + _size
            };
        }

        GSTD_CONSTEXPR auto end() GSTD_NOEXCEPT -> Iterator {
            return Iterator {
                _pointer,
                _pointer + _size
            };
        }

        GSTD_CONSTEXPR auto begin() const GSTD_NOEXCEPT -> ConstIterator {
            return ConstIterator {
                _pointer,
                _pointer + _size
            };
        }

        GSTD_CONSTEXPR auto end() const GSTD_NOEXCEPT -> ConstIterator {
            return ConstIterator {
                _pointer,
                _pointer + _size
            };
        }

    public:

        GSTD_CONSTEXPR auto operator=(const Span &span) GSTD_NOEXCEPT -> Span & {
            if (this == &span) {
                return *this;
            }

            _pointer = span._pointer;
            _size = span._size;

            return *this;
        }

        GSTD_CONSTEXPR auto operator=(Span &&span) GSTD_NOEXCEPT -> Span & {
            _pointer = std::move(span._pointer);
            _size = span._size;

            return *this;
        }

        GSTD_CONSTEXPR auto operator[](const IndexType &index) const -> Reference {
            return At(index).UnwrapOrElse([] () {
                Panic("Index out of range!");
            });
        }

        // TODO: CHECK!
        GSTD_CONSTEXPR auto operator[](const Slice &slice) const -> Span {
            auto start = slice.GetStart();
            auto stop = slice.GetStop();
            auto step = slice.GetStep();

            if (step != 1) {
                Panic("Step in slice operator in `Span` must be 1!");
            }

            IndexType absoluteStart = 0;
            IndexType absoluteStop = 0;

            if (start == NoPos) {
                absoluteStart = 0;
            } else if (start >= 0) {
                absoluteStart = start;
            } else {
                absoluteStart = (_size - start);
            }

            if (stop == NoPos) {
                absoluteStop = _size;
            } else if (stop >= 0) {
                absoluteStop = stop;
            } else {
                absoluteStop = (_size - stop);
            }

            if (absoluteStart > absoluteStop) {
                Panic("Invalid slice!");
            }

            return Span {
                _pointer + absoluteStart,
                absoluteStart - absoluteStop
            };
        }

    private:

        Pointer _pointer;

        SizeType _size;
    };

    template<typename ValueT>
    Span(ValueT *pointer,
         std::uint64_t size) -> Span<ValueT>;

    template<typename ValueT,
             std::uint64_t SizeV>
    Span(ValueT (&array)[SizeV]) -> Span<ValueT>;

    template<typename ValueT>
    GSTD_CONSTEXPR auto MakeSpan(ValueT *pointer,
                                 std::uint64_t size) -> Span<ValueT> {
        return Span<ValueT> {
            pointer,
            size
        };
    }

}

#endif //GSTD_SPAN_H
