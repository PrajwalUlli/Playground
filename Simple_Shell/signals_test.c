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

// Each process has an attribute called the process signal mask which is maintained by the kernel.
// Any signal that is added to the signal mask gets blocked, unless the signal is removed from it.
// The terminal driver detects events (user keypresses). • The kernel then sends the associated signals (e.g., SIGINT, SIGTSTP) to the affected process group. • The shell, being a process itself, may have its own signal handling as part of job control, but the delivery mechanism is provided by the kernel.
// -----
// Once a signal (say, SIGX) is delivered:
// 1. The kernel adds SIGX to the process's signal mask, blocking further SIGX (and any other signals specified by the handler) from immediately interrupting the process.
// 2. The signal handler for SIGX is invoked.
// 3. Any SIGX received while the handler is running are not immediately delivered; they're marked as pending (subject to system-specific queuing behavior).
// 4. After the handler returns, the original signal mask is restored.
// 5. The kernel then checks the pending signals against the current mask and delivers any that are no longer blocked.
// Thus, during a signal handler, further arrivals of blocked signals are deferred, and once the handler is finished, those pending signals will be delivered.

/* Are all signals blocked during the running of a signal handler */
// When a signal handler is running, the kernel automatically blocks the signal that invoked the handler.
// + you can specify a set of additional signals to block for the duration of that handler, these will be marked pending.
// + and can be specified when setting up the handler, (for example, using sigaction), you specify in its configuration (using the sa_mask field)
// Any signals that are not in this blocked set (i.e. neither the signal that triggered the handler nor those explicitly added to sa_mask) are still allowed to be delivered and may run concurrently with—or interrupt—the running handler.
