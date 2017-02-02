typedef struct record {
 int uid1;
 int uid2;
} Record;

const int MAX_CHARS_PER_LINE = 1024;
const int MB = 1024 * 1024;

void convert_to_record(char line[], Record * current_record){
