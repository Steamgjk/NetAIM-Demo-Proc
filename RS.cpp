#include "RS.h"

RS::RS()
{
	h = 1;
	N = 9;
	n = 3;
}
void SendThread()
{

}
void ReceiveThread()
{

}
void RS::Scatter(int rank, float*& alleic_array, int& array_len, int peer_array[])
{

	float* portion_array[n];
	int portion_lens[n];
	Utils::array_split(alleic_array, array_len, n, portion_array, portion_lens);
	int right_idx = (rank + 1) % n;
	int left_idx = (rank + n - 1) % n;
	int right_neighbor = peer_array[right_idx];
	int left_neighbor = peer_array[left_idx];
	int step = 1;
	for (step = 1; step <= n - 1; step++)
	{
		int index = (rank + n - step) % n;
		float* portion_send = portion_array[index];
		int portion_send_len = portion_lens[index];
		//To Send

		//To Recv and Merge
	}
	//alleic_array = portion_array[rank];
	//array_len =

}
void RS::Gather(int rank, float*& alleic_array, int& array_len, int peer_array[])
{
	int right_idx = (rank + 1) % n;
	int left_idx = (rank + n - 1) % n;



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