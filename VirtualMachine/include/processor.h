#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "command.h"
#include "memory.h"

class Processor final
{
public:
    static constexpr int ADDRESS_REGS = 256;
    static constexpr int AMOUNT_COMMANDS = 55;
    static constexpr int START_STACK = 240; // Register from which the stack simulation starts

    Memory memory = Memory();  // Memory class
    uint16_t address_regs[ADDRESS_REGS]; //Address registers
    uint16_t flags; // Status Flags

    Processor();

    // Resetting values ​​in memory and registers
    void reset() noexcept;

    // Starting the processor
    void run(uint16_t start_address);

    void set_flag(uint8_t flag_index, bool is_true) noexcept;
    bool get_flag(uint8_t flag_index) const noexcept;

    uint16_t get_ip() const noexcept;
    void set_ip(uint16_t instruction_pointer) noexcept;

    void push(uint16_t adrs) noexcept; // Loading an address onto the stack
    uint16_t pop() noexcept; // Unloading an address from the stack

private:
    uint16_t ip; // Instruction Pointer
    uint8_t sp; // Pointer to the top of the stack

    // Array of pointers to processor instructions
    Command* commands[AMOUNT_COMMANDS] = { nullptr, new JumpCm(), new JEqCm(), new JEqUCm(), new JEqFCm(),
        new JGrCm(), new JGrUCm(), new JGrFCm(), new JLsCm(), new JLsUCm(), new JLsFCm(),
        new JNEqCm(), new JNEqUCm(), new JNEqFCm(), new JGEqCm(), new JGEqUCm(), new JGEqFCm(),
        new JLEqCm(), new JLEqUCm(), new JLEqFCm(), new PrintCm(), new PrintUCm(), new PrintFCm(),
        new LoadCm(), new NegCm(), new NegFCm(), new CmpCm(), new CmpUCm(), new CmpFCm(), new AddCm(),
        new AddFCm(), new SubCm(), new SubFCm(), new MulCm(), new MulFCm(), new DivUCm(), new DivCm(),
        new DivFCm(), new ModUCm(), new ModCm(), new IncCm(), new DecCm(), new ReadCm(), new ReadUCm(),
        new ReadFCm(), new AndCm(), new OrCm(), new XorCm(), new NotCm(), new LoadRCm(), new LoadRVCm(),
        new CallCm(), new LoadF(), new SetF(), new EndpCm() };
};

#endif // PROCESSOR_H
