#ifndef COMMAND_H
#define COMMAND_H

#include <stdint.h>
#include <cmath>
#include <iostream>

class Processor;
union Word;

// Base abstract command class
class Command
{
public:
    virtual void operator()(Word word, Processor& proc) const noexcept = 0;

    // Get value from processor register
    Word get_reg_val(uint8_t reg, Processor& proc) const noexcept;
    // Set a value for a processor register
    void set_reg_val(uint8_t reg, Word word, Processor& proc) const noexcept;

    // Setting flags
    void set_flags_int(Word word, Processor& proc) const noexcept;
    void set_flags_float(Word word, Processor& proc) const noexcept;
};


// Loading the address register
class LoadCm : public Command
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};


// Printing the integer value pointed to by the address register
class PrintCm : public Command
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Print the unsigned integer value pointed to by the address register
class PrintUCm : public Command
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Printing the fractional value pointed to by the address register
class PrintFCm : public Command
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};


// Abstract class for arithmetic operations
class ArithCm : public Command
{
public:
    virtual void operator()(Word word, Processor& proc) const noexcept = 0;

    // Addition with setting flags (for subtraction, pass the argument is_sub = true)
    Word add_int_check_overflow(uint8_t reg1, uint8_t reg2, Processor& proc, bool is_sub = false) const noexcept;
    Word add_float_check_overflow(uint8_t reg1, uint8_t reg2, Processor& proc, bool is_sub = false) const noexcept;

    // Multiplying with setting flags
    Word mul_int_check_overflow(uint8_t reg1, uint8_t reg2, Processor& proc) const noexcept;
    Word mul_float_check_overflow(uint8_t reg1, uint8_t reg2, Processor& proc) const noexcept;

    // Increment and decrement with setting flags
    Word inc_check_overflow(Word word, Processor& proc) const noexcept;
    Word dec_check_overflow(Word word, Processor& proc) const noexcept;
};

// Addition of integers
class AddCm : public ArithCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Adding fractions
class AddFCm : public ArithCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Subtracting Integers
class SubCm : public ArithCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Subtracting fractions
class SubFCm : public ArithCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Multiplying Integers
class MulCm : public ArithCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Multiplying fractions
class MulFCm : public ArithCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Division of unsigned integers
class DivUCm : public ArithCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Dividing signed integers
class DivCm : public ArithCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Division of fractional numbers
class DivFCm : public ArithCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Taking the remainder when dividing signed integers
class ModCm : public ArithCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Taking the remainder of division of unsigned integers
class ModUCm : public ArithCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Sign conversion
class NegCm : public ArithCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Fractional sign conversion
class NegFCm : public ArithCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Increment
class IncCm : public ArithCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Decrement
class DecCm : public ArithCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};


// Comparison of signed integers
class CmpCm : public Command
{
    void operator()(Word word, Processor& proc) const noexcept;
};


// Comparison without signed integers
class CmpUCm : public Command
{
    void operator()(Word word, Processor& proc) const noexcept;
};

// Comparison of fractional numbers
class CmpFCm : public Command
{
    void operator()(Word word, Processor& proc) const noexcept;
};

// Abstract class for jump commands
class TransCm : public Command
{
public:
    virtual void operator()(Word word, Processor& proc) const noexcept = 0;

    // Searching for a new IP to transition to. Same for all jump commands.
    uint16_t calc_instraction_pointer(Word word, Processor& proc) const noexcept;
};

// Unconditional jump command
class JumpCm : public TransCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Signed conditional jump command val1 == val2
class JEqCm : public TransCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Conditional jump command for unsigned val1 == val2
class JEqUCm : public TransCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Conditional jump command for fractional numbers val1 == val2
class JEqFCm : public TransCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Signed conditional jump command val1 > val2
class JGrCm : public TransCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Unsigned conditional jump command val1 > val2
class JGrUCm : public TransCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Conditional fractional transition command val1 > val2
class JGrFCm : public TransCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Signed conditional jump command val1 < val2
class JLsCm : public TransCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Unsigned conditional jump instruction val1 < val2
class JLsUCm : public TransCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Conditional fractional jump command val1 < val2
class JLsFCm : public TransCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Signed conditional jump command val1 != val2
class JNEqCm : public TransCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Conditional jump command for unsigned val1 != val2
class JNEqUCm : public TransCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Conditional jump command for fractional numbers val1 != val2
class JNEqFCm : public TransCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Signed conditional jump command val1 >= val2
class JGEqCm : public TransCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Conditional jump command for unsigned val1 >= val2
class JGEqUCm : public TransCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Conditional fractional jump command val1 >= val2
class JGEqFCm : public TransCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Signed conditional jump command val1 <= val2
class JLEqCm : public TransCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Unsigned conditional jump instruction val1 <= val2
class JLEqUCm : public TransCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Conditional fractional jump command val1 <= val2
class JLEqFCm : public TransCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};


// Command to read a signed integer from the console
class ReadCm : public Command
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Command to read unsigned integer from console
class ReadUCm : public Command
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Command to read unsigned integer from console
class ReadFCm : public Command
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};


// Abstract class for bitwise operations
class BitCm : public Command
{
public:
    virtual void operator()(Word word, Processor& proc) const noexcept = 0;
};

// Bitwise AND instruction
class AndCm : public BitCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Bitwise OR instruction
class OrCm : public BitCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Bitwise Exclusive OR Instruction
class XorCm : public BitCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Bitwise NOT instruction
class NotCm : public BitCm
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Command to load address from register 2 directly into register 1
class LoadRCm : public Command
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// The instruction to load the value pointed to by register 2 there
// where register 1 points to
class LoadRVCm : public Command
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// A command to load a flag into the memory location pointed to by a register
class LoadF : public Command
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Command to load a flag from a register
class SetF : public Command
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};


// Calling a subroutine
class CallCm : public Command
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};

// Returning from a subroutine
class EndpCm : public Command
{
public:
    void operator()(Word word, Processor& proc) const noexcept;
};


#endif // COMMAND_H
