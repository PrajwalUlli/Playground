#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <readline/readline.h>

char **tokenize(char *input)
{
	char **tokens = malloc(strlen(input) * sizeof(char *));
	if (tokens == NULL)
	{
		perror("Malloc Failed!");
		exit(1);
	}

	char *parsed;
	char *delim = " ";
	int index = 0;
	parsed = strtok(input, delim);
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
	pid_t child_pid, wait_result;

	while (1)
	{
		input = readline("sh$ "); // parent process
		output = tokenize(input);

		child_pid = fork();
		if (child_pid < 0)
		{
			perror("Fork Failed!");
			exit(1); // terminates entire program
		}
		else if (child_pid == 0)
		{
			execvp(output[0], output);
			perror(output[0]);
			exit(1); // only terminates child process
		}
		else
		{
			wait_result = waitpid(child_pid, &status, WUNTRACED); // parent process wait
		}
		free(input);
		free(output); // readline allocates memory using malloc, hence to be freed
	}
	return 0;
}
