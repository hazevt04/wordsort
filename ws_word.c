#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
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
      int scrabble_index = tolower( word[i] ) - 'a';

      HDEBUG_PRINTF( "Inside %s(): word[%d] (lowercase) is %c\n", 
         __func__, i, tolower( word[i] ) ); 
      HDEBUG_PRINTF( "Inside %s(): 'a' as an int is %d\n", __func__, ( ( int )'a' ) ); 
      HDEBUG_PRINTF( "Inside %s(): %d: scrabble index is %d, '%c'\n", 
         __func__, i, scrabble_index, scrabble_letter_scores[scrabble_index][0]  ); 

      HDEBUG_PRINTF( "Inside %s(): scrabble score for '%c' is %d\n\n", __func__,
        scrabble_letter_scores[scrabble_index][0], scrabble_letter_scores[scrabble_index][1]  ); 

      // Accumulate the scrabble score via lookup
      scrabble_score += scrabble_letter_scores[scrabble_index][1];
   }

   HDEBUG_PRINTF( "Inside %s(): Scrabble Score for %s is %d\n\n", 
      __func__, word, scrabble_score ); 
   return scrabble_score; 
} // end of calc_scrabble_score( ...

void initialize_ws_words( ws_word_t *ws_words ) {
   ws_words = NULL;
}

void destroy_ws_words( ws_word_t *ws_words ) {
   assert( ws_words != NULL );

   ws_word_t *curr_ws_word = ws_words;

   // Free the char arrays in each ws_word_t node
   while( curr_ws_word != NULL ) {
      FREE_AND_NULL_PTR( curr_ws_word->word );
      curr_ws_word = curr_ws_word->next;
   } 
   // curr_ws_word will be NULL here
   FREE_AND_NULL_PTR( ws_words );
}

// Checks whether the linked list of ws_word_t nodes
// is empty
bool is_ws_words_empty( const ws_word_t *ws_words ) {
   return ( ws_words == NULL );
}

ws_word_t *find_ws_word( const ws_word_t *key_ws_word, ws_word_t *ws_words ) {
   assert( key_ws_word != NULL );
   assert( ws_words != NULL );

   ws_word_t *curr_ws_word = ws_words;
   while( curr_ws_word != NULL ) {
      if ( strcmp( key_ws_word->word, curr_ws_word->word ) == 0 ) {
         return curr_ws_word;
      }
      curr_ws_word = curr_ws_word->next;
   } 
   return curr_ws_word;
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


void print_ws_words( int num_words_to_print, ws_word_t *ws_words, bool do_unique ) {
   if ( ws_words == NULL ) {
      printf( "EMPTY.\n" ); 
      return;
   }

   ws_word_t *curr_ws_word = ws_words;
   // Line 1
   printf( "%80s   %5s   %14s\n",
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
      curr_ws_word = curr_ws_word->next;
   } // end of while 
   printf( "\n" ); 
}

// Normal insert. No consideration of order
void insert_ws_word( ws_word_t *new_ws_word, ws_word_t *ws_words ) {
   assert( new_ws_word != NULL );
   assert( ws_words != NULL );
    
   ws_word_t *old_next = ws_words->next;
   ws_words->next = new_ws_word;
   new_ws_word->count++;
   new_ws_word->next = old_next;

}

///////////////////////////////////////////////
// LEXICOGRAPHICAL WORD COMPARISON FUNCTIONS
///////////////////////////////////////////////
int word_cmp( const ws_word_t *left, const ws_word_t *right ) {
   assert( left != NULL );
   assert( right != NULL );
	assert( left->word != NULL );
	assert( right->word != NULL );

   return strcmp( left->word, right->word );
}


int word_cmp_rev( const ws_word_t *left, const ws_word_t *right ) {
   assert( left != NULL );
   assert( right != NULL );
	assert( left->word != NULL );
	assert( right->word != NULL );

   return strcmp( right->word, left->word );
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
int word_as_num_cmp( const ws_word_t *left, const ws_word_t *right ) {
   assert( left != NULL );
   assert( right != NULL );
	assert( left->word != NULL );
	assert( right->word != NULL );
   
   return ( atoi( left->word ) - atoi( right->word ) );
}

int word_as_num_cmp_rev( const ws_word_t *left, const ws_word_t *right ) {
   assert( left != NULL );
   assert( right != NULL );
	assert( left->word != NULL );
	assert( right->word != NULL );
   
   return ( atoi( right->word ) - atoi( left->word ) );
}


/////////////////////////////////////////
// SCRABBLE SCORE COMPARISON FUNCTIONS
/////////////////////////////////////////
int scrabble_score_cmp( const ws_word_t *left, const ws_word_t *right ) {
   assert( left != NULL );
   assert( right != NULL );
  
   return ( left->scrabble_score - right->scrabble_score ); 
}


int scrabble_score_cmp_rev( const ws_word_t *left, const ws_word_t *right ) {
   assert( left != NULL );
   assert( right != NULL );
  
   return ( right->scrabble_score - left->scrabble_score ); 
}


// Comparison Function Selector Function
int ( *select_cmp_func( int select, bool do_reverse ) ) ( const ws_word_t *left, const ws_word_t *right ) {
   int ( *func_ptr ) ( const ws_word_t *left, const ws_word_t *right );

   HDEBUG_PRINTF( "Inside %s(): select (in hex) is %#x\n", 
      __func__, select );

   switch( select ) {
		case N_ORDER_SELECT:
			HDEBUG_PRINTF( "(-n) order as if numbers selected.\n" );
         func_ptr = do_reverse ? word_as_num_cmp_rev : word_as_num_cmp;
			break;
		case L_ORDER_SELECT:
			HDEBUG_PRINTF( "(-l) order by word len selected.\n" );
         func_ptr = do_reverse ? word_len_cmp_rev : word_len_cmp;
			break;
		case S_ORDER_SELECT:
			HDEBUG_PRINTF( "(-s) order by scrabble score selected.\n" );
         func_ptr = do_reverse ? scrabble_score_cmp_rev : scrabble_score_cmp;
			break;
		case A_ORDER_SELECT:
			HDEBUG_PRINTF( "(-a) order lexicographically selected.\n" );
         func_ptr = do_reverse ? word_cmp_rev : word_cmp;
			break;
      default:
         fprintf( stderr, "Inside %s(): ERROR: Invalid order select: %#x\n", 
            __func__, select  ); 
         exit(EXIT_FAILURE);
   }
   return func_ptr;
   
}


// Insert in sorted order.
// Last arg is a function pointer to the comparison function 
// to determine where the new ws_word_t is inserted into the linked list
void insert_ws_word_sorted( ws_word_t *new_ws_word, ws_word_t *ws_words,
      int *num_ws_words, bool do_unique, int ( *comp )( const ws_word_t *, const ws_word_t * ) ) {

   assert( new_ws_word != NULL );
   assert( comp != NULL );
   
   ws_word_t *insertion_ptr = ws_words;
   ws_word_t *search_ptr = ws_words;

   ws_word_t *found_ptr;
   ws_word_t *result_ptr;

   int found_index = 0;

   if ( do_unique && ( ws_words != NULL ) ) {
      
      HDEBUG_PRINTF( "Inside %s(): do_unique specified\n", 
         __func__ ); 
      
      found_ptr = find_ws_word( new_ws_word, search_ptr );
      if ( found_ptr != NULL ) {
         HDEBUG_PRINTF( "Inside %s(): found_ptr->count is %d\n",
            __func__, found_ptr->count ); 
         HDEBUG_PRINTF( "Inside %s(): do_unique specified and Word '%s' already in ws_words.\n",
             __func__, new_ws_word->word ); 
         found_ptr->count++;

         HDEBUG_PRINTF( "Inside %s(): found_ptr->count incremented to %d\n",
            __func__, found_ptr->count ); 
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

   *num_ws_words++;
   
   HDEBUG_PRINTF( "Inside %s(): Num words incremented to %d\n",
      __func__, *num_ws_words ); 

}


