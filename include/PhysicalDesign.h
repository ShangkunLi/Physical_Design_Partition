#pragma once
#include "parser.h"
#include "FMPartition.h"

class PhysicalDesign
{
public:

    std::string filename;

    PhysicalDesign(parser &p)
    {
        graph = p.getGraph();
        filename = p.returnFilename();
    };

    void FMPartitionRand();

    void FMPartitionSingle();

    void outputfile();

private:
    std::vector<std::vector<int>> graph;

    std::vector<std::vector<int>> partition;

    Partition pt;
};
