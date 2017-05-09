#include <sys/utsname.h>

#define BIT 1
#define OCTET_BITS 8
#define KILO_OCTET_BITS 8192
#define MEGA_OCTET_BITS 8388608
#define GIGA_OCTET_BITS 8589934592

/* Return pid used by process */
int get_pid_of_process(void);

/* Store system info in utsname struct */
void get_system_info(struct utsname* const uname_pointer);

/* Returns the size of physical memory (RAM) in bytes */
size_t get_memory_size(void);
