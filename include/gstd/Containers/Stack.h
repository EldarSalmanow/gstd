#ifndef GSTD_STACK_H
#define GSTD_STACK_H

#include <gstd/Utils/Match.h>

namespace gstd {

    inline constexpr std::uint64_t DynamicSize = 0xFFFFFFFFFFFFFFFF;

    namespace detail {

        template<typename ValueT,
                 std::uint64_t MaxSizeV>
        class StaticStackStorage {
        public:

            using ValueType = ValueT;

            using SizeType = std::uint64_t;

            inline static constexpr SizeType MaxSizeValue = MaxSizeV;

        public:

            GSTD_CONSTEXPR StaticStackStorage() GSTD_NOEXCEPT
                    : _buffer(),
                      _cursorIndex(0) {}

        public:

            GSTD_CONSTEXPR auto Push(ValueType &&value) -> void {
                ++_cursorIndex;

                _buffer[_cursorIndex] = std::move(value);
            }

            GSTD_CONSTEXPR auto Top() const -> const ValueType & {
                return _buffer[_cursorIndex];
            }

            GSTD_CONSTEXPR auto Pop() -> ValueType {
                auto &&value = std::move(_buffer[_cursorIndex]);

                --_cursorIndex;

                return std::move(value);
            }

            GSTD_CONSTEXPR auto Size() const GSTD_NOEXCEPT {
                return _cursorIndex;
            }

        private:

            std::array<ValueT,
                       MaxSizeValue> _buffer;

            std::uint64_t _cursorIndex;
        };

        template<typename ValueT>
        class DynamicStackStorage {
        public:


        };

        template<typename ValueT,
                 std::uint64_t MaxSizeV>
        using StackStorage = std::conditional_t<MaxSizeV == DynamicSize,
                                                DynamicStackStorage<ValueT>,
                                                StaticStackStorage<ValueT,
                                                                   MaxSizeV>>;

    }

    template<typename ValueT,
             std::uint64_t MaxSizeV = DynamicSize>
    class Stack : private detail::StackStorage<ValueT,
                                               MaxSizeV> {
    public:

        using ValueType = ValueT;

        using SizeType = std::uint64_t;

        inline static constexpr SizeType MaxSizeValue = MaxSizeV;

    public:

        using Storage = detail::StackStorage<ValueType,
                                             MaxSizeValue>;

    public:

        GSTD_CONSTEXPR auto Push(ValueType &&value) -> void {
            if (Size() >= MaxSizeV) {
                return;
            }

            Storage::Push(std::forward<ValueType>(value));
        }

        GSTD_CONSTEXPR auto Top() const -> Optional<Ref<const ValueType>> {
            if (Size() <= 0) {
                return MakeNone();
            }

            return MakeSome(MakeRef(Storage::Top()));
        }

        GSTD_CONSTEXPR auto Pop() -> Optional<ValueType> {
            if (Size() <= 0) {
                return MakeNone();
            }

            return MakeSome(Storage::Pop());
        }

        GSTD_CONSTEXPR auto Size() const GSTD_NOEXCEPT -> SizeType {
            return Storage::Size();
        }
    };

    enum class InstructionType {
        Push,
        Pop,
        Add
    };

    class Instruction {
    public:

        constexpr virtual ~Instruction() = default;

        constexpr virtual auto GetType() const noexcept -> InstructionType = 0;
    };

    class PushInstruction : public Instruction {
    public:

        constexpr explicit PushInstruction(std::int64_t value) noexcept
                : _value(value) {}

    public:

        constexpr auto GetValue() const noexcept -> std::int64_t {
            return _value;
        }

    public:

        constexpr auto GetType() const noexcept -> InstructionType override {
            return InstructionType::Push;
        }

    private:

        std::int64_t _value;
    };

    class PopInstruction : public Instruction {
    public:

        constexpr PopInstruction() noexcept = default;

    public:

        constexpr auto GetType() const noexcept -> InstructionType override {
            return InstructionType::Pop;
        }
    };

    class AddInstruction : public Instruction {
    public:

        constexpr AddInstruction() noexcept = default;

    public:

        constexpr auto GetType() const noexcept -> InstructionType override {
            return InstructionType::Add;
        }
    };

    template<std::uint64_t SizeV>
    constexpr auto Interpret(std::array<Instruction *, SizeV> instructions) {
        Stack<std::int64_t, 16> stack;

        for (auto &instruction : instructions) {
            auto instructionType = instruction->GetType();

            if (instructionType == InstructionType::Push) {
                auto pushInstruction = static_cast<PushInstruction *>(instruction);

                stack.Push(pushInstruction->GetValue());
            } else if (instructionType == InstructionType::Pop) {
                auto popInstruction = static_cast<PopInstruction *>(instruction);

                stack.Pop().Unwrap();
            } else if (instructionType == InstructionType::Add) {
                auto addInstruction = static_cast<AddInstruction *>(instruction);

                auto secondValue = stack.Pop().Unwrap();
                auto firstValue = stack.Pop().Unwrap();

                stack.Push(firstValue + secondValue);
            }
        }

        auto result = stack.Top().Unwrap().Get();

        return result;
    }

    constexpr std::int64_t Calc() {
        std::array<Instruction *, 5> instructions = {
                new PushInstruction(10),
                new PushInstruction(20),
                new AddInstruction(),
                new PushInstruction(30),
                new AddInstruction()
        }; // 10 + 20 + 30

        auto result = Interpret(instructions);

        for (auto &instruction : instructions) {
            delete instruction;
        }

        return result;
    }

    void y() {
        static_assert(Calc() == 60);
    }

}

#endif //GSTD_STACK_H
