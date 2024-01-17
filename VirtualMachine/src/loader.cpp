#include "loader.h"

// Splitting a string into pieces separated by a space
std::vector<std::string> split(const std::string& line) noexcept
{
    std::stringstream ss(line);
    std::string line_part;
    std::vector<std::string> splitted = std::vector<std::string>();
    while (std::getline(ss, line_part, ' '))
    {
        if (line_part == "#")
            return splitted;
        if (line_part != "")
            splitted.push_back(line_part);
    }
    return splitted;
}

// Parsing a string with memory allocation for a variable
void parse_variable(std::vector<std::string>& parts, uint16_t adrs, Processor& cpu) noexcept
{
    Word word = Word();
    if (parts[0] == "i") word.ival = stoi(parts[1]);
    else if (parts[0] == "u") word.uval = stoi(parts[1]);
    else if (parts[0] == "f") word.fval = stof(parts[1]);
    cpu.memory.set_word(adrs, word);
}

// Parsing strings of code
Word parse_command(std::vector<std::string>& parts, Processor& cpu) noexcept
{
    Word command = Word();
    command.cmd3ops.cmd = std::stoi(parts[1]);
    if (parts.size() == 3) // 3 parts per line
    {
        if (command.cmd3ops.cmd == 51) command.cmd2ops.adrs = std::stoi(parts[2]);
        else command.cmd3ops.regs[2] = std::stoi(parts[2]);
    }
    else if (parts.size() == 4) // 4 pieces per line
    {
        command.cmd2ops.reg = std::stoi(parts[2]);
        command.cmd2ops.adrs = std::stoi(parts[3]);
    }
    else if (parts.size() > 4) // 5 pieces per line
        for (int i = 0; i < 3; i++)
            command.cmd3ops.regs[i] = std::stoi(parts[i + 2]);
    return command;
}

// Parsing all strings
bool parse_line_parts(std::vector<std::string>& parts, uint16_t address, Processor& cpu) noexcept
{
    Word command = Word();

    if (parts[0] == "i" || parts[0] == "u" || parts[0] == "f")
        parse_variable(parts, address, cpu);
    else if (parts[0] == "e")
    {
        command.uval = 0;
        cpu.memory.set_word(address, command);
    }
    else if (parts[0] == "k")
    {
        command = parse_command(parts, cpu);
        cpu.memory.set_word(address, command);
    }
    else return false; // The command is not written to memory

    return true; // The command is written to memory
}

// Function that implements the bootloader
void load(Processor& cpu, char* filename) noexcept
{
    std::string line;
    std::vector<std::string> line_parts;
    std::ifstream fin;
    fin.open(filename);
    uint16_t code_address = 0, run_address = 0;
    if (fin)
    {
        // Loading commands and variables into memory
        while (std::getline(fin, line))
        {
            line_parts = split(line);

            if (line_parts.size() > 0)
            {
                if (line_parts[0] == "a")
                    code_address = std::stoi(line_parts[1]);
                else
                {
                    if (line_parts[0] == "e")
                        run_address = std::stoi(line_parts[1]) - 2;
                    if (parse_line_parts(line_parts, code_address, cpu))
                        code_address += 2;
                }
            }
        }

        cpu.run(run_address);
    }
    else std::cout << "Failed to open file.\n";
}
