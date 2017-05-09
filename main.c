#define _GNU_SOURCE

#include <stdio.h>         /* printf...                  */
#include <stdlib.h>        /* exit(), strtol() ...       */
#include <signal.h>        /* signal(), SIGALRM          */
#include <string.h>        /* strcat() ...               */
#include <sched.h>				 /* cores                      */
#include <unistd.h> 			 /* How many cores...			     */
#include <errno.h> 			 	 /* errno  										 */
#include <sys/resource.h>  /* setpriority() for nice ... */

#include "console.h"   /* Get arguments in command line    */
#include "utils.h"     /* Compare timespecs ...            */
#include "functions.h" /* Function which will measure time */
#include "debug.h"     /* Simply debug our program         */
#include "sys_info.h"  /* Get system information           */
#include "signal.h"    /* Set up signals ...               */

/* Values by default */
#define NICE_MAX 19
#define NICE_MIN -20
#define NICE_WHICH PRIO_PROCESS
#define TIME_DEFAULT 5
#define CORES_DEFAULT 1

#define SIG_DEFAULT SIGALRM

/* In the release version, DEBUG is 0. */
#define DEBUG 0

#if DEBUG
#define print_debug(x) print_debug(x);
#else
#define print_debug(x) { }
#endif

#define error_exit(msg)  do { perror(msg); exit(EXIT_FAILURE); } while (0)

/* passive function from functions.h */
void ma_fonction_passive(int signal, siginfo_t * siginfo, void * context);

/* For signal handler */
volatile extern Mesure* MAIN_MESURE;

int main(int argc, char* argv[])
{
	/*******************************/
	/*         FLAG VALUES         */
	/*******************************/

	const int SHOW  = get_flag_index(argc, argv, FLAG_SHOW ) > -1 ? 1 : 0;
	const int CLEAR = get_flag_index(argc, argv, FLAG_CLEAR) > -1 ? 1 : 0;
	const int HELP  = get_flag_index(argc, argv, FLAG_HELP ) > -1 ? 1 : 0;

	const enum MESURE_ACTION ACTION = get_action(argc, argv);

	/* if there is nothing to do, we stop there */
	if (ACTION == MESURE_NONE || HELP)
	{
		print_debug("No Action Specified\n");

		print_no_option_message();

		exit(EXIT_SUCCESS);
	}

	/* Get NICE value */
	int nice_arg = 0;
	char* flag_value = get_value_associated_to_flag(argc, argv, FLAG_NICE);
	if (flag_value != NULL)
	{
		nice_arg = (int) strtol(flag_value, NULL, 10);
		if (nice_arg > NICE_MAX)
			nice_arg = NICE_MAX;
		else if (nice_arg < NICE_MIN)
			nice_arg = NICE_MIN;
	}

	const int* const NICE = &nice_arg;

	/* Get DELAY */
	struct timespec time_delay = { DEFAULT_DT_S, DEFAULT_DT_NS };
	flag_value = get_value_associated_to_flag(argc, argv, FLAG_DELAY);
	if (flag_value != NULL)
	{
		long delay_ms = strtol(flag_value, NULL, 10);
		if (delay_ms > 0)
		{
			if (delay_ms >= MICRO)
			{
				int seconds = delay_ms / MICRO;
				time_delay.tv_sec = seconds;

				delay_ms -= seconds * MICRO;
			}

			time_delay.tv_nsec = delay_ms * (NANO / MICRO);
		}
	}

	const struct timespec* const DELAY = &time_delay;

	/* Get time */
	long time = TIME_DEFAULT;
	flag_value = get_value_associated_to_flag(argc, argv, FLAG_TIME);
	if (flag_value != NULL)
	{
		time = strtol(flag_value, NULL, 10);
		if (time <= 0)
			time = TIME_DEFAULT;
	}

	const long* const TIME = &time;

	/* Set number of cores */
	const int NB_CORES = sysconf(_SC_NPROCESSORS_ONLN);
	int nb_cores_wanted = NB_CORES;
	flag_value = get_value_associated_to_flag(argc, argv, FLAG_CORES);
	if (flag_value != NULL)
	{
		nb_cores_wanted = (int) strtol(flag_value, NULL, 10);
		if (nb_cores_wanted <= 0 || nb_cores_wanted > NB_CORES)
			nb_cores_wanted = NB_CORES;
	}

	const int* CORES = &nb_cores_wanted;

	/* Get output filename */
	const char* const OUTPUT_FILE =
		get_value_associated_to_flag(argc, argv, FLAG_OUTPUT);

	/*******************************/
	/*      INITIALIZE PROGRAM     */
	/*******************************/

  if (SHOW && CLEAR)
	{
		initialize_screen();
	}

	/* Set NICE value */
	if (*NICE != 0)
	{
		/* On my system, I need to be root to be able to set NICE */
		if (setpriority(NICE_WHICH, get_pid_of_process(), *NICE) == -1)
		{
			printf("Could not set NICE Value : %s\n", strerror(errno));
		}
	}

	/* set number of cores */
	#if defined(__CYGWIN__) || defined(__CYGWIN32__)
	#else
	cpu_set_t cpu_set;
	CPU_ZERO(&cpu_set);

	if (sched_getaffinity(0, sizeof(cpu_set), &cpu_set) != 0)
	{
		printf("Could not get CPU set : %s\n", strerror(errno));
	}
	else
	{
		int cores_used = CPU_COUNT_S(sizeof(cpu_set), &cpu_set);
		if (SHOW)
			printf("Initially, %d cores are used\n", cores_used);

		if (cores_used != *CORES)
		{
			CPU_ZERO(&cpu_set);

			int i = 0;
			for (; i < *CORES; i++)
			{
				CPU_SET(i, &cpu_set);
			}

			if (sched_setaffinity(0, sizeof(cpu_set), &cpu_set) != 0)
			{
				printf("Could not set CPU set : %s\n", strerror(errno));
				CORES = &cores_used;
			}
		}
	}
	#endif

	/* Print what has been initialized */
	print_init(ACTION, NICE, TIME, CORES, DELAY, OUTPUT_FILE);

	/*******************************/
	/*         START PROGRAM       */
	/*******************************/

	/* difference between next hop and current time */
	struct timespec difference = { 0, 0 };

	/* Create mesure */
	Mesure mesure;

	MAIN_MESURE = &mesure;

	mesure_initialize(&mesure, DELAY);

	if (ACTION == MESURE_ACTIVE)
	{
		while (mesure_is_over(&mesure, TIME) == 0)
		{
			/* We wait till next hop */
			mesure_wait_till_next_hop(&mesure, &difference);

			/* Temporary struct for comparing current time */
			struct timespec temp_current_time;

			/* Update here just before calling function */
			mesure_update_current_time(&mesure);

			/* Function to test, it will measure the difference */
			ma_fonction_active(&mesure, &temp_current_time, &difference);

			/* Update histogram */
			mesure_update_histogram(&mesure, &difference);

			/* Calculate next hop */
			mesure_set_next_hop(&mesure);

			/* Treatments are done here, we can print some information */
			if (SHOW)
			{
				if (CLEAR)
					clear_screen();

				mesure_print(&mesure);
			}
		}
	}
	else if (ACTION == MESURE_PASSIVE)
	{
		/* Signal vars */
		struct sigevent se;
		struct sigaction sa;
		timer_t timer_id;
		const int sig_no = SIG_DEFAULT;

		/* Create signal handler and start a timer */
		set_up_signal_handler(&sa, ma_fonction_passive, sig_no);
		set_up_timer(&se, &timer_id, CLOCK_ID, sig_no);
		start_timer(&timer_id, DELAY);

		while (mesure_is_over(&mesure, TIME) == 0)
		{
			struct timespec temp_current_time;

			pause();

			/* Signal was interrupted, mesure has now current time updated */

			/* We get current time now to see time difference */
			set_time_to_current(CLOCK_ID, &temp_current_time);
			get_difference(&temp_current_time, &mesure.currentTime, &difference);

			mesure_update_histogram(&mesure, &difference);

			mesure_update_current_time(&mesure);
		}
	}
	else if (ACTION == MESURE_THREAD)
	{
		// TODO
	}
	else
	{
		error_exit("Action not handled\n");
	}

	mesure_histogram_print(&mesure);

	if (OUTPUT_FILE != NULL)
	{
		char init_message[INIT_STRING_SIZE];

		get_init_message(
			init_message, ACTION, NICE, TIME, CORES, DELAY, OUTPUT_FILE);

		mesure_write_in_output(&mesure, init_message, OUTPUT_FILE);
	}

	exit(EXIT_SUCCESS);
}
