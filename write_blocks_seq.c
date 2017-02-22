#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/timeb.h>
#include "utils.h"

int write_blocks_seq(char * filename, int blocksize){

	struct timeb t_begin, t_end;
	long time_spent_ms; 
	long total_records_time = 0;
	   
	int block_size = blocksize;
	int records_per_block = block_size/sizeof(Record);
	char current_line[MAX_CHARS_PER_LINE];
	int total_records = 0, in_buff = 0;

	FILE *fp_read;
	FILE *fp_write;
	
	ftime(&t_begin);  

	/* open text file for reading */
	if (!(fp_read= fopen(filename, "r"))) {
		printf ("Could not open file \"%s\" for reading \n", filename);
		return (-1);
	}

	if (!(fp_write = fopen ("records.dat" , "wb" ))){
		printf ("Could not open file records.dat for writing \n");
		return (-1);
	}
	     
	Record * buffer = (Record *) calloc (records_per_block, sizeof (Record));


	/* reading lines */
	while(fgets(current_line, MAX_CHARS_PER_LINE, fp_read) !=NULL ) {
		if (current_line[0] != '\0'){
			current_line [strcspn (current_line, "\r\n")] = '\0'; //remove end-of-line characters
			Record * current = (Record *) malloc(sizeof(Record));
			convert_to_record(current_line, current);

			if (in_buff == records_per_block){
				fwrite (buffer, sizeof(Record), total_records, fp_write);
				memset(buffer, 0, sizeof(*buffer));
				in_buff = 0;

			}

			memcpy(&buffer[in_buff], current, sizeof(Record));
			total_records_time ++;
			total_records = total_records + 1;
			free(current);
		}
	}
	
	fwrite (buffer, sizeof(Record), total_records, fp_write);
	fflush (fp_write); 
	free (buffer);
	fclose(fp_write);
	fclose (fp_read);

	ftime(&t_end);     
	 
	/* time elapsed in milliseconds */
	time_spent_ms = (long) (1000 *(t_end.time - t_begin.time)
	       + (t_end.millitm - t_begin.millitm)); 
	 
	/* result in MB per second */
	// replace MB with integer values
	printf ("Data rate: %.3f MBPS\n", ((total_records_time*sizeof(Record))/(float)time_spent_ms * 1000)/MB);
	return 0;
}

int main(int argc, char **argv){
	write_blocks_seq(argv[1], atoi(argv[2]));	
	return 0;
} 