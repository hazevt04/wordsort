#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <errno.h>
#include <string.h>

#include "my_util.h"

//////////////////////////////////
// PROJECT 2 COMP 3220
// Wordsort (ws)
// Glenn Hazelwood
// 11/3/2017
//
// my_util.c
//
// Implementation of a function 
// for getting words from a file
//////////////////////////////////

// If file_name exists, opens that file and reads words from it. Saves the words in a 2-D
// array called words and the lengths in an array called word_lengths. Also increments the 
// number or words in the 2-D words array to allow the words array to be resused to read another file.
void get_words_from_file( char *file_name, char **words, int *word_lengths, int *num_words ) {
   FILE *fp;
   char *line;
   int line_len = 0;
  
   int word_index = *num_words; 
   char *word_token;
   int word_token_len = 0;
   char delimiters[] = " \n";

   HDEBUG_PRINTF( "Inside %s(): File name is %s\n", __func__,
      file_name ); 

   fp = fopen( file_name, "r" );
   if ( errno == ENOENT ) {
      fprintf( stderr, "Inside %s(): ERROR: File %s could not be found\n", 
         __func__, file_name ); 
      exit( EXIT_FAILURE );
   }

   if ( errno == EACCES ) {
      fprintf( stderr, "Inside %s(): ERROR: File %s could not be accessed\n", 
         __func__, file_name ); 
   }
   
   if ( !fp ) {
      fprintf( stderr, "ERROR: file %s could not be opened.\n", file_name );
      exit( EXIT_FAILURE );
   } else {
      MALLOC_AND_CHECK_ERROR( line, char, MAX_NUM_CHARS );
      while( fgets( line, MAX_NUM_CHARS, fp ) != NULL ) {
         word_token = strtok( line, delimiters );
         
         HDEBUG_PRINTF( "Inside %s(): file %s Word Index is %d\n", 
            __func__, file_name, word_index ); 

         while ( word_token != NULL ) {
            word_token_len = strlen( word_token );
            
            HDEBUG_PRINTF( "Inside %s(): file %s Word Token is '%s'\n", 
            __func__, file_name, word_token );
            HDEBUG_PRINTF( "Inside %s(): file %s Word Token Length is %d\n\n", 
            __func__, file_name, word_token_len );  

            MALLOC_AND_CHECK_ERROR( words[word_index], char, MAX_NUM_CHARS );
            
            HDEBUG_PRINTF( "Inside %s(): file %s Words[%d] (ptr) is %p\n", 
               __func__, file_name, word_index, words[word_index] ); 
            
            strcpy( words[word_index], word_token );
            words[word_index][word_token_len] = '\0';
            word_lengths[word_index] = word_token_len;

            HDEBUG_PRINTF("Inside %s(): file %s Words[%d] is '%s'\n\n", 
               __func__, file_name, word_index, words[word_index]); 
            word_index++;

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

} // end of get_words_from_file( char *file_name, char **words, int *num_words ) {

