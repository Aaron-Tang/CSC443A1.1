#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/timeb.h>
#include "utils.c"

static int write_blocks_seq(char * filename, int blocksize){

	struct timeb t_begin, t_end;
	long time_spent_ms; 
	long total_records_time = 0;
	   
	int block_size = blocksize;
	int records_per_block = block_size/sizeof(Record);
	printf("%d\n", records_per_block);
	char current_line[MAX_CHARS_PER_LINE];
	int total_records = 0;
	printf("total_records, %d\n", total_records);


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
	while( fgets (current_line, MAX_CHARS_PER_LINE, fp_read)!=NULL ) {
		current_line [strcspn (current_line, "\r\n")] = '\0'; //remove end-of-line characters
		Record * current = (Record *) malloc(sizeof(Record));
		convert_to_record(current_line, current);

		printf("THERE\n");
		if (total_records == records_per_block){
			printf("NOW HERE\n");
			fwrite (buffer, sizeof(Record), total_records, fp_write);
			/* force data to disk */
			fflush (fp_write); 
			printf("MAYBE?\n");
			total_records = 0;

		}

			/* flush buffer when full */

		printf("HERE\n");	
		buffer[total_records] = *current;
		free(current);
		total_records = total_records + 1;

	}
	


	fclose(fp_write);
	fclose (fp_read);

	free (buffer);

	total_records_time ++;
	ftime(&t_end);     
	 
	/* time elapsed in milliseconds */
	time_spent_ms = (long) (1000 *(t_end.time - t_begin.time)
	       + (t_end.millitm - t_begin.millitm)); 
	 
	/* result in MB per second */
	printf ("Data rate: %.3f MBPS\n", ((total_records_time*sizeof(Record))/(float)time_spent_ms * 1000)/(1024 * 1024));
	return 0;
}

int main(int argc, char **argv){
	write_blocks_seq(argv[1], atoi(argv[2]));	
}