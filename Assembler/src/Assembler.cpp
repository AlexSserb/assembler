#include "Assembler.h"


// Translation of an assembler program into codes
bool assem::asm_to_code(const string& source_file_path, const string& target_file_path) noexcept
{
    assem::priv::name_address = name_address_t();
    assem::priv::cur_address = 0;
    assem::priv::exprSolver = IntExprSolver();
    // First pass
    list<vector<string>> code_lines = priv::read_source_file(source_file_path);
    // Second pass
    return !code_lines.empty() && priv::write_target_file(target_file_path, code_lines);
}

// Starting the program
int assem::run(const string& vm_path, const string& target_file_path)
{
    return std::system((vm_path + " '" + target_file_path + '\'').c_str());
}

// Reading assembly code from a file
list<vector<string>> assem::priv::read_source_file(const string& source_file_path) noexcept
{
    list<vector<string>> code_lines;
    std::ifstream fin;
    fin.open(source_file_path);
    if (fin)
    {
        string line;
        while (std::getline(fin, line))
        {
            auto codes_line = priv::parse_asm_line(line); // Splitting an assembly line into parts
            if (codes_line.size() > 0 && codes_line[0] != "proc" && codes_line[0].back() != ':')
            {
                // Parsing variable declarations
                if (is_var_type(codes_line[0]))
                {
                    auto lines = parse_var_definitions(fin, codes_line);
                    for (auto it = lines.begin(); it != lines.end(); it++)
                        code_lines.push_back(*it);
                }
                else // Parsing strings of code
                {
                    codes_line.insert(codes_line.begin(), "k");
                    code_lines.push_back(codes_line);
                }
            }
        }
        return code_lines;
    }
    else std::cout << "Failed to open file \"" << source_file_path << "\" for reading.\n";
    return code_lines;
}

// Parsing multiple lines with variable definitions
list<vector<string>> assem::priv::parse_var_definitions(std::ifstream& fin, vector<string>& first_var_def) noexcept
{
    string line = ""; // the line to be read
    int jmp_param = cur_address; // address parameter for the Jump command

    // offset of the current address due to the appearance of the Jump command before defining the variables
    cur_address += 2;
    // the definition address of the previously read variable is now 2 larger than it was (indentation for the Jump command)
    name_address[first_var_def[1]] += 2;
    auto code_lines = list<vector<string>> { first_var_def }; // strings with variable definitions
    bool is_var_def = true; // is the last line a variable definition

    while (is_var_def && std::getline(fin, line))
    {
        auto codes_line = parse_asm_line(line); // Splitting an assembly line into parts
        if (codes_line.size() > 0)
        {
            // Parsing variable declarations
            if (is_var_type(codes_line[0]))
                code_lines.push_back(codes_line);
            else // Parsing strings of code
            {
                codes_line.insert(codes_line.begin(), "k");
                code_lines.push_back(codes_line);
                is_var_def = false;
            }
            jmp_param += 2; // Jump should traverse one more line
        }
    }

    // Inserting a Jump Command
    code_lines.push_front(vector<string> { "k", "1", "3", std::to_string(jmp_param) });
    return code_lines;
}

// String parsing (first pass)
vector<string> assem::priv::parse_asm_line(const string& line_asm) noexcept
{
    vector<string> parts = vector<string>();
    string substring = "", prev = "";
    int i = 0;
    while (line_asm[i] == ' ' || line_asm[i] == '\t') i++;
    int j = i;
    while (i < line_asm.size())
    {
        if (line_asm[i] == ' ' || line_asm[i] == ',')
        {
            if (i != j)
            {
                substring = line_asm.substr(j, i - j);
                if (substring == "#") break;
                substring = replace_substr_code(substring, prev);
                parts.push_back(substring);
                prev = substring;
            }
            j = i + 1;
        }
        i++;
    }
    if (i != j && substring != "#")
    {
        substring = line_asm.substr(j, i - j);
        parts.push_back(replace_substr_code(substring, prev));
    }

    change_addresses_using_parts(parts);
    return parts;
}

// Changing addresses using a split assembly line
void assem::priv::change_addresses_using_parts(vector<string>& parts) noexcept
{
    if (parts.size() > 0)
    {
        if (parts[0] == "start")
        {
            start_prog_adrs = cur_address;
            parts.clear();
        }
        else if (parts[0].back() != ':' && (std::isdigit(parts[0][0]) || is_var_type(parts[0])))
        {
            cur_address += 2;
        }
    }
}

// Replacing assembly keyword with code
string assem::priv::replace_substr_code(string& asm_key_word, const string& prev) noexcept
{
    if (asm_key_word == "") return "";
    try
    {
        asm_key_word = command_code.at(asm_key_word);
        int res_i = std::stoi(asm_key_word);
        if (res_i < 20 && res_i > 0)
            asm_key_word += " 0";
        else if (res_i == 0)
            asm_key_word += " " + std::to_string(start_prog_adrs);
    }
    catch (const std::out_of_range& ex)
    {
        replace_asm_dir(asm_key_word, prev);
    }

    return asm_key_word;
}

// Replacing an assembly directive with a shorter one
void assem::priv::replace_asm_dir(string& asm_key_word, const string& prev) noexcept
{
    if (asm_key_word == "uint") asm_key_word = "u";
    else if (asm_key_word == "int") asm_key_word = "i";
    else if (asm_key_word == "float") asm_key_word = "f";
    else solve_asm_unknown_word(asm_key_word, prev);
}

// Processing a new name or expression
void assem::priv::solve_asm_unknown_word(string& asm_key_word, const string& prev) noexcept
{
    // 1. The name must begin with a letter
    // 2. The previous word "prev" must allow the word to be replaced by the address
    if (std::isalpha(asm_key_word[0]) && (is_next_changeable(prev) || asm_key_word[asm_key_word.size()-1] == ':') &&
        command_code.find(asm_key_word) == command_code.end())
    {
        // Assigning an address for a keyword
        if (asm_key_word[asm_key_word.size()-1] == ':')
            name_address[asm_key_word.substr(0, asm_key_word.size() - 1)] = cur_address;
        else name_address[asm_key_word] = cur_address;
    }
    else if (IntExprSolver::is_expr(asm_key_word)) // Solving the expression
    {
        asm_key_word = std::to_string(exprSolver.solve(asm_key_word));
    }
}

// Is it possible after this word to replace the following with an address
bool assem::priv::is_next_changeable(const string& prev) noexcept
{
    return prev == "proc" || is_var_type(prev);
}

// Is a word a variable type
bool assem::priv::is_var_type(const string &name) noexcept
{
    return name == "u" || name == "i" || name == "f";
}

// Writing finished code to a file
bool assem::priv::write_target_file(const string& target_file_path, list<vector<string>>& code_lines) noexcept
{
    std::ofstream fout;
    fout.open(target_file_path, std::ios::out | std::ios::trunc);
    string prev = "";
    if (fout)
    {
        for (auto it = code_lines.begin(); it != code_lines.end(); it++)
        {
            for (int i = 0; i < (*it).size(); i++)
            {
                if (!is_var_type(prev))
                    fout << replace_name_address((*it)[i]) << ' ';
                prev = (*it)[i];
            }
            fout << '\n';
        }
        return true;
    }
    else std::cout << "Failed to open file \"" << target_file_path << "\" for reading.\n";
    return false;
}

// Replacing a name with an address
string assem::priv::replace_name_address(const string& name)
{
    string res = name;
    try
    {
        res = std::to_string(name_address.at(name));
    }
    catch(std::exception ex) {};
    return res;
}

