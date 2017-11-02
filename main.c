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



void usage( char **argv ) {
   printf( "%s <options> <optional filename>\n", argv[0] );
   printf( "An options can be:\n" );
   printf( "-c <n>      Print the first n results of the sorted list of words\n" );
   printf( "-r          Print the list of words in sorted in reverse order\n" );
   printf( "-n          Print the list of words sorted as if they were numbers\n" );
   printf( "-l          Print the list of words sorted by lengths\n" );
   printf( "-s          Print the list of words sorted by their Scrabble scores\n" );
   printf( "-a          Print the list of words sorted lexicographically\n" );
   printf( "-u          Print the list of words sorted lexicographically with no duplicates\n" );
   printf( "-h          Print help\n" );
   printf( "\n" );
   printf( "Most recently seen option will take precedence. Example:\n" );
   printf( "'ws -r -l file1' will print the words in file1 sorted by their lengths\n" );
   printf( "\n" );
   printf( "Each pair of -r options cancels each other out.\n" );
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
   HDEBUG_PRINTF( "Inside %s(): scrabble_score_cmp_rev() ptr is %p\n\n", __func__, scrabble_score_cmp_rev );

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
   char* token_str;
   int token_len = 0;
   char* word;
   int word_len = 0;
   int num_ws_words = 0;
   ws_word_t *ws_words_head;
   ws_word_t *new_ws_word;
   
   initialize_ws_words( ws_words_head );
   
   if ( num_files > 0 ) {
      file_index = 0;
      file_names = ( char** )malloc( num_files * sizeof( char * ) );

      // opt_ind is first non-option argument
      for ( int i = optind; i < argc; i++ ) {

         // Parse non-option arguments (filenames)
         HDEBUG_PRINTF( "Inside %s(): Beyond the options: argv[%d] is %s\n", 
            __func__, i, argv[i] );

         argv_len = strlen( argv[i] );

         HDEBUG_PRINTF( "Inside %s(): argv_len is %d\n", 
            __func__, argv_len ); 

         file_names[file_index] = ( char* )malloc( argv_len + 1 );
         file_names[file_index][argv_len] = '\0'; 

         strcpy( file_names[file_index], argv[i] );
         file_index++;

      } // end of for ( int i = opt_ind; i < argc; i++ )

   } else {
      
      // Try to allocate arrays for line and token_str.
      // Exit if errors
      MALLOC_AND_CHECK_ERROR( line, char, MAX_NUM_CHARS );
      MALLOC_AND_CHECK_ERROR( token_str, char, MAX_NUM_CHARS );

      HDEBUG_PRINTF( "Inside %s(): Need to get input from user: ", __func__ ); 

      printf( "Enter words (100 max, each a max of 80 characters; "
            "Cntrl+D to exit): " ); 

      while( fgets(line, MAX_NUM_CHARS, stdin ) != NULL ) {
         token_str = strtok( line, " \n" );
         HDEBUG_PRINTF( "Inside %s(): token is '%s'\n", 
            __func__, token_str ); 

         while( token_str != NULL ) {
            token_len = strlen( token_str );
            
            HDEBUG_PRINTF( "Inside %s():\tWhile loop. Word Num is %d:\n", 
               __func__, num_ws_words ); 
            HDEBUG_PRINTF( "Inside %s():\tWhile loop. Token len is %d:\n", 
               __func__, token_len ); 
            
            word_len = token_len;

            HDEBUG_PRINTF( "Inside %s():\tWhile loop. Word len is %d", 
               __func__, word_len ); 

            // Try to allocate array for word
            // Exit if error
            MALLOC_AND_CHECK_ERROR( word, char, ( word_len + 1 ) );

            strcpy( word, token_str );
            word[word_len] = '\0';

            // Try to make a new ws_word_t node.
            // This won't do anything if -u and the word is already in ws_words
            new_ws_word = create_ws_word( word, word_len );
            insert_ws_word_sorted( new_ws_word, ws_words_head, &num_ws_words, 
               do_unique, comp_func );

            HDEBUG_PRINTF( "Inside %s(): After insert_ws_word_sorted for word %s num_ws_words is %d\n", 
               __func__, word, num_ws_words );

            // Get the next token from line
            token_str = strtok( NULL, " " );

         } // end of while( token_str != NULL )
      } // end of while( fgets(line, MAX_NUM_CHARS, stdin ) != NULL )

      print_order_select( order_select );
      HDEBUG_PRINTF( "\n" ); 

      int num_print_words;
      if ( do_first_n ) {
         num_print_words = c_flag_num_words;
         HDEBUG_PRINTF( "Inside %s(): Due to -c option, only printing %d words\n",
            __func__, num_print_words ); 

      } else {
         num_print_words = num_ws_words;
         HDEBUG_PRINTF( "Inside %s(): No -c option, printing all %d words\n",
            __func__, num_print_words ); 
      }
      HDEBUG_PRINTF( "Inside %s(): Print Sorted Words", __func__ ); 
      print_ws_words( num_print_words, ws_words_head, do_unique );
   }


   return EXIT_SUCCESS;
} 
