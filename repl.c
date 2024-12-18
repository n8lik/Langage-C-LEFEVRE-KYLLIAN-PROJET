#define _GNU_SOURCE 
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include "load.h"
#include "repl.h"
Row rows[MAX_ROWS];
size_t num_rows = 0;

InputBuffer* new_input_buffer() {
  InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
  input_buffer->buffer = NULL;
  input_buffer->buffer_length = 0;
  input_buffer->input_length = 0;

  return input_buffer;
}

void print_prompt() { printf("db > "); }




void read_input(InputBuffer* input_buffer) {
  ssize_t bytes_read =
      getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

  if (bytes_read <= 0) {
    printf("Error reading input\n");
    exit(EXIT_FAILURE);
  }

  // Ignore trailing newline
  input_buffer->input_length = bytes_read - 1;
  input_buffer->buffer[bytes_read - 1] = 0;
}


void close_input_buffer(InputBuffer* input_buffer) {
    free(input_buffer->buffer);
    free(input_buffer);
}


MetaCommandResult do_meta_command(InputBuffer* input_buffer) {
  if (strcmp(input_buffer->buffer, ".exit") == 0) {
    close_input_buffer(input_buffer);
    exit(EXIT_SUCCESS);
  } else if(strcmp(input_buffer->buffer, ".help") == 0) {
    printf("Commands available : ");
    printf(".help : Show help message\n");
    printf(".exit : exit the buffer\n");
    printf(".count : Show the amount of lines in the database\n");
    printf(".load : load a database\n");
    return META_COMMAND_SUCCESS;
    } else if (strcmp(input_buffer->buffer, ".count") == 0) {
        printf("There are %zu lines in the database.\n", num_rows);
        return META_COMMAND_SUCCESS;
    }else if(strcmp(input_buffer->buffer, ".load") == 0) {
        load("test.sql");
        return META_COMMAND_SUCCESS;
    } else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

PrepareResult prepare_statement(InputBuffer* input_buffer,
                                Statement* statement) {

  if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
    statement->type = STATEMENT_INSERT;
    return PREPARE_SUCCESS;
  }
  if (strcmp(input_buffer->buffer, "select") == 0) {
    statement->type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  }

  return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(Statement* statement) {
  switch (statement->type) {
    case (STATEMENT_INSERT):
    if (num_rows >= MAX_ROWS) {
                printf("Error: Table full. Cannot insert new row.\n");
                return;
            }
            Row* row = &rows[num_rows];
            printf("Enter id: ");
            scanf("%d", &row->id); 
            printf("Enter name: ");
            char name_buffer[256];
            scanf("%s", name_buffer); 
            strcpy(row->name, name_buffer); 
            num_rows++; 
            printf("Inserted.\n");
            break;
        
      break;
    case (STATEMENT_SELECT):

       for (size_t i = 0; i < num_rows; i++) {
                printf("Row %ld: id = %d, name = %s\n", i, rows[i].id, rows[i].name);
            
            break;
        }
      break;
  
}
}


void repl(void){
  InputBuffer* input_buffer = new_input_buffer();
  while (true) {
    print_prompt();
    read_input(input_buffer);
    if (input_buffer->buffer[0] == '.') {
      switch (do_meta_command(input_buffer)) {
        case (META_COMMAND_SUCCESS):
          continue;
        case (META_COMMAND_UNRECOGNIZED_COMMAND):
          printf("Unrecognized command '%s'\n", input_buffer->buffer);
          continue;
      }
    }
    Statement statement;
    switch (prepare_statement(input_buffer, &statement)) {
      case (PREPARE_SUCCESS):
        printf("recognized statement\n");
        break;
      case (PREPARE_UNRECOGNIZED_STATEMENT):
        printf("Unrecognized keyword at start of '%s'.\n",
               input_buffer->buffer);
        continue;
    }
     execute_statement(&statement);
     printf("Executed.\n");
  }
}