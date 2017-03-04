#include <stdio.h>
#include "constants.h"
#include "parsetools.h"
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

void pipe_dream(char* left[], char* right[]);
void syserror(const char *);

int main()
{
  // Buffer for reading one line of input
  char line[MAX_LINE_CHARS];
  char* line_words[MAX_LINE_WORDS + 1];
  int pipes_num = 0;
  // Loop until user hits Ctrl-D (end of input)
  // or some other input error occurs
  printf("\n $: ");
  while( fgets(line, MAX_LINE_CHARS, stdin) )
    {
      int num_words = split_cmd_line(line, line_words);
		
      int child = fork();
			
      if(child == 0)
	{
	  char *leftPipe[MAX_LINE_WORDS +1];
	  char *rightPipe[MAX_LINE_WORDS +1];
	  //memset(leftPipe, '\0', sizeof(leftPipe));
	  //memset(rightPipe, '\0', sizeof(rightPipe));

	  //int pipeFlag = 0;
	  for(int i = 0; i < num_words; i++){
	    //printf("line_words[%d] = %s\n", i, line_words[i]);
	    
	    //if(pipeFlag){

	    //for(int j=0; j < i; j++)
	    //rightPipe[j] = line_words[i];
	    //strcpy(*rightPipe, *line_words, i)
	    // }
      
	    if(strcmp(line_words[i], "|") == 0){
	      pipes_num ++;
	      pipeFlag = 1;
	      strncpy(*leftPipe, *line_words, i);
	    }
	  }
	  pipe_dream(leftPipe, rightPipe);
	}
    		
      if(pipes_num == 0)
	{
	  execvp(*line_words, line_words);
	}
    }			
      else
	printf("Found a pipe. \n");

		
while( wait(NULL) != -1);
  printf("\n $: ");
	
return 0;
}

void pipe_dream(char* left[], char* right[]){

  int pfd[2];
  pid_t pid;
  
  if ( pipe (pfd) == -1 )
    syserror( "Could not create a pipe" );
  switch ( pid = fork() ) {
  case -1:
    syserror( "First fork failed" );
  case  0:
    printf("We hit the first Case");
    if ( close( 0 ) == -1 ) // Close stdIN
      syserror( "Could not close stdin" );
    
    dup(pfd[0]);
    if ( close (pfd[0]) == -1 || close (pfd[1]) == -1 )
      syserror( "Could not close pfds from first child" );
    execvp(*right, right); // take the right argument of the pipe
    syserror( "Could not exec wc");
  }
  fprintf(stderr, "The first child's pid is: %d\n", pid);
  switch ( pid = fork() ) {
  case -1:
    syserror( "Second fork failed" );
  case  0:
    if ( close( 1 ) == -1 ) // close stdOut
      syserror( "Could not close stdout" );
    dup(pfd[1]);
    if ( close (pfd[0]) == -1 || close (pfd[1]) == -1 )
      syserror( "Could not close pfds from second child" );
    execvp(*left, left);
    syserror( "Could not exec who" );
  }
  fprintf(stderr, "The second child's pid is: %d\n", pid);
  if (close(pfd[0]) == -1)
    syserror( "Parent could not close stdin" );
  if (close(pfd[1]) == -1)
    syserror( "Parent could not close stdout" );
  while ( wait(NULL) != -1) ;
}

void syserror(const char *s)
{
  extern int errno;

  fprintf( stderr, "%s\n", s );
  fprintf( stderr, " (%s)\n", strerror(errno) );
  exit( 1 );
}

