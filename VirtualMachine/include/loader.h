#ifndef LOADER_H
#define LOADER_H

#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include "processor.h"

// Splitting a string into pieces separated by a space
std::vector<std::string> split(const std::string& line) noexcept;

// Parsing a string with memory allocation for a variable
void parse_variable(std::vector<std::string>& parts, uint16_t adrs, Processor& cpu) noexcept;

// Parsing strings with command
Word parse_command(std::vector<std::string>& parts, Processor& cpu) noexcept;

// Parsing all strings
bool parse_line_parts(std::vector<std::string>& parts, uint16_t address, Processor& cpu) noexcept;

// Function that implements the bootloader
void load(Processor& cpu, char* filename) noexcept;

#endif // LOADER_H
