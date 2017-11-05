#ifndef MY_UTIL_H
#define MY_UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

///////////////////////////////
// PROJECT 2 COMP 3220
// Wordsort (ws)
// Glenn Hazelwood
// 11/3/2017
//
// my_util.h
//
// Header file with my favorite
// macro functions and reusable
// functions used in Project 2's
// Wordsort
//
// Also includes a functions
// for:
// left circular shift (lcshift()),
// right circular shift (lcshift()),
// anf 
// getting words from a file
//////////////////////////////

#define MAX_NUM_FILES 100
#define MAX_NUM_CHARS 1000

//#define MAX_NUM_WORDS 1000
#define MAX_NUM_WORDS 5

#ifndef MALLOC_AND_CHECK_ERROR 
#   define MALLOC_AND_CHECK_ERROR(ptr, type, nbytes) \
      ptr = (type *)malloc(nbytes); \
      if (!ptr) { \
         fprintf(stderr, "In %s on line %d, function %s: " \
            "ERROR malloc of " #ptr \
            " of type " #type \
            " for %d bytes failed.\n", \
             __FILE__, __LINE__, __func__, nbytes ); \
         exit(EXIT_FAILURE); \
      }
#endif

#ifndef FREE_AND_NULL_PTR
#   define FREE_AND_NULL_PTR(ptr) \
      free(ptr); \
      ptr = NULL; 
#endif

#ifndef HDEBUG_PRINTF
#  ifdef HDEBUG
#     define HDEBUG_PRINTF(fmt, ...) {\
         printf(fmt, ##__VA_ARGS__); \
      }
#  else
#     define HDEBUG_PRINTF(fmt, ...) {\
         ; \
      }
#  endif
#endif


#ifndef FUNC_AND_CHECK_ERROR
#   define FUNC_AND_CHECK_ERROR(fun) {\
      if ((fun) != EXIT_SUCCESS) { \
         fprintf(stderr, "In %s on line %d, function %s: ERROR " #fun \
              " failed.\n", \
             __FILE__, __LINE__, __func__ ); \
         exit (EXIT_FAILURE); \
      } \
}
#endif

#ifndef CHECK_PTR
#  define CHECK_PTR(ptr) {\
      if (!ptr) { \
         fprintf(stderr, "In %s on line %d, function %s: ERROR " #ptr \
              " is NULL. Malloc failed?\n", \
             __FILE__, __LINE__, __func__ ); \
         exit (EXIT_FAILURE); \
      } \
}
#endif

#define NUM_INT_BITS (8*sizeof(int))

inline unsigned int lcshift(unsigned int var, int num_bits) {
   return (var << num_bits) | (var >> (32-num_bits));
}

inline unsigned int rcshift(unsigned int var, int num_bits) {
   return (var << (32-num_bits)) | (var >> num_bits);
}

// If file_name exists, opens that file and reads words from it. Saves the words in a 2-D
// array called words and the lengths in an array called word_lengths. Also increments the 
// number or words in the 2-D words array to allow the words array to be resused to read another file.
void get_words_from_file( char *file_name, char **words, int *word_lengths, int *num_words );



#endif // MY_UTIL_H
