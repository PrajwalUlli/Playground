# Shell Functions

### FORK()
![fork flow diagram](https://github.com/PrajwalUlli/Playground/blob/main/Simple_Shell/images/Fork.png)

### EXECVP()
![execvp flow diagram](https://github.com/PrajwalUlli/Playground/blob/main/Simple_Shell/images/Execvp.png)

###WAITPID()
basic syntax: `waitpid(child_pid, status, 0)`

#### Common `W` Flags for `waitpid()`

| **Flag**         | **Description** |
|------------------|----------------|
| `WNOHANG`       | Do not block; return immediately if no child has exited. (default) |
| `WUNTRACED`     | Return if a child process has stopped (but not terminated). |
| `WCONTINUED`    | Return if a stopped child has resumed execution. |

#### Common `W` Macros for Status Checking

| **Macro**            | **Description** |
|----------------------|----------------|
| `WIFEXITED(status)`  | Returns **true** if the child exited normally. |
| `WIFCONTINUED(status)` | Returns true if the child was resumed (`SIGCONT`). |
| `WIFSTOPPED(status)` | Returns **true** if the child is currently stopped. |
| `WIFSIGNALED(status)` | Returns **true** if the child was terminated by a signal. |
| `WEXITSTATUS(status)` | Returns the exit status of the child. |
| `WTERMSIG(status)`   | Returns the signal that terminated the child. |
| `WSTOPSIG(status)`   | Returns the signal that stopped the child. |

