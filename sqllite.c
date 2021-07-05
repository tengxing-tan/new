// Part 1 : REPL (Read eval print-loop)
// Part 2 : compiler (check syntax) & virtual machine (execute command)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)

const uint32_t ID_SIZE = size_of_attribute(Row, id);
const uint32_t USERNAME_SIZE = size_of_attribute(Row, user);
const uint32_t EMAIL_SIZE = size_of_attribute(Row, email);
const uint32_t ID_OFFSET = 0;
const uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const uint32_t EMIAL_OFFSET = USERNAME_OFFSET + USERNAME_OFFSET;
const uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255
typedef struct {
    uint32_t id;
    char username[COLUMN_USERNAMAE_SIZE];
    char email[COLUMN_EMAIL_SIZE];
} Row;

// table structure that points to pages of rows
// and keep track of how many rows there are

const uint32_t PAGE_SIZE = 4096;
#define TABLE_MAX_PAGES 100
const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

typedef struct {
    uint32_t num_rows;
    void* pages[TABLE_MAX_PAGES];
} Table;

// declare stucture "InputBuffer"
typedef struct{
	char* buffer;
	size_t buffer_length;
	ssize_t input_length;
} InputBuffer;

// declare enum, for more result codes can added in the future
typedef enum {
	META_COMMAND_SUCCESS, 
	META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum {
	PREPARE_SUCCESS, 
    PREPARE_SYNTAX_ERROR,
	PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

typedef enum {
	STATEMENT_INSERT, 
	STATEMENT_SELECT,
} StatementType;

typedef enum {
    EXECUTE_SUCCESS,
    EXECUTE_TABLE_FULL
} ExecuteResult;

typedef struct {
	StatementType type;
    row_to_insert; // only used by insert statement
} Statement;

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

// virtual machine function
MetaCommandResult do_meta_command(InputBuffer* input_buffer)
{
	if (strcmp(input_buffer->buffer, ".exit") == 0)
	{
        close_input_buffer(input_buffer);
        free_table(table);
		exit(EXIT_SUCCESS);
	} else {
		return META_COMMAND_UNRECOGNIZED_COMMAND;
	}
}

PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement)
{
	if (strncmp(input_buffer->buffer, "insert", 6) == 0) 
	{
		statement->type = STATEMENT_INSERT;
        int args_assigned = sscanf(
            input_buffer->buffer, "insert %d %s %s", &(statement->row_to_insert.id),
            statement->row_to_insert.username, statement->row_to_insert.email);
        if (args_assigned < 3)
            return PREPARE_SYNTAX_ERROR;
		return PREPARE_SUCCESS;
	}
	if (strcmp(input_buffer->buffer, "select") == 0)
	{
		statement->type = STATEMENT_SELECT;
		return PREPARE_SUCCESS;
	}

	return PREPARE_UNRECOGNIZED_STATEMENT;
}

// convert to and from the compact representation

void serialize_row(Row* source, void destination)
{
    memcpy(destination + ID_OFFSET, &(source->id), ID_SIZE);
    memcpy(destination + USERNAME_OFFSET, &(source->username), USERNAME_SIZE);
    memcpy(destination + EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
}

void deserialize_row(void* source, Row* destination)
{
    mmemcpy(&(destination->id), source + ID_OFFSET, ID_SIZE);
    memcpy(&(destination->username), source + USERNAME_OFFSET, USERNAME_SIZE);
    memcpy(&(destination->email), source + EMAIL_OFFSET, EMAIL_SIZE);
}
// done compact representation

// to read/write in memory for a particular row

void* row_slot(Table* table, uint32_t row_num)
{
    uint32_t page_num = row_num / ROWS_PER_PAGE;
    void* page = table->pages[page_num];
    if (page == NULL) {
        // Allocate memory only when we try to access page
        page = table->pages[page_num] = malloc(PAGE_SIZE);
    }
    uint32_t row_offset = row_num % ROWS_PER_PAGE;
    uint32_t byte_offset = row-num % ROW_SIZE;
    return page + byte_offset;
}

ExecuteResult execute_insert(Statement* statement, Table* table)
{
    if (table->num_rows >= TABLE_MAX_ROWS) {
        return EXECUTE_TABLE_FULL;
    }

    Row* row_to_insert = &(statement->row_to_insert);

    serialize_row(row_to_insert, row_slot(table, table->num_rows));

    return EXECUTE_SUCCESS;
}

void print_row(Row* row)
{
    printf("(%d, %s, %s)\n", row->id, row->username, row->email);
}

ExecuteResult execute_select(Statement* statement, Table* table)
{
    Row row;
    for (uint32_t i=0; i< table->num_rows; i++)
    {
        deserialize_row(row_slot(table, i), &row);
        print_row(&row);
    }
    return EXECUTE_SUCCESS;
}

ExecuteResult execute_statement(Statement* statement, Table* table)
{
	switch (statement->type)
	{
		case (STATEMENT_INSERT):
			return execute_insert(statement, table);
		case (STATEMENT_SELECT):
            return execute_select(statement, table);
	}
}	

// Inititalize the table

Table* new_table()
{
    Table* table = malloc(sizeof(Table));
    table->num_rows = 0;
    for (uint32_t i=0; i < TABLE_MAX_PAGES; i++)
        table->pages[i] = NULL;

    return table;
}

void free_table(Table* table)
{
    for(int i=0; table->pages[i]; i++)
        free(table->pages[i]);
    free(table);
}

int main()
{
    Table* table = new_table();
	InputBuffer* input_buffer = new_input_buffer();

	while(1)
	{
		// prompt out and read input
		print_prompt();
		// input_buffer : ?
		read_input(input_buffer);

		if (strcmp(input_buffer->buffer, ".exit") == 0)
		{
			if (input_buffer->buffer[0] == '.')
			{
				switch (do_meta_command(input_buffer)) 
				{
					case (META_COMMAND_SUCCESS):
						continue;
					case (META_COMMAND_UNRECOGNIZED_COMMAND):
					 	printf("Unrecognized command '%s'\n", input_buffer->buffer); 
					 	continue;
				}
			}
		}

		Statement statement;
		switch (prepare_statement(input_buffer, &statement))
		{
			case (PREPARE_SUCCESS):
				break;
            case (PREPARE_SYNTAX_ERROR):
                printf("Syntax error. Could not parse statement.");
                continue;
			case (PREPARE_UNRECOGNIZED_STATEMENT):
				printf("Unrecognized keymword at start of '%s'.\n", input_buffer->buffer);
				continue;
		}
	
        switch (execute_statement(&statement, table))
        {
            case (EXECUTE_SUCCESS):
                printf("Executed.\n");
                break;
            case (EXECUTE_TABLE_FULL):
                printf("Error: Table full.\n");
                break;
        }
    }
}
