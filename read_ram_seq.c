#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/timeb.h>
#include <sys/stat.h>
#include <errno.h>
#include "utils.c"

off_t fsize(const char *filename) {
    struct stat st;

    if (stat(filename, &st) == 0)
        return st.st_size;

    fprintf(stderr, "Cannot determine size of %s: %s\n",
            filename, strerror(errno));

    return -1;
}

static int read_ram_seq(char * filename){
	int max_followers = 0;

	int unique_ids = 0;
	int total_follows = 0;

	int current_amount_for_id = 0;
	int current_id;

	size_t bytes_read = 0;
	int filesize;

	FILE *fp_read;
	
	/* allocate buffer for 1 block */
	filesize = (int) fsize(filename);
	Record * buffer = (Record *) calloc (1, filesize);
	

	if (!(fp_read = fopen ( filename , "rb" ))){
		printf ("Could not open file \"%s\" for reading \n", filename);
		return (-1);

	} 


	/* read records into buffer */
	while((bytes_read = fread (buffer, filesize, 1, fp_read)) > 0){
		for (int i = 0; i < filesize; i++){
			if (buffer[i * 8].uid1 != current_id){
				if (current_amount_for_id > max_followers){
					max_followers = current_amount_for_id;
				}

				unique_ids += 1;
				current_id = buffer[i * 8].uid1;
				current_amount_for_id = 1;
				total_follows += 1;
			}
			else{
				current_amount_for_id += 1;
				total_follows += 1;

			}
		}
	}
	
	fclose (fp_read);
	free (buffer);

	float average = total_follows / (float) unique_ids;

	printf("Max follows: %d Average follows: %f \n", max_followers, average);

	return 0;
}

int main(int argc, char **argv){
	read_ram_seq(argv[1]);	
}