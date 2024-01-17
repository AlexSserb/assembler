// Laboratory work 1 in the discipline "System software"
// Completed by student Serbin Alexander
// Virtual Machine VM09.

#include <iostream>
#include "loader.h"


int main(int argc, char **argv)
{
    Processor proc = Processor();

    // Loading a program from a file into memory and running it
    if (argc > 1)
        load(proc, argv[1]);
    else
        std::cout << "Specify the file to execute.\n";
    return 0;
}
