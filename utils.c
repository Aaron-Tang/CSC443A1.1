#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"

const int MAX_CHARS_PER_LINE = 1024;
const int MB = 1024 * 1024;

void convert_to_record(char line[], Record * current_record){

	char * uid1 = strtok(line, ",");
	char * uid2 = strtok(NULL, ",");
	current_record->uid1 = atoi(uid1);
	current_record->uid2 = atoi(uid2);

}


