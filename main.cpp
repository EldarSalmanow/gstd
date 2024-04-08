#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

#include <gstd/gstd.h>

class LeaksTester {
public:

    LeaksTester() {
        std::cout << "LeaksTester constructed!" << std::endl;
    }

    LeaksTester(const LeaksTester &leaksTester) {
        std::cout << "LeaksTester copied!" << std::endl;
    }

    LeaksTester(LeaksTester &&leaksTester) noexcept {
        std::cout << "LeaksTester moved!" << std::endl;
    }

public:

    ~LeaksTester() {
        std::cout << "LeaksTester destroyed!" << std::endl;
    }

public:

    LeaksTester &operator=(const LeaksTester &leaksTester) {
        std::cout << "LeaksTester copied by assignment!" << std::endl;

        return *this;
    }

    LeaksTester &operator=(LeaksTester &&leaksTester) noexcept {
        std::cout << "LeaksTester moved by assignment!" << std::endl;

        return *this;
    }

};

#define GSTD_RAISE_EXCEPTION(message) gstd::GlobalEH().Handle(gstd::Exception {});

template<typename FunctionT,
         typename... ArgumentsT>
struct IsPredicate {

    inline static constexpr auto Value = std::is_same_v<std::invoke_result_t<FunctionT &&, ArgumentsT &&...>, bool>;

};

namespace saservice {

    class ComponentsContext;
    class ComponentConfig;

    class Component {
    public:

        Component(ComponentsContext &context,
                  const ComponentConfig &config);

    public:

        virtual ~Component() = default;
    };

    using ComponentPtr = std::unique_ptr<Component>;

    template<typename ComponentT>
    struct ComponentTraits {

        /* using ComponentType = ComponentT; */

        /* GSTD_INLINE static GSTD_CONSTEXPR auto Name = "<name>"; */

    };

    class ComponentInfo {
    public:

        ComponentInfo();

        ComponentInfo(ComponentPtr component,
                      std::string_view name);

    public:

        template<typename ComponentT>
        auto GetComponent() -> ComponentT & {
            if (auto ptr = dynamic_cast<ComponentT *>(_component.get())) {
                return *ptr;
            }

            throw std::runtime_error("Err!");
        }

        GSTD_CONSTEXPR auto GetName() -> std::string_view {
            return _name;
        }

    private:

        ComponentPtr _component;

        std::string_view _name;
    };

    class ComponentsContext {
    public:

        template<typename ComponentT>
        auto AddComponent() {
            _components[ComponentTraits<ComponentT>::Name] = ComponentInfo {
                std::make_unique<ComponentT>(),
                ComponentTraits<ComponentT>::Name
            };
        }

        template<typename ComponentT>
        auto FindComponent(std::string_view name) -> ComponentT & {
            auto &componentPtr = _components[name];

            auto &component = componentPtr.template GetComponent<ComponentT>();

            return component;
        }

        template<typename ComponentT>
        auto FindComponent() -> ComponentT & {
            return FindComponent<ComponentT>(ComponentTraits<ComponentT>::Name);
        }

    private:

        std::unordered_map<std::string_view, ComponentInfo> _components;
    };

    class ComponentConfig {

    };

    class ComponentsManager : public Component {
    public:

        ComponentsManager(ComponentsContext &context,
                          const ComponentConfig &config)
                : Component(context,
                            config) {}
    };

    class Request {

    };

    class Response {

    };

    class Controller : public Component {
    public:

        Controller(ComponentsContext &context,
                   const ComponentConfig &config)
                : Component(context,
                            config) {}

    public:

        virtual auto HandleRequest(const std::shared_ptr<Request> &request) -> std::shared_ptr<Response> {
            return nullptr;
        }
    };

    template<typename ControllerT>
    struct ControllerTraits : ComponentTraits<ControllerT> {

        /* using ControllerType = ControllerT; */

    };

    class HttpRequest : public Request {

    };

    class HttpResponse : public Response {
    public:

        HttpResponse(std::string body);
    };

    class HttpController : public Controller {
    public:

        HttpController(ComponentsContext &context,
                       const ComponentConfig &config)
                : Controller(context,
                             config) {}

    public:

        virtual auto HandleHttpRequest(const std::shared_ptr<HttpRequest> &httpRequest) -> std::shared_ptr<HttpResponse> {
            return nullptr;
        }

    public:

        auto HandleRequest(const std::shared_ptr<Request> &request) -> std::shared_ptr<Response> override {
            return HandleHttpRequest(std::reinterpret_pointer_cast<HttpRequest>(request));
        }
    };

    template<typename HttpControllerT>
    struct HttpControllerTraits : public ControllerTraits<HttpControllerT> {

        /* using HttpControllerType = HttpControllerT; */

        /* GSTD_INLINE static GSTD_CONSTEXPR auto Route = "<http_route>"; */

    };

    class DB : public Component {
    public:


    };

    class Model {

    };

    template<typename ModelT>
    struct ModelTraits {

        /* using ModelType = ModelT; */

        /* GSTD_INLINE static GSTD_CONSTEXPR auto Name = "<name>"; */

        /* GSTD_INLINE static auto ToJson(const ModelT &model) -> std::string; */

    };

    class Server : public Component {

    };

    class Logger : public Component {
    public:

        Logger(ComponentsContext &context,
               const ComponentConfig &config)
                : Component(context,
                            config) {}

    public:

        void Note(std::string_view message);
    };

    template<>
    struct ComponentTraits<Logger> {

        GSTD_INLINE static GSTD_CONSTEXPR auto Name = "Logger";

    };

}

class Book : public saservice::Model {
public:

    std::string name;

    std::string author;
};

template<>
struct saservice::ModelTraits<Book> {

    using ModelType = Book;

    GSTD_INLINE static GSTD_CONSTEXPR auto Name = "Book";

    GSTD_INLINE static auto ToJson(const Book &book) -> std::string {
        return "Name: " + book.name + "; Author: " + book.author + ";";
    }

};

class BooksDB : public saservice::DB {
public:

    BooksDB();

public:

    std::vector<Book> GetBooks();
};

template<>
struct saservice::ComponentTraits<BooksDB> {

    GSTD_INLINE static GSTD_CONSTEXPR auto Name = "BooksDB";

};

class AllBooksResponse : public saservice::HttpResponse {
public:

    static std::shared_ptr<saservice::HttpResponse> New(std::vector<Book> books);
};

struct Route {

    constexpr Route(const char *) {

    }

    const char *route;

};

inline constexpr auto operator""_route(const char *string, size_t size) -> Route {
    return Route { string };
}

class BooksController : public saservice::HttpController {
public:

    BooksController();

    BooksController(saservice::ComponentsContext &context,
                    const saservice::ComponentConfig &config)
            : saservice::HttpController(context,
                                        config),
              _db(context.FindComponent<BooksDB>()) {}

public:

    auto HandleHttpRequest(const std::shared_ptr<saservice::HttpRequest> &httpRequest) -> std::shared_ptr<saservice::HttpResponse> override {
        auto books = _db.GetBooks();
        
        return AllBooksResponse::New(books);
    }

public:

    static constexpr auto Route() -> Route {
        return "/books/{id: int}"_route;
    }

private:

    BooksDB &_db;
};

template<>
struct saservice::ComponentTraits<BooksController> {

    GSTD_INLINE static GSTD_CONSTEXPR auto Name = "BooksController";

};

template<>
struct saservice::HttpControllerTraits<BooksController> {

    GSTD_INLINE static GSTD_CONSTEXPR auto Route = "/books";

};

class Stream {
public:

    Stream &operator<<(const gstd::String &string);

    Stream &operator>>(gstd::String &string);
};

class Console {
public:

    static Stream *In();

    static Stream *Out();
};

class SmartPtr {
public:

    class SmartPtrRef;

    friend class SmartPtrRef;

public:

    bool Unused();

    bool IsUsed();

public:

    bool _used;
};

class SmartPtrRef {
public:

    SmartPtrRef(SmartPtr &pointer);

public:

    ~SmartPtrRef() {
        _pointer._used = false;
    }

private:

    SmartPtr _pointer;
};

class GC {
public:

    constexpr GC();

public:

    void Collect() {
        std::vector<SmartPtr> newPointers;

        for (auto &pointer : _pointers) {
            if (!pointer.IsUsed()) {

            }

            newPointers.emplace_back(pointer);
        }

        _pointers = newPointers;
    }

public:

    std::vector<SmartPtr> _pointers;
};

class App {
public:


};

namespace examples  {

    class MyClass {
    public:

        constexpr MyClass(int a)
                : _a(a) {}

    public:

        constexpr int get_a() const {
            return _a;
        }

    private:

        int _a;
    };

    namespace match {

        void MatchExample() {
            constexpr auto result = gstd::Match<std::string_view>(MyClass {10})
                    .Case([] (auto &&my_class) -> bool {
                        return my_class.get_a() > 10;
                    }, "Greater than 10!")
                    .Case([] (auto &&my_class) -> bool {
                        return my_class.get_a() == 10;
                    }, "Equal to 10!")
                    .Default("Less than 10!")
                    .Result()
                    .Unwrap();

            static_assert(result == "Equal to 10!");
        }

    }

    namespace optional {

        void OptionalExample() {
            constexpr auto result = gstd::MakeSomeOptional(10)
                                                            .Filter([] (const auto &value) -> bool {
                return value >= 10;
            }).Map([] (auto &&value) -> MyClass {
                return MyClass { std::forward<decltype(value)>(value) };
            }).Match([] (auto &value) -> MyClass {
                return MyClass { value.get_a() * 2 };
            },
                     [] () -> MyClass {
                return MyClass { 0 };
            });

            static_assert(result.get_a() == 20);
        }

    }

    namespace result {

        enum class Version {
            V1,
            V2,
            V3
        };

        enum class ParseVersionError {
            UnknownVersion
        };

        constexpr auto ParseVersion(std::string_view version) -> gstd::Result<Version, ParseVersionError> {
            if (version == "v1") {
                return gstd::MakeOk(Version::V1);
            } else if (version == "v2") {
                return gstd::MakeOk(Version::V2);
            } else if (version == "v3") {
                return gstd::MakeOk(Version::V3);
            }

            return gstd::MakeErr(ParseVersionError::UnknownVersion);
        }

        void ResultExample() {
            static_assert(ParseVersion("<?>").UnwrapErr() == ParseVersionError::UnknownVersion);
        }

    }

}

enum class ExpressionType {
    ValueExpression,
    BinaryExpression
};

class Expression {
public:

    GSTD_CONSTEXPR virtual auto GetType() const GSTD_NOEXCEPT -> ExpressionType = 0;
};

class ValueExpression : public Expression {
public:

    GSTD_CONSTEXPR GSTD_EXPLICIT ValueExpression(std::int64_t value) GSTD_NOEXCEPT
            : _value(value) {}

public:

    static GSTD_CONSTEXPR auto New(std::int64_t value) GSTD_NOEXCEPT -> ValueExpression {
        return ValueExpression(value);
    }

public:

    GSTD_CONSTEXPR auto GetValue() const GSTD_NOEXCEPT -> std::int64_t {
        return _value;
    }

public:

    GSTD_CONSTEXPR auto GetType() const GSTD_NOEXCEPT -> ExpressionType override {
        return ExpressionType::ValueExpression;
    }

private:

    std::int64_t _value;
};

enum class BinaryOperation {
    Add,
    Sub,
    Mul,
    Div
};

class BinaryExpression : public Expression {
public:

    GSTD_CONSTEXPR BinaryExpression(Expression *firstExpression,
                                    Expression *secondExpression,
                                    BinaryOperation operation) GSTD_NOEXCEPT
            : _firstExpression(firstExpression),
              _secondExpression(secondExpression),
              _operation(operation) {}

public:

    static GSTD_CONSTEXPR auto New(Expression *firstExpression,
                                   Expression *secondExpression,
                                   BinaryOperation operation) GSTD_NOEXCEPT -> BinaryExpression {
        return BinaryExpression(firstExpression,
                                secondExpression,
                                operation);
    }

public:

    GSTD_CONSTEXPR auto GetFirstExpression() const GSTD_NOEXCEPT -> Expression * {
        return _firstExpression;
    }

    GSTD_CONSTEXPR auto GetSecondExpression() const GSTD_NOEXCEPT -> Expression *{
        return _secondExpression;
    }

    GSTD_CONSTEXPR auto GetOperation() const GSTD_NOEXCEPT -> BinaryOperation {
        return _operation;
    }

public:

    GSTD_CONSTEXPR auto GetType() const GSTD_NOEXCEPT -> ExpressionType override {
        return ExpressionType::BinaryExpression;
    }

private:

    Expression *_firstExpression;

    Expression *_secondExpression;

    BinaryOperation _operation;
};

GSTD_CONSTEXPR auto Interpret(Expression *expression) -> std::int64_t {
    std::int64_t result = 0;

    auto expressionType = expression->GetType();

    if (expressionType == ExpressionType::ValueExpression) {
        auto valueExpression = static_cast<ValueExpression *>(expression);

        result = valueExpression->GetValue();
    } else if (expressionType == ExpressionType::BinaryExpression) {
        auto binaryExpression = static_cast<BinaryExpression *>(expression);

        auto firstExpression = binaryExpression->GetFirstExpression();
        auto secondExpression = binaryExpression->GetSecondExpression();
        auto operation = binaryExpression->GetOperation();

        auto firstExpressionResult = Interpret(firstExpression);
        auto secondExpressionResult = Interpret(secondExpression);

        if (operation == BinaryOperation::Add) {
            result = firstExpressionResult + secondExpressionResult;
        } else if (operation == BinaryOperation::Sub) {
            result = firstExpressionResult - secondExpressionResult;
        } else if (operation == BinaryOperation::Mul) {
            result = firstExpressionResult * secondExpressionResult;
        } else if (operation == BinaryOperation::Div) {
            result = firstExpressionResult / secondExpressionResult;
        }
    }

    return result;
}

enum class TokenType {
    Number,
    Plus,
    Minus,
    Star,
    Slash
};

class Token {
public:

    GSTD_CONSTEXPR Token(TokenType type,
                         gstd::Optional<std::string_view> value) GSTD_NOEXCEPT
            : _type(type),
              _value(std::move(value)) {}

public:

    static GSTD_CONSTEXPR auto New(TokenType type,
                                   gstd::Optional<std::string_view> value) -> Token {
        return Token(type,
                     std::move(value));
    }

    static GSTD_CONSTEXPR auto New(TokenType type,
                                   std::string_view value) -> Token {
        return Token::New(type,
                          gstd::MakeSome(std::move(value)));
    }

    static GSTD_CONSTEXPR auto New(TokenType type) -> Token {
        return Token::New(type,
                          gstd::MakeNone());
    }

public:

    GSTD_CONSTEXPR auto GetType() const GSTD_NOEXCEPT -> TokenType {
        return _type;
    }

    GSTD_CONSTEXPR auto GetValue() const GSTD_NOEXCEPT -> gstd::Optional<std::string_view> {
        return _value;
    }

public:

    TokenType _type;

    gstd::Optional<std::string_view> _value;
};

void y() {
    static constexpr auto valExpr1 = ValueExpression::New(10);
    constexpr auto *expr1 = (Expression *) &valExpr1;
    static constexpr auto valExpr2 = ValueExpression::New(1);
    constexpr auto *expr2 = (Expression *) &valExpr2;
    static constexpr auto binExpr = BinaryExpression::New(expr1, expr2, BinaryOperation::Div);
    constexpr auto *expr3 = (Expression *) &binExpr;
    constexpr auto result = Interpret(expr3);
    static_assert(result == 10);
}

class Context {

};

class Resource {

};

class VoidResource : public Resource {

};

enum class ActionError {
    Invoke
};

class ActionInformation {
public:


};

class ActionContext {
public:


};

class Action {
public:

    using ActionFunctionPtr = gstd::Result<Resource,
                                           ActionError> (*) (Context &context,
                                                             const Resource &resource);

public:

    static auto New(ActionFunctionPtr function) -> Action;

public:

    virtual auto RunOn(Context &context,
                       const Resource &resource) -> gstd::Result<Resource,
                                                                 ActionError>;
};

class PipelineError {

};

class Pipeline {
public:

    static auto New(std::vector<Action> actions) -> std::unique_ptr<Pipeline>;

public:

    auto RunOn(Context &context,
               const Resource &resource) -> gstd::Result<Resource,
                                                         PipelineError>;
};

class PipelineBuilder {
public:

    static auto New() -> PipelineBuilder;

public:

    auto AddAction(const Action &action) -> PipelineBuilder &;

    auto AddActionAfter();

    auto Build() -> std::unique_ptr<Pipeline>;
};

class PipelineId {
public:

    explicit PipelineId(std::uint64_t id);

public:

    static auto New() -> PipelineId;
};

class PipelinesBuffer {
public:

    static auto New() -> PipelinesBuffer;

public:

    auto Append(std::unique_ptr<Pipeline> pipeline) -> PipelineId;

    auto Get(const PipelineId &id) -> Pipeline &;

    auto Get(const PipelineId &id) const -> const Pipeline &;

    auto Remove(const PipelineId &id) -> void;
};

class PipelinesExecutionEngine {
public:

    static auto New(Context &context) -> PipelinesExecutionEngine;

public:

    gstd::Result<Resource, ActionError> RunOn(const PipelineId &id);

    template<typename ResourceT>
    auto RunOn(const PipelineId &id) -> gstd::Result<ResourceT, PipelineError>;
};

class PipelinesSystem : public PipelinesBuffer,
                        public PipelinesExecutionEngine {
public:

    static auto New() -> PipelinesSystem;


};

class Token {
public:

    std::string Debug() const;
};

enum class LexerError {

};

gstd::Result<std::vector<Token>, LexerError> Tokenize(std::string_view source);

class SourceBuffer : public Resource {
public:

    std::string Source() const;
};

class TokensBuffer : public Resource {
public:

    static auto New(const std::vector<Token> &tokens) -> TokensBuffer;

public:

    std::vector<Token>::const_iterator begin() const;

    std::vector<Token>::const_iterator end() const;
};

class TokenizeAction : public Action {
public:

    static auto New() -> TokenizeAction;

public:

    auto RunOn(Context &context, const Resource &resource) -> gstd::Result<Resource, ActionError> override {
        auto sourceBuffer = gstd::As<const SourceBuffer &>(resource);

        auto source = sourceBuffer.Source();

        return Tokenize(source)
                .Map([] (auto &&value) -> Resource {
                    return TokensBuffer::New(value);
                })
                .MapErr([] (auto &&error) -> ActionError {
                    return ActionError::Invoke;
                });
    }
};

class QueryResult {
public:


};

class Query {
public:

    virtual QueryResult Run() = 0;
};

class QueriesBuffer {
public:

    static auto New() -> QueriesBuffer;

public:

    template<typename QueryT>
    auto Register(QueryT &&query);

    template<typename QueryT>
    auto Get() -> QueryT &;

    template<typename QueryT>
    auto Get() const -> const QueryT &;

    template<typename QueryT>
    auto Remove() -> void;
};

class QueriesExecutionEngine {
public:


};

class QueriesCache {
public:


};

class QueriesSystem {
public:

    QueriesBuffer _buffer;

    QueriesExecutionEngine _engine;

    QueriesCache _cache;
};

class PrintQuery : public Query {
public:

    auto Run() -> void override {
        std::cout << "Print!";
    }
};

int main() {
    // <compile command> text -> QuerySystem -> QueriesBuffer -> QueriesEE -> Query -> QueryCache -> <result>

    auto context = Context {};
    auto buffer = PipelinesBuffer::New();
    auto engine = PipelinesExecutionEngine::New(context);

    auto pipeline = PipelineBuilder::New()
                        .AddAction(TokenizeAction::New())
                        .AddAction(Action::New([] (Context &context, const Resource &resource) -> gstd::Result<Resource, ActionError> {
                            auto tokensBuffer = gstd::As<const TokensBuffer &>(resource);

                            for (auto &token : tokensBuffer) {
                                std::cout << token.Debug() << std::endl;
                            }

                            return gstd::MakeOk<Resource>(std::move(tokensBuffer));
                        }))
                        .Build();
    auto pipelineId = buffer.Append(std::move(pipeline));

    // ...

    auto resource = engine.RunOn<TokensBuffer>(pipelineId).Unwrap();

    return 0;
}
