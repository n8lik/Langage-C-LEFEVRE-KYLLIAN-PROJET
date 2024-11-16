#ifndef load_h
#define load_h

#define MAX_LINE_LENGTH 300
#define MAX_TABLES 10
#define MAX_COLUMNS 20
#define MAX_ROWS 100
#define MAX_CELL_LENGTH 50
#define MAX_TYPE_LENGTH 20
typedef struct Table Table;

extern int num_tables;
struct Column {
    char name[50];
    char type[MAX_TYPE_LENGTH];
    int is_auto_increment;
    int is_primary_key;
    int is_foreign_key;
    int is_unique;
    int is_not_null;
};

struct Table{
    int id;
    char name[50];
    struct Column columns[MAX_COLUMNS];
    char data[MAX_ROWS][MAX_COLUMNS][MAX_CELL_LENGTH];
    int num_columns;
    int num_rows;
    
};

void trim(char *str);
void create_table(char *full_statement);
void insert_into(char *full_statement);
void load();

#endif