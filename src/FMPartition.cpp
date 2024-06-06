#include "FMPartition.h"
#include <limits>

void Partition::OneSwap(Bucket &bu, NodeArray &nodearray_local, NodeArray &nodearray_global, int currentBest)
{
	int record = std::numeric_limits<int>::max();
	for (int loop = 0; loop < nodearray_local.data_array.size() / 2; loop++)
	{
		int Areplace, Breplace;
		// select a vertex with maximal gain in A
		Areplace = bu.maxGain(A_PART, nodearray_local);
		// move to B
		nodearray_local.data_array[Areplace]->Node_status = FIXED;
		nodearray_local.data_array[Areplace]->Node_part = B_PART;
		// update gains
		int prevGain, updateGain;
		prevGain = nodearray_local.data_array[Areplace]->Node_gain;
		updateGain = nodearray_local.updateGain(Areplace);
		bu.updateExchange(A_PART, Areplace, updateGain, prevGain);
		for (int i = 0; i < nodearray_local.data_array[Areplace]->Node_connect.size(); i++)
		{
			prevGain = nodearray_local.data_array[nodearray_local.data_array[Areplace]->Node_connect[i]]->Node_gain;
			updateGain = nodearray_local.updateGain(nodearray_local.data_array[Areplace]->Node_connect[i]);
			bu.updateLocal(nodearray_local.data_array[nodearray_local.data_array[Areplace]->Node_connect[i]]->Node_part, nodearray_local.data_array[Areplace]->Node_connect[i], updateGain, prevGain);
		}

		// select a vertex with maximal gain in B
		Breplace = bu.maxGain(B_PART, nodearray_local);
		// move to A
		nodearray_local.data_array[Breplace]->Node_status = FIXED;
		nodearray_local.data_array[Breplace]->Node_part = A_PART;
		// update gains
		prevGain = nodearray_local.data_array[Breplace]->Node_gain;
		updateGain = nodearray_local.updateGain(Breplace);
		bu.updateExchange(B_PART, Breplace, updateGain, prevGain);
		for (int i = 0; i < nodearray_local.data_array[Breplace]->Node_connect.size(); i++)
		{
			prevGain = nodearray_local.data_array[nodearray_local.data_array[Breplace]->Node_connect[i]]->Node_gain;
			updateGain = nodearray_local.updateGain(nodearray_local.data_array[Breplace]->Node_connect[i]);
			bu.updateLocal(nodearray_local.data_array[nodearray_local.data_array[Breplace]->Node_connect[i]]->Node_part, nodearray_local.data_array[Breplace]->Node_connect[i], updateGain, prevGain);
		}
		// record the gain
		if (nodearray_local.cutSize() < record && nodearray_local.cutSize() < currentBest)
		{
			record = nodearray_local.cutSize();
			nodearray_global.reset(nodearray_local);
		}
	}
	nodearray_global.recover();
}

std::vector<std::vector<int>> Partition::FMPartition(std::vector<std::vector<int>> &graph)
{
	std::vector<std::vector<int>> partition;
	std::vector<int> tpA;
	std::vector<int> tpB;
	NodeArray nodearray_global(graph);
	int currentBest = std::numeric_limits<int>::max();
	nodearray_global.init_half();
	// nodearray_global.init_even();
	nodearray_global.updateGain();
	for (int loop = 0; loop < MaxIteration; loop++)
	{
		int flag = 0;
		Bucket bu;
		NodeArray nodearray_local;
		nodearray_local.copy(nodearray_global);
		bu.load(nodearray_local);
		OneSwap(bu, nodearray_local, nodearray_global, currentBest);
		if (currentBest > nodearray_global.cutSize())
		{
			currentBest = nodearray_global.cutSize();
			flag = 1;
		}
		std::cout << "iteration " << loop << ": Cut Size = " << nodearray_global.cutSize() << std::endl;
		for (int i = 0; i < nodearray_local.data_array.size(); i++)
		{
			free(nodearray_local.data_array[i]);
		}
		if (flag == 0)
		{
			break;
		}
	}

	std::cout << "FM Partition done. " << "Cut Size = " << nodearray_global.cutSize() << std::endl;
	std::cout << "Partition Result: " << std::endl;
	std::cout << "A Part: " << std::endl;
	for (int i = 0; i < nodearray_global.data_array.size(); i++)
	{
		if (nodearray_global.data_array[i]->Node_part == A_PART)
		{
			std::cout << nodearray_global.data_array[i]->Node_index << " ";
			tpA.push_back(nodearray_global.data_array[i]->Node_index);
		}
	}
	std::cout << std::endl;
	std::cout << "B Part: " << std::endl;
	for (int i = 0; i < nodearray_global.data_array.size(); i++)
	{
		if (nodearray_global.data_array[i]->Node_part == B_PART)
		{
			std::cout << nodearray_global.data_array[i]->Node_index << " ";
			tpB.push_back(nodearray_global.data_array[i]->Node_index);
		}
	}
	partition.push_back(tpA);
	partition.push_back(tpB);
	mincutsize = currentBest;
	return partition;
}

std::vector<std::vector<int>> Partition::FMPartitionRandPoint(std::vector<std::vector<int>> &graph)
{
	int min_cut_size = std::numeric_limits<int>::max();
	std::vector<std::vector<int>> partition;
	std::vector<int> tpA;
	std::vector<int> tpB;
	for (int l = 0; l < 5; l++)
	{
		NodeArray nodearray_global(graph);

		std::cout << "FM Rand Point: " << l << std::endl;
		int currentBest = std::numeric_limits<int>::max();

		nodearray_global.init_rand();
		nodearray_global.updateGain();

		for (int loop = 0; loop < MaxIteration; loop++)
		{
			int flag = 0;
			Bucket bu;
			NodeArray nodearray_local;
			nodearray_local.copy(nodearray_global);

			bu.load(nodearray_local);
			OneSwap(bu, nodearray_local, nodearray_global, currentBest);
			if (currentBest > nodearray_global.cutSize())
			{
				currentBest = nodearray_global.cutSize();
				flag = 1;
			}
			std::cout << "iteration " << loop << ": Cut Size = " << nodearray_global.cutSize() << std::endl;
			for (int i = 0; i < nodearray_local.data_array.size(); i++)
			{
				free(nodearray_local.data_array[i]);
			}
			if (flag == 0)
			{
				break;
			}
		}

		std::cout << "FM Partition " << l << " done. " << "Cut Size = " << nodearray_global.cutSize() << std::endl;
		std::cout << "Partition Result: " << std::endl;
		std::cout << "A Part: " << std::endl;
		for (int i = 0; i < nodearray_global.data_array.size(); i++)
		{
			if (nodearray_global.data_array[i]->Node_part == A_PART)
			{
				std::cout << nodearray_global.data_array[i]->Node_index << " ";
			}
		}
		std::cout << std::endl;
		std::cout << "B Part: " << std::endl;
		for (int i = 0; i < nodearray_global.data_array.size(); i++)
		{
			if (nodearray_global.data_array[i]->Node_part == B_PART)
			{
				std::cout << nodearray_global.data_array[i]->Node_index << " ";
			}
		}
		if (min_cut_size > nodearray_global.cutSize())
		{
			min_cut_size = nodearray_global.cutSize();
			tpA.clear();
			tpB.clear();
			for (int i = 0; i < nodearray_global.data_array.size(); i++)
			{
				if (nodearray_global.data_array[i]->Node_part == A_PART)
				{
					tpA.push_back(nodearray_global.data_array[i]->Node_index);
				}
			}
			for (int i = 0; i < nodearray_global.data_array.size(); i++)
			{
				if (nodearray_global.data_array[i]->Node_part == B_PART)
				{
					tpB.push_back(nodearray_global.data_array[i]->Node_index);
				}
			}
		}
		std::cout << std::endl;
	}
	partition.push_back(tpA);
	partition.push_back(tpB);
	std::cout << "The Best Partition Result: Cut Size =  " << min_cut_size << std::endl;
	mincutsize = min_cut_size;
	return partition;
}