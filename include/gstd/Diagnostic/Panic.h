#ifndef GSTD_PANIC_H
#define GSTD_PANIC_H

namespace gstd {

    [[noreturn]] void Panic(const char *message);

    using PanicHandlerFn = void (*) (const char *message);

    PanicHandlerFn GetPanicHandler();

    PanicHandlerFn SetPanicHandler(PanicHandlerFn handler);

//    class PanicHandler {
//    public:
//
//        [[noreturn]] static auto Call(const char *message) -> void;
//
//        static auto Get() -> PanicHandlerFn;
//
//        static auto Set(PanicHandlerFn handler) -> PanicHandlerFn;
//    };

}

#endif //GSTD_PANIC_H
