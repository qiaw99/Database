#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h>

// The command which begins with '.' like .exit
typedef enum{
	META_COMMAND_SUCCESS,
	META_COMMAND_UNRECOGNIZED_COMMAND
}MetaCommandResult;

// The flag which decides whether the operation is valid
typedef enum{
	PREAPARE_SUCCESS,
	PREAPARE_UNRECOGNIZED_STATMENT
}PrepareResult;

// The possible operations
typedef enum{
	STATEMENT_INSERT,
	STATEMENT_SELECT
}StatementType;

typedef struct{
	StatementType type;
}Statement;

typedef struct{
	char* buffer;
	size_t buffer_length;
	ssize_t input_length;
}InputBuffer;

MetaCommandResult do_meta_command(InputBuffer* input_buffer){
	if(strcmp(input_buffer -> buffer, ".exit") == 0){
		exit(EXIT_SUCCESS);
	}else{
		return META_COMMAND_UNRECOGNIZED_COMMAND;
	}
}

PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement){
	if(strncmp(input_buffer -> buffer, "insert", 6) == 0){
		statement -> type = STATEMENT_INSERT;
		return PREAPARE_SUCCESS;
	}
	if(strncmp(input_buffer -> buffer, "select", 6) == 0){
		statement -> type = STATEMENT_SELECT;
		return PREAPARE_SUCCESS;
	}
	return PREAPARE_UNRECOGNIZED_STATMENT;
}

void execute_statement(Statement* statement){
	switch(statement -> type){
		case(STATEMENT_SELECT):
			printf("select.\n");
			break;
			
		case(STATEMENT_INSERT):
			printf("insert.\n");
			break;
	}
}

InputBuffer* new_input_buffer(){
	InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
	input_buffer -> buffer = NULL;
	input_buffer -> buffer_length = 0;
	input_buffer -> input_length = 0;
	return input_buffer;
}

void print_prompt(){
	printf("db > ");
}

void read_input(InputBuffer* input_buffer){
	/**
	ssize_t getline(char** lineptr, size_t *n, FILE *stream);

	@para lineptr: a pointer to the variable we use to point to the buffer containing the read line
	@para n: a pointer to the variable we use to save the size of allocated buffer 
	@para stream: the input stream to read from
	return: the number of bytes
	*/
	ssize_t bytes_read = getline(&(input_buffer -> buffer), &(input_buffer -> buffer_length), stdin);
	if(bytes_read <= 0){
		printf("Erro reading input.\n");
		exit(EXIT_FAILURE);
	}

	// ignoring trailing newline
	input_buffer -> input_length = bytes_read - 1;
	input_buffer -> buffer[bytes_read - 1] = 0; 
}

void close_input_buffer(InputBuffer* input_buffer){
	free(input_buffer -> buffer);
	free(input_buffer);
}

int main(int argc, char* argv[]){
	InputBuffer* input_buffer = new_input_buffer();
	while(true){
		print_prompt();
		read_input(input_buffer);

		// check the meta command
		if(input_buffer -> buffer[0] == '.'){
			switch(do_meta_command(input_buffer)){
				case(META_COMMAND_SUCCESS):
					continue;
				case(META_COMMAND_UNRECOGNIZED_COMMAND):
					printf("Unrecognized command '%s'\n", input_buffer -> buffer);
					continue;
			}
		}

		// check the command
		Statement statement;
		switch(prepare_statement(input_buffer, &statement)){
			case(PREAPARE_SUCCESS):
				break;
			case(PREAPARE_UNRECOGNIZED_STATMENT):
				printf("Unrecognized keyword at start of '%s', input_buffer -> buffer\n");
				continue;
		}

		execute_statement(&statement);
		printf("Executed.\n");
	}
}