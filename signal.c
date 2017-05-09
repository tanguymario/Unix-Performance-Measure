#include <stdio.h>
#include <string.h>
#include <time.h>
#include "signal.h"

void set_up_signal_handler(
  struct sigaction* const sa,
  void (*f)(int, siginfo_t*, void*),
  const int sig_no)
{
  /* Creation of signal handler */
  memset(sa, '\0', sizeof(*sa));
  sa->sa_sigaction = (*f);
  sa->sa_flags = SA_SIGINFO;

  /* Clear signal set */
  sigemptyset(&sa->sa_mask);

  /* Catch signal */
  if (sigaction(sig_no, sa, NULL) == -1)
    fprintf(stderr, "Error : Cannot handle signal : %d\n", sig_no);
}

void set_up_timer(
  struct sigevent* const se, timer_t* const timer_id,
  const clockid_t clock, int sig_no)
{
  se->sigev_notify = SIGEV_SIGNAL;
  se->sigev_signo = sig_no;
  se->sigev_value.sival_ptr = timer_id;
  if (timer_create(clock, se, timer_id) == -1)
    fprintf(stderr, "Error : Cannot timer_create\n");
}

void start_timer(timer_t* timer_id, const struct timespec* const DELAY)
{
  struct itimerspec it;

  it.it_interval = *DELAY;
  it.it_value = *DELAY;

  if (timer_settime(*timer_id, 0, &it, NULL) == -1)
    fprintf(stderr, "Error : Cannot timer_settime\n");
}

void block_signal(sigset_t* const mask, const int sig_no)
{
  sigaddset(mask, sig_no);
  if (sigprocmask(SIG_SETMASK, mask, NULL) == -1)
    fprintf(stderr, "Error : Cannot block signal : %d\n", sig_no);
}

void unblock_signals(sigset_t* const mask)
{
  if (sigprocmask(SIG_UNBLOCK, mask, NULL) == -1)
    fprintf(stderr, "Error : Cannot unblock sigset mask");
}
