#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"

static int convert_to_records(const char * filename){
	/* returns: -1 if there is an error. */
	
	char current_line[MAX_CHARS_PER_LINE];

	FILE *fp_read;
	
	/* open text file for reading */
	if (!(fp_read= fopen ( filename , "r" ))) {
		printf ("Could not open file \"%s\" for reading \n", filename);
		return (-1);
	}
	     
	/* reading lines */
	while( fgets (current_line, MAX_CHARS_PER_LINE, fp_read)!=NULL ) {
		current_line [strcspn (current_line, "\r\n")] = '\0'; //remove end-of-line characters

	}
	
	fclose (fp_read);

	return 0;
}

static record convert_to_record(char * line[]){
	struct record * new_record = (struct record *) malloc(sizeof(struct recrod));
		
}