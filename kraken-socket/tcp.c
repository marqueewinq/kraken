# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <string.h>
# include <signal.h>

void h (int a)
{
//	printf("sig\n");
//	fflush(stdout);
	while (waitpid(-1, NULL, WNOHANG) > 0);
//	{}
}

int main ()
{
//	int pid = fork()
//	{

	struct sigaction p;
	p.sa_handler = h;
	sigaction(SIGCHLD, &p, NULL);


	int fd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(10000);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(fd, ((struct sockaddr*)&addr), sizeof(addr));

	listen(fd, 10);
	int fd1;
	
	struct sockaddr_in addr2;
	int addrlen = sizeof(addr2);

	printf("before while\n");
	while (1)
	{	
		fd1 = accept(fd, (struct sockaddr*)&addr2, &addrlen);
//		printf("%d\n", fd1);
//		printf("uuuu\n");
		fflush(stdout);
		if (fd1 > 0)
		{
			printf("%d\n", fd1);
			int t = fork();
			if (!t)
			{
//				printf("child\n");			
				fflush(stdout);
				int i =0;				
//				for (i = 0; i < 5; i++)
//				{
//					char buf[] = "nyanya\n";
					char * buf = malloc(sizeof(char)*100);
					gets(buf);
					write(fd1, buf, strlen(buf));	
					printf("send: %s %s\n", buf, inet_ntoa(addr2.sin_addr));
					fflush(stdout);
					free(buf);
//				}
				close(fd1);				
				return 0;			
			}
			else
			{
//				printf("par\n");
				fflush(stdout);

			}
		}
	}
}


