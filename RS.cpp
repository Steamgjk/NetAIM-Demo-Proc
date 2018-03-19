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
	if (dtuple->step < h * n)
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
int RS::SendDataTuple(int fd, DataTuple* dtp)
{
	int len = dtp->len;
	size_t data_len = sizeof(DataTuple) + sizeof(float) * len ;
	void* data_buf = (void*)malloc(data_len);
	memcpy(data_buf, dtp, data_len);

	DataTuple* dtuple = static_cast<DataTuple*>(data_buf);
	char* ppttr = static_cast<char*>(data_buf) + sizeof(DataTuple);
	dtuple->data = static_cast<float*>(static_cast<void*>(ppttr));

	int ret = send(fd, data_buf, data_len, 0);
	if (ret < 0)
	{
		printf("ret = %d errno =%d\n", ret, errno );
	}
	free(data_buf);
	return ret;

}
void RS::SendThread_0()
{
	int right_neighbor_0 = right_neighbors[0];
	client_fd[0] = Utils::on_connect(ipstr_0[rank], ports_0[rank], ipstr_0[right_neighbor_0], ports_0[right_neighbor_0]);

	while (1 == 1)
	{
		LinkNode* lptr = send_link_0;
		while (send_link_0 != send_link_0_tail)
		{
			lptr = send_link_0->next;
			free(send_link_0);
			send_link_0 = lptr;
			DataTuple* dtp = send_link_0->cur_tuple;
			SendDataTuple(client_fd[0], dtp);
			if (dtp->data != NULL)
			{
				free(dtp->data);
			}
			free(dtp);
		}
	}


}
void RS::ReceiveThread_0()
{
	server_fd[0] = Utils::pre_connect(ipstr_0[rank], ports_0[rank] );
	int recv_fd = server_fd[0];
	size_t header_size = sizeof(DataTuple);
	while ( 1 == 1 )
	{
		int buf_len = recv(recv_fd, working_memory_0, header_size, 0) ;
		if (buf_len < 0)
		{
			printf("Recv Fail\n");
		}
		DataTuple* header_dt = static_cast<DataTuple*>(working_memory_0);
		size_t data_len = header_dt->len;
		if (data_len > 0)
		{
			char* data_buf = static_cast<char*>(working_memory_0) + header_size;
			buf_len = recv(recv_fd, data_buf, data_len, 0) ;
			header_dt->data = static_cast<float*>(static_cast<void*>(data_buf));
		}
		else
		{
			header_dt->data = NULL;
		}
		LinkNode* ritem = (LinkNode*)malloc(sizeof(LinkNode));
		ritem->next = NULL;
		ritem->cur_tuple = header_dt;
		recv_link_0_tail->next = ritem;
		recv_link_0_tail = ritem;
	}
}

void RS::SendThread_1()
{
	int right_neighbor_1 = right_neighbors[1];
	client_fd[1] = Utils::on_connect(ipstr_1[rank], ports_1[rank], ipstr_1[right_neighbor_1], ports_1[right_neighbor_1]);
	while (1 == 1)
	{
		LinkNode* lptr = send_link_1;
		while (send_link_1 != send_link_0_tail)
		{
			lptr = send_link_1->next;
			free(send_link_1);
			send_link_1 = lptr;
			DataTuple* dtp = send_link_1->cur_tuple;
			SendDataTuple(client_fd[1], dtp);
			if (dtp->data != NULL)
			{
				free(dtp->data);
			}
			free(dtp);
		}
	}

}
void RS::ReceiveThread_1()
{
	server_fd[1] = Utils::pre_connect(ipstr_1[rank], ports_1[rank] );
	int recv_fd = server_fd[1];
	size_t header_size = sizeof(DataTuple);
	while ( 1 == 1 )
	{
		int buf_len = recv(recv_fd, working_memory_1, header_size, 0) ;
		if (buf_len < 0)
		{
			printf("Recv Fail _1 \n");
		}
		DataTuple* header_dt = static_cast<DataTuple*>(working_memory_1);
		size_t data_len = header_dt->len;
		if (data_len > 0)
		{
			char* data_buf = static_cast<char*>(working_memory_1) + header_size;
			buf_len = recv(recv_fd, data_buf, data_len, 0) ;
			header_dt->data = static_cast<float*>(static_cast<void*>(data_buf));
		}
		else
		{
			header_dt->data = NULL;
		}


		LinkNode* ritem = (LinkNode*)malloc(sizeof(LinkNode));
		ritem->next = NULL;
		ritem->cur_tuple = header_dt;
		recv_link_1_tail->next = ritem;
		recv_link_1_tail = ritem;
	}
}

RS::RS(int rank)
{
	h = 2;
	N = 9;
	n = 3;
	working_memory_0 = (char*) malloc(MEM_SIZE);
	working_memory_1 = (char*) malloc(MEM_SIZE);

	send_link_0 = (LinkNode*)malloc(sizeof(LinkNode));
	send_link_1 = (LinkNode*)malloc(sizeof(LinkNode));
	recv_link_0 = (LinkNode*)malloc(sizeof(LinkNode));
	recv_link_1 = (LinkNode*)malloc(sizeof(LinkNode));


	int kk = 0;
	int mm = 0;
	for (kk = 0; kk < MAX; kk++)
	{
		steps[kk].resize(CAPICITY);

		for (mm = 0; mm < CAPICITY; mm++)
		{
			steps[kk][mm] = -1;
		}
	}
	g_arrays.resize(CAPICITY);
	g_array_lens.resize(CAPICITY);
	for (kk = 0; kk < CAPICITY; kk++)
	{
		for (mm = 0; mm < MAX; mm++)
		{
			portion_info[kk][mm] = -1;
		}
	}

	this->rank = rank;
	Utils::Val2Addr(rank, this->addr, h, n);
	int peer_array[n];
	int i = 0;
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
	thread b0(&RS::BGThread_0, this);
	thread b1(&RS::BGThread_1, this);

	s0.join();
	s1.join();
	r0.join();
	r1.join();
	b0.join();
	b1.join();

}

void RS::BGThread_0()
{
	while (1 == 1)
	{
		int send_level = 0;
		for (send_level = 0; send_level < 2 * h * (n - 1) * 2; send_level += 2)
		{
			int step_idx = send_level / 2;
			int idx = 0;
			for (idx = 0; idx < CAPICITY; idx++)
			{
				if (steps[send_level][idx] < 0 )
				{
					continue;
				}
				if (idx % 2 == 0)
				{
					if (step_idx == 0 || step_idx == 1 || step_idx == 6 || step_idx == 7)
					{
						int array_id = steps[send_level][idx];
						int portion_sta = portion2send_sta_even[rank][step_idx];
						int portion_len = portion_lens[step_idx];
						int portion_sta_idx = portion_info[idx][portion_sta];
						int portion_end_idx = portion_info[idx][portion_sta + portion_len];
						int data_num = portion_end_idx - portion_sta_idx;
						size_t data_size = data_num * sizeof(float);
						DataTuple* dtuple = (DataTuple*)malloc(sizeof(DataTuple) + data_size);
						dtuple->array_id = array_id;
						dtuple->step = step_idx;
						dtuple->sta_idx = portion_sta_idx;
						char* ptr = static_cast<char*>(static_cast<void*>(dtuple));
						dtuple->data = static_cast<float*>(static_cast<void*>(ptr + sizeof(DataTuple)));
						if (data_size > 0 )
						{
							memcpy(dtuple->data, g_arrays[array_id] + portion_sta_idx, data_size);
						}
						else
						{
							dtuple->data = NULL;
						}

						LinkNode* ln = (LinkNode*) malloc(sizeof(LinkNode));
						ln->cur_tuple = dtuple;
						ln->next = NULL;
						send_link_0_tail->next = ln;
						send_link_0_tail = ln;

						steps[send_level][idx] = -1;
						steps[send_level + 1][idx] = array_id;

					}
				}
				else
				{
					if (step_idx == 2 || step_idx == 3 || step_idx == 4 || step_idx == 5)
					{
						int array_id = steps[send_level][idx];
						int portion_sta = portion2send_sta_odd[rank][step_idx];
						int portion_len = portion_lens[step_idx];
						int portion_sta_idx = portion_info[idx][portion_sta];
						int portion_end_idx = portion_info[idx][portion_sta + portion_len];
						int data_num = portion_end_idx - portion_sta_idx;
						size_t data_size = data_num * sizeof(float);
						DataTuple* dtuple = (DataTuple*)malloc(sizeof(DataTuple) + data_size);
						dtuple->array_id = array_id;
						dtuple->step = step_idx;
						dtuple->sta_idx = portion_sta_idx;
						char* ptr = static_cast<char*>(static_cast<void*>(dtuple));
						dtuple->data = static_cast<float*>(static_cast<void*>(ptr + sizeof(DataTuple)));

						if (data_size > 0)
						{
							memcpy(dtuple->data, g_arrays[array_id] + portion_sta_idx, data_size);
						}
						else
						{
							dtuple->data = NULL;
						}


						LinkNode* ln = (LinkNode*) malloc(sizeof(LinkNode));
						ln->cur_tuple = dtuple;
						ln->next = NULL;
						send_link_0_tail->next = ln;
						send_link_0_tail = ln;


						steps[send_level][idx] = -1;
						steps[send_level + 1][idx] = array_id;
					}
				}


			}


		}
		LinkNode* lptr = recv_link_0;
		while (recv_link_0 != recv_link_0_tail)
		{
			lptr = recv_link_0->next;
			free(recv_link_0);
			recv_link_0 = lptr;
			DataTuple* dtp = recv_link_0->cur_tuple;
			int step_idx = dtp->step;
			int recv_level = step_idx * 2 - 1;
			int array_id = dtp->array_id;
			if (steps[recv_level][array_id] >= 0)
			{
				DataAggregate(dtp);
				steps[recv_level][array_id] = -1;
				steps[recv_level + 1][array_id] = array_id;
			}
			else
			{
				// Cache temprorarily
				LinkNode* cacheitem = (LinkNode*)malloc(sizeof(LinkNode));
				cacheitem->next = NULL;
				cacheitem->cur_tuple = dtp;
				cache_qu_0.push(cacheitem);
			}
		}
		queue<LinkNode*> tmp;
		while (!cache_qu_0.empty())
		{
			LinkNode* item = cache_qu_0.front();
			cache_qu_0.pop();
			DataTuple* dtp = recv_link_0->cur_tuple;
			int step_idx = dtp->step;
			int recv_level = step_idx * 2 - 1;
			int array_id = dtp->array_id;
			if (steps[recv_level][array_id] >= 0)
			{
				DataAggregate(dtp);
				steps[recv_level][array_id] = -1;
				steps[recv_level + 1][array_id] = array_id;
			}
			else
			{
				tmp.push(item);
			}

		}
		while (!tmp.empty())
		{
			LinkNode* item = tmp.front();
			tmp.pop();
			cache_qu_0.push(item);
		}
	}
}
void RS::BGThread_1()
{
	while (1 == 1)
	{
		int send_level = 0;
		for (send_level = 0; send_level < 2 * h * (n - 1) * 2; send_level += 2)
		{
			int step_idx = send_level / 2;
			int idx = 0;
			for (idx = 0; idx < CAPICITY; idx++)
			{
				if (steps[send_level][idx] < 0 )
				{
					continue;
				}
				if (idx % 2 == 1)
				{
					if (step_idx == 0 || step_idx == 1 || step_idx == 6 || step_idx == 7)
					{
						int array_id = steps[send_level][idx];
						int portion_sta = portion2send_sta_odd[rank][step_idx];
						int portion_len = portion_lens[step_idx];
						int portion_sta_idx = portion_info[idx][portion_sta];
						int portion_end_idx = portion_info[idx][portion_sta + portion_len];
						int data_num = portion_end_idx - portion_sta_idx;
						size_t data_size = data_num * sizeof(float);
						DataTuple* dtuple = (DataTuple*)malloc(sizeof(DataTuple) + data_size);
						dtuple->array_id = array_id;
						dtuple->step = step_idx;
						dtuple->sta_idx = portion_sta_idx;
						char* ptr = static_cast<char*>(static_cast<void*>(dtuple));
						dtuple->data = static_cast<float*>(static_cast<void*>(ptr + sizeof(DataTuple)));
						if (data_size > 0)
						{
							memcpy(dtuple->data, g_arrays[array_id] + portion_sta_idx, data_size);
						}
						else
						{
							dtuple->data = NULL;
						}


						LinkNode* ln = (LinkNode*) malloc(sizeof(LinkNode));
						ln->cur_tuple = dtuple;
						ln->next = NULL;
						send_link_1_tail->next = ln;
						send_link_1_tail = ln;


						steps[send_level][idx] = -1;
						steps[send_level + 1][idx] = array_id;
					}
				}
				else
				{
					if (step_idx == 2 || step_idx == 3 || step_idx == 4 || step_idx == 5)
					{
						int array_id = steps[send_level][idx];
						int portion_sta = portion2send_sta_even[rank][step_idx];
						int portion_len = portion_lens[step_idx];
						int portion_sta_idx = portion_info[idx][portion_sta];
						int portion_end_idx = portion_info[idx][portion_sta + portion_len];
						int data_num = portion_end_idx - portion_sta_idx;
						size_t data_size = data_num * sizeof(float);
						DataTuple* dtuple = (DataTuple*)malloc(sizeof(DataTuple) + data_size);
						dtuple->array_id = array_id;
						dtuple->step = step_idx;
						dtuple->sta_idx = portion_sta_idx;
						char* ptr = static_cast<char*>(static_cast<void*>(dtuple));
						dtuple->data = static_cast<float*>(static_cast<void*>(ptr + sizeof(DataTuple) ) );
						if (data_size > 0 )
						{
							memcpy(dtuple->data, g_arrays[array_id] + portion_sta_idx, data_size);
						}
						else
						{
							dtuple->data = NULL;
						}


						LinkNode* ln = (LinkNode*) malloc(sizeof(LinkNode));
						ln->cur_tuple = dtuple;
						ln->next = NULL;
						send_link_1_tail->next = ln;
						send_link_1_tail = ln;


						steps[send_level][idx] = -1;
						steps[send_level + 1][idx] = array_id;
					}
				}
			}
		}
		LinkNode* lptr = recv_link_1;
		while (recv_link_1 != recv_link_1_tail)
		{
			lptr = recv_link_1->next;
			free(recv_link_1);
			recv_link_0 = lptr;
			DataTuple* dtp = recv_link_1->cur_tuple;
			int step_idx = dtp->step;
			int recv_level = step_idx * 2 - 1;
			int array_id = dtp->array_id;
			if (steps[recv_level][array_id] >= 0)
			{
				DataAggregate(dtp);
				steps[recv_level][array_id] = -1;
				steps[recv_level + 1][array_id] = array_id;
			}
			else
			{
				// Cache temprorarily
				LinkNode* cacheitem = (LinkNode*)malloc(sizeof(LinkNode));
				cacheitem->next = NULL;
				cacheitem->cur_tuple = dtp;
				cache_qu_1.push(cacheitem);
			}
		}
		queue<LinkNode*> tmp;
		while (!cache_qu_1.empty())
		{
			LinkNode* item = cache_qu_1.front();
			cache_qu_1.pop();
			DataTuple* dtp = recv_link_1->cur_tuple;
			int step_idx = dtp->step;
			int recv_level = step_idx * 2 - 1;
			int array_id = dtp->array_id;
			if (steps[recv_level][array_id] >= 0)
			{
				DataAggregate(dtp);
				steps[recv_level][array_id] = -1;
				steps[recv_level + 1][array_id] = array_id;
			}
			else
			{
				tmp.push(item);
			}

		}
		while (!tmp.empty())
		{
			LinkNode* item = tmp.front();
			tmp.pop();
			cache_qu_1.push(item);
		}

	}
}

void RS::genPortionInfo(int arr_id)
{
	int len = g_array_lens[arr_id];
	int portion_num = h * (n - 1);
	int portion_base = len / portion_num;
	int residual = len - portion_base * portion_num;
	int i = 0;
	portion_info[arr_id][0] = 0;
	for (i = 1; i < portion_num; i++)
	{
		if (i - 1 < residual )
		{
			portion_info[arr_id][i] = portion_info[arr_id][i - 1] + portion_base + 1;
		}
		else
		{
			portion_info[arr_id][i] = portion_info[arr_id][i - 1] + portion_base ;
		}
	}
	portion_info[arr_id][portion_num] = len;

}
void RS::Sync(float g_array[], int W, int id)
{
	float* allelic_array = (float*)malloc(sizeof(float) * W );
	memcpy(allelic_array, g_array, sizeof(float) * W );
	int array_len = W;
	g_arrays[id] = allelic_array;
	g_array_lens[id] = array_len;

	genPortionInfo(id);

	steps[0][id] = id;

}

RS::~RS()
{

}