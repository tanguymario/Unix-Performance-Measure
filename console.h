#define FLAG_ACTIVE   "-a"
#define FLAG_PASSIVE  "-p"
#define FLAG_THREAD   "-t"
#define FLAG_NICE     "--nice"
#define FLAG_DELAY    "--delay"
#define FLAG_TIME     "--time"
#define FLAG_CORES    "--cores"
#define FLAG_SHOW     "--show"
#define FLAG_CLEAR    "--clear"
#define FLAG_OUTPUT   "--output"
#define FLAG_HELP     "--help"

#define INIT_STRING_SIZE 1024

/* Program executes one action */
enum MESURE_ACTION
{
  MESURE_ACTIVE,
  MESURE_PASSIVE,
  MESURE_THREAD,
  MESURE_NONE
};

enum MESURE_ACTION get_action(int argc, char* argv[]);

int get_flag_index(int argc, char* argv[], const char* flag);

char* get_value_associated_to_flag(
  int argc, char* argv[], const char* const flag);

void print_no_option_message(void);

void get_init_message(
	char* const init_message,
	const enum MESURE_ACTION ACTION,
	const int* const NICE, const long* const TIME,
	const int* const CORES, const struct timespec* const DELAY,
	const char* const OUTPUT_FILENAME);

  void print_init(
  	const enum MESURE_ACTION ACTION,
  	const int* const NICE, const long* const TIME,
  	const int* const CORES, const struct timespec* const DELAY,
  	const char* const OUTPUT_FILENAME);
