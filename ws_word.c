#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>

#include "my_util.h"

#include "ws_word.h"

////////////////////////////////////////
// PROJECT 2 COMP 3220
// Wordsort (ws)
// Glenn Hazelwood
// 11/3/2017
//
// ws_word.c
//
// Implementation file for ws_word_t, a data type
// for storing a word, it's length, 
// its scrabble score and frequency
// count. Also provides functions for a
// linked list of ws_word_ts
////////////////////////////////////////

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


// Free all the space allocated for ws_words
// linked list and the char arrays in each node
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
   HDEBUG_PRINTF( "Inside %s(): SANITY CHECK: NULL is %p\n", 
      __func__, (void *)NULL ); 

   HDEBUG_PRINTF( "Inside %s(): SANITY CHECK: ws_words is %p\n",
       __func__, ws_words ); 
   HDEBUG_PRINTF( "Inside %s(): ws_words is %s\n", 
      __func__,
      ( ws_words == NULL ) ? "NULL" : "not NULL?"  
   ); 
   return ( ws_words == NULL );
}

// Returns pointer to ws_word_t with the same word as key_ws_word.
// Reeturns NULL if no ws_word_t has matching word
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
   MALLOC_AND_CHECK_ERROR( new_ws_word->word, char, MAX_NUM_CHARS );

   // Set the word
   strcpy( new_ws_word->word, word );

   HDEBUG_PRINTF( "Inside %s(): After strcpy of word to new_ws_word->word. "
      "new_ws_word->word is %s\n", 
      __func__, new_ws_word->word ); 

   // Set word_len
   new_ws_word->word_len = word_len;
   HDEBUG_PRINTF( "Inside %s(): new_ws_word->word_len is %d\n", 
      __func__, new_ws_word->word_len ); 

   HDEBUG_PRINTF( "Inside %s(): After strcpy of word to new_ws_word->word. "
         "new_ws_word->word_len is %d\n",
          __func__, new_ws_word->word_len ); 

   // Set the scrabble score
   new_ws_word->scrabble_score = calc_scrabble_score( word, word_len );
   
   HDEBUG_PRINTF( "Inside %s(): new_ws_word->word is %s\n", __func__, new_ws_word->word ); 
   HDEBUG_PRINTF( "Inside %s(): Scrabble score for new_ws_word '%s' is %d\n\n", 
      __func__, new_ws_word->word, new_ws_word->scrabble_score ); 

   // Set the pointer to the next node
   new_ws_word->next = NULL;
 
   HDEBUG_PRINTF( "Inside %s():\t\tEND OF create_ws_word\n", __func__ );  
   HDEBUG_PRINTF( "Inside %s(): new_ws_word is %s\n", 
      __func__, 
      ( ( new_ws_word == NULL ) ? "Ugh-oh" : "ok" )
   );

   // Set count
   new_ws_word->count = 0;

   return new_ws_word;

} // end of ws_word_t *create_ws_word( ...


// If -u skips over words with counts equal to 1
// Otherwise prints all of the words
void print_ws_words( int num_words_to_print, ws_word_t *ws_words, bool do_unique, bool show_counts ) {
   if ( ws_words == NULL ) {
      printf( "EMPTY.\n" ); 
      return;
   }

   int print_count = 0; 

   ws_word_t *curr_ws_word = ws_words;
   if ( show_counts ) {
      // Line 1
      printf( "%50s   %5s   %15s   %6s\n",
         "Word","Length","Scrabble Score", "Counts" );
      
      // Line 2
      for( int i = 0; i < 86; i++ ) {
         printf( "=" );
      } 

   } else {
      printf( "%50s   %5s   %15s\n",
         "Word","Length","Scrabble Score" );
   

      for( int i = 0; i < 80; i++ ) {
         printf( "=" );
      }
   } // end of else 
   printf( "\n" ); 

   // Print the rest of the lines
   while( ( print_count < num_words_to_print ) && ( curr_ws_word != NULL ) ) {
      // If do_unique skip over words with count > 1
      if ( ( !do_unique ) || ( do_unique && ( curr_ws_word->count == 1 ) ) ) {
         if ( show_counts ) {
            printf( "%50s   %5d   %15d   %6d\n",
               curr_ws_word->word, curr_ws_word->word_len, curr_ws_word->scrabble_score,
               curr_ws_word->count
            );
         } else {
            printf( "%50s   %5d   %15d\n",
               curr_ws_word->word, curr_ws_word->word_len, curr_ws_word->scrabble_score 
            );
         }
         print_count++;
      }
      curr_ws_word = curr_ws_word->next;
   } // end of while 
   printf( "\n" ); 

} // end of print_ws_words( ...


///////////////////////////////////////////////
// LEXICOGRAPHICAL WORD COMPARISON FUNCTIONS
///////////////////////////////////////////////
int word_cmp( const ws_word_t *left, const ws_word_t *right ) {
   assert( left != NULL );
   assert( right != NULL );
	assert( left->word != NULL );
	assert( right->word != NULL );

   if ( strcmp( left->word, right->word ) == 0 ) {
      HDEBUG_PRINTF( "Inside %s(): left ('%s') == right('%s')\n",
       __func__, left->word, right->word ); 
   } else if ( strcmp( left->word, right->word ) > 0 ) {
      HDEBUG_PRINTF( "Inside %s(): left ('%s') > right('%s')\n",
       __func__, left->word, right->word ); 
   } else {
      HDEBUG_PRINTF( "Inside %s(): left ('%s') < right('%s')\n",
       __func__, left->word, right->word ); 
   }

   return strcmp( left->word, right->word );
}

// Reverse version
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


// Reverse version
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

// Reverse version
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


// Reverse version
int scrabble_score_cmp_rev( const ws_word_t *left, const ws_word_t *right ) {
   assert( left != NULL );
   assert( right != NULL );
  
   return ( right->scrabble_score - left->scrabble_score ); 
}


// Comparison Function Selector Function
// Highlight of this class-- FUNCTION POINTERS!!
// Returns function pointer to a function that returns an int and takes in a constant ws_word_t pointer
// for the left side of the comparison and another constant ws_word_t pointer for the right side of the
// comparison
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
   
} // end of select_cmp_func( ...


// Insert in sorted order.
// Last arg is a function pointer to the comparison function 
// to determine where the new ws_word_t is inserted into the linked list
ws_word_t *insert_ws_word_sorted( ws_word_t *new_ws_word, ws_word_t *ws_words,
      int *num_ws_words, bool do_unique, int ( *comp )( const ws_word_t *, const ws_word_t * ) ) {

   assert( new_ws_word != NULL );
   assert( comp != NULL );
   
   ws_word_t *insertion_ptr = ws_words;
   ws_word_t *prev_ptr = ws_words;

   ws_word_t *found_ptr;

   int found_index = 0;
   int l_num_ws_words = *num_ws_words;
   HDEBUG_PRINTF( "Inside %s(): At start l_num_ws_words is %d\n", __func__, l_num_ws_words ); 
   if ( do_unique && ( ws_words != NULL ) ) {
      
      HDEBUG_PRINTF( "Inside %s(): do_unique specified\n", 
         __func__ ); 
      
      found_ptr = find_ws_word( new_ws_word, ws_words );
      if ( found_ptr != NULL ) {
         HDEBUG_PRINTF( "Inside %s(): found_ptr->count is %d\n",
            __func__, found_ptr->count ); 
         HDEBUG_PRINTF( "Inside %s(): do_unique specified and Word '%s' already in ws_words.\n",
             __func__, new_ws_word->word ); 
         found_ptr->count++;

         HDEBUG_PRINTF( "Inside %s(): found_ptr->count incremented to %d\n",
            __func__, found_ptr->count ); 
         return ws_words;
      }      
   }

   HDEBUG_PRINTF( "Inside %s(): Insertion_ptr is %p\n", __func__, insertion_ptr ); 
   HDEBUG_PRINTF( "Inside %s(): Num words before insertion is %d\n", __func__, l_num_ws_words ); 

   int insertion_index = 0;
   // Move insertion_ptr to the position the new ws_word should be inserted to
   if ( !is_ws_words_empty( ws_words ) ) {

      HDEBUG_PRINTF( "Inside %s(): ws_words is not empty.\n", __func__ ); 
      while( insertion_ptr != NULL ) {
         HDEBUG_PRINTF( "Inside %s(): Inside While loop: Looking at word %d: '%s'\n", 
            __func__, insertion_index, insertion_ptr->word );

         if ( comp( new_ws_word, insertion_ptr ) < 0 ) {
            HDEBUG_PRINTF( "Inside %s(): Found location to insert new ws_word '%s'- %d\n", 
               __func__, new_ws_word->word, insertion_index ); 

            break;
         } else {
            HDEBUG_PRINTF( "Inside %s(): Skipping to next location to try to insert new ws_word '%s'\n", 
               __func__, new_ws_word->word ); 
            prev_ptr = insertion_ptr;
            insertion_ptr = insertion_ptr->next;
            HDEBUG_PRINTF( "Inside %s(): After skipping, ws_words is %p\n", __func__, ws_words ); 
            HDEBUG_PRINTF( "Inside %s(): After skipping, prev_ptr is %p\n", __func__, prev_ptr ); 
            HDEBUG_PRINTF( "Inside %s(): After skipping, insertion_ptr is %p\n", __func__, insertion_ptr ); 
            insertion_index++;
         }
      } // end of while loop

      HDEBUG_PRINTF( "Inside %s(): ws_words not empty. Inserting '%s' at index %d\n",
         __func__, new_ws_word->word, insertion_index ); 
      HDEBUG_PRINTF( "Inside %s(): l_num_ws_words is %d\n", __func__, l_num_ws_words ); 

      if ( insertion_index == ( l_num_ws_words ) ) {
         HDEBUG_PRINTF( "Inside %s(): insertion_index (%d) == ( l_num_ws_words (%d) ) (%d)\n", 
            __func__, insertion_index, l_num_ws_words, ( l_num_ws_words ) ); 

         HDEBUG_PRINTF( "Inside %s(): APPENDING '%s' (pointed to by %p)\n",
            __func__, new_ws_word->word, new_ws_word ); 

         prev_ptr->next = new_ws_word;
         new_ws_word->next = NULL;
      } else if ( insertion_index == 0 ) {

         HDEBUG_PRINTF( "Inside %s(): insertion_index (%d) == 0\n", 
            __func__, insertion_index ); 

         HDEBUG_PRINTF( "Inside %s(): PREPENDING '%s' (pointed to by %p)\n",
            __func__, new_ws_word->word, new_ws_word ); 
         
         new_ws_word->next = ws_words;
         ws_words = new_ws_word;

      } else {
         HDEBUG_PRINTF( "Inside %s(): insertion_index (%d) != 0 and != ( l_num_ws_words (%d) )\n", 
            __func__, insertion_index, l_num_ws_words ); 

         HDEBUG_PRINTF( "Inside %s(): PREPENDING '%s' (pointed to by %p) to insertion point\n",
            __func__, new_ws_word->word, new_ws_word ); 

         new_ws_word->next = insertion_ptr;
         prev_ptr->next = new_ws_word;

      }

      HDEBUG_PRINTF( "Inside %s(): After insertion to nonempty, prev_ptr is %p\n", __func__, prev_ptr ); 
      HDEBUG_PRINTF( "Inside %s(): After insertion to nonempty, new_ws_word for %s ptr is %p\n\n", 
         __func__, new_ws_word->word, new_ws_word ); 
      HDEBUG_PRINTF( "Inside %s(): After insertion to nonempty, ws_words ptr is %p\n", __func__, ws_words ); 

   } else {
      HDEBUG_PRINTF( "Inside %s(): ws_words is empty. Just insert the new ws_word at %d.\n", 
         __func__, insertion_index ); 

      ws_words = new_ws_word;
      ws_words->next = NULL;
      
      HDEBUG_PRINTF( "Inside %s(): After insertion to empty, ws_words ptr is %p\n", __func__, ws_words ); 
      HDEBUG_PRINTF( "Inside %s(): After insertion to empty, new_ws_word ptr is %p\n\n", __func__, new_ws_word ); 

   } // end of else ( if is_ws_empty( ... 

   // Increment frequency count for new word
   new_ws_word->count++;
   l_num_ws_words++;

   HDEBUG_PRINTF( "Inside %s(): After insertion l num words is %d\n",
      __func__, l_num_ws_words ); 

   // Extra check. Print after insertion!
   HDEBUG_PRINTF( "Inside %s(): PRINTING AFTER INSERTION of '%s'\n", 
      __func__, new_ws_word->word ); 
   //print_ws_words( l_num_ws_words, ws_words, false, true );

   *num_ws_words = l_num_ws_words;
   HDEBUG_PRINTF( "Inside %s(): After insertion num words is %d\n",
      __func__, *num_ws_words ); 
   return ws_words;
} // end of insert_ws_word_sorted


