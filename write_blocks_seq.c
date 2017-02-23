#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/timeb.h>

#include "write_blocks_seq.h"

// Assume processing time = time it takes to read/convert the entire .csv file
int main(int argc, char const *argv[])
{
  if (argc < 3) {
    printf("Too few arguments!\n");
    return -1;  
  }

  char *input_file = (char *)argv[1];
  int block_size = atoi(argv[2]);
  if (block_size % sizeof(Record) != 0) {
    printf("Block size must be a multiple of sizeof(Record).\n");
    return -1;
  }
  struct timeb t_begin, t_end;
  long time_spent_ms;
  long total_records = 0;
   
  ftime(&t_begin);  
  total_records = write_blocks_seq(input_file, block_size);  
  ftime(&t_end);     
   
  /* time elapsed in milliseconds */
  time_spent_ms = (long) (1000 *(t_end.time - t_begin.time)
         + (t_end.millitm - t_begin.millitm)); 
   
  /* result in MB per second */
  long total_bytes = get_total_bytes(total_records);
  printf("\tTotal Records: %ld\n", total_records);
  printf("\tTotal Bytes Read: %lu, ", total_bytes);
  printf("\tTimespent (ms): %ld\n", time_spent_ms);
  printf("\tData rate: %.3f MBPS\n", (total_bytes/(float)time_spent_ms * 1000)/MB);
  return 0;
}

int write_blocks_seq (char * input_file, int block_size) {
  
  int records_per_block = block_size/sizeof(Record);
  FILE *fp_write;
  FILE *fp_read;

  if (!(fp_write = fopen ( OUTPUT_FILE , "wb" ))) {
    printf ("Could not open file \"%s\" for writing \n", OUTPUT_FILE);
  }
  if (!(fp_read = fopen ( input_file , "r" ))) {
    printf ("Could not open file \"%s\" for reading \n", input_file);
  }

  int result = write_blocks_helper(fp_read, fp_write, records_per_block);
  printf("\nwrite_blocks_seq completed.\n");
  fclose(fp_write);
  fclose(fp_read);
  
  return result;
}

int write_blocks_helper(FILE* fp_read, FILE* fp_write, int num_records_per_block) {
  char current_line[MAX_CHARS_PER_LINE];
  char *tmp_string, *dup_pointer;
  int record_index_in_buffer = 0, total_records = 0;
  Record *buffer = (Record *) calloc (num_records_per_block, sizeof (Record));
  Record *new_record;

  while( fgets (current_line, MAX_CHARS_PER_LINE, fp_read)!=NULL ) {
    if (current_line[0] != '\0') {
      current_line [strcspn (current_line, "\r\n")] = '\0'; //remove end-of-line characters  
      dup_pointer = tmp_string = strdup(current_line);

      new_record = (Record*)malloc(sizeof(Record));
      new_record->uid1 = atoi(strsep(&tmp_string, ","));
      new_record->uid2 = atoi(strsep(&tmp_string, ","));
      
      if (record_index_in_buffer == num_records_per_block) {
        fwrite (buffer, sizeof(Record), record_index_in_buffer, fp_write);
        //Reset the buffer after writing
        memset(buffer, 0, sizeof(*buffer));
        record_index_in_buffer = 0;
      }
      
      memcpy(&buffer[record_index_in_buffer], new_record, sizeof(Record));
      total_records++;
      record_index_in_buffer++;

      free(dup_pointer);
      free(new_record);
    }
  }
  /* Write anything that is left in the buffer */
  fwrite(buffer, sizeof(Record), record_index_in_buffer, fp_write);
  fflush(fp_write);
  free(buffer);
  return total_records;
}

