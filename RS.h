#ifndef  _RS_H
#define _RS_H
#include "Utils.h"
#include <stdio.h>
#include <string.h>
#include <vector>
#include <string>
#include <queue>
#include <thread>

#define MAX 10
#define MEM_SIZE 10000
#define SERVER_NUM 9
using namespace std;
struct DataTuple
{
	int array_id;
	int round;
	int level;
	int step;
	float* data;
	int sta_idx;
	int len;
};
struct Portion
{
	float* ptr[MAX];
	int portion_lens[MAX];
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
	void DataAggregate(DataTuple*dtuple);
	void Sync(float g_array[], int W, int id);
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

	queue<DataTuple*> send_qu_0;
	queue<DataTuple*> send_qu_1;
	queue<DataTuple*> recv_qu_0;
	queue<DataTuple*> recv_qu_1;
	int addr[MAX];
	int rank;
	int server_fd[MAX];
	int client_fd[MAX];

	void* working_memory_0;
	void* working_memory_1;
	int right_neighbor_0;
	int right_neighbor_1;

	vector<float*> g_arrays;
	vector<int> g_array_lens;

	vector<Portion*> portion4array_r1 ;
	vector<Portion*> portion4array_r2 ;

};

#endif