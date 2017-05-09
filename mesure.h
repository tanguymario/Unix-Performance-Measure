#include <time.h>

/* Default values for Mesure struct*/
#define DEFAULT_DT_S 0
#define DEFAULT_DT_NS 999999999L

/* Histogram is defined by different slices of interval times */
#define HISTOGRAM_SIZE 21
#define HISTOGRAM_STEP 10 // ns

#define INTERVAL_TIME_STRING_SIZE LONG_SIZE * 3

/* Default clock used by Mesure struct */
#define CLOCK_ID CLOCK_MONOTONIC

/* Buffer size to print Mesure struct */
#define MESURE_STRING_SIZE 512

#define HISTOGRAM_STRING_SIZE 2048

/* Struct to retrieve info */
typedef struct
{
	struct timespec startTime, currentTime, nextHop, dt;
	size_t histogram[HISTOGRAM_SIZE];
} Mesure;

Mesure* mesure_initialize(
	Mesure* const mesure, const struct timespec* const timeDt);

void mesure_update_current_time(Mesure* const mesure);

void mesure_set_next_hop(Mesure* const mesure);

void mesure_update_histogram(
	Mesure* const mesure, struct timespec* const difference);

void mesure_wait_till_next_hop(
	Mesure* const mesure, struct timespec* const difference);

void mesure_to_string(char* const mesure_string, const Mesure* const mesure);

void mesure_print(const Mesure* const mesure);

void mesure_histogram_to_string(
	char* const histo_string, const Mesure* const mesure);

int mesure_is_over(const Mesure* const mesure, const long* const TIME);

void mesure_histogram_print(const Mesure* const mesure);

void mesure_get_histogram_interval_time(char* const interval_time, int index);

void mesure_write_in_output(
	const Mesure* const mesure,
	const char* const init_message, const char* const filename);
