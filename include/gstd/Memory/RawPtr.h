#ifndef GSTD_RAWPTR_H
#define GSTD_RAWPTR_H

#include <fmt/format.h>

namespace gstd {

    template<typename ValueT>
    class RawPtr {
    public:

        using ValueType = ValueT;

        using ReferenceType = ValueType &;

        using ConstReferenceType = const ValueType &;

        using PointerType = ValueType *;

        using ConstPointerType = const ValueType *;

    public:

        GSTD_CONSTEXPR RawPtr(PointerType pointer) GSTD_NOEXCEPT
                : _pointer(pointer) {}

    public:

        static GSTD_CONSTEXPR auto New(PointerType pointer) GSTD_NOEXCEPT -> RawPtr {
            return RawPtr {
                pointer
            };
        }

        static GSTD_CONSTEXPR auto New() GSTD_NOEXCEPT -> RawPtr {
            return RawPtr::New(nullptr);
        }

    public:

        GSTD_CONSTEXPR auto Value() GSTD_NOEXCEPT -> PointerType {
            return _pointer;
        }

        GSTD_CONSTEXPR auto Value() const GSTD_NOEXCEPT -> ConstPointerType {
            return _pointer;
        }

        GSTD_CONSTEXPR auto HasValue() const GSTD_NOEXCEPT -> bool {
            return Value() != nullptr;
        }

        // TODO
        GSTD_CONSTEXPR auto Expect(std::string_view message) const -> void {
            if (!HasValue()) {
                Panic(fmt::format("`RawPtr` failed expect some value in pointer with message '{}'!", message).c_str());
            }
        }

        GSTD_CONSTEXPR auto Clone() const GSTD_NOEXCEPT -> RawPtr {
            return RawPtr::New(_pointer);
        }

    public:

        GSTD_CONSTEXPR auto operator*() -> ReferenceType {
#if GSTD_VALIDATION_LEVEL > 0
            Expect("Dereference of null pointer");
#endif

            return *_pointer;
        }

        GSTD_CONSTEXPR auto operator*() const -> ConstReferenceType {
#if GSTD_VALIDATION_LEVEL > 0
            Expect("Dereference of null pointer");
#endif

            return *_pointer;
        }

        GSTD_CONSTEXPR auto operator->() GSTD_NOEXCEPT -> PointerType {
#if GSTD_VALIDATION_LEVEL > 0
            Expect("Dereference of null pointer");
#endif

            return _pointer;
        }

        GSTD_CONSTEXPR auto operator->() const GSTD_NOEXCEPT -> ConstPointerType {
#if GSTD_VALIDATION_LEVEL > 0
            Expect("Dereference of null pointer");
#endif

            return _pointer;
        }

        GSTD_CONSTEXPR operator bool() GSTD_NOEXCEPT {
            return HasValue();
        }

        GSTD_CONSTEXPR operator PointerType() GSTD_NOEXCEPT {
            return _pointer;
        }

    private:

        PointerType _pointer;
    };

}

#endif //GSTD_RAWPTR_H
