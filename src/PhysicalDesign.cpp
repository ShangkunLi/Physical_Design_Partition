#include "PhysicalDesign.h"
#include <fstream>
#include <iostream>

void PhysicalDesign::FMPartitionSingle()
{
	partition = pt.FMPartition(graph);
}

void PhysicalDesign::FMPartitionRand()
{
	partition = pt.FMPartitionRandPoint(graph);
}

void PhysicalDesign::outputfile()
{
	std::string filepath(filename + "out.txt");
	std::ofstream output_file(filepath);
	if (!output_file.is_open())
	{
		std::cout << "Failed to open output file!" << std::endl;
		return;
	}

	for (auto i : partition)
	{
		output_file << "partition size: " << i.size() << ": " << std::endl;
		for (auto value : i)
			output_file << value << " ";
		output_file << std::endl;
	}
	output_file << "cut size: " << pt.mincutsize << std::endl;

	output_file.close();
	std::cout << "Output file: " << filepath << std::endl;
}
