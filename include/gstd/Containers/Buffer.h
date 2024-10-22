#ifndef GSTD_BUFFER_H
#define GSTD_BUFFER_H

#include <gstd/Type/Optional.h>

namespace gstd {

    template<typename ValueT>
    class Buffer {
    public:

        using ValueType = ValueT;

        using SizeType = std::uint64_t;

    public:

        static_assert(std::is_unsigned_v<SizeType>, "`SizeType` must be unsigned!");

    public:

        Buffer(ValueType *data,
               SizeType size)
                : _data(data),
                  _size(size) {}

    public:

        GSTD_CONSTEXPR auto At(SizeType index) GSTD_NOEXCEPT -> Optional<Ref<ValueT>> {
            if (!InBounds(index)) {
                return MakeNone();
            }

            return MakeSome(_data[index]);
        }

    public:

        GSTD_CONSTEXPR auto Size() const GSTD_NOEXCEPT {
            return _size;
        }

    private:

        GSTD_CONSTEXPR auto InBounds(SizeType index) const GSTD_NOEXCEPT -> bool {
            return index < Size();
        }

    public:

        GSTD_CONSTEXPR auto operator[](const SizeType &index) {
            return At(index).template UnwrapOrElse([] () {
                Panic("Index out of range!");
            });
        }

    private:

        ValueType *_data;

        SizeType _size;
    };

    class CircularBuffer {

    };

}

#endif //GSTD_BUFFER_H
