#ifndef GSTD_STRING_H
#define GSTD_STRING_H

#include <gstd/Containers/Span.h>

namespace gstd {

    using CodePoint = std::uint32_t;

    class Symbol {
    public:

        Symbol(CodePoint codePoint);

    public:

        static Symbol New(CodePoint codePoint);

    private:

        CodePoint _codePoint;
    };

    constexpr auto Size(const char *string) -> size_t {
        size_t size = 0;

        for (size_t index = 0; string[index] != '\0'; ++index, ++size) {}

        return size;
    }

    class StringIterator;

    class String {
    public:

        using ValueType = std::uint8_t;

        using Iterator = StringIterator;

        using ConstIterator = const StringIterator;

        using value_type = ValueType;

        using pointer = ValueType *;

        using const_pointer = const ValueType *;

        using reference = ValueType &;

        using const_reference = const ValueType &;

        using iterator = Iterator;

        using const_iterator = ConstIterator;

    public:

        GSTD_CONSTEXPR GSTD_EXPLICIT String(const char *string)
                : _buffer(const_cast<char *>(string),
                          Size(string)) {}

    public:

        static GSTD_CONSTEXPR auto New(const char *string) -> String {
            return String {
                string
            };
        }

        static GSTD_CONSTEXPR auto New(const std::string &string) -> String {
            return String::New(string.data());
        }

        static GSTD_CONSTEXPR auto New(const char8_t *string) -> String;

//        static GSTD_CONSTEXPR auto New(const std::u8string &string) -> String {
//            return String::New(string.data());
//        } TODO

        static GSTD_CONSTEXPR auto New(const char16_t *string) -> String;

        static GSTD_CONSTEXPR auto New(const std::u16string &string) -> String {
            return String::New(string.data());
        }

        static GSTD_CONSTEXPR auto New(const char32_t *string) -> String;

        static GSTD_CONSTEXPR auto New(const std::u32string &string) -> String {
            return String::New(string.data());
        }

        static GSTD_CONSTEXPR auto New(const wchar_t *string) -> String;

        static GSTD_CONSTEXPR auto New(const std::wstring &string) -> String {
            return String::New(string.data());
        }

    public:

        Optional<Ref<char>> At(const size_t &index) {
            return _buffer.At(index);
        }

    public:

        GSTD_CONSTEXPR auto operator[](const size_t &index) -> char {
            return _buffer[index];
        }

        GSTD_CONSTEXPR auto operator[](const size_t &index) const -> const char {
            return _buffer[index];
        }

    private:

        Span<char> _buffer;
    };

    class StringIterator {
    public:

        StringIterator(char *pointer);

    public:

        bool has_next() const;

        void next();

        Symbol current() const;
    };

    class StringRef {
    private:

        const char *_data;

        std::uint64_t _size;
    };

    class SmallString {
    public:


    };

    class StaticString {
    public:


    };

    inline std::istream &operator>>(std::istream &stream,
                                    const String &string);

    inline std::ostream &operator<<(std::ostream &stream,
                                    const String &string);

}

#endif //GSTD_STRING_H
