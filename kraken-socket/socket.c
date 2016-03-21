# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <string.h>

int main ()
{
//	int pid = fork()
//	{
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(10000);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(fd, ((struct sockaddr*)&addr), sizeof(addr));
	int len = 100, addrlen = sizeof(addr);

	while (1)
	{		
		char * buf = calloc(len, sizeof(char));
		recvfrom(fd, buf, len, 0, (struct sockaddr*)&addr, &addrlen);		
		printf("%s\n", buf);
		free(buf);	
		//sleep(2);
		}
}


