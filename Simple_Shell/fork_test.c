//SO: https://igupta.in/blog/writing-a-unix-shell-part-1/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

int main(void)
{
	pid_t child_pid = fork();
	pid_t wait_result;
	int status;

	if (child_pid == 0)
	{
		printf("Command Process...\n");
		sleep(2);
		printf("Parent_Proc: %d, Child_Proc: %d, Command_Proc: %d\n", getppid(), getpid(), child_pid);
		sleep(2);
		// good to include. ensures this block exits first, helps in cases where code structure is not properly defined and this block leaks into the parent block and starts exec-ing its code.
		// It ensures uniformity when multiple exit points exist in a process.
		exit(0);
	}
	else
	{
		printf("Parent: %d\n", getpid());
		printf("Fork created: %d\n", child_pid);
		
		wait_result = waitpid(child_pid, &status, 0);
		
		printf("\ncommand: exit_status (%d), result (%d)\n", status, wait_result);
	}
	return 0;
}
