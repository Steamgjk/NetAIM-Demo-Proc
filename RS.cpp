#include "RS.h"

RS::RS()
{
	h = 1;
	N = 9;
	n = 3;

}
void RS::DataAggregate(DataTuple*dtuple)
{
	int i = dtuple->sta_idx;
	int j = 0;
	int array_id = dtuple->array_id;
	if (dtuple->step < n)
	{
		//is Scatter
		for ( j = 0; j < dtuple->len; j++)
		{
			g_arrays[array_id][i + j] += dtuple->data[j];
		}
	}
	else
	{
		for ( j = 0; j < dtuple->len; j++)
		{
			g_arrays[array_id][i + j] = dtuple->data[j];
		}
	}

}
void RS::SendThread_0()
{
	int right_neighbor_0 = right_neighbors[0];
	client_fd[0] = Utils::on_connect(ipstr_0[rank], ports_0[rank], ipstr_0[right_neighbor_0], ports_0[right_neighbor_0]);
}
void RS::ReceiveThread_0()
{
	server_fd[0] = Utils::pre_connect(ipstr_0[rank], ports_0[rank] );
	int recv_fd = server_fd[0];
	size_t header_size = sizeof(DataTuple);
	size_t buf_len = -1;
	while ( 1 == 1 )
	{
		buf_len = recv(recv_fd, working_memory_0, header_size, 0) ;
		DataTuple* header_dt = static_cast<DataTuple*>(working_memory_0);
		size_t data_len = header_dt->len;
		char* data_buf = static_cast<char*>(working_memory_0) + header_size;
		buf_len = recv(recv_fd, data_buf, data_len, 0) ;
		header_dt->data = static_cast<float*>(static_cast<void*>(data_buf));
		DataAggregate(header_dt);
		header_dt->data = g_arrays[header_dt->array_id] + header_dt->sta_idx;

		/*
				if (header_dt->round == 1)
				{
					int id = header_dt->id;
					int sta_idx = header_dt->sta_idx;
					int level = header_dt->level;

					header_dt->data = g_arrays[id] + sta_idx;
					if (header_dt->step == 2 * (n - 1))
					{
						//FIN
					}
					else
					{
						header_dt->step = header_dt->step + 1;
						if (level == 1)
						{
							send_qu_1.push(header_dt);
						}
						else
						{
							send_qu_0.push(header_dt);
						}

					}
				}
				else if (header_dt->round = 0)
				{
					int id = header_dt->id;
					int sta_idx = header_dt->sta_idx;
					int level = header_dt->level;
					header_dt->data = g_arrays[id] + sta_idx;
					if (header_dt->step == 2 * (n - 1))
					{
						header_dt->step = 0;

						//turn to the other queue
						if (level == 1)
						{
							send_qu_0.push(header_dt);
						}
						else
						{
							send_qu_1.push(header_dt);
						}
					}
					else
					{
						header_dt->step = header_dt->step + 1;
						if (level == 1)
						{
							send_qu_1.push(header_dt);
						}
						else
						{
							send_qu_0.push(header_dt);
						}

					}
				}
				**/
	}
}

void RS::SendThread_1()
{
	int right_neighbor_1 = right_neighbors[1];
	client_fd[1] = Utils::on_connect(ipstr_1[rank], ports_1[rank], ipstr_1[right_neighbor_1], ports_1[right_neighbor_1]);

}
void RS::ReceiveThread_1()
{
	server_fd[1] = Utils::pre_connect(ipstr_1[rank], ports_1[rank] );
	int recv_fd = server_fd[1];
	size_t header_size = sizeof(DataTuple);
	size_t buf_len = -1;
	while ( 1 == 1 )
	{
		buf_len = recv(recv_fd, working_memory_1, header_size, 0) ;
		DataTuple* header_dt = static_cast<DataTuple*>(working_memory_1);
		size_t data_len = header_dt->len;
		char* data_buf = static_cast<char*>(working_memory_1) + header_size;
		buf_len = recv(recv_fd, data_buf, data_len, 0) ;
		header_dt->data = static_cast<float*>(static_cast<void*>(data_buf));
		DataAggregate(header_dt);
	}
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
	working_memory_0 = (char*) malloc(MEM_SIZE);
	working_memory_1 = (char*) malloc(MEM_SIZE);

	this->rank = rank;
	Utils::Val2Addr(rank, this->addr, h, n);
	int peer_array[n];
	int i = 0;
	int right_idx = (rank + 1) % n;
	int left_idx = (rank + n - 1) % n;
	for ( i = 0; i < h; i++)
	{
		Utils::GetPeers(i, this->addr, peer_array, h, n);
		left_neighbors[i] = peer_array[i];
		right_neighbors[i] = peer_array[i];
	}
	thread s0(&RS::SendThread_0, this);
	thread s1(&RS::SendThread_1, this);
	thread r0(&RS::ReceiveThread_0, this);
	thread r1(&RS::ReceiveThread_1, this);
	s0.join();
	s1.join();
	r0.join();
	r1.join();

}
void RS::Sync(float g_array[], int W, int id)
{
	float* allelic_array = (float*)malloc(sizeof(float) * W );
	memcpy(allelic_array, g_array, sizeof(float) * W );
	int array_len = W;
	g_arrays.push_back(allelic_array);
	g_array_lens.push_back(array_len);

	Portion* pitem = (Portion*) malloc(sizeof(Portion));
	float* portion_array[n];
	int portion_lens[n];
	Utils::array_split(allelic_array, array_len, n, pitem->ptr, pitem->portion_lens);
	portion4array_r1.push_back(pitem);

	int rank = this->addr[0];
	int peer_array[n];
	Utils::GetPeers(0, this->addr, peer_array, h, n);
	int right_idx = (rank + 1) % n;
	right_neighbor_0 = peer_array[right_idx];


	rank = this->addr[1];
	Utils::GetPeers(1, this->addr, peer_array, h, n);
	right_idx = (rank + 1) % n;
	right_neighbor_1 = peer_array[right_idx];


	Scatter(rank, allelic_array, array_len, peer_array);

	Gather(rank, allelic_array, array_len, peer_array);


}

RS::~RS()
{

}