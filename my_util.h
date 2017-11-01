#ifndef MY_UTIL_H
#define MY_UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

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

/*
void uint_to_bit_string( char *bit_str, unsigned int a ) {
   assert(bit_str != NULL);
   
   unsigned int t_a = a;
   for(int i=0; i < NUM_INT_BITS; i++) {
      bit_str[NUM_INT_BITS - 1 - i] = (t_a & 1) ? '1' : '0';
      t_a >>= 1;
   }
   bit_str[NUM_INT_BITS] = '\0';
   HDEBUG_PRINTF("Inside %s(): The bit string for int a, %u (%#08x in hex) is %s\n",
      __func__, a, a, bit_str); 

}
*/

// Out of place reverse from bit_str to bit_str_rev
/*
void reverse_string( char *bit_str_rev, char *bit_str ) {
   assert( bit_str != NULL);
   assert( bit_str_rev != NULL);

   char tempc;

   // Pre-calc loop invariant
   int half_index = (NUM_INT_BITS/2);

   // Swap only needs to go halfway
   for(int i=0; i < half_index; i++) {
      bit_str_rev[NUM_INT_BITS-i-1] = bit_str[i];
      bit_str_rev[i] = bit_str[NUM_INT_BITS-i-1];
   }
   bit_str_rev[NUM_INT_BITS] = '\0';
   HDEBUG_PRINTF("Inside %s(): bit_str_rev is %s\n",
      __func__, bit_str_rev ); 
}
*/

/*
// In place reverse
void reverse_string( char *bit_str ) {
   assert( bit_str != NULL );

   HDEBUG_PRINTF("Original String is %s\n", bit_str); 
   
   int bit_str_len = strlen(bit_str);
   HDEBUG_PRINTF("Bit string length is %d\n", bit_str_len); 
   char *start_ptr = bit_str;
   char *end_ptr = start_ptr + NUM_INT_BITS - 1;
   char tempc;
   int count = 0;

   while(start_ptr < end_ptr) {
      tempc = *start_ptr;
      HDEBUG_PRINTF("\t%d: tempc is %c\n", count, tempc); 
      HDEBUG_PRINTF("\t%d: start_ptr points to %c\n", count, *start_ptr); 
      HDEBUG_PRINTF("\t%d: end_ptr points to %c\n", count, *end_ptr); 
      *start_ptr++ = *end_ptr;
      *end_ptr-- = tempc;
      HDEBUG_PRINTF("\t%d: start_ptr points to %c after swap\n", count, *start_ptr); 
      HDEBUG_PRINTF("\t%d: end_ptr points to %c after swap\n", count, *end_ptr); 
      HDEBUG_PRINTF("\t%d: start_ptr is %p\n", count, start_ptr); 
      HDEBUG_PRINTF("\t%d: end_ptr is %p\n", count, end_ptr); 
      HDEBUG_PRINTF("\t%d: Reversed String is %s\n\n", count, bit_str); 
      count++;
   }
   HDEBUG_PRINTF("Reversed String is %s\n", bit_str); 
}
*/

/*
int put_line_in_array(char *line) {
   char *line_token;

   line_token = strtok( line, " " );
   
   for( ; line_token != NULL; line_token = strtok( NULL, " ")) {
      HDEBUG_PRINTF("line_token is %s\n", line_token); 
   }

   
}
*/



#endif // MY_UTIL_H
