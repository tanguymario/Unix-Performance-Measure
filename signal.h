#include <signal.h>
#include <time.h>

void set_up_signal_handler(
  struct sigaction* const sa,
  void (*f)(int, siginfo_t*, void*),
  const int sig_no);

void set_up_timer(
  struct sigevent* const se, timer_t* const timer_id,
  const clockid_t clock, int sig_no);

void start_timer(timer_t* timer_id, const struct timespec* const DELAY);

void block_signal(sigset_t* const mask, const int sig_no);

void unblock_signals(sigset_t* const mask);
