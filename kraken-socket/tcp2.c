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
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(10000);
	addr.sin_addr.s_addr = inet_addr("192.168.0.218");
	int addrlen = sizeof(addr);

//	while (1)
//	{
		connect(fd, (struct sockaddr*)&addr, addrlen);
		char *buf = malloc(sizeof(char)*100);

		read(fd, buf, 100);	
		printf("receive: %s %s\n", buf, inet_ntoa(addr.sin_addr));
		fflush(stdout);
		free(buf);
//	}
}

