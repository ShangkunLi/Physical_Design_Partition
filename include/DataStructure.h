#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include <vector>
#include <algorithm>
#include <map>
#include <utility>
#include <ctime>
#include <random>

typedef enum
{
	NO_PART = -1,
	A_PART = 0,
	B_PART = 1
} NODE_PART;

typedef enum
{
	FREE = 0,
	FIXED = 1
} STATUS;

// NODE class

class NODE
{
public:
	int Node_index;
	NODE_PART Node_part;
	STATUS Node_status;
	int Node_gain;
	std::vector<int> Node_connect;

	NODE()
	{
		Node_index = -1;
		Node_part = NO_PART;
		Node_status = FREE;
		Node_gain = 0;
		Node_connect.clear();
	}
	NODE(int i, std::vector<int> connect)
	{
		Node_index = i;
		Node_part = NO_PART;
		Node_status = FREE;
		Node_gain = 0;
		Node_connect.assign(connect.begin(), connect.end());
	}
	NODE(int i, NODE_PART part, STATUS status, int gain, std::vector<int> connect)
	{
		Node_index = i;
		Node_part = part;
		Node_status = status;
		Node_gain = gain;
		Node_connect.assign(connect.begin(), connect.end());
	}
};

class NodeArray
{
public:
	std::vector<NODE *> data_array;

	NodeArray() {}

	NodeArray(std::vector<std::vector<int>> &graph)
	{
		for (int i = 0; i < graph.size(); i++)
		{
			data_array.push_back(new NODE(i, graph[i]));
		}
	}
	void copy(NodeArray na)
	{
		for (int i = 0; i < na.data_array.size(); i++)
		{
			this->data_array.push_back(new NODE(na.data_array[i]->Node_index, na.data_array[i]->Node_part, na.data_array[i]->Node_status, na.data_array[i]->Node_gain, na.data_array[i]->Node_connect));
		}
	}
	void reset(NodeArray na)
	{
		for (int i = 0; i < na.data_array.size(); i++)
		{
			data_array[i]->Node_status = na.data_array[i]->Node_status;
			data_array[i]->Node_gain = na.data_array[i]->Node_index;
			data_array[i]->Node_part = na.data_array[i]->Node_part;
		}
	}
	void recover()
	{
		for (int i = 0; i < data_array.size(); i++)
		{
			data_array[i]->Node_status = FREE;
		}
	}
	void init_half()
	{
		for (int i = 0; i < data_array.size() / 2; i++)
		{
			data_array[i]->Node_part = A_PART;
		}
		for (int i = data_array.size() / 2; i < data_array.size(); i++)
		{
			data_array[i]->Node_part = B_PART;
		}
	}
	void init_even()
	{
		for (int i = 0; i < data_array.size(); i++)
		{
			if (i % 2 == 0)
			{
				data_array[i]->Node_part = A_PART;
			}
			else if (i % 2 == 1)
			{
				data_array[i]->Node_part = B_PART;
			}
		}
	}
	void init_rand()
	{
		int a_num = 0;
		int b_num = 0;
		std::default_random_engine rd;
		std::uniform_int_distribution<int> u(0, 1);
		rd.seed(time(0));
		for (int i = 0; i < data_array.size(); i++)
		{
			if (a_num == data_array.size() / 2)
			{
				data_array[i]->Node_part = B_PART;
			}
			else if (b_num == data_array.size() / 2)
			{
				data_array[i]->Node_part = A_PART;
			}
			else if (u(rd) == 0)
			{
				data_array[i]->Node_part = A_PART;
				a_num++;
			}
			else
			{
				data_array[i]->Node_part = B_PART;
				b_num++;
			}
		}
	}
	void updateGain()
	{
		for (int i = 0; i < data_array.size(); i++)
		{
			int internal = 0;
			int external = 0;
			int record = data_array[i]->Node_part;
			for (auto iter = data_array[i]->Node_connect.begin(); iter != data_array[i]->Node_connect.end(); iter++)
			{
				if (data_array[(*iter)]->Node_part == record && record != -1)
				{
					internal++;
				}
				else if (data_array[(*iter)]->Node_part == 1 - record)
				{
					external++;
				}
			}
			data_array[i]->Node_gain = external - internal;
		}
	}
	int updateGain(int i)
	{
		int internal = 0;
		int external = 0;
		int record = data_array[i]->Node_part;
		for (auto iter = data_array[i]->Node_connect.begin(); iter != data_array[i]->Node_connect.end(); iter++)
		{
			if (data_array[(*iter)]->Node_part == record && record != -1)
			{
				internal++;
			}
			else if (data_array[(*iter)]->Node_part == 1 - record)
			{
				external++;
			}
		}
		data_array[i]->Node_gain = external - internal;
		return data_array[i]->Node_gain;
	}

	int cutSize()
	{
		int cutsize = 0;
		for (int i = 0; i < data_array.size(); i++)
		{
			if (data_array[i]->Node_part == A_PART)
			{
				for (auto iter = data_array[i]->Node_connect.begin(); iter != data_array[i]->Node_connect.end(); iter++)
				{
					if (data_array[(*iter)]->Node_part != data_array[i]->Node_part)
					{
						cutsize++;
					}
				}
			}
		}
		return cutsize;
	}
};

class BucketNode
{
public:
	int index;
	BucketNode *next;
	BucketNode *prev;

	BucketNode(int i, BucketNode *n, BucketNode *p)
	{
		index = i;
		next = n;
		prev = p;
	}
};

class Bucket
{
public:
	std::map<int, BucketNode *, std::greater<int>> BucketAtoB;
	std::map<int, BucketNode *, std::greater<int>> BucketBtoA;

	Bucket(){}
	void load(NodeArray &narr)
	{
		for (int i = 0; i < narr.data_array.size(); i++)
		{
			if (narr.data_array[i]->Node_part == A_PART)
			{
				auto iter = BucketAtoB.find(narr.data_array[i]->Node_gain);
				if (iter != BucketAtoB.end())
				{
					BucketNode *temp = iter->second;
					BucketNode *insert = iter->second;
					for (temp = iter->second; temp != NULL; temp = temp->next)
					{
						insert = temp;
					}
					insert->next = new BucketNode(narr.data_array[i]->Node_index, NULL, insert);
				}
				else if (iter == BucketAtoB.end())
				{
					BucketNode *head = new BucketNode(-1, NULL, NULL);
					BucketAtoB.insert(std::pair<int, BucketNode *>(narr.data_array[i]->Node_gain, head));
					head->next = new BucketNode(narr.data_array[i]->Node_index, NULL, head);
				}
			}
			if (narr.data_array[i]->Node_part == B_PART)
			{
				auto iter = BucketBtoA.find(narr.data_array[i]->Node_gain);
				if (iter != BucketBtoA.end())
				{
					BucketNode *temp = iter->second;
					BucketNode *insert = iter->second;
					for (temp = iter->second; temp != NULL; temp = temp->next)
					{
						insert = temp;
					}
					insert->next = new BucketNode(narr.data_array[i]->Node_index, NULL, insert);
				}
				else if (iter == BucketBtoA.end())
				{
					BucketNode *head = new BucketNode(-1, NULL, NULL);
					BucketBtoA.insert(std::pair<int, BucketNode *>(narr.data_array[i]->Node_gain, head));
					head->next = new BucketNode(narr.data_array[i]->Node_index, NULL, head);
				}
			}
		}
	}
	int maxGain(NODE_PART partition, NodeArray &narr)
	{
		if (partition == A_PART)
		{
			for (auto iter = BucketAtoB.begin(); iter != BucketAtoB.end(); iter++)
			{
				BucketNode *temp;
				for (temp = iter->second; temp != NULL; temp = temp->next)
				{
					if (temp->index > -1)
					{
						if (narr.data_array[temp->index]->Node_status == FREE)
						{
							return temp->index;
						}
					}
				}
			}
		}
		else if (partition == B_PART)
		{
			for (auto iter = BucketBtoA.begin(); iter != BucketBtoA.end(); iter++)
			{
				BucketNode *temp;
				for (temp = iter->second; temp != NULL; temp = temp->next)
				{
					if (temp->index > -1)
					{
						if (narr.data_array[temp->index]->Node_status == FREE)
						{
							return temp->index;
						}
					}
				}
			}
		}
	}
	void ChangeBetweenPart(NODE_PART partition, int i, int updateGain, int prevGain)
	{
		if (partition == A_PART)
		{
			auto iter = BucketAtoB.find(prevGain);
			BucketNode *temp = iter->second;
			for (temp = iter->second; temp != NULL; temp = temp->next)
			{
				if (temp->index == i)
				{
					break;
				}
			}
			if (temp->next == NULL)
			{
				// delete temp from bucket A
				temp->prev->next = temp->next;
				temp->prev = NULL;
			}
			else if (temp->next != NULL)
			{
				temp->prev->next = temp->next;
				temp->next->prev = temp->prev;
				temp->next = NULL;
				temp->prev = NULL;
			}
			// add temp to bucket B
			auto iter0 = BucketBtoA.find(updateGain);
			if (iter0 != BucketBtoA.end())
			{
				BucketNode *temp0 = iter0->second;
				BucketNode *insert0 = iter0->second;
				for (temp0 = iter0->second; temp0 != NULL; temp0 = temp0->next)
				{
					insert0 = temp0;
				}
				insert0->next = temp;
				temp->prev = insert0;
			}
			else if (iter0 == BucketBtoA.end())
			{
				BucketNode *head = new BucketNode(-1, NULL, NULL);
				BucketBtoA.insert(std::pair<int, BucketNode *>(updateGain, head));
				head->next = temp;
				temp->prev = head;
			}
		}
		else if (partition == B_PART)
		{
			auto iter = BucketBtoA.find(prevGain);
			BucketNode *temp = iter->second;
			for (temp = iter->second; temp != NULL; temp = temp->next)
			{
				if (temp->index == i)
				{
					break;
				}
			}
			if (temp->next == NULL)
			{
				// delete temp from bucket B
				temp->prev->next = temp->next;
				temp->prev = NULL;
			}
			else if (temp->next != NULL)
			{
				temp->prev->next = temp->next;
				temp->next->prev = temp->prev;
				temp->next = NULL;
				temp->prev = NULL;
			}
			// add temp to bucket A
			auto iter0 = BucketAtoB.find(updateGain);
			if (iter0 != BucketAtoB.end())
			{
				BucketNode *temp0 = iter0->second;
				BucketNode *insert0 = iter0->second;
				for (temp0 = iter0->second; temp0 != NULL; temp0 = temp0->next)
				{
					insert0 = temp0;
				}
				insert0->next = temp;
				temp->prev = insert0;
			}
			else if (iter0 == BucketAtoB.end())
			{
				BucketNode *head = new BucketNode(-1, NULL, NULL);
				BucketAtoB.insert(std::pair<int, BucketNode *>(updateGain, head));
				head->next = temp;
				temp->prev = head;
			}
		}
	}
	void ChangeWithinPart(NODE_PART partition, int i, int updateGain, int prevGain)
	{
		if (partition == A_PART)
		{
			auto iter = BucketAtoB.find(prevGain);
			BucketNode *temp = iter->second;
			for (temp = iter->second; temp != NULL; temp = temp->next)
			{
				if (temp->index == i)
				{
					break;
				}
			}
			if (temp->next == NULL)
			{
				// delete temp from bucket A
				temp->prev->next = temp->next;
				temp->prev = NULL;
			}
			else if (temp->next != NULL)
			{
				temp->prev->next = temp->next;
				temp->next->prev = temp->prev;
				temp->next = NULL;
				temp->prev = NULL;
			}
			// add temp to bucket B
			auto iter0 = BucketAtoB.find(updateGain);
			if (iter0 != BucketAtoB.end())
			{
				BucketNode *temp0 = iter0->second;
				BucketNode *insert0 = iter0->second;
				for (temp0 = iter0->second; temp0 != NULL; temp0 = temp0->next)
				{
					insert0 = temp0;
				}
				insert0->next = temp;
				temp->prev = insert0;
			}
			else if (iter0 == BucketAtoB.end())
			{
				BucketNode *head = new BucketNode(-1, NULL, NULL);
				BucketAtoB.insert(std::pair<int, BucketNode *>(updateGain, head));
				head->next = temp;
				temp->prev = head;
			}
		}
		else if (partition == B_PART)
		{
			auto iter = BucketBtoA.find(prevGain);
			BucketNode *temp = iter->second;
			for (temp = iter->second; temp != NULL; temp = temp->next)
			{
				if (temp->index == i)
				{
					break;
				}
			}
			if (temp->next == NULL)
			{
				// delete temp from bucket B
				temp->prev->next = temp->next;
				temp->prev = NULL;
			}
			else if (temp->next != NULL)
			{
				temp->prev->next = temp->next;
				temp->next->prev = temp->prev;
				temp->next = NULL;
				temp->prev = NULL;
			}
			// add temp to bucket B
			auto iter0 = BucketBtoA.find(updateGain);
			if (iter0 != BucketBtoA.end())
			{
				BucketNode *temp0 = iter0->second;
				BucketNode *insert0 = iter0->second;
				for (temp0 = iter0->second; temp0 != NULL; temp0 = temp0->next)
				{
					insert0 = temp0;
				}
				insert0->next = temp;
				temp->prev = insert0;
			}
			else if (iter0 == BucketBtoA.end())
			{
				BucketNode *head = new BucketNode(-1, NULL, NULL);
				BucketBtoA.insert(std::pair<int, BucketNode *>(updateGain, head));
				head->next = temp;
				temp->prev = head;
			}
		}
	}
};

#endif
