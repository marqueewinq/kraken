
// ------------------------------- // 
// Kraken's Shell r3               //
// Small command shell for Unix    //
//	(c) marqueewinq, 2013          //
// 	[marqueewinq@gmail.com]        // 
// ASCII-art by (c) jgs            // 
// ------------------------------- //

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// #define bug

#define mq_push_back_c(cont, sz, elem) \
	(cont) = realloc(cont, sizeof(elem) * (++(sz) + 2));\
	if ((cont) == NULL)\
		mq_memleak();\
	(cont)[(sz) - 1] = elem;\
	(cont)[(sz)] = 0
	
#define mq_push_back_s(cont, sz, elem) \
	(cont) = realloc(cont, sizeof(elem) * (++(sz) + 2));\
	if ((cont) == NULL)\
		mq_memleak();\
	(cont)[(sz) - 1] = malloc(0);\
	strcpy((cont)[(sz) - 1], elem);\
	(cont)[(sz)] = 0

#define mq_resize(cont, sz, sz_new, etype) \
	cont = realloc(cont, sizeof(etype) * (sz_new + 2));\
	if (cont == NULL)\
		mq_memleak();\
	sz = (sz_new);\
	cont[0] = 0

char  name[255]    = "mq",
      domain[255]  = "botanstvo";

int pics = 0;
char **arg;
size_t arg_sz;
int (*pip)[2];

void mq_pgreet()
{
	printf("\tWelcome to kraken's shell.\n\tFeel warm and cozy inside the monster's body.\n");
	if (pics)
	{
		if (name[0] % 2)
		{
// code generated by ascii-to-printf by marqueewinq
printf("              _..--~~--.._\n");
printf("           .-~`            `~-.\n");
printf("          / .------.  .------. \\\n");
printf("         / (       ()(       () \\\n");
printf("         \\  '------'  '------'  /\n");
printf("          )                    (\n");
printf("         /  .-.  .-.  .-.  .-.  \\\n");
printf("        (  (  /  /  )  ) \\  \\ )  )\n");
printf("         \\  \\/  / \\/  /  /\\  /  /\n");
printf("          \\  \\ /\\ (  (  /  )/  /\n");
printf("           )  )  ) )  )(  /(  (\n");
printf("          /  /( / /  /  \\/  \\  \\\n");
printf("         /  /  \\ /  / ) /  / \\  \\\n");
printf("        (  ( \\  \\  / / (  ( \\ )  )\n");
printf("         \\  \\ \\  \\/ /  /)  ) /  /\n");
printf("          \\__) \\  \\(  (/  / (__/\n");
printf("            /  /)  )\\ (  ( \\  \\\n");
printf("           (  ((__// \\ \\  \\ )  )\n");
printf("            \\__)  /  /\\ \\__)  /\n");
printf("                 (  (  )  )  (\n");
printf("                  \\__)/  / \\__)\n");
printf("                     (__/\n");
// generated code finishes here
		}
		else
		{
// code generated by ascii-to-printf by marqueewinq
printf("                       ___\n");
printf("                    .-'   `'.\n");
printf("                   /         \\\n");
printf("                   |         ;\n");
printf("                   |         |           ___.--,\n");
printf("          _.._     |0) ~ (O) |    _.---'`__.-( (_.\n");
printf("   __.--'`_.. '.__.\\    '--. \\_.-' ,.--'`     `\"\"`\n");
printf("  ( ,.--'`   ',__ /./;   ;, '.__.'`    __\n");
printf("  _`) )  .---.__.' / |   |\\   \\__..--\"\"  \"\"\"--.,_\n");
printf(" `---' .'.''-._.-'`_./  /\\ '.  \\ _.-~~~````~~~-._`-.__.'\n");
printf("       | |  .' _.-' |  |  \\  \\  '.               `~---`\n");
printf("        \\ \\/ .'     \\  \\   '. '-._)\n");
printf("         \\/ /        \\  \\    `=.__`~-.\n");
printf("         / /\\         `) )    / / `\"\".`\\\n");
printf("   , _.-'.'\\ \\        / /    ( (     / /\n");
printf("    `--~`   ) )    .-'.'      '.'.  | (\n");
printf("           (/`    ( (`          ) )  '-;\n");
printf("            `      '-;         (-'\n");
// generated code finishes here

		}
	}
	printf("\tType .help for list of available commands\n");
}
void mq_ppfx()
{
	printf("\n %s @ %s > ", name, domain);
}
void mq_login()
{
	printf(" nicknm  > ");
	scanf("%s", name);
	printf(" domain > ");
	scanf("%s", domain);
	printf(" Login OK\n");
	mq_pgreet();
}
void mq_memleak()
{
// code generated by ascii-to-printf by marqueewinq
printf("  ERROR OCCURED!      ,____\n");
printf("                      |---.\\\n");
printf("              ___     |    `\n");
printf("             / .-\\  ./=)\n");
printf("            |  |\"|_/\\/|\n");
printf("            ;  |-;| /_|\n");
printf("           / \\_| |/ \\ |\n");
printf("          /      \\/\\( |\n");
printf("          |   /  |` ) |\n");
printf("          /   \\ _/    |\n");
printf("         /--._/  \\    |\n");
printf("         `/|)    |    /\n");
printf("           /     |   |\n");
printf("         .'      |   |\n");
printf("        /         \\  |\n");
printf("       (_.-.__.__./  /\n");
// generated code finishes here

	perror("");

	exit(1);
}

int mq_is_escape(char c)
{
	return c == ';'|| c == '\n';	
}
int mq_is_delim(char c)
{
	return c == ' ' || c == ';';
}
int mq_is_delim_s(char* s)
{
	if (mq_is_delim(s[0]))
		return 1;
	if (strcmp(s, "||") == 0 || strcmp(s, "&&") == 0 
	 || strcmp(s, "&")  == 0 || strcmp(s, "|")  == 0)
		return 1;
	return 0;
}

int mq_ssplit(char *cmd)
{
#ifdef bug
	puts("mq_ssplit started");
#endif
	if (cmd == NULL)
		return 1;
		
	char* word = malloc(sizeof(char*));
	size_t word_sz = 0;

	char* tmp = malloc(sizeof(char) * 2);

	size_t cmd_sz = strlen(cmd);

#ifdef bug
	puts("\tLooping");
#endif

	for (size_t i = 0; i < cmd_sz;	i++)
	{
		if (i == cmd_sz - 1 || mq_is_delim(cmd[i]))	
		{
			if (word != NULL && word_sz != 0)
			{		 
#ifdef bug
				printf("pushing '%s'\n", word);
#endif
				mq_push_back_s(arg, arg_sz, word);
#ifdef bug
				puts("\tpush_back ok");
#endif
			
			word = realloc(word, 0), word_sz = 0;
			}
#ifdef bug
			else
				puts("\t NO push_back");
#endif

#ifdef bug
			printf("\tdelim: '%c'\n", cmd[i]);
			puts("");
#endif	
		
			if (cmd[i] != ' ' && cmd[i] != '\n')
			{
				char *tmp = malloc(sizeof(char));
				tmp[0] = cmd[i];
				mq_push_back_c(arg, arg_sz, tmp);
#ifdef bug
				puts("\tescape catched");
#endif
			}

#ifdef bug
			puts("\tsuccess");
#endif
		}
		else
		{
			mq_push_back_c(word, word_sz, cmd[i]);
#ifdef bug
			printf("\tfetchc: %c", cmd[i]);
			puts("");
#endif
		}
	}
	
#ifdef bug
	printf("%d total\n", arg_sz);
	for (size_t i = 0; i < arg_sz; i++)
	{
		printf("'%s', ", arg[i]);	
	}
	printf(" OK\n");
#endif

	return 0;
}

void form_lp(char* line, char*** path, size_t* psz, int* ii)
{
	int i = *ii;
	int path_sz = 0;
//	free(*pth);
	
	mq_push_back_s(*path, path_sz, line);
	do
	{
		if (i >= arg_sz || arg[i] == NULL || mq_is_delim_s(arg[i]))
			break;
		mq_push_back_s(*path, path_sz, arg[i]);
#ifdef bug
		printf("\t%d '%s' ", i, arg[i]); puts("stcated");
#endif
		i++;
		if (i >= arg_sz || arg[i] == NULL)
			break;
	} while (!mq_is_delim_s(arg[i]));
	*ii = i;
	*psz = path_sz;
#ifdef bug
	puts("form_lp finished");
#endif
}

int main(void)
{
	/* initializing */
	printf("..Creating nothing from nothing...\n");
	char *cmd = NULL;
	printf("..Drilling tubes...\n");
	pip = malloc(0);
	if (pip < 0)
		mq_memleak();
	printf("..Initializing social disturbances...\n");
//	mq_login();
	
	if (strcmp(name, "bill") == 0 || strcmp(name, "gates") == 0)
		mq_memleak();
	
	do
	{
		// shell here

		sleep(0);
		mq_ppfx();
		
		/* reading user's input */
		cmd = malloc(sizeof(char) * 511);
		cmd = fgets(cmd, 511, stdin);
#ifdef bug
		printf("Received line of length %d: \n'%s'\n", strlen(cmd), cmd);
#endif

		/* checking on shell special commands ".*" */
		if (strlen(cmd) <= 1)
			continue;
		if (strcmp(cmd, ".login\n") == 0)
		{
			mq_login();
			continue;
		}
		if (strcmp(cmd, ".exit\n") == 0 || strcmp(cmd, ".e\n") == 0)
		{
			printf("\t bye!\n");
			return 0;	
		}
		if (strcmp(cmd, ".v\n") == 0 || strcmp(cmd, ".version\n") == 0)
		{
			printf("Kraken's Shell v0.3 (c) by marqueewinq @ phystech\n");
			printf("Feel free to mail me: marqueewinq@gmail.com\n");
			printf("All ASCII-art by (c) jgs. Visit http://www.geocities.com/spunk1111/	for his gallery :)\n");
			continue;
		}
		if (strcmp(cmd, ".g\n") == 0 || strcmp(cmd, ".greet\n") == 0)
		{
			  mq_pgreet();
			  continue;
		}
		if (strcmp(cmd, ".help\n") == 0)
		{
			printf("This is help button\n");
			printf("\t .v .version\n");
			printf("\t .e .exit\n");
			printf("\t .login\n");
			printf("\t .g .greet\n");
			continue;
		}

		arg = malloc(sizeof(char*));
		arg_sz = 0;

		if (mq_ssplit(cmd) != 0)
			printf("\n\tError occured!\n");			
			
#ifdef bug
		for (int i = 0; i < arg_sz; i++)
		{
			printf("'%s', ", arg[i]);
		}
		puts("");
#endif

#ifdef bug
		puts("Init exec_lp");
#endif
		for (int i = 0; i < arg_sz; i++)
		{
			if (mq_is_delim_s(arg[i]))
				continue;
			char* line = malloc(sizeof(arg[i]));
			strcpy(line, arg[i]);
			
			char** path = malloc(sizeof(char*));
			size_t path_sz = 0;

#ifdef bug
			printf("\ti = %d, arg_sz = %d ", i, arg_sz);
			puts("");
#endif
			i++;
			form_lp(line, &path, &path_sz, &i);

#ifdef bug
			printf("executing: \npath:\n");
			for (int i = 0; i < path_sz; i++)
			{
				printf("\t\t[%s]\n", path[i]);
			}
			printf("\n line: '%s'\n", line);
			puts("");
#endif

			int background = 0;
			if (i < arg_sz && strcmp(arg[i], "&") == 0)
				background = 1;
			
			int fdi = STDIN_FILENO;
			if (i < arg_sz && strcmp(arg[i], "<") == 0)
			{
				if (i + 1 >= arg_sz)
				{
					printf("[kraken]: incorrect syntax near argument #%d", i);
					continue;
				}
				
				// redirect to arg[i + 1]
				fdi = open(arg[i + 1], 0);
			}
			int fdo = STDOUT_FILENO;
			if (i < arg_sz && (strcmp(arg[i], ">") == 0 || strcmp(arg[i], ">>") == 0))
				// redirect output
			{
				if (i + 1 >= arg_sz)
				{
					printf("[kraken]: incorrect syntax near argument #%d", i);
					continue;
				}
				
				// redirect to arg[i + 1]
				if (strcmp(arg[i], ">>") == 0))
					fdo = open(arg[i + 1], 0, O_CREAT | O_APPEND);
				else
					fdo = open(arg[i + 1], 0, O_CREAT);
			}
 
			// run
			int pid = fork();
			int retval = 0;
			if (pid != 0)
			{
				printf("[kraken]: '%s' run @ %d\n", line, pid);
				if (background == 0)
					waitpid(pid, &retval, 0);
			}
			else
			{
				dup2(STDIN_FILENO, fdi);
				dup2(STDOUT_FILENO, fdo);
				execvp(line, path);
				
				printf("[kraken]: error occured while executing '%s'", line);
				return 0;
			}
			if (i >= arg_sz)
				continue;
			
			if (strcmp(arg[i], "&&") == 0)
			{
#ifdef bug
			puts("\t&& catched");
			printf("retval %d", retval);
			puts("");
#endif
				if (!WIFEXITED(retval))
				{
					i++;
#ifdef bug
					puts("\t SKIPPING:");
#endif
					form_lp(line, &path, &path_sz, &i); // skip next
#ifdef bug
					printf("\tskipped: \npath:\n");
					for (int i = 0; i < path_sz; i++)
					{
						printf("\t\t'%s'\n", path[i]);
					}
					printf("\n line: '%s'\n", line);
					puts("");
#endif
					continue;	
				}
			}
			
			if (strcmp(arg[i], "||") == 0)
			{
#ifdef bug
			puts("\t|| catched");
			printf("retval %d", retval);
			puts("");
#endif
				if (WIFEXITED(retval))
				{
					i++;
#ifdef bug
					puts("\t SKIPPING:");
#endif
					form_lp(line, &path, &path_sz, &i); // skip next	
#ifdef bug
					printf("\tskipped: \npath:\n");
					for (int i = 0; i < path_sz; i++)
					{
						printf("\t\t'%s'\n", path[i]);
					}
					printf("\n line: '%s'\n", line);
					puts("");
#endif
					continue;
				}
			}
			
#ifdef bug
			puts("\t ++");
#endif			
		}

#ifdef bug
		puts("\n\t continue");
#endif
		
	} while (strcmp(cmd, "exit") != 0);

	return 0;
}

/* eof */
