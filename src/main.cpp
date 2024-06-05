#include <iostream>
#include "parser.h"
#include "PhysicalDesign.h"


int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "Command Error" << std::endl;
        std::cout << "Use: " << argv[0] << "command[-single][-rand]" << " filename" << std::endl;
        return -1;
    }
    std::string command = argv[1];
    std::string filename = argv[2];
    parser p(filename);

    if (p.parse() != 0)
    {
        std::cout << "parsing error, " << p.returnInfo() << "\n";
        return -1;
    }

    PhysicalDesign PD(p);

    PD.filename = filename;
    if (command == "-single")
    {
        PD.FMPartitionSingle();
    }
    else if (command == "-rand")
    {
        PD.FMPartitionRand();
    }

    PD.outputfile();
}
