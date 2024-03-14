#ifndef GSTD_INITIALIZERLIST_H
#define GSTD_INITIALIZERLIST_H

#include <initializer_list>

#include <gstd/Type/Types.h>

namespace gstd {

    /**
     * Class for extend standard initializer list class
     * @tparam ValueT Value type
     */
    template<typename ValueT>
    class InitializerList {
    public:

        /*
         *
         * InitializerList PUBLIC TYPES
         *
         */

        /**
         * Value type
         */
        using ValueType = ValueT;

        /**
         * Size type
         */
        using SizeType = U64;

        /**
         * Iterator type
         */
        using Iterator = const ValueType *;

        /**
         * Const iterator type
         */
        using ConstIterator = const ValueType *;

    public:

        /*
         *
         * InitializerList PUBLIC CONSTRUCTORS
         *
         */

        /**
         * Default constructor for initializer list
         */
        GSTD_CONSTEXPR InitializerList() GSTD_NOEXCEPT
                : InitializerList(nullptr,
                                  nullptr) {}

        /**
         * Constructor for initializer list
         * @param begin Begin of elements list
         * @param end End of elements list
         */
        GSTD_CONSTEXPR InitializerList(const ValueT *begin,
                                       const ValueT *end) GSTD_NOEXCEPT
                : _begin(begin),
                  _end(end) {}

        /**
         * Constructor for initializer list
         * @param initializerList Standard initializer list
         */
        GSTD_CONSTEXPR InitializerList(std::initializer_list<ValueT> initializerList) GSTD_NOEXCEPT
                : InitializerList(initializerList.begin(),
                                  initializerList.end()) {}

    public:

        /*
         *
         * InitializerList PUBLIC METHODS
         *
         */

        /**
         * Getting size of initializer list
         * @return Size of initializer list
         */
        GSTD_CONSTEXPR auto Size() const GSTD_NOEXCEPT -> SizeType {
            return StaticCast<SizeType>(_end - _begin);
        }

    public:

        /*
         *
         * InitializerList PUBLIC ITERATOR METHODS
         *
         */

        /**
         * Getting begin of initializer list
         * @return Begin iterator
         */
        GSTD_CONSTEXPR auto begin() const GSTD_NOEXCEPT -> ConstIterator {
            return _begin;
        }

        /**
         * Getting end of initializer list
         * @return End iterator
         */
        GSTD_CONSTEXPR auto end() const GSTD_NOEXCEPT -> ConstIterator {
            return _end;
        }

    private:

        /*
         *
         * InitializerList PRIVATE FIELDS
         *
         */

        /**
         * Begin element
         */
        const ValueT *_begin;

        /**
         * End element
         */
        const ValueT *_end;
    };

}

#endif //GSTD_INITIALIZERLIST_H
