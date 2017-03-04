#include <string.h>
#include "constants.h"
#include "parsetools.h"

#include <stdio.h>

// MAX_LINE_CHARS 1024



// Parse a command line into a list of words,
// separated by whitespace
// Return the number of words 
int split_cmd_line(char* line, char** list_to_populate) {
   char* saveptr;  // for strtok_r; see http://linux.die.net/man/3/strtok_r
   char* delimiters = " \t\n"; // whitespace
   int i = 0;

   list_to_populate[0] = strtok_r(line, delimiters, &saveptr);

   while(list_to_populate[i] != NULL && i < MAX_LINE_WORDS - 1)  { // MAX_LINE_WORDS 100
       list_to_populate[++i] = strtok_r(NULL, delimiters, &saveptr);
   };

   return i;
   
}

