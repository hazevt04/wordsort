#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "my_util.h"
 
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

int main( int argc, char** argv ) {

   int c_flag = 0;
   int r_flag = 0;
   int n_flag = 0;
   int l_flag = 0;
   int s_flag = 0;
   int a_flag = 0;
   int u_flag = 0;
   int h_flag = 0;

   int c_flag_num_words = 0;
   
   int opt;

   while( ( opt = getopt( argc, argv, "rnlsauhc:" ) ) != -1 ) {
      // Parse option arguments
      switch( opt ) {
         case 'h':
            usage( argv );
            break;
         case 'r':
            // Toggle r_flag whenever it is seen
            r_flag ^= 1;
            HDEBUG_PRINTF( "Sort%s",
               ( ( r_flag ) ? " in reverse order\n" : "\n" )
            ); 
            break;
         case 'n':
            n_flag = 1;
            HDEBUG_PRINTF( "Sort as if numbers\n" ); 
            break;
         case 'l':
            l_flag = 1;
            HDEBUG_PRINTF( "Sort by word length\n" ); 
            break;
         case 's':
            s_flag = 1;
            HDEBUG_PRINTF( "Sort by Scrabble scoring rules\n" ); 
            break;
         case 'a':
            a_flag = 1;
            HDEBUG_PRINTF( "Sort lexicographically (same as default)\n" ); 
            break;
         case 'u':
            u_flag = 1;
            HDEBUG_PRINTF( "Sort with no duplicates\n" ); 
            break;
         case 'c':
            c_flag = 1;
            // Convert optarg to number
            HDEBUG_PRINTF( "Inside %s(): optarg is %s\n", __func__, optarg ); 
            c_flag_num_words = atoi(optarg);
            HDEBUG_PRINTF( "Sort lexicographically and only show %d words\n",
               c_flag_num_words ); 
            break;
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
   
   HDEBUG_PRINTF( "Inside %s(): optind is %d\n", __func__, optind ); 
   HDEBUG_PRINTF( "Inside %s(): argc is %d\n", __func__, argc ); 
   // opt_ind is first non-option argument
   for ( int i = optind; i < argc; i++ ) {
      // Parse non-option arguments (filenames)
      HDEBUG_PRINTF( "Inside %s(): Beyond the options: argv[%d] is %s\n", 
         __func__, i, argv[i] ); 
   } // end of for ( int i = opt_ind; i < argc; i++ )
    
   return EXIT_SUCCESS;
} 
