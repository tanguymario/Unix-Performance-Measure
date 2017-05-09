#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/utsname.h>

#include "utils.h"
#include "console.h"
#include "sys_info.h"

const char message_no_option[] =
	" ----------------------------\n"
	" - Mesure options available -\n"
	" ----------------------------\n"
	" -a  : active method\n"
	" -p  : passive method\n"
	" -t  : thread method\n"
	" ----------------------------\n"
	" --nice   : set nice (priority) of a process (root sometimes needed)\n"
	" --delay  : set delay between each call\n"
	" --time   : set time of total execution\n"
	" --cores  : set how many cores will be used (default is all cores)\n"
	" --show   : show an output to the terminal of what is going on\n"
	" --clear  : clear the terminal if option '--show' is true\n"
	" --output : prints data to a file\n"
	" --help   : prints this message\n"
	" ----------------------------\n"
	" Concatenated options like '-apd' (active passive delay) are not supported\n"
	" ----------------------------\n"
	;

enum MESURE_ACTION get_action(int argc, char* argv[])
{
	const int ACTIVE  = get_flag_index(argc, argv, FLAG_ACTIVE ) > -1 ? 1 : 0;
	const int PASSIVE = get_flag_index(argc, argv, FLAG_PASSIVE) > -1 ? 1 : 0;
	const int THREAD  = get_flag_index(argc, argv, FLAG_THREAD ) > -1 ? 1 : 0;

	if (ACTIVE)
		return MESURE_ACTIVE;
	else if (PASSIVE)
		return MESURE_PASSIVE;
	else if (THREAD)
		return MESURE_THREAD;

	return MESURE_NONE;
}

/* Does not support concatenated options */
int get_flag_index(int argc, char* argv[], const char* const flag)
{
	int i = 1;
	for (; i < argc; i++)
	{
		if (strcmp(argv[i], flag) == 0)
			return i;
	}

	return -1;
}

char* get_value_associated_to_flag(
	int argc, char* argv[], const char* const flag)
{
	int index = get_flag_index(argc, argv, flag);
	if (index != -1 && index < argc - 1)
		return argv[index + 1];

	return NULL;
}

inline void print_no_option_message(void)
{
	printf("%s\n", message_no_option);
}

void get_init_message(
	char* const init_message,
	const enum MESURE_ACTION ACTION,
	const int* const NICE, const long* const TIME,
	const int* const CORES, const struct timespec* const DELAY,
	const char* const OUTPUT_FILENAME)
{
	memset(init_message, 0, INIT_STRING_SIZE);

	/* Get system info */
	struct utsname uname;
	get_system_info(&uname);

	/* Buffer to store long values */
	char buffer[LONG_SIZE * 3];

	/* Buffer for timespec struct */
	char buffer_timespec[TIMESPEC_STRING_SIZE];

	strcat(init_message,
		"--------------------\n"
	 	" -      MESURE     - \n"
		"--------------------\n\n");

	/* Physical RAM size in bytes */
	size_t ram = get_memory_size();
	ram /= MEGA_OCTET_BITS;

	strcat(init_message, " - RAM : ");
	sprintf(buffer, "%zu.Mo\n", ram);
	strcat(init_message, buffer);

	strcat(init_message, " - System name : ");
	strcat(init_message, uname.sysname);

	strcat(init_message, "\n - Node name : ");
	strcat(init_message, uname.nodename);

	strcat(init_message, "\n - Release : ");
	strcat(init_message, uname.release);

	strcat(init_message, "\n - Machine : ");
	strcat(init_message, uname.machine);

	strcat(init_message, "\n\n - PID : ");
	sprintf(buffer, "%d\n", getpid());
	strcat(init_message, buffer);

	strcat(init_message, " - ACTION : ");
	switch (ACTION)
	{
		case MESURE_ACTIVE:
			strcat(init_message, "ACTIVE\n");
			break;
		case MESURE_PASSIVE:
			strcat(init_message, "PASSIVE\n");
			break;
		case MESURE_THREAD:
			strcat(init_message, "THREAD\n");
			break;
		default:
			break;
	}

	strcat(init_message, " - NICE : ");
	sprintf(buffer, "%d\n", *NICE);
	strcat(init_message, buffer);

	strcat(init_message, " - TIME : ");
	sprintf(buffer, "%lds\n", *TIME);
	strcat(init_message, buffer);

	strcat(init_message, " - CORES : ");
	sprintf(buffer, "%d\n", *CORES);
	strcat(init_message, buffer);

	strcat(init_message, " - DELAY : ");
	timespec_to_string_inline(buffer_timespec, DELAY);
	strcat(init_message, buffer_timespec);
	strcat(init_message, "\n");

	strcat(init_message, " - NUMBERS OF EXECUTIONS : ");
	long long nb_times = 0;
	if (DELAY->tv_sec != 0)
	{
		nb_times += *TIME / DELAY->tv_sec;
	}

	if (DELAY->tv_nsec != 0)
	{
		nb_times += (*TIME * NANO) / DELAY->tv_nsec;
	}

	sprintf(buffer, "%lld\n", nb_times);
	strcat(init_message, buffer);

	strcat(init_message, " - OUTPUT : ");
	if (OUTPUT_FILENAME == NULL)
	{
		strcat(init_message, " NONE\n");
	}
	else
	{
		sprintf(buffer, "%s\n", OUTPUT_FILENAME);
		strcat(init_message, buffer);
	}

	strcat(init_message, "\n--------------------\n\n");
}

void print_init(
	const enum MESURE_ACTION ACTION,
	const int* const NICE, const long* const TIME,
	const int* const CORES, const struct timespec* const DELAY,
	const char* const OUTPUT_FILENAME)
{
	char output[INIT_STRING_SIZE];

	get_init_message(output, ACTION, NICE, TIME, CORES, DELAY, OUTPUT_FILENAME);

	printf("%s\n", output);
}
