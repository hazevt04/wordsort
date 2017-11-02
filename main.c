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

#define NO_FLAG_SELECT  0

// "One-Hot" Select Values
#define C_FLAG_SELECT ( ( unsigned int ) 1 << 0 )

// Sort order flags, mutually exclusive
#define N_FLAG_SELECT ( ( unsigned int ) 1 << 1 )
#define L_FLAG_SELECT ( ( unsigned int ) 1 << 2 )
#define S_FLAG_SELECT ( ( unsigned int ) 1 << 3 )
#define A_FLAG_SELECT ( ( unsigned int ) 1 << 4 )

// Optional flags adding to the sort order flags
#define R_FLAG_SELECT ( ( unsigned int ) 1 << 5 )
#define U_FLAG_SELECT ( ( unsigned int ) 1 << 6 )

#define H_FLAG_SELECT ( ( unsigned int ) 1 << 7 )




#define MAX_NUM_CHARS 80
#define MAX_NUM_FILES 100
#define MAX_NUM_WORDS 100

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

void print_flag_select( int flag_sel ) {
   HDEBUG_PRINTF( "Inside %s(): flag_sel (in hex) is %#x\n", 
      __func__, flag_sel ); 
   int u_flag_masked_sel = flag_sel & ~(U_FLAG_SELECT); 

   HDEBUG_PRINTF( "Inside %s(): ~(U_FLAG_SELECT) (in hex) is %#x\n", 
      __func__, ~(U_FLAG_SELECT) );

   HDEBUG_PRINTF( "Inside %s(): masked_sel (in hex) is %#x\n", 
      __func__, u_flag_masked_sel ); 

   switch( u_flag_masked_sel ) {
		case C_FLAG_SELECT:
			printf( "c flag set.\n" );
			break;
		case R_FLAG_SELECT:
			printf( "r flag set.\n" );
			break;
		case N_FLAG_SELECT:
			printf( "n flag set.\n" );
			break;
		case L_FLAG_SELECT:
			printf( "l flag set.\n" );
			break;
		case S_FLAG_SELECT:
			printf( "s flag set.\n" );
			break;
		case A_FLAG_SELECT:
			printf( "a flag set.\n" );
			break;
		case H_FLAG_SELECT:
			printf( "h flag set.\n" );
			break;
      case NO_FLAG_SELECT:
			printf( "no flag set.\n" );
			break;
      default:
         fprintf( stderr, "Inside %s(): ERROR: Invalid flag select: %#x\n", 
            __func__, flag_sel  ); 
         exit(EXIT_FAILURE);
   }
   // Not mutually exclusive with the other flags
   if ( (flag_sel & U_FLAG_SELECT) == U_FLAG_SELECT ) {
		printf( "u flag set.\n" );
   }

}

int main( int argc, char** argv ) {
   // "One-hot" selecton (only one bit can be set at a time)
   // The last option passed in getopt will ultimately
   // determine the final flag_select, which is 
   // guaranteed to only have one flag set by the "one-hot" selection.
   int flag_select = NO_FLAG_SELECT;

   int c_flag_num_words = 0;
   
   int opt;
   bool do_unique = false;
   bool do_reverse = false;

   while( ( opt = getopt( argc, argv, "rnlsauhc:" ) ) != -1 ) {
      // Parse option arguments
      switch( opt ) {
         case 'r':
            // Toggle r_flag whenever it is seen
            flag_select ^= R_FLAG_SELECT; 
            HDEBUG_PRINTF( "Sort%s",
               ( ( flag_select == R_FLAG_SELECT ) ? " in reverse order\n" : "\n" )
            ); 
            do_reverse !do_reverse;
            break;
         case 'n':
            flag_select = N_FLAG_SELECT;
            HDEBUG_PRINTF( "Sort as if numbers\n" ); 
            break;
         case 'l':
            flag_select = L_FLAG_SELECT;
            HDEBUG_PRINTF( "Sort by word length\n" ); 
            break;
         case 's':
            flag_select = S_FLAG_SELECT;
            HDEBUG_PRINTF( "Sort by Scrabble scoring rules\n" ); 
            break;
         case 'a':
            flag_select = A_FLAG_SELECT;
            HDEBUG_PRINTF( "Sort lexicographically "
                  "(same as default)\n" ); 
            break;
         case 'u':
            flag_select |= U_FLAG_SELECT;
            do_unique = true;
            HDEBUG_PRINTF( "Sort with no duplicates\n" ); 
            break;
         case 'c':
            flag_select = C_FLAG_SELECT;
            // Convert optarg to number
            HDEBUG_PRINTF( "Inside %s(): optarg is %s\n", __func__, optarg ); 
            c_flag_num_words = atoi(optarg);
            HDEBUG_PRINTF( "Sort lexicographically and only show %d words\n",
               c_flag_num_words ); 
            break;
         case 'h':
            flag_select = H_FLAG_SELECT;
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
  
   

   HDEBUG_PRINTF( "Inside %s(): optind is %d\n", 
      __func__, optind ); 
   HDEBUG_PRINTF( "Inside %s(): argc is %d\n", 
      __func__, argc ); 

   int num_files = argc - optind;

   HDEBUG_PRINTF( "Inside %s(): num_files is %d\n", __func__, num_files ); 

   bool do_unique = ( (flag_select & (U_FLAG_SELECT)) == U_FLAG_SELECT );
   bool do_reverse = ( (flag_select & (R_FLAG_SELECT)) == R_FLAG_SELECT );

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
               __func__, word_num ); 
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
            new_ws_word = create_ws_word( word, word_len, ws_words_head );
            insert_ws_word_sorted( new_ws_word, ws_words_head, &num_ws_words, do_unique, comp_func );

            HDEBUG_PRINTF( "Inside %s(): After insert_ws_word_sorted for word %s num_ws_words is %d\n", 
               __func__, word, num_ws_words );

            // Get the next token from line
            token_str = strtok( NULL, " " );

         } // end of while( token_str != NULL )
      } // end of while( fgets(line, MAX_NUM_CHARS, stdin ) != NULL )

      print_flag_select( flag_select );
      HDEBUG_PRINTF( "\n" ); 

      HDEBUG_PRINTF( "Inside %s(): Print Sorted Words", __func__ ); 
      print_ws_words( num_ws_words, ws_words_head );
   }


   return EXIT_SUCCESS;
} 
