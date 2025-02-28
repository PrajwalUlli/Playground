//SO: https://igupta.in/blog/writing-a-unix-shell-part-2/
//SO: https://igupta.in/blog/writing-a-unix-shell-part-3/

#include <nob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <setjmp.h>
#include <sys/wait.h>
#include <readline/readline.h>

#define INITIAL_CAP 8
#define NOB_IMPLEMENTATION

static sigjmp_buf env;
static volatile sig_atomic_t jump_active = 0;

void sigint_handler(int signo)
{
	siglongjmp(env, 42);
}

char **tokenize(char *input)
{
	// If input is NULL, calling strlen on it will cause a segmentation fault, hence NULL check before len check
	if (input == NULL || strlen(input) == 0)
	{
		// Allocate memory for one pointer (for the NULL terminator)
		char **empty_tokens = malloc(sizeof(char *));
		if (empty_tokens == NULL)
		{
			perror("Malloc failed for empty_tokens!");
			exit(1);
		}
		empty_tokens[0] = NULL;
		return empty_tokens;
	}

	// 8*char* more memory efficient not over-allocating based and enough for typical commands but not wasteful
	char **tokens = malloc(INITIAL_CAP * sizeof(char *));

	if (tokens == NULL)
	{
		perror("Malloc failed for tokens!");
		exit(1);
	}

	char *parsed;
	char *delim = " ";
	int index = 0;
	// returns a pointer to the token
	parsed = strtok(input, delim); // replaces delims with a null char '\0'
	while (parsed != NULL)
	{
		tokens[index] = parsed;
		parsed = strtok(NULL, delim);
		index++;
	}
	tokens[index] = NULL;
	return tokens;
}

int main(void)
{
	char *input;
	char **output;

	int status;
	pid_t child_pid;

	while (1)
	{
		signal(SIGINT, sigint_handler);

		// important to place it before the input/output identifiers
		if (sigsetjmp(env, 1) == 42)
		{
			printf("\n");
			continue;
		}
		jump_active = 1;

		/* Parent Process */
		input = readline("pcsh$ ");
		output = tokenize(input);

		if (input == NULL || strlen(input) == 0 || output == NULL)
		{
			free(input);
			free(output);
			continue;
		}

		if (strcmp(output[0], "exit") == 0)
		{
			free(input);
			free(output);
			exit(0);
		}

		if (strcmp(output[0], "cd") == 0)
		{
			if (output[1] == NULL)
			{
				printf("cd: No path specified\n");
				continue;
			}
			// chdir not only returns the status but also sets errno to some +ve int to indicate the specific error.
			int result = chdir(output[1]);
			if (result < 0)
			{
				perror(output[1]);
				continue;
			}
			free(input);
			free(output);
			// no exit(0); here as it will exit the main program, as we are not creating a fork() here
			continue;
		}

		child_pid = fork();
		if (child_pid < 0)
		{
			perror("Fork Failed!");
			exit(1); // terminates entire program
		}
		else if (child_pid == 0)
		{
			/* Child Process */
			signal(SIGINT, SIG_DFL); // call before execvp so that the signal configs inherited from the parent process is removed for this particular signal
			execvp(output[0], output);
			perror(output[0]);
			exit(1); // only terminates child process
		}
		else
		{
			// printf("Hi, parent here!\n");
			waitpid(child_pid, &status, WUNTRACED); // parent process wait
		}
		free(input);
		free(output); // readline allocates memory using malloc so its to be freed
	}
	return 0;
}

/* why some cmds run (like pwd, ls, ps, etc) and not others (like cd, ...) */
// Commands like pwd, ls, and ps are external programs. When you type one of these, your shell forks a child process and then calls execvp, which replaces the child process with the external program. Since these programs are separate binaries (located in /bin, /usr/bin, etc.), they run without any special handling in your shell.
// In contrast, cd is a shell built-in command. Changing directories must affect your shell process (the parent), not a child process. That's why you check for "cd" before forking and then call chdir() in the parent. If you were to run cd by forking and exec'ing an external command, the working directory change would happen only in the child; when it exits, your shell's working directory would stay the same.
