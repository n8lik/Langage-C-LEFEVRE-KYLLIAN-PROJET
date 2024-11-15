#ifndef load_h
#define load_h

#define MAX_LINE_LENGTH 300
#define MAX_TABLES 10
#define MAX_COLUMNS 20
#define MAX_ROWS 100
#define MAX_CELL_LENGTH 50
typedef struct Table Table;

extern int num_tables;
struct Table{
    int id;
    char name[50];
    char columns[MAX_COLUMNS][50];
    char data[MAX_ROWS][MAX_COLUMNS][MAX_CELL_LENGTH];
    int num_columns;
    int num_rows;
};


void trim(char *str);
void load();

#endif