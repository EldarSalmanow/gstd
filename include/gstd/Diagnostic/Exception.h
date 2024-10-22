#ifndef GSTD_EXCEPTION_H
#define GSTD_EXCEPTION_H

namespace gstd {

    // class Exception : public std::exception {
    // public:

        // Exception();
    // };

//    class ErrorId {
//    public:
//
//        /*constexpr*/ auto operator==(const ErrorId &errorId) const -> bool;
//    };
//
//    class Error {
//    public:
//
//        ErrorId GetId() const;
//    };
//
//    class RaiseInfo {
//    public:
//
//        RaiseInfo(gstd::SourceLocation location);
//
//    public:
//
//        gstd::SourceLocation GetLocation() const;
//    };
//
//    class ErrorHandler {
//    public:
//
//        virtual void Handle(const Error &error,
//                            const RaiseInfo &raiseInfo) = 0;
//
//        virtual bool CanHandle(const Error &error) = 0;
//    };
//
////template<typename ErrorT>
////struct ErrorTraits {
////
////    /* static void Handle(const ErrorT &error) noexcept; */
////
////};
//
//    class ErrorHandlerPool {
//    public:
//
//        [[noreturn]] virtual void Handle(const Error &error,
//                                         const RaiseInfo &raiseInfo) {
//            for (auto &handler : _handlers) {
//                if (handler.CanHandle(error)) {
//                    handler.Handle(error,
//                                   raiseInfo);
//
//                    break;
//                }
//            }
//
//            panic();
//        }
//
//    private:
//
//        std::vector<ErrorHandler> _handlers;
//    };
//
//    class MyError : public Error {
//    public:
//
//        static ErrorId ErrorStaticId();
//
//    public:
//
//        std::string GetMessage() const;
//    };
//
//    template<typename OutputErrorT,
//            typename InputErrorT>
//    constexpr auto ErrorCast(InputErrorT &&error) -> OutputErrorT {
//        return reinterpret_cast<const OutputErrorT &>(std::forward<InputErrorT>(error));
//    }
//
//    class MyErrorHandler : public ErrorHandler {
//    public:
//
//        void Handle(const Error &error,
//                    const RaiseInfo &raiseInfo) override {
//            auto myError = ErrorCast<MyError>(error);
//
//            auto loc = raiseInfo.GetLocation();
//            std::cout << myError.GetMessage() << " " << loc.File() << " " << loc.Function();
//        }
//
//        bool CanHandle(const Error &error) override {
//            return error.GetId() == MyError::ErrorStaticId();
//        }
//    };
//
//    template<typename ErrorT>
//    struct ErrorTraits {
//
//        static auto Handle(ErrorT &&error) -> void {
//            std::cerr << "Error `" << As_<std::string>(std::forward<ErrorT>(error)) << "` was thrown!" << std::endl;
//        }
//
//    };
//
//    template<typename ErrorT>
//    [[noreturn]] auto HandleError(ErrorT &&error) -> void {
//        ErrorTraits<ErrorT>::Handle(std::forward<ErrorT>(error));
//
//        panic();
//    }
//
//    struct Error_ {
//
//        virtual auto GetMessage() const noexcept -> std::string = 0;
//
//    };
//
//    struct EnumError_ : Error_ {
//
//        template<typename EnumT>
//        explicit EnumError_(EnumT &&value) {
//            std::stringstream stringStream;
//
//            stringStream << "`" << gstd::AsString(std::forward<EnumT>(value)) << "` was thrown!";
//
//            _message = stringStream.str();
//        }
//
//        auto GetMessage() const noexcept -> std::string override {
//            return _message;
//        }
//
//    private:
//
//        std::string _message;
//    };
//
//    struct RaiseContext_ {
//
//        constexpr auto File() const noexcept -> std::string_view {
//            return "main.cpp";
//        }
//
//        constexpr auto Function() const noexcept -> std::string_view {
//            return "main";
//        }
//
//        constexpr auto Line() const noexcept -> std::uint64_t {
//            return 92;
//        }
//
//        constexpr auto Column() const noexcept -> std::uint64_t {
//            return 10;
//        }
//
//    };
//
////struct EHContext_ {
////
////    std::ostream &GetIOHandler() {
////        return std::cerr;
////    }
////
////};
//
//    struct ErrorHandler_ {
//
//        explicit ErrorHandler_(std::ostream &ioHandler)
//                : _ioHandler(ioHandler) {}
//
//        virtual auto Handle(const Error_ &error,
//                            const RaiseContext_ &raiseContext) -> void {
//            std::stringstream stringStream;
//
//            stringStream << "`" << raiseContext.File() << "`@" << raiseContext.Function()
//                         << "[" << raiseContext.Line() << ":" << raiseContext.Column() << "]";
//            stringStream << " -> ";
//            stringStream << "\"" << error.GetMessage() << "\"";
//
//            _ioHandler << stringStream.str() << std::endl;
//        }
//
//    private:
//
//        std::ostream &_ioHandler;
//    };
//
//    enum class MyErr {
//        NoMem,
//        NoAccess
//    };
//
//    template<>
//    struct gstd::detail::As<std::string,
//            MyErr> {
//
//        using ToType = std::string;
//
//        using FromType = MyErr;
//
//        static auto Convert(FromType &&value) -> ToType {
//            return match<std::string>(std::forward<FromType>(value))
//                    .Case(MyErr::NoMem, "MyErr::NoMem")
//                    .Case(MyErr::NoAccess, "MyErr::NoAccess")
//                    .Default("MyErr::<unknown>")
//                    .Result()
//                    .Unwrap();
//        }
//
//    };
//
//    ErrorHandler_ &GlobalHandler() {
//        static ErrorHandler_ handler(std::cerr);
//
//        return handler;
//    }

    // error - сама ошибка
    // raise_context - информация о месте выброса ошибки и т.д.
    // pool_context - информация о хранилище обработчиков, потоках ввода-вывода и т.д.
    // error(<error>, <raise_context>, <pool_context>) -> EHPool -> <error_handler>.CanHandle(<error>) -> <error_handler>.Handle(<error>) -> <handling> -> panic() -> <panic_handler> -> <end>

    // template<typename ExceptionT,
             // typename FunctionT,
             // typename... ArgumentsT>
    // auto Catch(FunctionT &&function,
               // ArgumentsT &&...arguments) -> gstd::Result<std::invoke_result_t<FunctionT &&,
                                                                               // ArgumentsT &&...>,
                                                          // ExceptionT> {
        // try {
            // return gstd::MakeOk(std::invoke(std::forward<FunctionT>(function),
                                            // std::forward<ArgumentsT>(arguments)...));
        // } catch (ExceptionT &exception) {
            // return gstd::MakeErr(std::move(exception));
        // }
    // }

}

#endif //GSTD_EXCEPTION_H
