#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "IntExprSolver.h"

#include <fstream>
#include <unordered_map>
#include <list>
#include <vector>
#include <cstdlib>
#include <string>

using std::string, std::vector, std::list;
using cmd_code_map_t = std::unordered_map<string, string>;
using name_address_t = std::unordered_map<string, uint16_t>;

namespace assem
{
    // --- Public functions ---

    // Translation of an assembler program into codes
    bool asm_to_code(const string& source_file_path, const string& target_file_path) noexcept;

    // Starting the program
    int run(const string& vm_path, const string& target_file_path);

    // --- Private functions ---
    namespace priv
    {
        // Hash table for storing variable addresses
        static name_address_t name_address;

        // Hash table for commands and their codes
        static const cmd_code_map_t command_code = {
            { "end", "0" },   { "jmp", "1" },   { "je", "2" },    { "jeu", "3" },   { "jef", "4" },
            { "jg", "5" },    { "jgu", "6" },   { "jgf", "7" },   { "jl", "8" },    { "jlu", "9" },     { "jlf", "10" },
            { "jne", "11" },  { "jneu", "12" }, { "jnef", "13" }, { "jge", "14" },  { "jgeu", "15" },   { "jgef", "16" },
            { "jle", "17" },  { "jleu", "18" }, { "jlef", "19" }, { "print", "20"}, { "printu", "21" }, { "printf", "22" },
            { "load", "23" }, { "neg", "24" },  { "negf", "25" }, { "cmp", "26" },  { "cmpu", "27" },   { "cmpf", "28" },
            { "add", "29" },  { "addf", "30" }, { "sub", "31"},   { "subf", "32" }, { "mul", "33" },    { "mulf", "34" },
            { "divu", "35" }, { "div", "36" },  { "divf", "37" }, { "modu", "38" }, { "mod", "39" },    { "inc", "40" },
            { "dec", "41" },  { "read", "42" }, { "readu", "43"}, { "readf", "44"}, { "and", "45" },    { "or", "46" },
            { "xor", "47" },  { "not", "48" },  { "loadr", "49" },{ "loadrv", "50"},{ "call", "51" },   { "loadf", "52" },
            { "setf", "53" }, { "endp", "54" }
        };

        // Current address for replacing names with addresses
        static uint16_t cur_address;

        // Address for starting the program
        static uint16_t start_prog_adrs;

        static IntExprSolver exprSolver;

        // Reading assembly code from a file
        list<vector<string>> read_source_file(const string& source_file_path) noexcept;

        // String parsing (first pass)
        vector<string> parse_asm_line(const string& line_asm) noexcept;

        // Changing addresses using a split assembly line
        void change_addresses_using_parts(vector<string>& parts) noexcept;

        // Parsing multiple lines with variable definitions
        list<vector<string>> parse_var_definitions(std::ifstream& fin, vector<string>& first_var_def) noexcept;

        // Replacing assembly keyword with code
        string replace_substr_code(string& asm_key_word, const string& prev) noexcept;

        // Replacing an assembly directive with a shorter one
        void replace_asm_dir(string& asm_key_word, const string& prev) noexcept;

        // Processing a new name or expression
        void solve_asm_unknown_word(string& asm_key_word, const string& prev) noexcept;

        // Writing finished code to a file
        bool write_target_file(const string& target_file_path, list<vector<string>>& code_lines) noexcept;

        // Replacing a name with an address
        string replace_name_address(const string& name);

        // Is it possible after this word to replace the following with an address
        bool is_next_changeable(const string& prev) noexcept;

        // Is a word a variable type
        bool is_var_type(const string &name) noexcept;
    }
}


#endif // ASSEMBLER_H
