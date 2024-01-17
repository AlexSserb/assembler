#include "processor.h"

Processor::Processor()
{
    for (size_t i = 0; i < ADDRESS_REGS; i++)
        address_regs[i] = 0;

    flags = 0;
    sp = START_STACK;
}

// Resetting values ​​in memory and registers
void Processor::reset() noexcept
{
    memory.clear();
    for (size_t i = 0; i < ADDRESS_REGS; i++)
        address_regs[i] = 0;
}

// Starting the processor
void Processor::run(uint16_t start_address)
{
    ip = start_address;
    Word word = memory.get_word(ip);
    while (word.cmd3ops.cmd != 0)
    {
        (*commands[word.cmd3ops.cmd])(word, *this); // Run CPU command

        // If processed command isnt a jump command, then increase the Instraction Pointer
        if (word.cmd3ops.cmd > 19) ip += 2;

        word = memory.get_word(ip); // Getting the command code by the Instruction Pointer
    }
}

// Setting a Flag Value
void Processor::set_flag(uint8_t flag_index, bool is_true) noexcept
{
    if (is_true) flags |= (1 << flag_index);
    else flags &= ~(1 << flag_index);
}
// Getting the value of a flag
bool Processor::get_flag(uint8_t flag_index) const noexcept
{
    int16_t mask = 1 << flag_index;
    return (flags & mask) != 0;
}

// Getting the Instruction Pointer
uint16_t Processor::get_ip() const noexcept
{
    return ip;
}

// Setting the Instruction Pointer
void Processor::set_ip(uint16_t instruction_pointer) noexcept
{
    ip = instruction_pointer;
}

// Loading an address onto the stack
void Processor::push(uint16_t adrs) noexcept
{
    address_regs[sp] = adrs;
    sp++;
    if (sp >= ADDRESS_REGS) sp = START_STACK;
}

// Unloading an address from the stack
uint16_t Processor::pop() noexcept
{
    sp--;
    if (sp < START_STACK) sp = ADDRESS_REGS - 1;
    return address_regs[sp];
}
