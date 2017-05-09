#include <stdio.h>  /* printf() */
#include <stdlib.h> /* */
#include <string.h> /* */
#include <limits.h> /* */

#include "utils.h"

inline void initialize_screen(void)
{
	/* Avoid system calls */
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}

inline void clear_screen(void)
{
	/* Avoid system calls */
	printf("\33[2J");
}

inline void set_time_to_current(
	const clockid_t clock, struct timespec* const time)
{
	clock_gettime(clock, time);
}

void timespec_to_string(
	char* const string, const struct timespec* const time)
{
	/* Initialize output */
	memset(string, 0, TIMESPEC_STRING_SIZE * sizeof(char));

	/* store long values in char buffer */
	char buffer[LONG_SIZE + 1];

	strcat(string, "Seconds : ");

	/* long to buffer, then to output */
	sprintf(buffer, "%ld", (long) time->tv_sec);
	strcat(string, buffer);

	strcat(string, "\nNanoseconds : ");

	sprintf(buffer, "%.9ld", time->tv_nsec);
	strcat(string, buffer);

	strcat(string, "\n\0");
}

void timespec_to_string_inline(
	char* const string, const struct timespec* const time)
{
	/* Initialize output */
	memset(string, 0, TIMESPEC_STRING_SIZE * sizeof(char));

	/* store long values in char buffer */
	char buffer[LONG_SIZE + 1];

	strcat(string, " { Seconds : ");

	/* long to buffer, then to output */
	sprintf(buffer, "%ld", (long) time->tv_sec);
	strcat(string, buffer);

	strcat(string, ", Nanoseconds : ");

	sprintf(buffer, "%.9ld", time->tv_nsec);
	strcat(string, buffer);

	strcat(string, " }\0");
}

void timespec_print(const struct timespec* const time)
{
	char timespec_string[TIMESPEC_STRING_SIZE];

	timespec_to_string(timespec_string, time);

	printf("%s\n", timespec_string);
}

void add_timespec(struct timespec* const t1, const struct timespec* const t2)
{
	t1->tv_sec += t2->tv_sec;

	/* Store the sum in a long long to check overflow */
	long long value = t1->tv_nsec + t2->tv_nsec;

	if (value >= NANO)
	{
		/* there is a long overflow */
		t1->tv_sec++;
		t1->tv_nsec = (long) (value - NANO);
	}
	else
	{
		t1->tv_nsec = (long) value;
	}
}

/* t3 = t2 - t1 */
void get_difference(
	const struct timespec* const t1,
	const struct timespec* const t2,
	struct timespec* const t3)
{
	t3->tv_sec = t2->tv_sec - t1->tv_sec;
	t3->tv_nsec = t2->tv_nsec - t1->tv_nsec;

	return;

	if (t3->tv_sec > 0)
	{
		t3->tv_nsec = NANO - t1->tv_nsec + t2->tv_nsec;
	}
	else if (t1->tv_sec > t2->tv_sec)
	{
		t3->tv_nsec = -(NANO - t2->tv_nsec + t1->tv_nsec);
	}
	else
	{
		t3->tv_nsec = t2->tv_nsec - t1->tv_nsec;
	}
}
