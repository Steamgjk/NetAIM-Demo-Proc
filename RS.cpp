#include "RS.h"

RS::RS()
{
	h = 1;
	N = 9;

}
void RS::Scatter(int rank, float*& alleic_array, int& array_len, int peer_array[])
{

}
void RS::Gather(int rank, float*& alleic_array, int& array_len, int peer_array[])
{

}
RS::RS(int rank)
{
	h = 2;
	N = 9;
	n = 3;
	this->rank = rank;
	Utils::Val2Addr(rank, this->addr, h, n);


}
void RS::Sync(float g_array[], int W)
{
	int i = 0;
	float* allelic_array = g_array;
	int array_len = W;
	for ( i = 0; i < h ; i++)
	{
		int rank = this->addr[i];
		int peer_array[n];
		Utils::GetPeers(i, this->addr, peer_array, h, n);
		Scatter(rank, allelic_array, array_len, peer_array);
	}
	for (int i = h - 1; i >= 0 ; i--)
	{
		int rank = this->addr[i];
		int peer_array[n];
		Utils::GetPeers(i, this->addr, peer_array, h, n);
		Gather(rank, allelic_array, array_len, peer_array);
	}

}

RS::~RS()
{

}