#ifndef GSTD_SPAN_H
#define GSTD_SPAN_H

//#include <gstd/Containers/Optional.h>
//#include <gstd/Containers/Ref.h>

#include <array>

namespace gstd {

    template<typename ValueT>
    class Span {
    public:

        using ValueType = ValueT;

        using SizeType = std::uint64_t;

        using IndexType = std::uint64_t;

        using Pointer = ValueType *;

        using Reference = ValueType &;

        using Iterator = ValueType *;

        using ConstIterator = const ValueType *;

    public:

        constexpr Span()
                : _iterator(nullptr),
                  _size(0) {}

        constexpr Span(Iterator iterator,
                       SizeType size)
                : _iterator(iterator),
                  _size(size) {}

        template<typename InputValueT,
                 typename = EnableIfT<IsConvertibleV<InputValueT,
                                                     ValueType>>>
        constexpr Span(Span<InputValueT> span)
                : _iterator(static_cast<Iterator>(span._iterator)),
                  _size(span._size) {}

        template<SizeType SizeV>
        constexpr Span(ValueType (&array)[SizeV])
                : _iterator(static_cast<Iterator>(array)),
                  _size(SizeV) {}

        template<typename InputValueT,
                 SizeType SizeV,
                 typename = EnableIfT<IsConvertibleV<InputValueT,
                                                     ValueType>>>
        constexpr Span(const std::array<InputValueT, SizeV> &array)
                : _iterator(static_cast<Iterator>(array.data())),
                  _size(SizeV) {}

        template<typename InputValueT,
                 SizeType SizeV>
        constexpr Span(std::array<InputValueT, SizeV> &&array) = delete;

        template<typename ContainerT>
        constexpr Span(ContainerT &container) // TODO
                : _iterator(static_cast<Iterator>(std::data(container))),
                  _size(std::size(container)) {}

        constexpr Span(const Span<ValueType> &span)
                : _iterator(span._iterator),
                  _size(span._size) {}

        constexpr Span(Span<ValueType> &&span) noexcept
                : _iterator(std::move(span._iterator)),
                  _size(std::move(span._size)) {}

    public:

        constexpr auto Data() const -> Pointer {
            return _iterator;
        }

        constexpr auto Size() const -> SizeType {
            return _size;
        }

        constexpr auto SizeBytes() const -> SizeType {
            return _size * sizeof(ValueType);
        }

        constexpr auto IsEmpty() const -> bool {
            return _size == 0;
        }

        constexpr auto At(const SizeType &index) const -> Optional<Ref<ValueType>> {
            if (!InBounds(index)) {
                return None();
            }

            return Some<Ref<ValueType>>(_iterator[index]);
        }

//        constexpr auto Slice(const IndexType &startOffset,
//                             const IndexType &endOffset) const -> Span<ValueType> {
//
//        }

        constexpr auto Slice(const IndexType &offset) const -> Span<ValueType> {
            if (!InBounds(offset)) {
                //
            }

            return Span<ValueType>(_iterator + offset,
                                   _size - offset);
        }

        constexpr auto Slice(const IndexType &offset,
                             const SizeType &sizeToSlice) const -> Span<ValueType> {
            if (!InBounds(offset) ||
                !InBounds(offset + (sizeToSlice - 1))) {
                //
            }

            return Span<ValueType>(_iterator + offset,
                                   sizeToSlice);
        }

        constexpr auto First() const -> Optional<ValueType *> {
            if (IsEmpty()) {
                return None();
            }

            return Some(_iterator);
        }

        constexpr auto Last() const -> Optional<ValueType *> {
            if (IsEmpty()) {
                return None();
            }

            return Some(_iterator + (_size - 1));
        }

        template<typename InputValueT>
        constexpr auto Equals(Span<InputValueT> span) const -> bool {
            if (_size != span.Size()) {
                return false;
            }

            for (SizeType index = 0; index < span.Size(); ++index) {
                if (_iterator[index] != span[index]) {
                    return false;
                }
            }

            return true;
        }

        template <typename PredT,
                  typename = EnableIfT<IsSameV<std::invoke_result_t<PredT &&,
                                                                    ValueType &&>,
                                                bool>>>
        constexpr auto IsAny(PredT &&predicate) const -> bool {
            for (auto &element : *this) {
                auto condition = std::forward<PredT>(predicate)(element);

                if (condition) {
                    return true;
                }
            }

            return false;
        }

        template <typename PredT,
                  typename = EnableIfT<IsSameV<std::invoke_result_t<PredT &&,
                                                                    ValueType &&>,
                                               bool>>>
        constexpr auto IsAll(PredT &&predicate) const -> bool {
            for (auto &element : *this) {
                auto condition = std::forward<PredT>(predicate)(element);

                if (!condition) {
                    return false;
                }
            }

            return !IsEmpty();
        }

        template <typename PredT,
                  typename = EnableIfT<IsSameV<std::invoke_result_t<PredT &&,
                                                                    ValueType &&>,
                                               bool>>>
        constexpr auto IsNone(PredT &&predicate) const -> bool {
            for (auto &element : *this) {
                auto condition = std::forward<PredT>(predicate)(element);

                if (condition) {
                    return false;
                }
            }

            return true;
        }

        template<typename InputValueT>
        constexpr auto AllEquals(const InputValueT &inputValue) const -> bool {
            return IsAll([&inputValue] (const ValueType &value) {
                return value == inputValue;
            });
        }

        template<typename InputValueT>
        constexpr auto AnyEquals(const InputValueT &inputValue) const -> bool {
            return IsAny([&inputValue] (const InputValueT &value) {
                return value == inputValue;
            });
        }

        template<typename InputValueT>
        constexpr auto NoneEquals(const InputValueT &inputValue) const -> bool {
            return IsNone([&inputValue] (const ValueType &value) {
                return value == inputValue;
            });
        }

    private:

        template<typename InputValueT>
        constexpr auto Copy(const Span<InputValueT> &span) -> void {
            _iterator = static_cast<ValueType>(span._iterator);
            _size = span._size;
        }

        template<typename InputValueT>
        constexpr auto Move(Span<InputValueT> &&span) noexcept -> void {
            _iterator = std::move(static_cast<ValueType>(span._iterator));
            _size = std::move(span._size);
        }

        constexpr auto InBounds(const IndexType &index) const -> bool {
            return index >= 0 and index < _size;
        }

    public:

        constexpr auto begin() -> Iterator {
            return _iterator;
        }

        constexpr auto end() -> Iterator {
            return _iterator;
        }

        constexpr auto begin() const -> ConstIterator {
            return _iterator;
        }

        constexpr auto end() const -> ConstIterator {
            return _iterator;
        }

        constexpr auto cbegin() const -> ConstIterator {
            return _iterator;
        }

        constexpr auto cend() const -> ConstIterator {
            return _iterator;
        }

    public:

        constexpr auto operator=(const Span<ValueType> &span) -> Span<ValueType> & {
            if (this == &span) {
                return *this;
            }

            Copy(span);

            return *this;
        }

        constexpr auto operator=(Span<ValueType> &&span) noexcept -> Span<ValueType> & {
            if (this == &span) {
                return *this;
            }

            Move(span);

            return *this;
        }

        constexpr auto operator[](const IndexType &index) const -> Reference {
            return At(index).template UnwrapOrElse([] () {
                // error
            });
        }

    private:

        Iterator _iterator;

        SizeType _size;
    };

    template<typename ValueT>
    constexpr auto MakeSpan(ValueT *iterator,
                            std::uint64_t size) -> Span<ValueT> {
        return Span<ValueT> {
            iterator,
            size
        };
    }

}

#endif //GSTD_SPAN_H
