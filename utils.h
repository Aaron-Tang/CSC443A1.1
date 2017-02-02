#ifndef _utils_h
#define _utils_h

typedef struct record {
 int uid1;
 int uid2;
} Record;

const int MAX_CHARS_PER_LINE;
const int MB;

void convert_to_record(char line[], Record * current_record);

#endif