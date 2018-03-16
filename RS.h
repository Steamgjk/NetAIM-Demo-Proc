#ifndef  _RS_H
#define _RS_H
#include "Utils.h"
#include <stdio.h>
#include <string.h>
#include <vector>
#include <string>
#define MAX 10
#define SERVER_NUM 9
using namespace std;
class RS
{
public:
	RS();
	RS(int rank);
	void Scatter(int rank, float*& alleic_array, int& array_len, int peer_array[]);
	void Gather(int rank, float*& alleic_array, int& array_len, int peer_array[]);
	void Sync(float g_array[], int W);
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
	int addr[MAX];
	int rank;


};

#endif