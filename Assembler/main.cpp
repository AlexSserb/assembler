// Laboratory work 2 in the discipline System software.
// Completed by student Serbin Alexander
// Assembler for the virtual machine VM09.

#include <iostream>
#include <cstdlib>
#include "Assembler.h"

using std::string;


string get_dir_from_filepath(char* first_argv)
{
    string cur_file_path(first_argv);
    int i = cur_file_path.size() - 1;
    while (i >= 0 && cur_file_path[i] != '/') i--;
    return cur_file_path.substr(0, i);
}

// Translating assembler into codes and running the program
int asm_to_code_and_run(string cur_dir, string source_file, string target_file)
{
    int status = 1;
    if (assem::asm_to_code(source_file, target_file))
    {
        // Starting the program
        status = assem::run('\'' + cur_dir + string("/VirtualMachine9'"), target_file);
    }

    return status;
}

int main(int argc, char **argv)
{
    string cur_dir = get_dir_from_filepath(argv[0]);

    string source_file, source_dir;
    if (argc > 1)
    {
        source_file = string(argv[1]);
        source_dir = get_dir_from_filepath(argv[1]);
    }
    else // If there is no command line argument - work with the test file
    {
        source_file = cur_dir + string("/testfile.txt");
        source_dir = cur_dir;
    }

    string target_file = source_dir + string("/bin_code.txt");

    // Translating assembler into codes and running the program
    return asm_to_code_and_run(cur_dir, source_file, target_file);
}
