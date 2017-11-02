#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>


#include "my_util.h"
 

#include "ws_word.h"

// Get scrabble score for a letter by lookup
static int scrabble_letter_scores[26][2] = {
   { 'a',  1 }, { 'b',  3 }, { 'c',  3 }, { 'd',  2 }, { 'e', 1  }, { 'f',  4 }, 
   { 'g',  2 }, { 'h',  4 }, { 'i',  1 }, { 'j',  8 }, { 'k', 5  }, { 'l',  1 },
   { 'm',  3 }, { 'n',  1 }, { 'o',  1 }, { 'p',  3 }, { 'q', 10 }, { 'r',  1 },
   { 's',  1 }, { 't',  1 }, { 'u',  1 }, { 'v',  4 }, { 'w', 4  }, { 'x',  8 },
   { 'y',  4 }, { 'z', 10 }
};

// word has been allocated already
int calc_scrabble_score( char *word, int word_len ) {
   assert( word != NULL );
   int scrabble_score = 0;

   for( int i = 0; i < word_len; i++ ) {
      // Get the scrabbble score for the character at index i in the word
      int scrabble_index = word[i] - 'a';

      HDEBUG_PRINTF( "Inside %s(): word[%d] is %c\n", 
         __func__, i, word[i] ); 
      HDEBUG_PRINTF( "Inside %s(): 'a' as an int is %d\n", __func__, ( ( int )'a' ) ); 
      HDEBUG_PRINTF( "Inside %s(): %d: scrabble index is %d\n", 
         __func__, scrabble_index ); 

      // Accumulate the scrabble score via lookup
      scrabble_score += scrabble_letter_scores[scrabble_index][1];
   } 
} // end of calc_scrabble_score( ...

void initialize_ws_words( ws_word_t *ws_words ) {
   *ws_words = NULL;
}

void destroy_ws_words( ws_word_t *ws_words ) {
   assert( ws_words != NULL );

   ws_word_t *curr_ptr = ws_words;

   // Free the char arrays in each ws_word_t node
   while( curr_ptr != NULL ) {
      FREE_AND_NULL_PTR( curr_ptr->words );
      curr_ptr = curr_ptr->next;
   } 
   // curr_ptr will be NULL here
   FREE_AND_NULL_PTR( ws_words );
}

// Checks whether the linked list of ws_word_t nodes
// is empty
bool is_ws_words_empty( const ws_word_t *ws_words ) {
   return ( ws_words == NULL );
}

ws_word_t *find_ws_word( const ws_word_t *key_ws_word, const ws_word_t *ws_words ) {
   assert( word != NULL );
   assert( ws_words != NULL );

   ws_word_t curr_ptr = ws_words;
   while( curr_ptr != NULL ) {
      if ( strcmp( key_ws_word->word, curr_ptr->word ) == 0 ) {
         return curr_ptr;
      }
      curr_ptr = curr_ptr->next;
   } 
   return curr_ptr;
}

// Creates a new ws_word_t node and returns a pointer 
// to it. Called is responsible for free()-ing
ws_word_t *create_ws_word( char *word, int word_len ) {
   
   assert( word != NULL );

   ws_word_t *new_ws_word;
   int num_new_ws_bytes = sizeof( ws_word_t );

   // Allocate the new ws_word_t node
   MALLOC_AND_CHECK_ERROR( new_ws_word, ws_word_t, num_new_ws_bytes );

   HDEBUG_PRINTF( "Inside %s(): Word being created for new_ws_word is '%s'\n", __func__, word ); 
   HDEBUG_PRINTF( "Inside %s(): Word Length for new_ws_word is %d\n", 
      __func__, word_len ); 

   // Allocate the word array in the new ws_word_t node
   MALLOC_AND_CHECK_ERROR( new_ws_word->word, char, ( word_len + 1 ) );

   // Set the word
   strcpy( new_ws_word->word, word );

   HDEBUG_PRINTF( "Inside %s(): After strcpy of word to new_ws_word->word. "
      "new_ws_word->word is %s\n", 
      __func__, new_ws_word->word ); 

   // Set word_len
   new_ws_word->word_len = word_len;

   HDEBUG_PRINTF( "Inside %s(): After strcpy of word to new_ws_word->word. "
         "new_ws_word->word_len is %d\n",
          __func__, new_ws_word->word_len ); 

   // Set the scrabble score
   new_ws_word->scrabble_score = calc_scrabble_score( word, word_len );

   HDEBUG_PRINTF( "Inside %s(): Scrabble score for new_ws_word %s is %d\n", 
      __func__, new_ws_word->word, new_ws_word->scrabble_score ); 

   // Set the pointer to the next node
   new_ws_word->next = NULL;
  
   return new_ws_word;

} // ws_word_t *create_ws_word(  


void print_ws_words( int num_words_to_print, const ws_word_t *ws_words, bool do_unique ) {
   if ( ws_words == NULL ) {
      printf( "EMPTY.\n" ); 
      return;
   }

   ws_word_t *curr_ws_word = ws_words;
   // Line 1
   printf( "%80s   %5d   %14s\n",
      "Word","Length","Scrabble Score" );

   // Line 2
   for( int i = 0; i < 100; i++ ) {
      printf( "=" );
   } 
   printf( "\n" ); 

   // Rest of the lines
   while( curr_ws_word != NULL ) {
      // If do_unique skip over words with count > 1
      if ( ( !do_unique ) || ( do_unique && ( curr_ws_word->count == 1 ) ) ) {
         printf( "%80s   %5d   %14d\n",
            curr_ws_word->word, curr_ws_word->word_len, curr_ws_word->scrabble_score 
         );
      }
      cur_ws_word = ws_words->next;
   } // end of while 
   printf( "\n" ); 
}

// Normal insert. No consideration of order
void insert_ws_word( ws_word_t *new_ws_word, ws_word_t *ws_words ) {
   assert( new_ws_word != NULL );
   assert( ws_words != NULL );
    
   ws_word_t *old_next = ws_words->next;
   ws_words->next = new_ws_word;
   new_ws_word->next = old_next;
}

///////////////////////////////////////////////
// LEXICOGRAPHICAL WORD COMPARISON FUNCTIONS
///////////////////////////////////////////////
int word_cmp( const ws_word_t *left, const ws_word_t *right ) {
   assert( left != NULL );
   assert( right != NULL );

   return strcmp( left, right );
}


int word_cmp_rev( const ws_word_t *left, const ws_word_t *right ) {
   assert( left != NULL );
   assert( right != NULL );

   return strcmp( right, left );
}

/////////////////////////////////////////
// WORD LENGTH COMPARISON FUNCTIONS
/////////////////////////////////////////
int word_len_cmp( const ws_word_t *left, const ws_word_t *right ) {
   assert( left != NULL );
   assert( right != NULL );

   return left->word_len - right->word_len;
}


int word_len_cmp_rev( const ws_word_t *left, const ws_word_t *right ) {
   assert( left != NULL );
   assert( right != NULL );

   return right->word_len - left->word_len;
}

/////////////////////////////////////////
// WORDS AS NUM COMPARISON FUNCTIONS
/////////////////////////////////////////
int words_as_num_cmp( const ws_word_t *left, const ws_word_t *right ) {
   assert( left != NULL );
   assert( right != NULL );
   
   return ( atoi( left->word ) - atoi( right-word ) );
}

int words_as_num_cmp_rev( const ws_word_t *left, const ws_word_t *right ) {
   assert( left != NULL );
   assert( right != NULL );
   
   return ( atoi( right->word ) - atoi( left-word ) );
}

int ( *select_cmp_func( int select ) ) ( const ws_word_t *left, const ws_word_t *right ) {
   int ( *func_ptr ) ( const ws_word_t *left, const ws_word_t *right );

   HDEBUG_PRINTF( "Inside %s(): select (in hex) is %#x\n", 
      __func__, select ); 
   int u_flag_masked_sel = select & ~(U_FLAG_SELECT); 

   HDEBUG_PRINTF( "Inside %s(): ~(U_FLAG_SELECT) (in hex) is %#x\n", 
      __func__, ~(U_FLAG_SELECT) );

   HDEBUG_PRINTF( "Inside %s(): masked_sel (in hex) is %#x\n", 
      __func__, u_flag_masked_sel ); 

   switch( u_flag_masked_sel ) {
		case C_FLAG_SELECT:
			HDEBUG_PRINTF( "c flag set.\n" );
         func_ptr = word_cmp;
			break;
		case R_FLAG_SELECT:
			HDEBUG_PRINTF( "r flag set.\n" );
         func_ptr = 
			break;
		case N_FLAG_SELECT:
			HDEBUG_PRINTF( "n flag set.\n" );
			break;
		case L_FLAG_SELECT:
			HDEBUG_PRINTF( "l flag set.\n" );
			break;
		case S_FLAG_SELECT:
			HDEBUG_PRINTF( "s flag set.\n" );
			break;
		case A_FLAG_SELECT:
			HDEBUG_PRINTF( "a flag set.\n" );
			break;
		case H_FLAG_SELECT:
			HDEBUG_PRINTF( "h flag set.\n" );
			break;
      case NO_FLAG_SELECT:
			HDEBUG_PRINTF( "no flag set.\n" );
			break;
      default:
         fprintf( stderr, "Inside %s(): ERROR: Invalid flag select: %#x\n", 
            __func__, select  ); 
         exit(EXIT_FAILURE);
   }
   
}


// Insert in sorted order.
// Last arg is a function pointer to the comparison function 
// to determine where the new ws_word_t is inserted into the linked list
void insert_ws_word_sorted( ws_word_t *new_ws_word, ws_word_t *ws_words,
      int *num_ws_words, bool do_unique, int ( *comp )( ws_word_t *, ws_word_t * ) ) {

   assert( new_ws_word != NULL );
   assert( comp != NULL );
   
   ws_word_t *insertion_ptr = ws_words;
   ws_word_t *found_ptr = ws_words;
   ws_word_t *result_ptr;

   int found_index = 0;

   if ( do_unique && ( ws_words != NULL ) ) {
      bool word_found = false;
      
      HDEBUG_PRINTF( "Inside %s(): do_unique specified\n", 
         __func__ ); 
      
      word_found = find_word( new_ws_word, ws_words );
      if ( word_found != NULL ) {
         HDEBUG_PRINTF( "Inside %s(): do_unique specified and Word '%s' already in ws_words.\n",
             __func__, word ); 
         word_found->count++;
         return;
      }      
   }

   // Move insertion_ptr to the position the new ws_word should be inserted to
   while( insertion_ptr != NULL ) {
      if ( comp( new_ws_word, insertion_ptr ) > 0 ) {
         break;
      } else {
         insertion_ptr = insertion_ptr->next;
      }
   } 

   insert_ws_word( new_ws_word, insertion_ptr );
   *num_words++;

}


