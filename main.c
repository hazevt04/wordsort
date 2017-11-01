#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "my_util.h"

#define NO_FLAG_SELECT  0

#define C_FLAG_SELECT ( ( unsigned int ) 1 << 0 )

// Sort order flags, mutually exclusive
#define R_FLAG_SELECT ( ( unsigned int ) 1 << 1 )
#define N_FLAG_SELECT ( ( unsigned int ) 1 << 2 )
#define L_FLAG_SELECT ( ( unsigned int ) 1 << 3 )
#define S_FLAG_SELECT ( ( unsigned int ) 1 << 4 )
#define A_FLAG_SELECT ( ( unsigned int ) 1 << 5 )

// Optional flag adding to the sort order flag
#define U_FLAG_SELECT ( ( unsigned int ) 1 << 6 )

#define H_FLAG_SELECT ( ( unsigned int ) 1 << 7 )



#define NUM_FLAGS 8

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

   while( ( opt = getopt( argc, argv, "rnlsauhc:" ) ) != -1 ) {
      // Parse option arguments
      switch( opt ) {
         case 'h':
            flag_select = H_FLAG_SELECT;
            usage( argv );
            break;
         case 'r':
            // Toggle r_flag whenever it is seen
            flag_select ^= R_FLAG_SELECT; 
            HDEBUG_PRINTF( "Sort%s",
               ( ( flag_select == R_FLAG_SELECT ) ? " in reverse order\n" : "\n" )
            ); 
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
   
   int file_index = 0;
   char **file_names;
   int argv_len;

   char** words;
   char* line;
   char* token_str;
   int token_len;
   int word_num = 0;

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
      
      line = ( char * )malloc( MAX_NUM_CHARS );
      token_str = ( char * )malloc( MAX_NUM_CHARS );

      words = ( char ** )malloc( MAX_NUM_WORDS * sizeof( char * ) );
      
      HDEBUG_PRINTF( "Inside %s(): Need to get input from user: ", __func__ ); 

      printf( "Enter words (100 max, each a max of 80 characters; "
            "Cntrl+D to exit): " ); 

      while( fgets(line, MAX_NUM_CHARS, stdin ) != NULL ) {
         token_str = strtok( line, " \n" );
         HDEBUG_PRINTF( "Inside %s(): token_str is '%s'\n", 
            __func__, token_str ); 

         while( token_str != NULL ) {
            token_len = strlen( token_str );
            
            HDEBUG_PRINTF( "Inside %s():\tWhile loop. Word Num is %d:\n", 
               __func__, word_num ); 
            HDEBUG_PRINTF( "Inside %s():\tWhile loop. Token len is %d:\n", 
               __func__, token_len ); 

            words[word_num] =  ( char* )malloc( token_len + 1 );
            
            strcpy( words[word_num], token_str );
            HDEBUG_PRINTF( "Inside %s():\tWhile loop. After strcpy, "
                  "words[%d] is %s.\n", 
               __func__, word_num, words[word_num] ); 

            words[word_num][token_len] = '\0';

            word_num++;

            token_str = strtok( NULL, " " );
         } // end of while( token_str != NULL )
      } // end of while( fgets(line, MAX_NUM_CHARS, stdin ) != NULL )

      HDEBUG_PRINTF( "Inside %s(): You typed ", __func__ ); 
      for( int i = 0; i < word_num; i++ ) {
         HDEBUG_PRINTF( "%s ", words[i] ); 
      } 
      HDEBUG_PRINTF( "\n" );
   }

   print_flag_select( flag_select );
   HDEBUG_PRINTF( "Inside %s(): c_flag_num_words is %d\n", 
      __func__, c_flag_num_words ); 

   return EXIT_SUCCESS;
} 
