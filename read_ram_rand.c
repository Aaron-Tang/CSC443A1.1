#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/timeb.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include "utils.h"

int fsize(const char *filename) {
    struct stat st;

    if (stat(filename, &st) == 0)
        return (int) st.st_size;

    fprintf(stderr, "Cannot determine size of %s: %s\n",
            filename, strerror(errno));

    return -1;
}

int read_ram_rand(char * filename, int blocksize, int loop_amount){
	int max_followers = 0;

	int unique_ids = 0;
	int total_follows = 0;

	int current_amount_for_id = 0;
	int current_id = -1;

	int filesize;

	struct timeb t_begin, t_end;
	long time_spent_ms;
	 
	long total_records = 0;

	int iterations = loop_amount;
	 
	FILE *fp_read;
	
	/* allocate buffer for 1 block */

	if (blocksize % sizeof(Record) != 0){
		printf("Incorrect blocksize\n");
		return (-1);
	}

	filesize =  fsize(filename);
	Record * buffer = (Record *) calloc (1, filesize);
	

	if (!(fp_read = fopen ( filename , "rb" ))){
		printf ("Could not open file \"%s\" for reading \n", filename);
		return (-1);

	} 


	/* read records into buffer */
	if (!fread (buffer, filesize, 1, fp_read)){
		printf ("Could not read file \"%s\" for reading \n", filename);
		return (-1);
	}

	ftime(&t_begin);  
	while (iterations != 0){

		srand(time(NULL));
		int random_posiiton = rand() % (filesize / sizeof(Record) - 1);


		for (int i = 0; i < blocksize; i++){
			if (i + random_posiiton >= filesize)
				break;

			if (buffer[i + random_posiiton].uid1 != current_id){
				if (current_amount_for_id > max_followers){
					max_followers = current_amount_for_id;
				}

				unique_ids += 1;
				current_id = buffer[i + random_posiiton].uid1;
				current_amount_for_id = 1;
				total_follows += 1;
			}
			else{
				current_amount_for_id += 1;
				total_follows += 1;

			}
			/* code to be timed */
			total_records++;
 		}	
 		iterations--;
	 }
	ftime(&t_end);     
	
	fclose (fp_read);
	free (buffer);
	
	float average = total_follows / (float) unique_ids;
	/* time elapsed in milliseconds */
	time_spent_ms = (long) (1000 *(t_end.time - t_begin.time)
	       + (t_end.millitm - t_begin.millitm)); 
	 
	/* result in MB per second */
	printf ("Data rate: %.3f MBPS\n", ((total_records*sizeof(Record))/(float)time_spent_ms * 1000)/MB);


	printf("Max follows: %d Average follows: %f \n", max_followers, average);

	return 0;
}

int main(int argc, char **argv){
	read_ram_rand(argv[1], atoi(argv[2]), atoi(argv[3]));	
	return 0;
} 