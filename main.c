#include <stdio.h>
#include "constants.h"
#include "parsetools.h"
#include <sys/wait.h>
#include <unistd.h>

int main()
{
	// Buffer for reading one line of input
	char line[MAX_LINE_CHARS];
	char* line_words[MAX_LINE_WORDS + 1];
	int pipes_num = 0;
	// Loop until user hits Ctrl-D (end of input)
	// or some other input error occurs
	while( fgets(line, MAX_LINE_CHARS, stdin) )
	{
		int num_words = split_cmd_line(line, line_words);

		//execvp(*line_words, line_words);
		
		/* Just for demonstration purposes
		for (int i=0; i < num_words; i++)
			printf("%s\n", line_words[i]);
		*/

		printf("\n $: ");
		int child = fork();
			
		if(child == 0)
		{
			execvp(*line_words, line_words);
		
		}
		
		wait(NULL);
		printf("\n $: ");
	}

	
	
	return 0;
}
