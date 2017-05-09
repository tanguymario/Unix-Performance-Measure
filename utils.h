#include <time.h>

/* Buffer to print a timespec struct */
#define TIMESPEC_STRING_SIZE 64

/* Number of digits used by a long type number */
#define LONG_SIZE 10

/* Units for 1 second */
#define NANO  1000000000L
#define MICRO 1000000L

/* Set terminal screen to bottom */
void initialize_screen(void);

/* Clear terminal screen  */
void clear_screen(void);

void set_time_to_current(
  const clockid_t clock, struct timespec* const time);

void timespec_to_string(
	char* const string, const struct timespec* const time);

void timespec_to_string_inline(
	char* const string, const struct timespec* const time);

void timespec_print(const struct timespec* const time);

void add_timespec(struct timespec* const t1, const struct timespec* const t2);

/* Get difference between two timespecs (t2 - t1) */
void get_difference(
	const struct timespec* const t1,
	const struct timespec* const t2,
	struct timespec* const t3);
