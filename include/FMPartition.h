#ifndef FM_ALGORITHM_H
#define FM_ALGORITHM_H
#include "DataStructure.h"
#include <iostream>

#define MaxIteration 100
class Partition
{
public:
	void OneSwap(Bucket &bkt, NodeArray &nodearray_local, NodeArray &nodearray_global, int currentBest);
	std::vector<std::vector<int>> FMPartition(std::vector<std::vector<int>> &graph);
	std::vector<std::vector<int>> FMPartitionRandPoint(std::vector<std::vector<int>> &graph);
	int mincutsize;
	std::vector<std::vector<int>> partition;
};

#endif