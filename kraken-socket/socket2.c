# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <string.h>

int main ()
{
//	scanf("%s", str);
	int fd = socket(AF_INET, SOCK_DGRAM, 0);	
	struct sockaddr_in addr1, addr2;
	addr1.sin_family = AF_INET;
	addr1.sin_port = htons(0);
	addr1.sin_addr.s_addr = htonl(INADDR_ANY);

	addr2.sin_family = AF_INET;
	addr2.sin_port = htons(10000);
	addr2.sin_addr.s_addr = inet_addr("192.168.0.218");

	bind(fd, ((struct sockaddr*)&addr1), sizeof(addr1));

	int len = 10, addrlen = sizeof(addr2);
//	char buf[] = "ururu";
	while (1)
	{
		char * str = malloc(sizeof(char)* 100);		
		gets(str);
		sendto(fd, str, strlen(str), 0, (struct sockaddr*)&addr2, addrlen);
		free(str);		
//	sleep(1);
	}
}


