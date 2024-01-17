#ifndef TYPES_H
#define TYPES_H

#include<iostream>

// Structure for an instruction with three operands
struct Cmd3ops
{
    uint8_t cmd : 8;
    uint8_t regs[3];
};

// Structure for an instruction with two operands
struct Cmd2ops
{
    uint8_t cmd : 8;
    uint8_t reg : 8;
    uint16_t adrs : 16;
};

// Different representations of a word in memory
union Word
{
    Cmd3ops cmd3ops; // Instruction with three operands
    Cmd2ops cmd2ops; // Instruction with two operands
    uint16_t cells[2]; // Dividing a word into two memory cells
    int32_t ival; // Signed integer representation of a word
    uint32_t uval; // Unsigned integer representation of a word
    float fval; // Fractional representation of a word
};

#endif // TYPES_H
