#ifndef GSTD_IO_H
#define GSTD_IO_H

#include <future>

namespace gstd {

    class HandleId;

}

namespace std {

    template<>
    struct hash<gstd::HandleId> {

        GSTD_CONSTEXPR auto operator()(const gstd::HandleId &handle) const noexcept -> size_t {
            return 0;
        }

    };

}

namespace gstd {

    enum class IOError {
        ReadError
    };

    class Buffer {
    public:

        static GSTD_CONSTEXPR auto New(const char *string) -> Buffer;
    };

    class Handle {
    public:

        virtual Optional<IOError> Read(Buffer &buffer);

        virtual Optional<IOError> Write(const Buffer &buffer);
    };

    class FileHandle : public Handle {
    public:

        GSTD_CONSTEXPR GSTD_EXPLICIT FileHandle(FILE *file);

    public:

        Optional<IOError> Read(Buffer &buffer) override {
            return MakeNone();
        }
    };

    class HandleId {
    public:

        HandleId(); // TODO

        GSTD_CONSTEXPR GSTD_EXPLICIT HandleId(std::uint64_t id)
                : _id(id) {}

        GSTD_DEFAULT_COPY_CONSTRUCTOR(HandleId)

        GSTD_DEFAULT_MOVE_CONSTRUCTOR(HandleId)

    public:

        static auto New() -> HandleId {
            static std::atomic<std::uint64_t> counter { 1 };

            auto id = counter.load();

            counter.fetch_add(1);

            return HandleId {
                id
            };
        }

    public:

        GSTD_CONSTEXPR auto operator==(const HandleId &handle) const noexcept -> bool {
            return true;
        }

    private:

        std::uint64_t _id;
    };

    class HandlesPool {
    public:

        static auto Instance() -> HandlesPool & {
            static auto pool = std::make_unique<HandlesPool>();

            return *pool;
        }

    public:

        auto AddHandle(Handle &&handle) -> HandleId {
            auto id = HandleId::New();

            _handles[id] = std::move(handle);

            return id;
        }

        auto GetHandle(const HandleId &handle) -> Handle & {
            return _handles[handle];
        }

        auto RemoveHandle(const HandleId &handle) -> void {
            _handles.erase(handle);
        }

    private:

        std::unordered_map<HandleId, Handle> _handles;
    };

    auto InitCOutHandle() -> Result<HandleId, IOError> {
        FileHandle handle(stdout);

        auto id = HandlesPool::Instance().AddHandle(std::move(handle));

        return MakeOk(std::move(id));
    }

    auto InitCInHandle() -> Result<HandleId, IOError> {
        FileHandle handle(stdin);

        auto id = HandlesPool::Instance().AddHandle(std::move(handle));

        return MakeOk(std::move(id));
    }

    HandleId COut() {
        static auto outId = InitCOutHandle().Unwrap();

        return outId;
    }

    HandleId CIn() {
        static auto inId = InitCInHandle().Unwrap();

        return inId;
    }

    gstd::Optional<HandleId> Open(const char *name) {
        FILE *file;

        FileHandle handle(file);

        auto id = HandlesPool::Instance().AddHandle(std::move(handle));

        return MakeSome(std::move(id));
    }

    void Close(HandleId &&handleId) {
        auto handle = HandlesPool::Instance().GetHandle(handleId);

        auto fileHandle = static_cast<FileHandle &>(handle);
    }

    void Write(HandleId handleId,
               const char *message) {
        auto buffer = Buffer::New(message);

        HandlesPool::Instance()
            .GetHandle(handleId)
            .Write(buffer)
            .Inspect([] () {
                // TODO
            });
    }

    void Read(HandleId handleId,
              char *buffer);

    void Print(const char *message,
               HandleId handleId = COut());

    void Input(const char *message,
               HandleId handleId = CIn());

    void r() {
        auto h = Open("test.gs");
        char buffer[100];
        Read(h, buffer);
        Close(std::move(h));
        Print(buffer);
    }

}

//namespace gstd {
//
//    class File;
//    class Console;
//
//    class System {
//    public:
//
//        System();
//
//    public:
//
//        static System New();
//
//    public:
//
//        File *GetFile(const std::string &name);
//
//        Console *GetConsole();
//    };
//
//    class File {
//    public:
//
//
//    };
//
//    class FileSystem {
//    public:
//
//        FileSystem();
//
//    public:
//
//
//    };
//
//    void Print(const String &string);
//
//    void j() {
////        auto system = System::New();
////
////        auto console = system.GetConsole();
////        auto file = system.GetFile("test.txt");
//    }
//
//    // aio - Asynchronous Input/Output
//
//    class Stream {
//
//    };
//
//    class StreamId {
//
//    };
//
//    class Request {
//
//    };
//
//    class OpenRequest : public Request {
//    public:
//
//        explicit OpenRequest(std::string name)
//                : _name(std::move(name)) {}
//
//    private:
//
//        std::string _name;
//    };
//
//    class CloseRequest : public Request {
//    public:
//
//        constexpr explicit CloseRequest(StreamId streamId)
//                : _streamId(streamId) {}
//
//    private:
//
//        StreamId _streamId;
//    };
//
//    class ReadRequest : public Request {
//    public:
//
//        constexpr explicit ReadRequest(StreamId streamId)
//                : _streamId(streamId) {}
//
//    private:
//
//        StreamId _streamId;
//    };
//
//    class WriteRequest : public Request {
//    public:
//
//        WriteRequest(StreamId streamId,
//                     std::string string)
//                : _streamId(streamId),
//                  _string(std::move(string)) {}
//
//    private:
//
//        StreamId _streamId;
//
//        std::string _string;
//    };
//
//    class Response {
//
//    };
//
//    class OpenResponse : public Response {
//    public:
//
//        constexpr explicit OpenResponse(StreamId streamId) noexcept
//                : _streamId(streamId) {}
//
//    public:
//
//        constexpr auto GetStreamId() const noexcept -> StreamId {
//            return _streamId;
//        }
//
//    private:
//
//        StreamId _streamId;
//    };
//
//    class CloseResponse : public Response {
//    public:
//
//        constexpr explicit CloseResponse(bool result) noexcept
//                : _result(result) {}
//
//    public:
//
//        constexpr auto GetResult() const noexcept -> bool {
//            return _result;
//        }
//
//    private:
//
//        bool _result;
//    };
//
//    class ReadResponse : public Response {
//    public:
//
//        explicit ReadResponse(std::string result) noexcept
//                : _result(std::move(result)) {}
//
//    public:
//
//        auto GetResult() const noexcept -> std::string {
//            return _result;
//        }
//
//    private:
//
//        std::string _result;
//    };
//
//    class WriteResponse : public Response {
//    public:
//
//        constexpr explicit WriteResponse(bool result) noexcept
//                : _result(result) {}
//
//    public:
//
//        constexpr auto GetResult() const noexcept -> bool {
//            return _result;
//        }
//
//    private:
//
//        bool _result;
//    };
//
//    class StreamPool {
//    public:
//
//        std::future<Response> Process(const Request &request) {
//            std::promise<Response> promise;
//
//            promise.set_value(OpenResponse { StreamId {} });
//
//            return promise.get_future();
//        }
//
//    };
//
//    template<typename T>
//    class Singleton {
//    public:
//
//        static constexpr auto Instance() -> T & {
//            return *_object;
//        }
//
//    private:
//
//        static std::unique_ptr<T> _object;
//    };
//
//    template<typename T>
//    std::unique_ptr<T> Singleton<T>::_object = std::make_unique<T>();
//
//    using GlobalStreamPool = Singleton<StreamPool>;
//
//    constexpr auto GlobalPool() -> StreamPool & {
//        return GlobalStreamPool::Instance();
//    }
//
//    auto open(const std::string &name) -> std::future<StreamId> {
//        return GlobalPool()
//        .Process(OpenRequest { name })
//        .Then([] (Response &response) -> StreamId {
//            return static_cast<OpenResponse &>(response).GetStreamId();
//        });
//    }
//
//    auto close(const StreamId &streamId) -> std::future<bool> {
//        return GlobalPool()
//        .Process(CloseRequest { streamId })
//        .Then([] (Response &response) -> bool {
//            return static_cast<CloseResponse &>(response).GetResult();
//        });
//    }
//
//    auto read(const StreamId &streamId) -> std::future<std::string> {
//        return GlobalPool()
//        .Process(ReadRequest { streamId })
//        .Then([] (Response &response) -> std::string {
//            return static_cast<ReadResponse &>(response).GetResult();
//        });
//    }
//
//    auto write(const StreamId &streamId,
//               const std::string &string) -> std::future<bool> {
//        return GlobalPool()
//        .Process(WriteRequest { streamId,
//                                string })
//        .Then([] (Response &response) -> bool {
//            return static_cast<WriteResponse &>(response).GetResult();
//        });
//    }
//
//    auto print();
//
//    auto input();
//
//    auto error();
//
//    void t() {
//        auto stream_ = open("test.txt");
//
//        // some operations
//
//        auto stream = stream_.get();
//
//        auto text = read(stream).get();
//        close(stream).get();
//    }
//
//}

#endif //GSTD_IO_H
