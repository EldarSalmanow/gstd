#ifndef GSTD_TYPES_H
#define GSTD_TYPES_H

// TODO: remove?
#include <cstdint>

#include <gstd/Macro/Macro.h>

namespace gstd {

    namespace types {

#define GSTD_FALSE false
#define GSTD_TRUE true

        class Void {

        };

        class Bool {
        public:

            GSTD_CONSTEXPR Bool() GSTD_NOEXCEPT
                    : _value(GSTD_FALSE) {}

            GSTD_CONSTEXPR Bool(bool value) GSTD_NOEXCEPT
                    : _value(value) {}

            GSTD_CONSTEXPR Bool(const Bool &boolValue) GSTD_NOEXCEPT
                    : _value(boolValue._value) {}

            GSTD_CONSTEXPR Bool(Bool &&boolValue) GSTD_NOEXCEPT
                    : _value(std::move(boolValue._value)) {}

        public:

            GSTD_CONSTEXPR ~Bool() GSTD_NOEXCEPT = default;

        public:

            GSTD_CONSTEXPR auto Value() const GSTD_NOEXCEPT -> bool {
                return _value;
            }

        public:

            GSTD_CONSTEXPR auto operator=(const Bool &boolValue) GSTD_NOEXCEPT -> Bool & {
                if (this == &boolValue) {
                    return *this;
                }

                _value = boolValue._value;

                return *this;
            }

            GSTD_CONSTEXPR auto operator=(Bool &&boolValue) GSTD_NOEXCEPT -> Bool & {
                _value = std::move(boolValue._value);

                return *this;
            }

            GSTD_CONSTEXPR auto operator==(const Bool &boolValue) const GSTD_NOEXCEPT -> Bool {
                return Bool {
                    _value == boolValue._value
                };
            }

            GSTD_CONSTEXPR auto operator<=>(const Bool &boolValue) const GSTD_NOEXCEPT /*-> Bool*/ {
                return //Bool {
                    _value <=> boolValue._value;
                //};
            }

            GSTD_CONSTEXPR GSTD_EXPLICIT operator bool() const GSTD_NOEXCEPT {
                return _value;
            }

        private:

            bool _value;
        };

        GSTD_INLINE GSTD_CONSTEXPR const Bool True = Bool(GSTD_TRUE);

        GSTD_INLINE GSTD_CONSTEXPR const Bool False = Bool(GSTD_FALSE);

        void t(Bool b) {
            if (True >= b) {
            }
        }

        class Int8 {
        public:

            GSTD_CONSTEXPR Int8() GSTD_NOEXCEPT
                    : _value(0) {}

            GSTD_CONSTEXPR Int8(std::int8_t value) GSTD_NOEXCEPT
                    : _value(value) {}

            GSTD_CONSTEXPR Int8(const Int8 &int8Value) GSTD_NOEXCEPT
                    : _value(int8Value._value) {}

            GSTD_CONSTEXPR Int8(Int8 &&int8Value) GSTD_NOEXCEPT
                    : _value(std::move(int8Value._value)) {}

        public:

            GSTD_CONSTEXPR ~Int8() GSTD_NOEXCEPT = default;

        public:

            GSTD_CONSTEXPR auto Value() const GSTD_NOEXCEPT -> std::int8_t {
                return _value;
            }

        public:

            GSTD_CONSTEXPR auto operator=(const Int8 &boolValue) GSTD_NOEXCEPT -> Int8 & {
                if (this == &boolValue) {
                    return *this;
                }

                _value = boolValue._value;

                return *this;
            }

            GSTD_CONSTEXPR auto operator=(Int8 &&boolValue) GSTD_NOEXCEPT -> Int8 & {
                _value = std::move(boolValue._value);

                return *this;
            }

            GSTD_CONSTEXPR auto operator==(const Int8 &boolValue) const GSTD_NOEXCEPT -> Int8 {
                return Int8 {
                    std::int8_t(_value == boolValue._value)
                };
            }

            GSTD_CONSTEXPR auto operator<=>(const Int8 &boolValue) const GSTD_NOEXCEPT /*-> Bool*/ {
                return //Bool {
                        _value <=> boolValue._value;
                //};
            }

            GSTD_CONSTEXPR GSTD_EXPLICIT operator std::int8_t() const GSTD_NOEXCEPT {
                return _value;
            }

        private:

            std::int8_t _value;
        };

    }

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
