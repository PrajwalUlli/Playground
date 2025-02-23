#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main(void)
{
	// char *argv[] = {"echo", "hello", NULL};
	char *argv[] = {"ls", "-l", "-a", NULL}; 
	execvp(argv[0], argv);
	printf("Error: %d\n", errno);
	return 0;
}

/*Why the need to pass command name twice*/
// First "ls" (in execvp) - Tells the system which program to execute
// Second "ls" (in argv[0]) - Tells the program itself what name it was called with
// It's a Unix convention that programs expect argv[0] to contain their name
// A program can be called by different names (through symlinks) and might behave differently based on argv[0]
// Programs often use their name (argv[0]) in error messages or help text

/*why doesnt rest of the code run after execvp successful execution*/
// execvp replaces the entire current process with the new program - it's not like a normal function call that returns control
// once execvp("ls", argv) is called, entire program's memory, code, and data are replaced by the 'ls' program
// this can be solved using fork()
