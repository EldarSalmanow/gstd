#ifndef GSTD_SLICE_H
#define GSTD_SLICE_H

#include <gstd/Type/Types.h>

namespace gstd {

    /**
     * Position type
     */
    using Position = std::int64_t;

    /**
     * Constant for indicating about unknown position<br>
     * May indicate about begin or end of container, that getting slice with it, or about default step (1 step)
     */
    inline constexpr Position NoPos = std::numeric_limits<Position>::max();

    /**
     * Class for making slices from data by start and stop position and step<br>
     * For example
     * @code
     * Vector<int> vector = {1, 2, 3, 4, 5, 6};
     * vector[Slice::New(NoPos, 4)]; // [1, 2, 3, 4]
     * vector[Slice::New(NoPos, NoPos)]; // [1, 2, 3, 4, 5, 6]
     * vector[Slice::New(3, NoPos)]; // [4, 5, 6]
     * vector[Slice::New(3, NoPos, -1)]; // [4, 3, 2, 1]
     * vector[Slice::New(1, 5, 2)]; // [2, 4]
     * @endcode
     */
    class Slice {
    public:

        /*
         *
         * Slice PUBLIC CONSTRUCTORS
         *
         */

        /**
         * Main constructor for `Slice`
         * @param start Start position
         * @param stop Stop position
         * @param step Step
         */
        GSTD_CONSTEXPR Slice(Position start,
                             Position stop,
                             Position step) GSTD_NOEXCEPT
                : _start(start),
                  _stop(stop),
                  _step(step) {}

    public:

        /*
         *
         * Slice PUBLIC STATIC METHODS
         *
         */

        /**
         * Creating `Slice`
         * @param start Start position
         * @param stop Stop position
         * @param step Step
         * @return `Slice`
         */
        static GSTD_CONSTEXPR auto New(Position start,
                                       Position stop,
                                       Position step) GSTD_NOEXCEPT -> Slice {
            return Slice {
                start,
                stop,
                step
            };
        }

        /**
         * Creating `Slice` with default step (1 step)
         * @param start Start position
         * @param stop Stop position
         * @return `Slice`
         */
        static GSTD_CONSTEXPR auto New(Position start,
                                       Position stop) GSTD_NOEXCEPT -> Slice {
            return Slice::New(start,
                              stop,
                              1);
        }

    public:

        /*
         *
         * Slice PUBLIC METHODS
         *
         */

        /**
         * Getting start position
         * @return Start position
         */
        GSTD_CONSTEXPR auto GetStart() const GSTD_NOEXCEPT -> Position {
            return _start;
        }

        /**
         * Getting stop position
         * @return Stop position
         */
        GSTD_CONSTEXPR auto GetStop() const GSTD_NOEXCEPT -> Position {
            return _stop;
        }

        /**
         * Getting step
         * @return Step
         */
        GSTD_CONSTEXPR auto GetStep() const GSTD_NOEXCEPT -> Position {
            return _step;
        }

    private:

        /*
         *
         * Slice PRIVATE FIELDS
         *
         */

        /**
         * Start position
         */
        Position _start;

        /**
         * Stop position
         */
        Position _stop;

        /**
         * Step
         */
        Position _step;
    };

    /**
     * Smart creating of `Slice`
     * @param start Start position
     * @param stop Stop position
     * @param step Step
     * @return `Slice`
     */
    GSTD_CONSTEXPR auto MakeSlice(Position start,
                                  Position stop,
                                  Position step) GSTD_NOEXCEPT -> Slice {
        return Slice::New(start,
                          stop,
                          step);
    }

    /**
     * Smart creating of `Slice` with default step (1 step)
     * @param start Start position
     * @param stop Stop position
     * @return `Slice`
     */
    GSTD_CONSTEXPR auto MakeSlice(Position start,
                                  Position stop) GSTD_NOEXCEPT -> Slice {
        return Slice::New(start,
                          stop);
    }

}

#endif //GSTD_SLICE_H
