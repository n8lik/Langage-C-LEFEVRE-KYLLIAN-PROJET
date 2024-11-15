#ifndef repl_h
#define repl_h

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
typedef enum {
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum { PREPARE_SUCCESS, PREPARE_UNRECOGNIZED_STATEMENT } PrepareResult;

typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;

typedef struct Statement Statement;

struct Statement{
  StatementType type;
} ;
typedef struct Row Row;

struct Row {
    int id; 
    char name[256]; 
};

#define MAX_ROWS 100 

extern Row rows[MAX_ROWS];
extern size_t num_rows;
typedef struct InputBuffer InputBuffer;

struct InputBuffer{
  char* buffer;
  size_t buffer_length;
  ssize_t input_length;
} ;



InputBuffer* new_input_buffer();
void print_prompt();
void read_input(InputBuffer* input_buffer);
void close_input_buffer(InputBuffer* input_buffer);
MetaCommandResult do_meta_command(InputBuffer* input_buffer);
PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement);
void execute_statement(Statement* statement);
void repl(void);

#endif 