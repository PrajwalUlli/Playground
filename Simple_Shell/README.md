# Shell Functions

### FORK()
![fork flow diagram](https://github.com/PrajwalUlli/Playground/blob/main/Simple_Shell/images/Fork.png)

### EXECVP()
![execvp flow diagram](https://github.com/PrajwalUlli/Playground/blob/main/Simple_Shell/images/Execvp.png)


### Common `W` Flags for `waitpid()`

| **Flag**         | **Description** |
|------------------|----------------|
| `WNOHANG`       | Do not block; return immediately if no child has exited. |
| `WUNTRACED`     | Return if a child process has stopped (but not terminated). |
| `WCONTINUED`    | Return if a stopped child has resumed execution. |

### Common `W` Macros for Status Checking

| **Macro**            | **Description** |
|----------------------|----------------|
| `WIFEXITED(status)`  | Returns **true** if the child exited normally. |
| `WEXITSTATUS(status)` | Returns the exit status of the child. |
| `WIFSIGNALED(status)` | Returns **true** if the child was terminated by a signal. |
| `WTERMSIG(status)`   | Returns the signal that terminated the child. |
| `WIFSTOPPED(status)` | Returns **true** if the child is currently stopped. |
| `WSTOPSIG(status)`   | Returns the signal that stopped the child. |
