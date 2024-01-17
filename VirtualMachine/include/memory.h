#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"
#include <iostream>
#include <bitset>

// According to the laboratory work assignment option:
// Word - 32 bit
// Memory cell size - 16 bits
class Memory final
{
public:
    static constexpr uint32_t MEM_SIZE = 32768;

    Memory();
    ~Memory();

    void clear();

    // Setting a word in memory by address
    void set_word(uint16_t address, Word word);
    void set_word(uint16_t address, uint16_t word_part1, uint16_t word_part2);

    // Getting a word in memory by address
    Word get_word(uint16_t address) const noexcept;

    // Displaying the values ​​of memory cells
    void print_memory(uint16_t first, uint16_t last) const noexcept;

private:
    uint16_t* memory;
};

#endif // MEMORY_H
