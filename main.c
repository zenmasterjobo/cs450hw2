#include <stdio.h>
#include "constants.h"
#include "parsetools.h"
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

void pipe_dream(char* left[], char* right[]);
void syserror(const char *);
void redirectInputOutput(char* left[], char* right[], char* symbol[]);

int main(){
  // Buffer for reading one line of input
  char line[MAX_LINE_CHARS];
  char* line_words[MAX_LINE_WORDS + 1];
  char* left[MAX_LINE_WORDS +1];
  char* right[MAX_LINE_WORDS +1];
  char* symbol[1];
  // Loop until user hits Ctrl-D (end of input)
  // or some other input error occurs
  printf("\n $: ");
  while( fgets(line, MAX_LINE_CHARS, stdin)){
    int num_words = split_cmd_line(line, line_words);
    /*Just for demonstration purposes
    for (int i=0; i < num_words; i++)
        printf("%s\n", line_words[i]);
    printf("numwords: %d\n", num_words);
    */
    int child = fork();
    if(child == 0){
      int redirect = 0;
      for(int i = 0; i < num_words; i++){
        if(strcmp(line_words[i], ">") == 0 || strcmp(line_words[i], "<") == 0 || strcmp(line_words[i], "|") == 0){
          redirect = 1;
			    symbol[0] = line_words[i];
          for(int j = 0; j <= i; j++){
              if(j == i)
                  left[j] = NULL;
              else
                  left[j] = line_words[j];
              printf("left[%d]: %s\n", j, left[j]);
          }
          i++;
          for(int j = 0; j+i <= num_words; j++){
              if(j+i == num_words)
                  right[j] = NULL;
              else
                  right[j] = line_words[j+i];
              printf("right[%d]: %s\n", j, right[j]);
          }
          i--;
					if (*symbol[0] == '>')
          	redirectInputOutput(left, right, symbol);
					else if (*symbol[0] == '<')
						redirectInputOutput(left, right, symbol);
					else
						pipe_dream(left, right);
        }
      }
      if (!redirect)
      // no redirection needed
        execvp(*line_words, line_words);
    }
    wait(NULL);
    printf("\n $: ");
  }
  return 0;
}

void redirectInputOutput(char* left[], char* right[], char* symbol[]){
    // Input for command is read from a file when < symbol is passed
    // and output for command is sent to a file when > symbol is passed
    pid_t pid;
    if (*symbol[0] == '>'){
      int fd = open(right[0], O_WRONLY | O_CREAT | O_TRUNC, 0777);
      close(1); //close stdout
      dup(fd); // duplicate fd into stdout, reference line above
      execvp(*left, left); //execute the command
    }
    else{
      int fd = open(left[0], O_RDONLY, 0777);
      printf("%s\n", left[0]);
      close(0);
      dup(fd);
      execvp(*right, right);
    }
    wait(NULL); //parent will wait for child to finish
}

void pipe_dream(char* left[], char* right[]){
	int pfd[2];
	pid_t pid;
	printf("left: %s\n", left[0]);
	if ( pipe (pfd) == -1 )
		syserror( "Could not create a pipe" );
	switch ( pid = fork() ) {
	case -1:
		syserror( "First fork failed" );
	case  0:
		if ( close( 0 ) == -1 ) // Close stdIN
			syserror( "Could not close stdin" );
		dup(pfd[0]);
		if ( close (pfd[0]) == -1 || close (pfd[1]) == -1 )
			syserror( "Could not close pfds from first child" );
		execvp(*right, right);
		syserror( "Could not exec wc");
	}
	fprintf(stderr, "The first child's pid is: %d\n", pid);
	switch ( pid = fork() ) {
	case -1:
		syserror( "Second fork failed" );
	case  0:
		if ( close( 1 ) == -1 )
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
	printf("done with pipe_dream\n");
}

void syserror(const char *s){
	extern int errno;
	fprintf( stderr, "%s\n", s );
	fprintf( stderr, " (%s)\n", strerror(errno) );
	exit( 1 );
}
