#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include utils.c

/* Our structure */
typedef struct record  {
	int uid1,uid2;
} Record;


static int write_blocks_seq(char * filename, int block_size){
  
	int block_size = block_size;
	int records_per_block = block_size/sizeof(Record);
	char current_line[MAX_CHARS_PER_LINE];
	int total_records = 0;

	FILE *fp_read;
	FILE *fp_write;
	
	/* open text file for reading */
	if (!(fp_read= fopen(filename, "r"))) {
		printf ("Could not open file \"%s\" for reading \n", filename);
		return (-1);
	}
	     
	Record * buffer = (Record *) calloc (records_per_block, sizeof (Record)) ;

	/* reading lines */
	while( fgets (current_line, MAX_CHARS_PER_LINE, fp_read)!=NULL ) {
		current_line [strcspn (current_line, "\r\n")] = '\0'; //remove end-of-line characters
		Record *current = malloc(sizeof(Record));
		convert_to_record(current_line, current);

		if (total_records != records_per_block){
			buffer[total_records] = current;
			free(current);
			total_records++;
		}
		else{
			if (!(fp_write = fopen ( "records.dat" , "wb" ))){
				printf ("Could not open file records.dat for writing \n");
				return (-1);
			}
			/* flush buffer when full */
			fwrite ( buffer, sizeof(Record), total_records, fp_write);
			 
			/* force data to disk */
			fflush (fp_write); 
			fclose(fp_write);
			total_records = 0;

			buffer[total_records] = current;
			free(current);
			total_records++;
		}

	}
	
	fclose (fp_read);

	free (buffer);
}