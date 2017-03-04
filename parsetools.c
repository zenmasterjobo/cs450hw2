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

   run_command(list_to_populate);
   return i;
   
}
void run_command(char** list_to_populate){
  
  int child = fork();
  int hold;
  printf("child: %d\n",child);
  printf("current id %d\n", getpid());
  
  if(child == 0){

    
    execvp(list_to_populate,list_to_populate);
    
    printf("Test before Exec\n");
    execvp(cmd, args);
    printf ("Test after Exec\n");
  }
  else{
    hold = wait(NULL);
  }
  
  printf ("Test outside Exec\n");
  printf ("Wait: %d\n", hold);
  
}

ps -u jbergero
