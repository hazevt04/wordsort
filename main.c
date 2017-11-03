#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>

#include "my_util.h"

#include "ws_word.h"

///////////////////////////////
// PROJECT 2 COMP 3220
// Wordsort (ws)
// Glenn Hazelwood
// 11/3/2017
//
// main.c
//
// Main driver program for ws.
//////////////////////////////

void usage( char **argv ) {
   printf( "%s <options> <optional filenames>\n", argv[0] );
   printf( "Does a sort of the words either in all of the filenames or entered by the user at a prompt.\n" ); 
   printf( "The options determine sort order.\n" ); 
   printf( "An options can be:\n" );
   printf( "-c <n>      Print the first n results of the sorted list of words\n" );
   printf( "-r          Print the list of words in sorted in reverse order (descending)\n" );
   printf( "-n          Print the list of words sorted as if they were numbers\n" );
   printf( "-l          Print the list of words sorted by lengths\n" );
   printf( "-s          Print the list of words sorted by their Scrabble scores\n" );
   printf( "-a          Print the list of words sorted lexicographically (ascending)\n" );
   printf( "-u          Print the list of words with no duplicates ( like uniq -u in UNIX )\n" );
   printf( "-h          Print help\n" );
   printf( "\n" );
   printf( "Most recently seen order option will take precedence. Example:\n" );
   printf( "'ws -s -l file1' will print the words in file1 sorted by their lengths\n" );
   printf( "\n" );
   printf( "It is possible to stack -r -u and -c <n> with the ordering options-- -n -l -s -a\n" );
   printf( "Example: ws -rl file1 will print the words in file1 sorted in reverse by their lengths\n" );  
   printf( "\n" ); 
   printf( "Each pair of -r options cancels each other out.\n" );
   printf( "Example: ws -r -r file1 will print the words in file1 sorted in normal ascending order" ); 
   printf( "\n" );
   printf( "Trailing punctuation is considered part of the word.\n" );
   printf( "\n" );
   printf( "If no filename is provided then the words to be sorted will be entered\n" );
   printf( "at the command line. Cntrl+D when done.\n" );
   printf( "\n" ); 
}

void print_order_select( int order_select ) {
   HDEBUG_PRINTF( "Inside %s(): order_select (in hex) is %#x\n", 
      __func__, order_select ); 

   switch( order_select ) {
		case N_ORDER_SELECT:
			printf( "(-n) numerical order selected.\n" );
			break;
		case L_ORDER_SELECT:
			printf( "(-l) length order selected.\n" );
			break;
		case S_ORDER_SELECT:
			printf( "(-s) Scrabble Score order selected.\n" );
			break;
		case A_ORDER_SELECT:
			printf( "(-a) Lexicographical order selected.\n" );
			break;
      default:
         fprintf( stderr, "Inside %s(): ERROR: Invalid order select: %#x\n", 
            __func__, order_select  ); 
         exit(EXIT_FAILURE);
   }

}

int main( int argc, char** argv ) {
   // "One-hot" selecton (only one bit can be set at a time)
   // The last option passed in getopt will ultimately
   // determine the final order_select, which is 
   // guaranteed to only have one flag set by the "one-hot" selection.

   // Defaul order select is '-a', lexicographical
   int order_select = A_ORDER_SELECT;

   int c_flag_num_words = 0;
   bool do_reverse = false;
   bool do_unique = false;
   bool do_first_n = false;
   bool show_count = false; 
   int opt;

   while( ( opt = getopt( argc, argv, "rnlsauhc:" ) ) != -1 ) {
      // Parse option arguments
      switch( opt ) {
         case 'r':
            do_reverse = !do_reverse;
            HDEBUG_PRINTF( "Sort%s",
               ( ( do_reverse ) ? " in reverse order\n" : "\n" )
            ); 
            break;
         case 'n':
            order_select = N_ORDER_SELECT;
            HDEBUG_PRINTF( "Sort as if numbers\n" ); 
            break;
         case 'l':
            order_select = L_ORDER_SELECT;
            HDEBUG_PRINTF( "Sort by word length\n" ); 
            break;
         case 's':
            order_select = S_ORDER_SELECT;
            HDEBUG_PRINTF( "Sort by Scrabble scoring rules\n" ); 
            break;
         case 'a':
            order_select = A_ORDER_SELECT;
            HDEBUG_PRINTF( "Sort lexicographically "
                  "(same as default)\n" ); 
            break;
         case 'u':
            do_unique = true;
            HDEBUG_PRINTF( "Sort with no duplicates\n" ); 
            break;
         case 'c':
            HDEBUG_PRINTF( "Inside %s(): optarg is %s\n", __func__, optarg ); 
            do_first_n = true;
            // Convert optarg to number
            c_flag_num_words = atoi(optarg);
            HDEBUG_PRINTF( "Sort lexicographically and only show %d words\n",
               c_flag_num_words ); 
            break;
         case 'h':
            usage( argv );
            return EXIT_SUCCESS;
         case '?':
            if ( optopt == 'c' ) {
               fprintf( stderr, "Option '-%c' requires an argument.\n", 
                  optopt);
            } else if ( isprint( optopt ) ) {
               fprintf( stderr, "Unknown option '-%c'.\n", optopt );
            } else {
               fprintf( stderr, "Unknown option character `\\x%x'.\n",
                  optopt);   
            } // end of if ( optopt == 'c' )
            return EXIT_FAILURE;
         default:
            return EXIT_FAILURE;
      } // end of switch
   } // end of while
 
   // Select comp_func based on flags
   int ( *comp_func ) ( const ws_word_t *left, const ws_word_t *right );
   comp_func = select_cmp_func( order_select, do_reverse );
  
   HDEBUG_PRINTF( "Inside %s(): word_cmp() ptr is %p\n", __func__, word_cmp );  
   HDEBUG_PRINTF( "Inside %s(): word_cmp_rev() ptr is %p\n", __func__, word_cmp_rev );  
   HDEBUG_PRINTF( "Inside %s(): word_len_cmp() ptr is %p\n", __func__, word_len_cmp );  
   HDEBUG_PRINTF( "Inside %s(): word_len_cmp_rev() ptr is %p\n", __func__, word_len_cmp_rev );  
   HDEBUG_PRINTF( "Inside %s(): word_as_num_cmp() ptr is %p\n", __func__, word_as_num_cmp );  
   HDEBUG_PRINTF( "Inside %s(): word_as_num_cmp_rev() ptr is %p\n", __func__, word_as_num_cmp_rev );  
   HDEBUG_PRINTF( "Inside %s(): scrabble_score_cmp() ptr is %p\n", __func__, scrabble_score_cmp );  
   HDEBUG_PRINTF( "Inside %s(): scrabble_score_cmp_rev() ptr is %p\n\n", 
         __func__, scrabble_score_cmp_rev );

   HDEBUG_PRINTF( "Inside %s(): The selected function, comp_func is %p\n\n", __func__, comp_func ); 

   HDEBUG_PRINTF( "Inside %s(): optind is %d\n", 
      __func__, optind ); 
   HDEBUG_PRINTF( "Inside %s(): argc is %d\n", 
      __func__, argc ); 

   int num_files = argc - optind;

   HDEBUG_PRINTF( "Inside %s(): num_files is %d\n", __func__, num_files ); 

   int file_index = 0;
   char **file_names;
   int argv_len;

   char* line;
   int line_len = 0;
   char* token_str;
   int token_len = 0;
   char** words;
   int* word_lengths;
   int num_words = 0;

   int num_ws_words = 0;
   ws_word_t *ws_words_head = NULL;
   ws_word_t *new_ws_word = NULL;
   
   // Try to allocate arrays for line and token_str.
   // Exit if errors
   MALLOC_AND_CHECK_ERROR( line, char, MAX_NUM_CHARS );
   MALLOC_AND_CHECK_ERROR( token_str, char, MAX_NUM_CHARS );

   MALLOC_AND_CHECK_ERROR( word_lengths, int, ( MAX_NUM_WORDS ) );
   MALLOC_AND_CHECK_ERROR( words, char*, ( MAX_NUM_WORDS ) );
   for( int i = 0; i < MAX_NUM_WORDS; i++ ) {
      MALLOC_AND_CHECK_ERROR( words[i], char, MAX_NUM_CHARS );
   } 
  
   int num_file_names_bytes = num_files * sizeof( char * );
   int file_name_len = 0;
   char full_file_name[MAX_NUM_CHARS];
   if ( num_files > 0 ) {
      file_index = 0;
         
      MALLOC_AND_CHECK_ERROR( file_names, char*, MAX_NUM_CHARS );

      // opt_ind is first non-option argument
      for ( int i = optind; i < argc; i++ ) {

         // Parse non-option arguments (filenames)
         HDEBUG_PRINTF( "Inside %s(): Beyond the options: argv[%d] is %s\n", 
            __func__, i, argv[i] );

         argv_len = strlen( argv[i] );

         HDEBUG_PRINTF( "Inside %s(): argv_len is %d\n", 
            __func__, argv_len ); 

         MALLOC_AND_CHECK_ERROR( file_names[file_index], char, MAX_NUM_CHARS );
        
         sprintf( full_file_name, "/home/%s/intclabs/class/project2/%s",
            getenv("USER"), argv[i] );

         strcpy( file_names[file_index], full_file_name );

         file_name_len = strlen( file_names[file_index] );
         HDEBUG_PRINTF( "Inside %s(): File name len is %d\n", __func__,
              file_name_len ); 

         HDEBUG_PRINTF( "Inside %s(): Need to get input from file %d: %s\n", 
            __func__, file_index, file_names[file_index] ); 
         
         get_words_from_file( file_names[file_index], words, word_lengths, &num_words );

         HDEBUG_PRINTF( "Inside %s(): file %s\t\tAfter check of file %d\n", 
            __func__, file_names[file_index], file_index ); 
         HDEBUG_PRINTF( "Inside %s(): file %s num_words is %d\n", 
            __func__, file_names[file_index], num_words ); 
         
         file_index++;
            
      } // end of for ( int i = opt_ind; i < argc; i++ )

   } else {
      
      HDEBUG_PRINTF( "Inside %s(): Need to get input from user: \n", __func__ ); 

      printf( "Enter words (100 max, each a max of 80 characters; "
            "Cntrl+D to exit): " ); 

      while( fgets(line, MAX_NUM_CHARS, stdin ) != NULL ) {
         line_len = strlen( line );
         HDEBUG_PRINTF( "Inside %s(): Line is %s\n", __func__, line ); 
         HDEBUG_PRINTF( "Inside %s(): Line len is %d\n", __func__, line_len ); 

         if ( num_words >= MAX_NUM_WORDS ) {
            printf( "No More Words. Max number is %d\n", MAX_NUM_WORDS ); 
            break;
         }
         token_str = strtok( line, " \n" );
         HDEBUG_PRINTF( "Inside %s(): token is '%s'\n", 
            __func__, token_str ); 

         while( token_str != NULL ) {
            token_len = strlen( token_str );
            
            HDEBUG_PRINTF( "Inside %s():\tWhile loop. Word Num is %d:\n", 
               __func__, num_words ); 
            HDEBUG_PRINTF( "Inside %s():\tWhile loop. Token len is %d:\n", 
               __func__, token_len ); 
            
            word_lengths[num_words] = token_len;

            HDEBUG_PRINTF( "Inside %s():\tWhile loop. Word len %d is %d\n", 
               __func__, num_words, word_lengths[num_words] ); 

            // Try to allocate array for word
            // Exit if error
            MALLOC_AND_CHECK_ERROR( words[num_words], char, MAX_NUM_CHARS );

            strcpy( words[num_words], token_str );

            HDEBUG_PRINTF( "Inside %s():\tWhile loop. Word %d is %s\n", 
               __func__, num_words, words[num_words] ); 
            num_words++;

            // Get the next token from line
            token_str = strtok( NULL, " \n" );

         } // end of while( token_str != NULL )
      } // end of while( fgets(line, MAX_NUM_CHARS, stdin ) != NULL )

      //print_order_select( order_select );
      HDEBUG_PRINTF( "\n" ); 

   } // end of else

   HDEBUG_PRINTF( "Inside %s(): Num words read in is %d\n", 
      __func__, num_words ); 
   
   // Make a linked list of ws_word_t's based on the words
   for( int i = 0; i < num_words; i++ ) {
      
      // make a new ws_word_t node.
      new_ws_word = create_ws_word( words[i], word_lengths[i] );

      assert ( new_ws_word != NULL );
      // This won't do anything if -u and the word is already in ws_words
      ws_words_head = insert_ws_word_sorted( new_ws_word, ws_words_head, &num_ws_words, 
         do_unique, comp_func );

      HDEBUG_PRINTF( "Inside %s(): After insert_ws_word_sorted for word %s num_ws_words is %d\n", 
         __func__, words[i], num_ws_words );
      HDEBUG_PRINTF( "Inside %s(): Loop index is %d and num_ws_words is %d\n", __func__,
           i, num_ws_words ); 
      HDEBUG_PRINTF( "Inside %s(): new_ws_word is %p\n", __func__, new_ws_word ); 
      HDEBUG_PRINTF( "Inside %s(): ws_words_head is %p\n", __func__, ws_words_head ); 
   }

   int num_print_words;
   if ( do_first_n ) {
      num_print_words = c_flag_num_words;
      HDEBUG_PRINTF( "Inside %s(): Due to -c option, only printing %d words\n",
         __func__, num_print_words ); 

   } else {
      num_print_words = num_ws_words;
      HDEBUG_PRINTF( "Inside %s(): No -c option, printing %d %s.\n",
         __func__, num_print_words,
         ( num_print_words == 1 ) ? "word" : "words"
      ); 
   }

   HDEBUG_PRINTF( "Inside %s(): Final Print of Sorted Words: \n", __func__ ); 
   print_ws_words( num_print_words, ws_words_head, do_unique, show_count );
   
   destroy_ws_words( ws_words_head );
   
   return EXIT_SUCCESS;
} 
