#ifndef GSTD_ITERATOR_H
#define GSTD_ITERATOR_H

#include <gstd/Type/Optional.h>

namespace gstd {

    /**
     * Base class for all iterators
     * @tparam ValueT Value type of iterating
     */
    template<typename ValueT>
    class Iterator {
    public:

        /*
         *
         * Iterator PUBLIC TYPES
         *
         */

        /**
         * Value type of iterating
         */
        using ValueType = ValueT;

        /**
         * Reference type of value
         * @todo Translate to Ref<ValueType>?
         */
        using Reference = ValueType &;

    public:

        /*
         *
         * Iterator PUBLIC DESTRUCTORS
         *
         */

        /**
         * Virtual destructor for supporting inheritance
         */
        virtual GSTD_CONSTEXPR ~Iterator() = default;

    public:

        /*
         *
         * Iterator PUBLIC METHODS
         *
         */

        /**
         * Get current value of iterator and move it to the next value
         * @return Current value of iterator
         */
        GSTD_CONSTEXPR auto Next() -> Optional<Ref<ValueType>> {
            return DoNext();
        }

    protected:

        /*
         *
         * Iterator PROTECTED VIRTUAL METHODS
         *
         */

        /**
         * Implementation of 'Next' method
         * @return Current value of iterator
         */
        virtual GSTD_CONSTEXPR auto DoNext() -> Optional<Ref<ValueType>> = 0;
    };

    template<typename ValueT>
    class RangeIterator : public Iterator<ValueT> {
    public:

        using ValueType = typename Iterator<ValueT>::ValueType;

        using Pointer = ValueType *;

    public:

        GSTD_CONSTEXPR RangeIterator(Pointer pointer,
                                     Pointer sentinel) GSTD_NOEXCEPT
                : _pointer(pointer),
                  _sentinel(sentinel) {}

    protected:

        GSTD_CONSTEXPR auto DoNext() -> Optional<Ref<ValueType>> override {
            if (_pointer == _sentinel) {
                return MakeNone();
            }

            auto reference = MakeRef(*_pointer);

            ++_pointer;

            return MakeSome(reference);
        }

    public:

        Pointer _pointer;

        Pointer _sentinel;
    };

#define GSTD_FOREACH(iterable) auto __iterator = iterable.Iter(); for (auto __current = __iterator.Next(); __current.IsSome(); __current = __iterator.Next())

#define GSTD_FOREACH_BODY_BEGIN auto value = std::move(__current).Unwrap();

    template<typename IterableT,
             typename FunctionT>
    GSTD_CONSTEXPR auto ForEach(IterableT &&iterable,
                                FunctionT &&function) {
        GSTD_FOREACH(iterable) {
            GSTD_FOREACH_BODY_BEGIN

            std::invoke(std::forward<FunctionT>(function),
                        value);
        }
    }

}

#endif //GSTD_ITERATOR_H
