#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/timeb.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include "utils.c"

int fsize(const char *filename) {
    struct stat st;

    if (stat(filename, &st) == 0)
        return (int) st.st_size;

    fprintf(stderr, "Cannot determine size of %s: %s\n",
            filename, strerror(errno));

    return -1;
}

static int write_ram_rand(char * filename, int blocksize, int loop_amount){
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
	

	if (!(fp_read = fopen ( filename , "rb+" ))){
		printf ("Could not open file \"%s\" for reading \n", filename);
		return (-1);

	} 


	/* read records into buffer */
	fread (buffer, filesize, 1, fp_read);
	ftime(&t_begin); 

	while (iterations != 0){

		srand(time(NULL));
		int random_posiiton = rand() % (filesize / sizeof(Record) - 1);

		buffer[random_posiiton].uid1 = 2;
		buffer[random_posiiton].uid2 = 1;

		total_records++;
 		iterations--;
	 }
	ftime(&t_end);     
	
	fclose (fp_read);
	free (buffer);
	
	/* time elapsed in milliseconds */
	time_spent_ms = (long) (1000 *(t_end.time - t_begin.time)
	       + (t_end.millitm - t_begin.millitm)); 
	 
	/* result in MB per second */
	printf ("Data rate: %.3f MBPS\n", ((total_records*sizeof(Record))/(float)time_spent_ms * 1000)/MB;



	return 0;
}

