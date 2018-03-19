#ifndef  _RS_H
#define _RS_H
#include "Utils.h"
#include <stdio.h>
#include <string.h>
#include <vector>
#include <string>
#include <queue>
#include <thread>

#define MAX 16
#define MEM_SIZE 10000
#define SERVER_NUM 9
#define CAPICITY 100
#define STEP_NUM 8
using namespace std;
struct DataTuple
{
	int array_id;
	//int round;
	//int level;
	int step;
	float* data;
	int sta_idx;
	int len;
};
struct Portion
{
	int sta_idx;
	int len;
};
struct LinkNode
{
	DataTuple* cur_tuple;
	struct LinkNode* next;
};
class RS
{
public:
	RS();
	RS(int rank);
	void Scatter(int rank, float*& alleic_array, int& array_len, int peer_array[]);
	void Gather(int rank, float*& alleic_array, int& array_len, int peer_array[]);
	void SendThread_0();
	void SendThread_1();
	void ReceiveThread_0();
	void ReceiveThread_1();
	void BGThread_0();
	void BGThread_1();
	void DataAggregate(DataTuple*dtuple);
	int SendDataTuple(int fd, DataTuple* dtp);
	void Sync(float g_array[], int W, int id);
	void genPortionInfo(int arr_id);
	~RS();
private:
	int N;
	int h;
	int n;
	string ipstr_0[SERVER_NUM] = {"127.0.0.1", "127.0.0.1", "127.0.0.1", \
	                              "127.0.0.1", "127.0.0.1", "127.0.0.1", "127.0.0.1", "127.0.0.1", "127.0.0.1"
	                             };
	string ipstr_1[SERVER_NUM] = {"127.0.0.1", "127.0.0.1", "127.0.0.1", \
	                              "127.0.0.1", "127.0.0.1", "127.0.0.1", "127.0.0.1", "127.0.0.1", "127.0.0.1"
	                             };
	int ports_0[SERVER_NUM] = {4110, 4111, 4112, 4113, 4114, 4115, 4116, 4117, 4118};
	int ports_1[SERVER_NUM] = {4210, 4211, 4212, 4213, 4214, 4215, 4216, 4217, 4218};
	int left_neighbors[MAX];
	int right_neighbors[MAX];

	int portion2send_sta_even[SERVER_NUM][STEP_NUM] = { \
		{6, 3, 2, 1, 0, 2, 0, 6}, {0, 6, 5, 4, 3, 5, 3, 0}, {3, 0, 8, 7, 6, 8, 6, 3}, \
		{6, 3, 0, 2, 1, 0, 0, 6}, {0, 6, 3, 5, 4, 3, 3, 0}, {3, 0, 6, 8, 7, 6, 6, 3}, \
		{6, 3, 1, 0, 2, 1, 0, 6}, {0, 6, 4, 3, 5, 4, 3, 0}, {3, 0, 7, 6, 8, 7, 6, 3}
	};

	int portion2send_sta_odd[SERVER_NUM][STEP_NUM] = { \
		{6, 3, 2, 1, 0, 2, 0, 6}, {6, 3, 0, 2, 1, 0, 0, 6}, {6, 3, 1, 0, 2, 1, 0, 6}, \
		{0, 6, 5, 4, 3, 5, 3, 0}, {0, 6, 3, 5, 4, 3, 3, 0}, {0, 6, 4, 3, 5, 4, 3, 0}, \
		{3, 0, 8, 7, 6, 8, 6, 3}, {3, 0, 6, 8, 7, 6, 6, 3}, {3, 0, 7, 6, 8, 7, 6, 3}
	};

	int portion_lens[STEP_NUM] = {3, 3, 1, 1, 1, 1, 3, 3};



	LinkNode* send_link_0;
	LinkNode* send_link_1;
	LinkNode* recv_link_0;
	LinkNode* recv_link_1;

	queue<LinkNode*>cache_qu_0;
	queue<LinkNode*>cache_qu_1;

	LinkNode* send_link_0_tail;
	LinkNode* send_link_1_tail;
	LinkNode* recv_link_0_tail;
	LinkNode* recv_link_1_tail;


	int addr[MAX];
	int rank;
	int server_fd[MAX];
	int client_fd[MAX];

	void* working_memory_0;
	void* working_memory_1;


	vector<float*> g_arrays;
	vector<int> g_array_lens;

//ToSend-ToRecv-ToSend-ToRecv...
	vector<int> steps[MAX];


	int portion_info[CAPICITY][MAX];


};

#endif