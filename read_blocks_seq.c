#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/timeb.h>
#include "utils.h"

int read_blocks_seq(char * filename, int blocksize){
	int records_per_block = blocksize / sizeof(Record);
	int in_buff = 0;

	int max_followers = 0;

	int unique_ids = 0;

	int current_amount_for_id = 0;
	int current_id = 0;

	size_t bytes_read = 0;

	struct timeb t_begin, t_end;
	long time_spent_ms;
	 
	long total_records = 0;
 	
	if (blocksize % sizeof(Record) != 0){
		printf("Incorrect blocksize\n");
		return (-1);
	}



	FILE *fp_read;
	
	/* allocate buffer for 1 block */
	Record * buffer = (Record *) calloc (records_per_block, sizeof (Record));
	
	printf("%lu\n", sizeof(buffer));

	ftime(&t_begin);  


	if (!(fp_read = fopen ( filename , "rb" ))){
		printf ("Could not open file \"%s\" for reading \n", filename);
		return (-1);

	} 
	
	/* read records into buffer */
	while((bytes_read = fread (buffer, sizeof(Record), records_per_block, fp_read)) > 0){
		while (in_buff < bytes_read){
			if (buffer[in_buff].uid1 != current_id){
				current_id = buffer[in_buff].uid1;
				unique_ids++;
				current_amount_for_id = 0;
			}
			total_records ++;
			in_buff++;
			current_amount_for_id++;
			if (current_amount_for_id > max_followers)
				max_followers = current_amount_for_id;
		}
		memset(buffer, 0, sizeof(*buffer));
		in_buff = 0;
	}
	
	fclose (fp_read);
	free (buffer);

	 /* code to be timed */
	ftime(&t_end);     
	 
	/* time elapsed in milliseconds */
	time_spent_ms = (long) (1000 *(t_end.time - t_begin.time)
	       + (t_end.millitm - t_begin.millitm)); 
 

	float average = total_records / (float) unique_ids;

	printf("Max follows: %d Average follows: %f \n", max_followers, average);
	/* result in MB per second */
	printf ("Data rate: %.3f MBPS\n", ((total_records*sizeof(Record))/(float)time_spent_ms * 1000)/MB);
	return 0;
}

int main(int argc, char **argv){
	read_blocks_seq(argv[1], atoi(argv[2]));	
	return 0;
} 
