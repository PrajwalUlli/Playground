#include <string.h>
#include <stdio.h>

int main(void)
{
	char command[] = "ls -a"; // Memory: ['l','s',' ','-','a','\0']
	char *parsed;
	char *arr[3];
	char *delim = " ";
	int index = 0;

	parsed = strtok(command, delim); // first call: ls
	while (parsed != NULL)
	{
		arr[index] = parsed;
		index++;
		parsed = strtok(NULL, delim); // continue from last visited pos
	}

	arr[++index] = NULL;
	printf("%s\n", arr[0]);
	printf("%s\n", arr[1]);
	return 0;
}

// The NULL parameter tells strtok to continue from where it left off in the previous call
// strtok maintains an internal static pointer to keep track of its position in the string.
// If we called strtok(command, " ") each time instead: it still parses over the first chars until given delimiter
/*
parsed = strtok(command, " ");
arr[index] = parsed;
parsed = strtok(NULL, " ");
arr[++index] = parsed;
arr[++index] = NULL;
*/

