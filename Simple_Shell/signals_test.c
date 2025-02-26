//SO: https://igupta.in/blog/writing-a-unix-shell-part-3/

#include <stdio.h>
#include <signal.h>

void SigCall(int num)
{
  printf("Signal: %d\n", num);
}

int main(void)
{
  signal(SIGINT, SigCall);
  while (1);
}

/* SIGNALS */
// SIGINT (ctrl+C): Signal Interrupt, defaults to terminate process, but can take a disposition (ie func)
// SIGTSTP (ctrl+Z): Signal Terminal Stop, defaults to stop current process, but can take a disposition (ie func)
// SIG_IGN: Signal Ignore, passed as a disposition to ignore given signal
// SIG_DFL: Signal Default, passed as a disposition to let use default functionality of given signal
// SIGSTOP & SIGKILL: do not take any  disposition other than SIG_DFL, they cannot be caught
