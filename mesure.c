#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "utils.h"
#include "mesure.h"

Mesure* mesure_initialize(
	Mesure* const mesure, const struct timespec* const timeDt)
{
	/* start time */
	set_time_to_current(CLOCK_ID, &mesure->startTime);
	set_time_to_current(CLOCK_ID, &mesure->currentTime);

	/* delay */
	mesure->dt = *timeDt;

	/* next hop */
	set_time_to_current(CLOCK_ID, &mesure->nextHop);
	mesure_set_next_hop(mesure);

	/* histogram */
	int i = 0;
	for (; i < HISTOGRAM_SIZE; i++)
		mesure->histogram[i] = 0;

	return mesure;
}

inline void mesure_update_current_time(Mesure* const mesure)
{
	set_time_to_current(CLOCK_ID, &mesure->currentTime);
}

inline void mesure_set_next_hop(Mesure* const mesure)
{
	add_timespec(&mesure->nextHop, &mesure->dt);
}

void mesure_update_histogram(
	Mesure* const mesure, struct timespec* const difference)
{
	/* Enter value in histogram */
	int middleIndex = HISTOGRAM_SIZE / 2;
	int index = middleIndex;

	/* First of all, manage seconds */
	if (difference->tv_sec > 0)
	{
		index = HISTOGRAM_SIZE - 1;
	}
	else if (difference->tv_sec < 0)
	{
		index = 0;
	}
	else
	{
		/* Manage nanoseconds */
		if (difference->tv_nsec == 0)
		{
			/* Middle of histogram -> Exact */
			index = middleIndex;
		}
		else
		{
			long limit = HISTOGRAM_STEP;
			int cmpt = 1;

			while (index == middleIndex && cmpt <= middleIndex)
			{
				if (difference->tv_nsec > 0)
				{
					if (difference->tv_nsec < limit)
					{
						index = middleIndex + cmpt;
						break;
					}
				}
				else
				{
					if (difference->tv_nsec > -limit)
					{
						index = middleIndex - cmpt;
						break;
					}
				}

				limit *= HISTOGRAM_STEP;
				cmpt++;
			}
		}
	}

	mesure->histogram[index] ++;
}

void mesure_wait_till_next_hop(
	Mesure* const mesure, struct timespec* const difference)
{
	get_difference(&mesure->currentTime, &mesure->nextHop, difference);

	/* While current time is < than next hop, we update it */
	while (difference->tv_sec > 0 || difference->tv_nsec > 0)
	{
		mesure_update_current_time(mesure);
		get_difference(&mesure->currentTime, &mesure->nextHop, difference);
	}
}

void mesure_to_string(char* const mesure_string, const Mesure* const mesure)
{
	/* Initialize output */
	memset(mesure_string, 0, MESURE_STRING_SIZE * sizeof(char));

	char buffer_timespec[TIMESPEC_STRING_SIZE];

	strcat(mesure_string, "Start Time : \n");

	timespec_to_string(buffer_timespec, &mesure->startTime);
	strcat(mesure_string, buffer_timespec);

	strcat(mesure_string, "\nCurrent Time : \n");

	/* For currentTime */
	timespec_to_string(buffer_timespec, &mesure->currentTime);
	strcat(mesure_string, buffer_timespec);

	strcat(mesure_string, "\nNext Hop : \n");

	/* For next hop */
	timespec_to_string(buffer_timespec, &mesure->currentTime);
	strcat(mesure_string, buffer_timespec);
}

void mesure_print(const Mesure* const mesure)
{
	char mesure_string[MESURE_STRING_SIZE];
	mesure_to_string(mesure_string, mesure);

	printf("%s\n", mesure_string);
}

void mesure_histogram_to_string(
	char* const histo_string, const Mesure* const mesure)
{
	/* Initialize output */
	memset(histo_string, 0, HISTOGRAM_STRING_SIZE * sizeof(char));

	char buffer_long[LONG_SIZE + 1];
	char buffer_interval_time[INTERVAL_TIME_STRING_SIZE];

	strcat(histo_string, ""
		"--------------------\n"
	 	" -   HISTOGRAMME  - \n"
		"--------------------\n\n");

	int i = 0;
	for (; i < HISTOGRAM_SIZE; i++)
	{
		/* Interval time */
		mesure_get_histogram_interval_time(buffer_interval_time, i);
		strcat(histo_string, buffer_interval_time);
		strcat(histo_string, "\n");

		/* Count */
		sprintf(buffer_long, "%.10zu", mesure->histogram[i]);
		strcat(histo_string, buffer_long);
		strcat(histo_string, "\n\n");
	}

	strcat(histo_string, "\n--------------------\n\n");
}

int mesure_is_over(const Mesure* const mesure, const long* const TIME)
{
	if (mesure->currentTime.tv_sec - mesure->startTime.tv_sec >= *TIME)
		return 1;
	else
		return 0;
}

void mesure_histogram_print(const Mesure* const mesure)
{
	char histogram_string[HISTOGRAM_STRING_SIZE];

	mesure_histogram_to_string(histogram_string, mesure);

	printf("%s\n", histogram_string);

	fflush(stdout);
}

void mesure_get_histogram_interval_time(char* const interval_time, const int index)
{
	memset(interval_time, 0, INTERVAL_TIME_STRING_SIZE * sizeof(char));

	if (index < 0 || index >= HISTOGRAM_SIZE)
	{
		printf(
			"Bad index \'%d\' for mesure_get_histogram_interval_time()\n", index);
	}

	int middleIndex = HISTOGRAM_SIZE / 2;

	if (index == middleIndex)
	{
		strcat(interval_time, "Exact");
	}
	else if (index == 0)
	{
		strcat(interval_time, "<<<");
	}
	else if (index == HISTOGRAM_SIZE - 1)
	{
		strcat(interval_time, ">>>");
	}
	else
	{
		long interval_temp = 1;
		char buffer[LONG_SIZE + 1];

		int index_difference = index - middleIndex;
		if (index_difference > 0)
		{
			interval_temp = pow(10, index_difference - 1);
		}
		else
		{
			interval_temp = -pow(10, abs(index_difference) - 1);
		}

		sprintf(buffer, "%ld", interval_temp);
		strcat(interval_time, buffer);

		strcat(interval_time, ":");

		interval_temp *= 10;
		sprintf(buffer, "%ld", interval_temp);
		strcat(interval_time, buffer);
	}
}

void mesure_write_in_output(
	const Mesure* const mesure,
	const char* const init_message, const char* const filename)
{
	FILE* f = fopen(filename, "w");
	if (f == NULL)
	{
		printf("Error opening file for output!\n");
		return;
	}

	char histo_string[HISTOGRAM_STRING_SIZE];

	mesure_histogram_to_string(histo_string, mesure);

	fprintf(f, "%s%s", init_message, histo_string);

	fflush(f);

	fclose(f);
}
