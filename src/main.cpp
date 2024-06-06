#include <iostream>
#include "parser.h"
#include "FMPartition.h"

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

    std::vector<std::vector<int>> graph = p.getGraph();
    Partition pt;

    if (command == "-single")
    {
        pt.partition = pt.FMPartition(graph);
    }
    else if (command == "-rand")
    {
        pt.partition = pt.FMPartitionRandPoint(graph);
    }
    else
    {
        std::cout << "Command Option Error" << std::endl;
        std::cout << "Use: " << argv[0] << "command[-single][-rand]" << " filename" << std::endl;
        return -1;
    }

    std::string filepath(filename + "out.txt");
    std::ofstream output_file(filepath);
    if (!output_file.is_open())
    {
        std::cout << "Failed to open output file!" << std::endl;
        return -1;
    }

    for (auto i : pt.partition)
    {
        output_file << "partition size: " << i.size() << ": " << std::endl;
        for (auto value : i)
            output_file << value << " ";
        output_file << std::endl;
    }
    output_file << "cut size: " << pt.mincutsize << std::endl;

    output_file.close();
    std::cout << std::endl;
    std::cout << "Output file: " << filepath << std::endl;
}
