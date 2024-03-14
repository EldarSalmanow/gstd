#ifndef GSTD_SLICE_H
#define GSTD_SLICE_H

namespace gstd {

    enum class DirectionKind {
        Begin,
        End
    };

    // -1 -> last element
    // 1 -> first element
    // (End, Begin, -1)
    // (Begin, End, 1)

    class Position {
    public:

        using OffsetType = std::int64_t;

    public:

        GSTD_CONSTEXPR Position(OffsetType offset = 0,
                                DirectionKind kind = DirectionKind::Begin) GSTD_NOEXCEPT
                : _offset(offset),
                  _kind(kind) {}

    public:

        static GSTD_CONSTEXPR auto New(OffsetType offset = 0,
                                       DirectionKind kind = DirectionKind::Begin) -> Position {
            return Position {
                offset,
                kind
            };
        }

    public:

        GSTD_CONSTEXPR auto GetOffset() const -> OffsetType {
            return _offset;
        }

        GSTD_CONSTEXPR auto GetDirection() const -> DirectionKind {
            return _kind;
        }

    private:

        OffsetType _offset;

        DirectionKind _kind;
    };

    inline constexpr auto PEnd = Position::New(0, DirectionKind::End);

    inline constexpr auto PBegin = Position::New(0, DirectionKind::Begin);

    class Slice {
    public:

        // explicit
        GSTD_CONSTEXPR Slice(Position start,
                             Position stop,
                             Position step) GSTD_NOEXCEPT
                : _start(start),
                  _stop(stop),
                  _step(step) {}

    public:

        static GSTD_CONSTEXPR auto New(Position start,
                                       Position stop,
                                       Position step) -> Slice {
            return Slice {
                start,
                stop,
                step
            };
        }

        static GSTD_CONSTEXPR auto New(Position start,
                                       Position stop) -> Slice {
            auto step = Position {
                1,
                PositionKind::Concrete
            };

            return Slice::New(start,
                              stop,
                              step);
        }

        static GSTD_CONSTEXPR auto New(Position start) -> Slice {
            auto stop = Position {
                std::numeric_limits<std::int64_t>::max(),
                PositionKind::End
            };

            return Slice::New(start,
                              stop);
        }

        static GSTD_CONSTEXPR auto New(std::int64_t start,
                                       std::int64_t stop,
                                       std::int64_t step) -> Slice {

        }

    public:

        GSTD_CONSTEXPR auto Start() const -> Position {
            return _start;
        }

        GSTD_CONSTEXPR auto Stop() const -> Position {
            return _stop;
        }

        GSTD_CONSTEXPR auto Step() const -> Position {
            return _step;
        }

    private:

        Position _start;

        Position _stop;

        Position _step;
    };

}

#endif //GSTD_SLICE_H
