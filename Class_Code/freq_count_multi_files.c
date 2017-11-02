#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "my_util.h"
 
#define MAX_LINE_LEN 100
#define NUM_WORDS 1000
#define WORD_LENGTH 100

void usage( char **argv ) {
   HDEBUG_PRINTF("Usage: %s filename\n", argv[0]); 
}

char *find_word(char *word_token, char **words, int num_words, int *found_position ) {
   assert( word_token != NULL );
   assert( words != NULL );

   HDEBUG_PRINTF( "Inside %s(): num_words is %d\n", __func__, num_words ); 
   HDEBUG_PRINTF( "Inside %s(): word_token is %s\n", __func__, word_token ); 
   for(int i=0; i < num_words; i++) {
      assert( words[i] != NULL );
      if(!strcmp(word_token, words[i])) {
         *found_position = i;
         HDEBUG_PRINTF("Inside %s(): Found '%s' at position %d\n", 
            __func__ , words[i], *found_position); 
         return words[i];
      }
   }   
   return NULL;
}

void check_file_for_word_frequencies( char *file_name, char **words, int *num_words, int *counts ) {
   FILE *fp;
   char word[MAX_LINE_LEN];
   char *found_word;
   int found_position;
  
   int word_index = *num_words; 
   char *word_token;
   char delimiters[] = " ~`!@#$%^&*()_-+={[}]|\\:;\"'<,>.?/\n";

   fp = fopen(file_name,"r");
   if (!fp) {
      fprintf(stderr, "ERROR: file %s could not be opened.\n", file_name);
      exit(EXIT_FAILURE);
   } else {
      //while(fscanf(fp,"%s", word) != EOF) {
      while(fgets( word, MAX_LINE_LEN, fp ) != NULL) {
         word_token = strtok( word, delimiters );
         HDEBUG_PRINTF("Inside %s(): file %s Word Token is '%s'\n", 
            __func__, file_name, word_token); 
         HDEBUG_PRINTF( "Inside %s(): file %s Word count is %d\n", 
            __func__, file_name, word_index ); 
         while (word_token != NULL) {
            found_word = find_word(word_token, words, word_index, &found_position);
            if (found_word == NULL) {
               HDEBUG_PRINTF("Inside %s(): file %s '%s' not found. Adding it\n", 
                  __func__, file_name, word_token); 
               HDEBUG_PRINTF("Inside %s(): file %s Word Count is %d\n", 
                  __func__, file_name, word_index); 
               HDEBUG_PRINTF("Inside %s(): file %s Words[%d] (ptr) is %p\n", 
                  __func__, file_name, word_index, words[word_index]); 

               strcpy(words[word_index],word_token);
               
               HDEBUG_PRINTF("Inside %s(): file %s Words[%d] is '%s'\n\n", 
                  __func__, file_name, word_index, words[word_index]); 
               counts[word_index]++;  
               word_index++;
            } else {
               HDEBUG_PRINTF("Inside %s(): file %s \t'%s' found at position %d. Not adding it.\n\n", 
                  __func__, file_name, word_token, found_position);
               counts[found_position]++;  
            }
            word_token = strtok( NULL, delimiters );
            HDEBUG_PRINTF( "Inside %s(): file %s word_index at end of while loops is %d\n", 
               __func__, file_name, word_index ); 
         } // tokenize while loop to get rid of symbols
      } // end of while loop

      *num_words = word_index;

      HDEBUG_PRINTF( "Inside %s(): file %s outside while loop: *num_words is %d\n", 
         __func__, file_name, *num_words ); 
      HDEBUG_PRINTF( "Inside %s(): file %s outside while loop: word_index is %d\n", 
         __func__, file_name, word_index ); 
      HDEBUG_PRINTF( "\n" ); 
   } // end of else (if fp) 
   fclose(fp); 
} // end of  check_file_for_word_frequencies()


int main( int argc, char** argv ) {

   char **words = (char**)malloc(NUM_WORDS*sizeof(char));
   assert(words != NULL);
   
   for(int i=0; i < NUM_WORDS; i++) {
      words[i] = (char*)malloc((WORD_LENGTH)*sizeof(char));
      assert(words[i] != NULL);
   }
   int *counts;
   char line[MAX_LINE_LEN];
   int line_len;
   char *token_str;
   int num_file_names = 0;
   int total_num_words = 0;
   int num_words = 0;
   
   char **file_names = (char**)malloc(NUM_WORDS*sizeof(char));
   assert(file_names != NULL);
   
   for(int i=0; i < NUM_WORDS; i++) {
      file_names[i] = (char*)malloc((WORD_LENGTH)*sizeof(char));
      assert(file_names[i] != NULL);
   }

   counts = (int*)malloc(NUM_WORDS*sizeof(int));
   assert( counts != NULL );

   printf( "Enter filenames: " ); 
   
   fgets( line, MAX_LINE_LEN, stdin );
   line_len = strlen( line );
   line[ line_len - 1 ] = '\0';

   HDEBUG_PRINTF( "Inside %s(): line is %s\n", 
      __func__, line ); 
   HDEBUG_PRINTF( "Inside %s(): line_len is %d\n", 
      __func__, line_len );

   // Parse the file_name entried
   token_str = strtok( line, " " );

   while( token_str != NULL ) {

      HDEBUG_PRINTF( "Inside %s():\t\tWhile loop: num_file_names is %d\n", 
         __func__, num_file_names ); 
      HDEBUG_PRINTF( "Inside %s():\t\tWhile loop: token_str is %s\n", 
         __func__, token_str ); 

      strcpy( file_names[num_file_names], token_str );
      num_file_names++;

      token_str = strtok( NULL, " " );
   } 
   
   // For each entry
   for( int i = 0; i < num_file_names; i++ ) {
      // Check the file
      check_file_for_word_frequencies( file_names[i], words, &num_words, counts );

      HDEBUG_PRINTF( "Inside %s(): file %s\t\tAfter check of file %d\n", 
         __func__, file_names[i], i ); 
      HDEBUG_PRINTF( "Inside %s(): file %s num_words is %d\n", 
         __func__, file_names[i], num_words ); 
   } 

   printf( "Word Frequencies across " ); 
   for( int i = 0; i < num_file_names; i++ ) {
      printf( "%s%s%s", 
         ((i == (num_file_names - 1)) ? " and " : ""),
         file_names[i],
         ((i == (num_file_names - 1)) ? " :" : ",") 
      );
   }
   printf( "\n" );

   printf( "%5s   %25s   %5s\n", "INDEX", "WORD", "COUNT" ); 
   for( int i = 0; i < num_words; i++ ) {
      printf( "%5d   %25s   %5d\n",i, words[i], counts[i] ); 
   } 
   printf( "\n" ); 

   return EXIT_SUCCESS;
}
 
