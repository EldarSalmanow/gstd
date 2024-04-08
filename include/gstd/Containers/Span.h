#ifndef GSTD_SPAN_H
#define GSTD_SPAN_H

#include <gstd/Containers/Slice.h>
#include <gstd/Type/Optional.h>

#include <array>

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

        using Iterator = ValueType *;

        using ConstIterator = const ValueType *;

        using IndexType = std::uint64_t;

    public:

        GSTD_CONSTEXPR Span() GSTD_NOEXCEPT = default;

        GSTD_CONSTEXPR Span(Iterator iterator,
                            SizeType size) GSTD_NOEXCEPT
                : _iterator(iterator),
                  _size(size) {}

        template<typename InputValueT,
                 typename = std::enable_if_t<std::is_convertible_v<InputValueT,
                                                                   ValueType>>>
        GSTD_CONSTEXPR Span(Span<InputValueT> span)
        GSTD_NOEXCEPT(std::is_nothrow_convertible_v<InputValueT,
                                                    ValueType>)
                : _iterator(static_cast<Iterator>(span._iterator)),
                  _size(span._size) {}

        template<SizeType SizeV>
        constexpr Span(ValueType (&array)[SizeV])
                : _iterator(static_cast<Iterator>(array)),
                  _size(SizeV) {}

        template<SizeType SizeV>
        GSTD_CONSTEXPR Span(std::array<ValueType,
                                       SizeV> &array) GSTD_NOEXCEPT
                : _iterator(static_cast<Iterator>(array.data())),
                  _size(SizeV) {}

        template<SizeType SizeV>
        GSTD_CONSTEXPR Span(const std::array<ValueType,
                                             SizeV> &array) GSTD_NOEXCEPT
                : _iterator(static_cast<Iterator>(array.data())),
                  _size(SizeV) {}

        template<typename InputValueT,
                 SizeType SizeV>
        GSTD_CONSTEXPR Span(std::array<InputValueT,
                                       SizeV> &&array) = delete;

        /**
         *
         * @tparam ContainerT
         * @param container
         * @todo Add check container type
         */
        template<typename ContainerT>
        GSTD_CONSTEXPR Span(ContainerT &container)
                : _iterator(static_cast<Iterator>(std::data(container))),
                  _size(std::size(container)) {}

        GSTD_CONSTEXPR Span(const Span<ValueType> &span) GSTD_NOEXCEPT = default;

        GSTD_CONSTEXPR Span(Span<ValueType> &&span) GSTD_NOEXCEPT = default;

    public:

        GSTD_CONSTEXPR auto Data() const GSTD_NOEXCEPT -> Pointer {
            return _iterator;
        }

        GSTD_CONSTEXPR auto Size() const GSTD_NOEXCEPT -> SizeType {
            return _size;
        }

        GSTD_CONSTEXPR auto SizeBytes() const GSTD_NOEXCEPT -> SizeType {
            return _size * sizeof(ValueType);
        }

        GSTD_CONSTEXPR auto Empty() const GSTD_NOEXCEPT -> bool {
            return _size == 0;
        }

        GSTD_CONSTEXPR auto At(const SizeType &index) const -> Optional<Ref<ValueType>> {
            if ((index < 0) || (index >= _size)) {
                return MakeNone();
            }

            return MakeSome(MakeRef(_iterator[index]));
        }

        GSTD_CONSTEXPR auto Front() const GSTD_NOEXCEPT -> Optional<ValueType *> {
            if (Empty()) {
                return MakeNone();
            }

            return MakeSome(_iterator);
        }

        GSTD_CONSTEXPR auto Back() const GSTD_NOEXCEPT -> Optional<ValueType *> {
            if (Empty()) {
                return None();
            }

            return Some(_iterator + (_size - 1));
        }

    public:

        GSTD_CONSTEXPR auto begin() GSTD_NOEXCEPT -> Iterator {
            return _iterator;
        }

        GSTD_CONSTEXPR auto end() GSTD_NOEXCEPT -> Iterator {
            return _iterator;
        }

        GSTD_CONSTEXPR auto begin() const GSTD_NOEXCEPT -> ConstIterator {
            return _iterator;
        }

        GSTD_CONSTEXPR auto end() const GSTD_NOEXCEPT -> ConstIterator {
            return _iterator;
        }

        GSTD_CONSTEXPR auto cbegin() const GSTD_NOEXCEPT -> ConstIterator {
            return _iterator;
        }

        GSTD_CONSTEXPR auto cend() const GSTD_NOEXCEPT -> ConstIterator {
            return _iterator;
        }

    public:

        GSTD_CONSTEXPR auto operator=(const Span<ValueType> &span) /*GSTD_NOEXCEPT*/ -> Span<ValueType> & = default;

        GSTD_CONSTEXPR auto operator=(Span<ValueType> &&span) GSTD_NOEXCEPT -> Span<ValueType> & = default;

        GSTD_CONSTEXPR auto operator[](const IndexType &index) const -> Reference {
            return At(index).template UnwrapOrElse([] () {
                Panic("Index out of range!");
            });
        }

        // TODO: CHECK!
        GSTD_CONSTEXPR auto operator[](const Slice &slice) const -> Span<ValueType> {
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

            return Span<ValueType> {
                _iterator + absoluteStart,
                absoluteStart - absoluteStop
            };
        }

    private:

        Iterator _iterator;

        SizeType _size;
    };

    template<typename ValueT>
    GSTD_CONSTEXPR auto MakeSpan(ValueT *iterator,
                                 std::uint64_t size) -> Span<ValueT> {
        return Span<ValueT> {
            iterator,
            size
        };
    }

}

#endif //GSTD_SPAN_H
