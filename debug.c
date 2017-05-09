#include <stdio.h>
#include <string.h>
#include "debug.h"

void print_debug(const char message[])
{
	char output[DEBUG_SIZE] = {};
	strcpy(output, DEBUG_PREFIX);
	strcat(output, message);
	strcat(output, "\n");

	printf("%s\n", output);
}