#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct record {
 int uid1;
 int uid2;
} Record;

const int MAX_CHARS_PER_LINE = 1024;

void convert_to_record(char line[], Record * current_record){

	char * uid1 = strtok(line, ",");
	char * uid2 = strtok(NULL, ",");
	//struct record new_record = {atoi(uid1), atoi(uid2)};
	current_record->uid1 = atoi(uid1);
	current_record->uid2 = atoi(uid2);
	//return new_record;

}

static int convert_to_records(char * filename, struct record * buffer[]){
	/* returns: -1 if there is an error. */
	
	char current_line[MAX_CHARS_PER_LINE];
	int counter = 0;

	FILE *fp_read;
	
	/* open text file for reading */
	if (!(fp_read= fopen(filename, "r"))) {
		printf ("Could not open file \"%s\" for reading \n", filename);
		return (-1);
	}
	     
	/* reading lines */
	while( fgets (current_line, MAX_CHARS_PER_LINE, fp_read)!=NULL ) {
		current_line [strcspn (current_line, "\r\n")] = '\0'; //remove end-of-line characters
		Record *current = malloc(sizeof(Record));
		convert_to_record(current_line, current);
		buffer[counter] = current;
		printf("%d\n", buffer[counter]->uid1);
		free(current);
	}
	
	fclose (fp_read);

	return 0;
}

int main(int argc, char **argv){
	struct Record* buffer [1024];
	convert_to_records(argv[1], &buffer);	
}
