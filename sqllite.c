// Part 1 : REPL (Read eval print-loop)

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// declare stucture "InputBuffer"
typedef struct{
	char* buffer;
	size_t buffer_length;
	ssize_t input_length;
} InputBuffer;

// declare object "new_input_buffer"
InputBuffer* new_input_buffer()
{
	InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
	input_buffer->buffer = NULL;
	input_buffer->buffer_length = 0;
	input_buffer->input_length = 0;

	return input_buffer;
}

void print_prompt() { printf("db > "); }

void read_input(InputBuffer* input_buffer)
{
	// ssize_t getline(char **lineptr, size_t *n, FILE *stream);
	ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

	// if input is nothing
	// 	exit
	if(bytes_read <= 0)
	{
		printf("Error reading input\n");
		exit(EXIT_FAILURE);
	}

	// Ignore trailing newline
	input_buffer->input_length = bytes_read - 1;
	input_buffer->buffer[bytes_read - 1] = 0;
}

void close_input_buffer(InputBuffer* input_buffer)
{
	free(input_buffer->buffer);
	free(input_buffer);
}

int main()
{
	InputBuffer* input_buffer = new_input_buffer();

	while(1){
		// prompt out and read input
		print_prompt();
		// input_buffer : ?
		read_input(input_buffer);

		// if input == exit
		// 	shut down
		// else
		// 	error message
		if(strcmp(input_buffer->buffer, ".exit") == 0){
			close_input_buffer(input_buffer);
			exit(EXIT_SUCCESS);
		}else{
			printf("Unrecoginized command '%s'.\n", input_buffer->buffer);
		}
	}
}
