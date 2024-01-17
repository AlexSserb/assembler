#include "command.h"
#include "processor.h"

// Loading an address into the address register
void LoadCm::operator()(Word word, Processor& proc) const noexcept
{
    proc.address_regs[word.cmd2ops.reg] = word.cmd2ops.adrs;
}

// Print the signed integer value pointed to by the address register
void PrintCm::operator()(Word word, Processor& proc) const noexcept
{
    word = proc.memory.get_word(proc.address_regs[word.cmd3ops.regs[2]]);
    std::cout << word.ival << std::endl;
}

// Outputting the unsigned integer value pointed to by the address register
void PrintUCm::operator()(Word word, Processor& proc) const noexcept
{
    word = proc.memory.get_word(proc.address_regs[word.cmd3ops.regs[2]]);
    std::cout <<  word.uval << std::endl;
}

// Printing the fractional value pointed to by the address register
void PrintFCm::operator()(Word word, Processor& proc) const noexcept
{
    word = proc.memory.get_word(proc.address_regs[word.cmd3ops.regs[2]]);
    std::cout << word.fval << std::endl;
}

// Get value from processor register
Word Command::get_reg_val(uint8_t reg, Processor& proc) const noexcept
{
    uint16_t adrs = proc.address_regs[reg];
    return proc.memory.get_word(adrs);
}

// Set a value for a processor register
void Command::set_reg_val(uint8_t reg, Word word, Processor& proc) const noexcept
{
    uint16_t adrs = proc.address_regs[reg];
    proc.memory.set_word(adrs, word);
}

// Setting flags
void Command::set_flags_int(Word word, Processor& proc) const noexcept
{
    proc.set_flag(0, word.ival == 0); // Equal to zero flag
    proc.set_flag(1, abs(word.ival) % 2 == 0); // Parity flag
    proc.set_flag(8, word.ival < 0); // Sign flag (1 if number is negative)
}

void Command::set_flags_float(Word word, Processor& proc) const noexcept
{
    proc.set_flag(0, word.fval == 0); // Equal to zero flag
    proc.set_flag(8, word.fval < 0); // Sign flag (1 if number is negative)
}

// Addition operations with setting flags (for subtraction, pass the is_sub=True argument)
Word ArithCm::add_int_check_overflow(uint8_t reg1, uint8_t reg2, Processor& proc, bool is_sub) const noexcept
{
    Word word1 = get_reg_val(reg1, proc);
    Word word2 = get_reg_val(reg2, proc);
    if (is_sub) word2.ival = -word2.ival;
    Word sum_result = Word();
    sum_result.uval = word1.uval + word2.uval;

    long long_res = (long)word1.ival + (long)word2.ival;
    proc.set_flag(9, long_res != sum_result.ival); // Signed integer overflow flag
    proc.set_flag(10, long_res != sum_result.uval); // Carry flag (unsigned integer overflow)

    set_flags_int(sum_result, proc);
    return sum_result;
}

Word ArithCm::add_float_check_overflow(uint8_t reg1, uint8_t reg2, Processor& proc, bool is_sub) const noexcept
{
    Word word1 = get_reg_val(reg1, proc);
    Word word2 = get_reg_val(reg2, proc);
    if (is_sub) word2.fval = -word2.fval;
    Word sum_result = Word();
    sum_result.fval = word1.fval + word2.fval;

    double double_res = (double)word1.fval + (double)word2.fval;
    proc.set_flag(11, double_res != sum_result.fval); // Fractional overflow flag

    set_flags_float(sum_result, proc);
    return sum_result;
}

// Addition of integers
void AddCm::operator()(Word word, Processor& proc) const noexcept
{
    Word sum_result = add_int_check_overflow(word.cmd3ops.regs[1], word.cmd3ops.regs[2], proc);
    set_reg_val(word.cmd3ops.regs[0], sum_result, proc);
}

// Adding Fractions
void AddFCm::operator()(Word word, Processor& proc) const noexcept
{
    Word sum_result = add_float_check_overflow(word.cmd3ops.regs[1], word.cmd3ops.regs[2], proc);
    set_reg_val(word.cmd3ops.regs[0], sum_result, proc);
}

// Subtracting Integers
void SubCm::operator()(Word word, Processor& proc) const noexcept
{
    Word sub_result = add_int_check_overflow(word.cmd3ops.regs[1], word.cmd3ops.regs[2], proc, true);
    set_reg_val(word.cmd3ops.regs[0], sub_result, proc);
}

// Subtracting fractions
void SubFCm::operator()(Word word, Processor& proc) const noexcept
{
    Word sub_result = add_float_check_overflow(word.cmd3ops.regs[1], word.cmd3ops.regs[2], proc, true);
    set_reg_val(word.cmd3ops.regs[0], sub_result, proc);
}

// Multiplying with flags
Word ArithCm::mul_int_check_overflow(uint8_t reg1, uint8_t reg2, Processor& proc) const noexcept
{
    Word word1 = get_reg_val(reg1, proc);
    Word word2 = get_reg_val(reg2, proc);
    Word result = Word();
    result.ival = word1.ival * word2.ival;

    long long_res = (long)word1.ival * (long)word2.ival;
    proc.set_flag(9, long_res != result.ival); // Sign integer overflow flag
    proc.set_flag(10, long_res != result.uval); // Carry flag (unsigned integer overflow)

    set_flags_int(result, proc);
    return result;
}

Word ArithCm::mul_float_check_overflow(uint8_t reg1, uint8_t reg2, Processor& proc) const noexcept
{
    Word word1 = get_reg_val(reg1, proc);
    Word word2 = get_reg_val(reg2, proc);
    Word result = Word();
    result.fval = word1.fval * word2.fval;

    double double_res = (double)word1.fval * (double)word2.fval;
    proc.set_flag(11, double_res != result.fval); // Fractional overflow flag

    set_flags_float(result, proc);
    return result;
}

// Multiplying Integers
void MulCm::operator()(Word word, Processor& proc) const noexcept
{
    Word result = mul_int_check_overflow(word.cmd3ops.regs[1], word.cmd3ops.regs[2], proc);
    set_reg_val(word.cmd3ops.regs[0], result, proc);
}

// Multiplying fractions
void MulFCm::operator()(Word word, Processor& proc) const noexcept
{
    Word result = mul_float_check_overflow(word.cmd3ops.regs[1], word.cmd3ops.regs[2], proc);
    set_reg_val(word.cmd3ops.regs[0], result, proc);
};

// Division of unsigned integers
void DivUCm::operator()(Word word, Processor& proc) const noexcept
{
    Word res = Word();
    proc.set_flag(12, get_reg_val(word.cmd3ops.regs[2], proc).uval == 0); // Flag indicating division by zero
    res.uval = get_reg_val(word.cmd3ops.regs[1], proc).uval / get_reg_val(word.cmd3ops.regs[2], proc).uval;
    set_flags_int(res, proc);
    set_reg_val(word.cmd3ops.regs[0], res, proc);
}

// Divide signed integers
void DivCm::operator()(Word word, Processor& proc) const noexcept
{
    Word res = Word();
    proc.set_flag(12, get_reg_val(word.cmd3ops.regs[2], proc).ival == 0); // Flag indicating division by zero
    res.ival = get_reg_val(word.cmd3ops.regs[1], proc).ival / get_reg_val(word.cmd3ops.regs[2], proc).ival;
    set_flags_int(res, proc);
    set_reg_val(word.cmd3ops.regs[0], res, proc);
}

// Division of fractional numbers
void DivFCm::operator()(Word word, Processor& proc) const noexcept
{
    Word res = Word();
    proc.set_flag(12, get_reg_val(word.cmd3ops.regs[2], proc).fval == 0); // Flag indicating division by zero
    res.fval = get_reg_val(word.cmd3ops.regs[1], proc).fval / get_reg_val(word.cmd3ops.regs[2], proc).fval;
    set_flags_float(res, proc);
    set_reg_val(word.cmd3ops.regs[0], res, proc);
}

// Taking the remainder of division of unsigned integers
void ModUCm::operator()(Word word, Processor& proc) const noexcept
{
    Word res = Word();
    proc.set_flag(12, get_reg_val(word.cmd3ops.regs[2], proc).uval == 0); // Flag indicating division by zero
    res.uval = get_reg_val(word.cmd3ops.regs[1], proc).uval % get_reg_val(word.cmd3ops.regs[2], proc).uval;
    set_flags_int(res, proc);
    set_reg_val(word.cmd3ops.regs[0], res, proc);
}

// Taking the remainder of division of signed integers
void ModCm::operator()(Word word, Processor& proc) const noexcept
{
    Word res = Word();
    proc.set_flag(12, get_reg_val(word.cmd3ops.regs[2], proc).ival == 0); // Flag indicating division by zero
    res.ival = get_reg_val(word.cmd3ops.regs[1], proc).ival % get_reg_val(word.cmd3ops.regs[2], proc).ival;
    set_flags_int(res, proc);
    set_reg_val(word.cmd3ops.regs[0], res, proc);
}

// Sign conversion
void NegCm::operator()(Word word, Processor& proc) const noexcept
{
    Word res = Word();
    res.ival = -get_reg_val(word.cmd3ops.regs[2], proc).ival;
    set_flags_int(res, proc);
    set_reg_val(word.cmd3ops.regs[2], res, proc);
}

// Changing the sign of fractions
void NegFCm::operator()(Word word, Processor& proc) const noexcept
{
    Word res = Word();
    res.fval = -get_reg_val(word.cmd3ops.regs[2], proc).fval;
    set_flags_float(res, proc);
    set_reg_val(word.cmd3ops.regs[2], res, proc);
}

// Increment and decrement with setting flags
Word ArithCm::inc_check_overflow(Word word, Processor& proc) const noexcept
{
    Word sum_result = Word();
    sum_result.uval = word.uval + 1;
    proc.set_flag(9, sum_result.ival < word.ival); // Signed integer overflow flag
    proc.set_flag(10, sum_result.uval < word.uval); // Carry flag (unsigned integer overflow)
    return sum_result;
}

Word ArithCm::dec_check_overflow(Word word, Processor& proc) const noexcept
{
    Word sub_result = Word();
    sub_result.uval = word.uval - 1;
    proc.set_flag(9, sub_result.ival > word.ival); // Signed integer overflow flag
    proc.set_flag(10, sub_result.uval > word.uval); // Carry flag (unsigned integer overflow)
    return sub_result;
}

// Increment
void IncCm::operator()(Word word, Processor& proc) const noexcept
{
    Word sum_result = inc_check_overflow(get_reg_val(word.cmd3ops.regs[2], proc), proc);
    set_reg_val(word.cmd3ops.regs[2], sum_result, proc);
}

// Decrement
void DecCm::operator()(Word word, Processor& proc) const noexcept
{
    Word sum_result = dec_check_overflow(get_reg_val(word.cmd3ops.regs[2], proc), proc);
    set_reg_val(word.cmd3ops.regs[2], sum_result, proc);
}

// Comparison of signed integers
void CmpCm::operator()(Word word, Processor& proc) const noexcept
{
    Word val1 = get_reg_val(word.cmd3ops.regs[0], proc);
    Word val2 = get_reg_val(word.cmd3ops.regs[1], proc);
    proc.set_flag(2, val1.ival == val2.ival); // Set the flag at index 2 if there is equality
    proc.set_flag(3, val1.ival > val2.ival); // Set the flag at index 3 if val1 > val2
}

// Comparison of unsigned integers
void CmpUCm::operator()(Word word, Processor& proc) const noexcept
{
    Word val1 = get_reg_val(word.cmd3ops.regs[0], proc);
    Word val2 = get_reg_val(word.cmd3ops.regs[1], proc);
    proc.set_flag(4, val1.uval == val2.uval); // Set the flag at index 4 if there is equality
    proc.set_flag(5, val1.uval > val2.uval); // Set the flag at index 5 if val1 > val2
}

// Comparison of fractional numbers
void CmpFCm::operator()(Word word, Processor& proc) const noexcept
{
    Word val1 = get_reg_val(word.cmd3ops.regs[0], proc);
    Word val2 = get_reg_val(word.cmd3ops.regs[1], proc);
    proc.set_flag(6, val1.fval == val2.fval); // Set the flag at index 6 if there is equality
    proc.set_flag(7, val1.fval > val2.fval); // Set the flag at index 7 if val1 > val2
}

// Searching for a new IP to transition to. Same for all jump commands.
uint16_t TransCm::calc_instraction_pointer(Word word, Processor& proc) const noexcept
{
    uint8_t code = word.cmd3ops.regs[0];

    if (code == 0)  // Direct jump, IP = address constant in command
        return word.cmd2ops.adrs;

    if (code == 1) // Direct indirect jump, IP = the value in memory that lies at the address.
                   // Address is a constant in the command
        return proc.memory.get_word(word.cmd2ops.adrs).uval;

    if (code == 2) // Direct indirect register jump, IP = address in register 1 + address in register 2
    {
        uint16_t adrs1 = proc.address_regs[word.cmd3ops.regs[2]];
        uint16_t adrs2 = proc.address_regs[word.cmd3ops.regs[1]];
        return adrs1 + adrs2;
    }
    // (code == 3)   Relative transition, IP = IP + offset. Offset is a constant in the command
    return proc.get_ip() + word.cmd2ops.adrs;
}

// Unconditional jump command
void JumpCm::operator()(Word word, Processor& proc) const noexcept
{
    proc.set_ip(calc_instraction_pointer(word, proc));
}

// Signed conditional jump command val1 == val2
void JEqCm::operator()(Word word, Processor& proc) const noexcept
{
    if (proc.get_flag(2)) // flag check for signed numbers
        proc.set_ip(calc_instraction_pointer(word, proc));
    else
        proc.set_ip(proc.get_ip() + 2);
}

// Conditional unsigned jump command val1 == val2
void JEqUCm::operator()(Word word, Processor& proc) const noexcept
{
    if (proc.get_flag(4)) // check the flag for unsigned
        proc.set_ip(calc_instraction_pointer(word, proc));
    else
        proc.set_ip(proc.get_ip() + 2);
}

// Conditional jump command for fractional val1 == val2
void JEqFCm::operator()(Word word, Processor& proc) const noexcept
{
    if (proc.get_flag(6)) // checking the flag for fractional
        proc.set_ip(calc_instraction_pointer(word, proc));
    else
        proc.set_ip(proc.get_ip() + 2);
}

// Signed conditional jump command val1 > val2
void JGrCm::operator()(Word word, Processor& proc) const noexcept
{
    if (proc.get_flag(3)) // check the flag for signed numbers
        proc.set_ip(calc_instraction_pointer(word, proc));
    else
        proc.set_ip(proc.get_ip() + 2);
}

// Conditional unsigned jump command val1 > val2
void JGrUCm::operator()(Word word, Processor& proc) const noexcept
{
    if (proc.get_flag(5)) // check the flag for unsigned
        proc.set_ip(calc_instraction_pointer(word, proc));
    else
        proc.set_ip(proc.get_ip() + 2);
}

// Command for conditional transition of fractional val1 > val2
void JGrFCm::operator()(Word word, Processor& proc) const noexcept
{
    if (!proc.get_flag(6) && proc.get_flag(7)) // check the flag for fractional
        proc.set_ip(calc_instraction_pointer(word, proc));
    else
        proc.set_ip(proc.get_ip() + 2);
}

// Signed conditional jump command val1 < val2
void JLsCm::operator()(Word word, Processor& proc) const noexcept
{
    if (!proc.get_flag(2) && !proc.get_flag(3)) // check the flag for signed
        proc.set_ip(calc_instraction_pointer(word, proc));
    else
        proc.set_ip(proc.get_ip() + 2);
}

// Conditional unsigned jump command val1 < val2
void JLsUCm::operator()(Word word, Processor& proc) const noexcept
{
    if (!proc.get_flag(4) && !proc.get_flag(5)) // check the flag for unsigned
        proc.set_ip(calc_instraction_pointer(word, proc));
    else
        proc.set_ip(proc.get_ip() + 2);
}

// Command for conditional transition of fractional val1 < val2
void JLsFCm::operator()(Word word, Processor& proc) const noexcept
{
    if (!proc.get_flag(6) && !proc.get_flag(7)) // check the flag for fractional
        proc.set_ip(calc_instraction_pointer(word, proc));
    else
        proc.set_ip(proc.get_ip() + 2);
}

// Signed conditional jump command val1 != val2
void JNEqCm::operator()(Word word, Processor& proc) const noexcept
{
    if (!proc.get_flag(2)) // checking the flag for signed ones
        proc.set_ip(calc_instraction_pointer(word, proc));
    else
        proc.set_ip(proc.get_ip() + 2);
}

// Conditional jump command for unsigned val1 != val2
void JNEqUCm::operator()(Word word, Processor& proc) const noexcept
{
    if (!proc.get_flag(4)) // check the flag for unsigned
        proc.set_ip(calc_instraction_pointer(word, proc));
    else
        proc.set_ip(proc.get_ip() + 2);
}

// Conditional jump command for fractional val1 != val2
void JNEqFCm::operator()(Word word, Processor& proc) const noexcept
{
    if (!proc.get_flag(6)) // checking the flag for fractional
        proc.set_ip(calc_instraction_pointer(word, proc));
    else
        proc.set_ip(proc.get_ip() + 2);
}

// Signed conditional jump command val1 >= val2
void JGEqCm::operator()(Word word, Processor& proc) const noexcept
{
    if (proc.get_flag(3) || proc.get_flag(2)) // check the flag for signed
        proc.set_ip(calc_instraction_pointer(word, proc));
    else
        proc.set_ip(proc.get_ip() + 2);
}

// Conditional unsigned jump command val1 >= val2
void JGEqUCm::operator()(Word word, Processor& proc) const noexcept
{
    if (proc.get_flag(5) || proc.get_flag(4)) // check the flag for unsigned
        proc.set_ip(calc_instraction_pointer(word, proc));
    else
        proc.set_ip(proc.get_ip() + 2);
}

// Conditional transition command for fractional val1 >= val2
void JGEqFCm::operator()(Word word, Processor& proc) const noexcept
{
    if (proc.get_flag(6) || proc.get_flag(7)) // check the flag for fractional
        proc.set_ip(calc_instraction_pointer(word, proc));
    else
        proc.set_ip(proc.get_ip() + 2);
}

// Signed conditional jump command val1 <= val2
void JLEqCm::operator()(Word word, Processor& proc) const noexcept
{
    if (!proc.get_flag(3)) // check the flag for signed ones
        proc.set_ip(calc_instraction_pointer(word, proc));
    else
        proc.set_ip(proc.get_ip() + 2);
}

// Conditional unsigned jump command val1 <= val2
void JLEqUCm::operator()(Word word, Processor& proc) const noexcept
{
    if (!proc.get_flag(5)) // check the flag for unsigned
        proc.set_ip(calc_instraction_pointer(word, proc));
    else
        proc.set_ip(proc.get_ip() + 2);
}

// Conditional transition command for fractional val1 <= val2
void JLEqFCm::operator()(Word word, Processor& proc) const noexcept
{
    if (!proc.get_flag(7)) // checking the flag for fractional
        proc.set_ip(calc_instraction_pointer(word, proc));
    else
        proc.set_ip(proc.get_ip() + 2);
}


// Command to read a signed integer from the console
void ReadCm::operator()(Word word, Processor& proc) const noexcept
{
    Word user_val = Word();
    std::cin >> user_val.ival;
    set_reg_val(word.cmd3ops.regs[2], user_val, proc);
}

// Command to read an unsigned integer from the console
void ReadUCm::operator()(Word word, Processor& proc) const noexcept
{
    Word user_val = Word();
    std::cin >> user_val.uval;
    set_reg_val(word.cmd3ops.regs[2], user_val, proc);
}

// Command to read a fractional from the console
void ReadFCm::operator()(Word word, Processor& proc) const noexcept
{
    Word user_val = Word();
    std::cin >> user_val.fval;
    set_reg_val(word.cmd3ops.regs[2], user_val, proc);
}

// Bitwise AND command
void AndCm::operator()(Word word, Processor& proc) const noexcept
{
    Word res = Word();
    res.uval = get_reg_val(word.cmd3ops.regs[1], proc).uval & get_reg_val(word.cmd3ops.regs[2], proc).uval;
    set_reg_val(word.cmd3ops.regs[0], res, proc);
    set_flags_int(res, proc);
}

// Bitwise OR instruction
void OrCm::operator()(Word word, Processor& proc) const noexcept
{
    Word res = Word();
    res.uval = get_reg_val(word.cmd3ops.regs[1], proc).uval | get_reg_val(word.cmd3ops.regs[2], proc).uval;
    set_reg_val(word.cmd3ops.regs[0], res, proc);
    set_flags_int(res, proc);
}

// Bitwise Exclusive OR Instruction
void XorCm::operator()(Word word, Processor& proc) const noexcept
{
    Word res = Word();
    res.uval = get_reg_val(word.cmd3ops.regs[1], proc).uval ^ get_reg_val(word.cmd3ops.regs[2], proc).uval;
    set_reg_val(word.cmd3ops.regs[0], res, proc);
    set_flags_int(res, proc);
}

// Bitwise NOT instruction
void NotCm::operator()(Word word, Processor& proc) const noexcept
{
    Word res = Word();
    res.uval = ~get_reg_val(word.cmd3ops.regs[2], proc).uval;
    set_reg_val(word.cmd3ops.regs[0], res, proc);
    set_flags_int(res, proc);
}

// Command to load address from register 2 directly into register 1
void LoadRCm::operator()(Word word, Processor& proc) const noexcept
{
    proc.address_regs[word.cmd3ops.regs[0]] = proc.address_regs[word.cmd3ops.regs[1]];
}

// The instruction to load the value pointed to by register 2 to where register 1 points
void LoadRVCm::operator()(Word word, Processor& proc) const noexcept
{
    set_reg_val(word.cmd3ops.regs[0], get_reg_val(word.cmd3ops.regs[1], proc), proc);
}

// The instruction to load a flag into the value pointed to by a register
void LoadF::operator()(Word word, Processor& proc) const noexcept
{
    Word val = Word();
    val.uval = int(proc.get_flag(word.cmd3ops.regs[1]));
    set_reg_val(word.cmd3ops.regs[0], val, proc);
}

// Command to load a flag from a register
void SetF::operator()(Word word, Processor& proc) const noexcept
{
    uint8_t flag = word.cmd3ops.regs[0];
    uint8_t reg_from = word.cmd3ops.regs[1];

    Word val = Word();
    val = proc.memory.get_word(proc.address_regs[reg_from]);
    proc.set_flag(flag, val.uval != 0);
}

// Calling a subroutine
void CallCm::operator()(Word word, Processor& proc) const noexcept
{
    uint16_t return_to = proc.get_ip();
    proc.push(return_to + 2); // Storing the return address onto a register-mimicking stack
    proc.set_ip(word.cmd2ops.adrs - 2);
}

// Return from subroutine
void EndpCm::operator()(Word word, Processor& proc) const noexcept
{
    proc.set_ip(proc.pop() - 2);
}

