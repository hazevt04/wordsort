#ifndef WS_WORD_H
#define WS_WORD_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

#include "my_util.h"

////////////////////////////////////////
// PROJECT 2 COMP 3220
// Wordsort (ws)
// Glenn Hazelwood
// 11/3/2017
//
// ws_word.h
//
// Header file for ws_word_t, a data type
// for storing a word, it's length, 
// its scrabble score and frequency
// count. Also provides functions for a
// linked list of ws_word_ts
////////////////////////////////////////


// "One-Hot" Select Values
// Sort order flags, mutually exclusive
#define N_ORDER_SELECT ( ( unsigned int ) 1 << 0 )
#define L_ORDER_SELECT ( ( unsigned int ) 1 << 1 )
#define S_ORDER_SELECT ( ( unsigned int ) 1 << 2 )
#define A_ORDER_SELECT ( ( unsigned int ) 1 << 3 )

// Forward declaration to allow for next
// pointer to be declared within the struct
typedef struct ws_word_t ws_word_t;


// Data type for word sort, ws_word_t
//
// This data type is intended to be a
// node in a linked list
//
// Stores:
// Total Scrabble Score for the Word
// Frequency Count of the Word
// Length of the Word
// The Word
// Pointer to Next ws_word_t in Linked List
struct ws_word_t {
   int scrabble_score;
   int count;
   int word_len;
   char *word;
   struct ws_word_t *next;
};


// Does a lookup on each character in the word
// and returns the Scrabble score for that word
int calc_scrabble_score( char *word, int word_len );


// Checks to see if the linked list for ws_words is empty
bool is_ws_words_empty( const ws_word_t *ws_words );


// Returns a pointer to the ws_word_t that matches key_ws_word
ws_word_t *find_ws_word( const ws_word_t *key_ws_word, 
      ws_word_t *ws_words );


void destroy_ws_words( ws_word_t *ws_words );

// This does not yet modify ws_words, it only creates a single ws_word_t node!
// Still need to insert the node separately
ws_word_t *create_ws_word( char *word, int word_len );


void print_ws_words( int num_words_to_print, ws_word_t *ws_words, bool do_unique, bool show_counts );

// Comparison Functions
int word_cmp( const ws_word_t *left, const ws_word_t *right );
int word_cmp_rev( const ws_word_t *left, const ws_word_t *right );
int word_len_cmp( const ws_word_t *left, const ws_word_t *right );
int word_len_cmp_rev( const ws_word_t *left, const ws_word_t *right );
int word_as_num_cmp( const ws_word_t *left, const ws_word_t *right );
int word_as_num_cmp_rev( const ws_word_t *left, const ws_word_t *right );
int scrabble_score_cmp( const ws_word_t *left, const ws_word_t *right );
int scrabble_score_cmp_rev( const ws_word_t *left, const ws_word_t *right );

// Selector for the comparison
int ( *select_cmp_func( int select, bool do_reverse ) ) ( const ws_word_t *left, const ws_word_t *right );

// Insert in sorted order.
// If unique flag is set, don't insert it again
// Last arg is a function pointer to a comparison function which is used
// to determine where the new ws_word_t is inserted into the linked list
ws_word_t *insert_ws_word_sorted( ws_word_t *new_ws_word, 
      ws_word_t *ws_words, int *num_ws_words, bool do_unique,
      int ( * )( const ws_word_t *, const ws_word_t * ) );



#endif /* end of include guard: WS_WORD_H */
