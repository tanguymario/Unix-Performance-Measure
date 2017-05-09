#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
#include "utils.h"

Mesure* MAIN_MESURE = NULL;

void ma_fonction_active(
	const Mesure* const mesure,
	struct timespec* const temp_current_time, struct timespec* const difference)
{
	/* First instruction is to measure current time */
	set_time_to_current(CLOCK_ID, temp_current_time);

	/* Then get the difference */
	get_difference(&mesure->currentTime, temp_current_time, difference);
}

void ma_fonction_passive(
	int signal, siginfo_t* siginfo, void* context)
{
	/* Get global var */
	Mesure* const temp_mesure_ptr = MAIN_MESURE;

	/* Find out which signal has been sent */
  switch (signal)
  {
    case SIGINT:
  		printf("\nCaught SIGINT, exiting now\n");
      exit(EXIT_SUCCESS);
		case SIGALRM:
			mesure_update_current_time(temp_mesure_ptr);
			break;
    default:
      fprintf(stderr, "Caught unhandled signal: %d\n", signal);
	      return;
  }
}
