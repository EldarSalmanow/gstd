#ifndef GSTD_SOURCELOCATION_H
#define GSTD_SOURCELOCATION_H

#include <gstd/Type/Types.h>

namespace gstd {

    class SourceLocation {
    public:

        GSTD_CONSTEXPR SourceLocation(const std::uint32_t line,
                                      const std::uint32_t column,
                                      const char *const file,
                                      const char *const function)
                : _line(line),
                  _column(column),
                  _file(file),
                  _function(function) {}

    public:

        static GSTD_CONSTEXPR SourceLocation New(const std::uint32_t line,
                                                 const std::uint32_t column,
                                                 const char *const file,
                                                 const char *const function) GSTD_NOEXCEPT {
            return SourceLocation(line,
                                  column,
                                  file,
                                  function);
        }

    public:

        GSTD_INLINE GSTD_CONSTEXPR std::uint32_t Line() const GSTD_NOEXCEPT {
            return _line;
        }

        GSTD_INLINE GSTD_CONSTEXPR std::uint32_t Column() const GSTD_NOEXCEPT {
            return _column;
        }

        GSTD_INLINE GSTD_CONSTEXPR const char *File() const GSTD_NOEXCEPT {
            return _file;
        }

        GSTD_INLINE GSTD_CONSTEXPR const char *Function() const GSTD_NOEXCEPT {
            return _function;
        }

    public:

        template <std::size_t Index>
        GSTD_CONSTEXPR auto get() & {
            static_assert(Index < 3,
                          "`Index` must be in [0, 3] range for `gstd::SourceLocation`!");

            if GSTD_CONSTEXPR (Index == 0) {
                return _line;
            } else if GSTD_CONSTEXPR (Index == 1) {
                return _column;
            } else if GSTD_CONSTEXPR (Index == 2) {
                return _file;
            } else if GSTD_CONSTEXPR (Index == 3) {
                return _function;
            }
        }

        template <std::size_t Index>
        GSTD_CONSTEXPR auto get() const & -> const std::tuple_element_t<Index,
                                                                        Pair<FirstType,
                                                                             SecondType>> & {
            static_assert(Index < 2,
                          "`Index` must be in [0, 1] range for `gstd::Pair<FirstType, SecondType>`!");

            if GSTD_CONSTEXPR (Index == 0) {
                return _first;
            } else if GSTD_CONSTEXPR (Index == 1) {
                return _second;
            }
        }

        template <size_t Index>
        GSTD_CONSTEXPR auto get() && -> std::tuple_element_t<Index,
                gstd::Pair<FirstType,
                        SecondType>> && {
            static_assert(Index >= 0 && Index < 2,
                          "`Index` must be in [0, 1] range for `gstd::Pair<FirstType, SecondType>`!");

            if GSTD_CONSTEXPR (Index == 0) {
                return std::move(_first);
            } else if GSTD_CONSTEXPR (Index == 1) {
                return std::move(_second);
            }
        }

        template <size_t Index>
        GSTD_CONSTEXPR auto get() const && -> const std::tuple_element_t<Index,
                gstd::Pair<FirstType,
                        SecondType>> && {
            static_assert(Index >= 0 && Index < 2,
                          "`Index` must be in [0, 1] range for `gstd::Pair<FirstType, SecondType>`!");

            if GSTD_CONSTEXPR (Index == 0) {
                return std::move(_first);
            } else if GSTD_CONSTEXPR (Index == 1) {
                return std::move(_second);
            }
        }

    private:

        std::uint32_t _line;

        std::uint32_t _column;

        const char *_file;

        const char *_function;
    };

}

#define GSTD_STRUCTURED_BINDING_DECL_BEGIN() namespace std {
#define GSTD_STRUCTURED_BINDING_DEFINE_TYPE(type, elements_size) template<> struct tuple_size<type> : integral_constant<size_t, (elements_size)> {};
#define GSTD_STRUCTURED_BINDING_DEFINE_ELEMENT(type_, index, subtype) template<> struct tuple_element<index, type_> { using type = subtype; };
//#define GSTD_STRUCTURED_BINDING_DEFINE_GET()
#define GSTD_STRUCTURED_BINDING_DECL_END() }

#define COMMA() ,

#define GSTD_STRUCTURED_BINDING_METHOD_GET()

GSTD_STRUCTURED_BINDING_DECL_BEGIN()

    GSTD_STRUCTURED_BINDING_DEFINE_TYPE(gstd::SourceLocation, 4)

    GSTD_STRUCTURED_BINDING_DEFINE_ELEMENT(gstd::SourceLocation, 0, std::uint32_t)
    GSTD_STRUCTURED_BINDING_DEFINE_ELEMENT(gstd::SourceLocation, 1, std::uint32_t)
    GSTD_STRUCTURED_BINDING_DEFINE_ELEMENT(gstd::SourceLocation, 2, const char *)
    GSTD_STRUCTURED_BINDING_DEFINE_ELEMENT(gstd::SourceLocation, 3, const char *)

GSTD_STRUCTURED_BINDING_DECL_END()

namespace std {

//    template<typename FirstT,
//             typename SecondT>
//    struct tuple_size<gstd::Pair<FirstT,
//                                 SecondT>> : integral_constant<size_t,
//            2> {};
//
//    template<typename FirstT,
//            typename SecondT>
//    struct tuple_element<0,
//            gstd::Pair<FirstT,
//                    SecondT>> {
//
//        using type = FirstT;
//
//    };

//    template<typename FirstT,
//            typename SecondT>
//    struct tuple_element<1,
//            gstd::Pair<FirstT,
//                    SecondT>> {
//
//        using type = SecondT;
//
//    };

    template<size_t Index,
            typename FirstT,
            typename SecondT>
    GSTD_CONSTEXPR auto get(gstd::Pair<FirstT,
            SecondT> &pair) -> std::tuple_element_t<Index,
            gstd::Pair<FirstT,
                    SecondT>> & {
        return pair.template get<Index>();
    }

    template<size_t Index,
            typename FirstT,
            typename SecondT>
    GSTD_CONSTEXPR auto get(const gstd::Pair<FirstT,
            SecondT> &pair) -> const std::tuple_element_t<Index,
            gstd::Pair<FirstT,
                    SecondT>> & {
        return pair.template get<Index>();
    }

    template<size_t Index,
            typename FirstT,
            typename SecondT>
    GSTD_CONSTEXPR auto get(gstd::Pair<FirstT,
            SecondT> &&pair) -> std::tuple_element_t<Index,
            gstd::Pair<FirstT,
                    SecondT>> && {
        return pair.template get<Index>();
    }

    template<size_t Index,
            typename FirstT,
            typename SecondT>
    GSTD_CONSTEXPR auto get(const gstd::Pair<FirstT,
            SecondT> &&pair) -> const std::tuple_element_t<Index,
            gstd::Pair<FirstT,
                    SecondT>> && {
        return pair.template get<Index>();
    }

}

#define GSTD_LOCATION() gstd::SourceLocation::New(GSTD_LINE, GSTD_COLUMN, GSTD_FILE, GSTD_FUNCTION)

#endif //GSTD_SOURCELOCATION_H
