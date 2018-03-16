#ifndef  _RS_H
#define _RS_H

#include <stdio.h>
#include <string.h>
#include <vector>

#define MAX 10
using namespace std;
class RS
{
public:
	RS();
	void Sync();
	~RS();
private:
	int N;
	int h;
	char* ipstr_0[] = {"127.0.0.1", "127.0.0.1","127.0.0.1", \
	"127.0.0.1","127.0.0.1", "127.0.0.1","127.0.0.1","127.0.0.1", "127.0.0.1"};
	char* ipstr_1[] = {"127.0.0.1", "127.0.0.1","127.0.0.1", \
	"127.0.0.1","127.0.0.1", "127.0.0.1","127.0.0.1","127.0.0.1", "127.0.0.1"};
	int ports_0[]={4110,4111,4112,4113,4114,4115,4116,4117,4118};
	int ports_1[]={4210,4211,4212,4213,4214,4215,4216,4217,4218};
	
	
};

#endif 