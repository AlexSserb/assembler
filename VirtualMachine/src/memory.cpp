#include "memory.h"

Memory::Memory()
{
    memory = new uint16_t[MEM_SIZE]();
}

Memory::~Memory()
{
    delete[] memory;
}

void Memory::clear()
{
    memory = new uint16_t[MEM_SIZE]();
}

void Memory::set_word(uint16_t address, Word word)
{
    memory[address] = word.cells[0];
    memory[address + 1] = word.cells[1];
}

void Memory::set_word(uint16_t address, uint16_t word_part1, uint16_t word_part2)
{
    memory[address] = word_part1;
    memory[address + 1] = word_part2;
}

Word Memory::get_word(uint16_t address) const noexcept
{
    Word word = Word();
    word.cells[1] = memory[address + 1];
    word.cells[0] = memory[address];
    return word;
}

void Memory::print_memory(uint16_t first, uint16_t last) const noexcept
{
    std::cout << "MEMORY:\n";
    while (first <= last)
    {
        std::cout << "Cell " << first << " = " << (memory[first] >> 8) << " - " <<  (int)(uint8_t)memory[first] << '\n';
        first++;
    }
}
