#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/timeb.h>
#include "utils.h"

int write_lines(char * filename){

	struct timeb t_begin, t_end;
	long time_spent_ms; 
	long total_records_time = 0;

	// remove later?
	int MB = 1024 * 1024;
	   
	
	char current_line[MAX_CHARS_PER_LINE];

	FILE *fp_read;
	FILE *fp_write;
	
	ftime(&t_begin);  

	/* open text file for reading */
	if (!(fp_read= fopen(filename, "r"))) {
		printf ("Could not open file \"%s\" for reading \n", filename);
		return (-1);
	}

	if (!(fp_write = fopen ("records_2.csv" , "w" ))){
		printf ("Could not open file records_2.csv for writing \n");
		return (-1);
	}
	     

	/* reading lines */
	while( fgets (current_line, MAX_CHARS_PER_LINE, fp_read)!=NULL ) {
		current_line [strcspn (current_line, "\r\n")] = '\0'; //remove end-of-line characters
		fwrite (current_line, sizeof(current_line), 1, fp_write);
		total_records_time ++;
	}
	
	fclose(fp_write);
	fclose (fp_read);


	ftime(&t_end);     
	 
	/* time elapsed in milliseconds */
	time_spent_ms = (long) (1000 *(t_end.time - t_begin.time)
	       + (t_end.millitm - t_begin.millitm)); 
	 
	/* result in MB per second */
	// replace MB with integer values
	printf ("Data rate: %.3f MBPS\n", ((total_records_time*8/(float)time_spent_ms * 1000)/MB));
	return 0;
}

int main(int argc, char **argv){
	write_lines(argv[1]);	
	return 0;
} 