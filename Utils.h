#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <map>
#include <string>
#include <vector>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <sstream>
#include <stdlib.h>
#include <math.h>
using namespace std;
class Utils
{
public:
	static int pre_connect(string ip, int port);
	static int on_connect(string local_ip, int local_port, string remote_ip, int remote_port);
	static void GetPeers(int level, int addr[], int peers[], int k, int n);
	static int Addr2Val(int addr[], int k, int n);
	static void Val2Addr(int val, int addr[], int k, int n);
	static void array_split(float*& arr, int arr_len, int n, float* portion_array[], int portion_lens[]);
};

#endif