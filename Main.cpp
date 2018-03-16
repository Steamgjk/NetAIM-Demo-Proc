#include <stdio.h>
#include "Utils.h"
int main(int argc,char* argv[]){
	printf("Hello\n");
	int client_fd, server_fd;
	if(argc >=2){
		if(argv[1][0] == 'c'){
			client_fd = Utils::on_connect("127.0.0.1",4112, "127.0.0.1", 4111);
			printf("client_fd = %d\n", client_fd );
		}else{
			 server_fd = Utils::pre_connect("127.0.0.1",4111);
			 printf("server_fd = %d\n", server_fd);
		}

	}
}