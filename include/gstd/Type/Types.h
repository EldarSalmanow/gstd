#ifndef GSTD_TYPES_H
#define GSTD_TYPES_H

// TODO: remove?
#include <cstdint>

#include <gstd/Macro/Macro.h>

namespace gstd {

    /**
     * Declaring Void type
     */
    using Void = void;

    /**
     * Declaring Bool type
     */
    using Bool = bool;

    /**
     * Declaring C(har) type
     */
    using C = char;

#if defined(GSTD_HAS_CHAR8T)

    /**
     * Declaring C(har)8 type
     */
    using C8 = char8_t;

#endif

    /**
     * Declaring C(har)16 type
     */
    using C16 = char16_t;

    /**
     * Declaring C(har)32 type
     */
    using C32 = char32_t;

    /**
     * Declaring W(ide)C(har) type
     */
    using WC = wchar_t;

    /**
     * Declaring I(nt)8 type
     */
    using I8 = std::int8_t;

    /**
     * Declaring U(nsigned int)8 type
     */
    using U8 = std::uint8_t;

    /**
     * Declaring I(nt)16 type
     */
    using I16 = std::int16_t;

    /**
     * Declaring U(nsigned int)16 type
     */
    using U16 = std::uint16_t;

    /**
     * Declaring I(nt)32 type
     */
    using I32 = std::int32_t;

    /**
     * Declaring U(nsigned int)32 type
     */
    using U32 = std::uint32_t;

    /**
     * Declaring I(nt)64 type
     */
    using I64 = std::int64_t;

    /**
     * Declaring U(nsigned int)64 type
     */
    using U64 = std::uint64_t;

}

#endif //GSTD_TYPES_H
