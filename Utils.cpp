#include "Utils.h"
int Utils::pre_connect(string ip, int port){
	printf("pre_connect\n");
	int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET; //ipv4
	sin.sin_port = htons(port);//server listen public ports
	sin.sin_addr.s_addr = INADDR_ANY;//listen any connects
	while (bind(listen_fd, (struct sockaddr*)&sin, sizeof(sin)) < 0)
	{
		//std::cerr << "error in bind socket" << std::endl;
		printf("Error in bind  %d\n", port  );
		sleep(1);
		//exit(0);
	}

	if (listen(listen_fd, 1) == -1)
	{
		std::cerr << "error in server listen..." << std::endl;
		exit(-1);
	}
	printf("Listening... %d\n", port );
	//getchar();
	struct sockaddr_in client_addr;
	int connected_fd, addr_len;
	addr_len = sizeof(struct sockaddr_in);
	connected_fd = accept(listen_fd, (struct sockaddr*) & (client_addr), (socklen_t*)&addr_len);
	return connected_fd;
}
int Utils::on_connect(string local_ip, int local_port,\
							 string remote_ip, int remote_port){
	printf("on_connect  %s %s  %d\n", local_ip.c_str(), remote_ip.c_str(), remote_port );
	struct sockaddr_in ser_in, local_in;/*server ip and local ip*/
	memset(&ser_in, 0, sizeof(ser_in));
	memset(&local_in, 0, sizeof(local_in));

	int tmp_skfd = socket(AF_INET, SOCK_STREAM, 0);/*local socket*/

	/*bind remote socket*/
	ser_in.sin_family = AF_INET;
	ser_in.sin_port = htons(remote_port);/*connect to public port remote*/
	inet_pton(AF_INET, remote_ip.c_str(), &ser_in.sin_addr);

	/*bind local part*/
	local_in.sin_family = AF_INET;
	//local_in.sin_port = htons();/*server listen public ports*/
	//local_in.sin_addr.s_addr = INADDR_ANY;/*listen any connects*/
	inet_pton(AF_INET, local_ip.c_str(), &local_in.sin_addr);



	while (bind(tmp_skfd, (struct sockaddr*) & (local_in), sizeof(local_in)) != 0)
	{
		printf("error in bind local addr\n");
		sleep(1);
	}

	if (1)/*connect to server*/
	{
		while (connect(tmp_skfd, (struct sockaddr*) & (ser_in), sizeof(ser_in)) != 0)
		{
			printf("Connected Failed\n");
			sleep(1);

		}

	}
	return tmp_skfd;
}

void Utils::GetPeers(int level, int addr[], int peers[], int k, int n){
	int i =0;
	for(i = 0; i< n-1; i++){
		addr[level] = i;
		peers[i] =  Utils::Addr2Val(addr, k, n);
	}
}
int Utils::Addr2Val(int addr[], int k, int n){
	int sum = 0;
	int i = 0;
	for( i = k-1; i >=0 ; i--){
		sum = sum*n + addr[i];
	}
	return sum;
}

void Utils::Val2Addr(int val, int addr[], int k, int n){
	int i = k -1;
	for(i = k-1; i>= 0; i--){
		addr[i] = 0;
	}
	for(i = 0; i< k; i++){
		addr[i] = val%n;
		val = val/n;
	}

}