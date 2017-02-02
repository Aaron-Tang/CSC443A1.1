#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/timeb.h>
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

int write_blocks_rand(char * filename, int blocksize, int loop_amount){
	int filesize;

	struct timeb t_begin, t_end;
	long time_spent_ms;
	 
	long total_records = 0;

	int iterations = loop_amount;
 
	if (blocksize % sizeof(Record) != 0){
		printf("Incorrect blocksize\n");
		return (-1);
	}


	filesize =  fsize(filename);


	FILE *fp_read;
	
	/* allocate buffer for 1 block */
	Record * buffer = (Record *) calloc (1, sizeof (Record));
	struct record new_record = {2, 1};
	buffer[0] = new_record;

	if (!(fp_read = fopen ( filename , "rb+" ))){
		printf ("Could not open file \"%s\" for reading \n", filename);
		return (-1);

	} 
	
	
	ftime(&t_begin);  

	while (iterations != 0){
		srand(time(NULL));
		int random_posiiton = rand() % (filesize / sizeof(Record) - 1);

		fseek(fp_read, random_posiiton * sizeof(Record), SEEK_SET);
		fwrite (buffer, sizeof(Record), 1, fp_read);

		total_records ++;
		iterations--;
	}
		
	fclose (fp_read);
	free (buffer);


	 /* code to be timed */
	ftime(&t_end);     
	 
	/* time elapsed in milliseconds */
	time_spent_ms = (long) (1000 *(t_end.time - t_begin.time)
	       + (t_end.millitm - t_begin.millitm)); 
 
	/* result in MB per second */
	printf ("Data rate: %.3f MBPS\n", ((total_records*sizeof(Record))/(float)time_spent_ms * 1000)/MB);



	return 0;
}

int main(int argc, char **argv){
	write_blocks_rand(argv[1], atoi(argv[2]), atoi(argv[3]));	
	return 0;
} 