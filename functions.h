#include <time.h>
#include <signal.h>
#include "mesure.h"

void ma_fonction_active(
  const Mesure* const mesure,
	struct timespec* const temp_current_time, struct timespec* const difference);

void ma_fonction_passive(
  int signal, siginfo_t* siginfo, void* context);
