#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "load.h"

struct Table tables[MAX_TABLES];
int num_tables = 0;

void trim(char *str)
{
    char *start = str;
    char *end = str + strlen(str) - 1;

    while (*start && (*start == ' ' || *start == '\t' || *start == '\n'))
        start++;
    while (end > start && (*end == ' ' || *end == '\t' || *end == '\n'))
        end--;

    *(end + 1) = '\0';
}


void create_table(char *full_statement)
{
    
    if (num_tables >= MAX_TABLES)
    {
        printf("You have reached the maximum number of tables\n");
        return;
    }

    char *token = strtok(full_statement, " ");
    while (token != NULL && strcmp(token, "TABLE") != 0)
    {
        token = strtok(NULL, " ");
    }

    token = strtok(NULL, " (");
    if (token == NULL)
    {
        printf("Error: Unable to extract table name\n");
        return;
    }

    strcpy(tables[num_tables].name, token);
    tables[num_tables].num_columns = 0;
    tables[num_tables].num_rows = 0;
    tables[num_tables].id = num_tables;
    printf("Table %s created\n", tables[num_tables].name);

    token = strtok(NULL, "(,");
   while (token != NULL && tables[num_tables].num_columns < MAX_COLUMNS)
    {
        
            strcpy(tables[num_tables].columns[tables[num_tables].num_columns].name, token);
            tables[num_tables].num_columns++;
        
        token = strtok(NULL, ",");
    } 

    printf("Table %s created with %d columns:\n", tables[num_tables].name, tables[num_tables].num_columns);
    for (int i = 0; i < tables[num_tables].num_columns; i++)
    {
        printf("  - %s %s\n", tables[num_tables].columns[i].name, tables[num_tables].columns[i].type);
    }

    num_tables++;
}

void insert_into(char *full_statement)
{
    char *token = strtok(full_statement, " ");
    while (token != NULL && strcmp(token, "INTO") != 0)
    {
        token = strtok(NULL, " ");
    }

    token = strtok(NULL, " (");
    if (token == NULL)
    {
        printf("Error: Unable to extract table name\n");
        return;
    }
    
    int table_id = -1;
    num_tables = MAX_TABLES;

    for (int i = 0; i < num_tables; i++)
    {
        if (strcmp(token, tables[i].name) == 0)
        {
            table_id = i;
            break;
        }
    }

    if (table_id == -1)
    {
        printf("Error: Table %s not found\n", token);
        return;
    }

    token = strtok(NULL, "(");
    char *test = malloc(strlen(token) + 1);
    strcpy(test, token);
    token = strtok(test, ")");
    if (token == NULL)
    {
        printf("Error: Unable to extract column names\n");
        free(test);
        return;
    }

    char *columns[MAX_COLUMNS];
    int num_columns = 0;
    token = strtok(token, ",");
    
    while (token != NULL && num_columns < MAX_COLUMNS)
    {
        trim(token);
        columns[num_columns] = malloc(strlen(token) + 1);
        strcpy(columns[num_columns], token);
        num_columns++;
        token = strtok(NULL, ",");
    }

    for (int i = 0; i < num_columns; i++)
    {
        printf("Column %d: %s\n", i, columns[i]);
    }

    char *values[MAX_COLUMNS];
    int num_values = 0;
    token = strtok(NULL, "(");
    token = strtok(NULL, ")");
    if (token != NULL)
    {
        char *value = strtok(token, ",");
        while (value != NULL && num_values < MAX_COLUMNS)
        {
            trim(value);
            if (value[0] == '\'' && value[strlen(value)-1] == '\'')
            {
                value[strlen(value)-1] = '\0';
                value++;
            }
            values[num_values] = malloc(strlen(value) + 1);
            strcpy(values[num_values], value);
            num_values++;
            value = strtok(NULL, ",");
        }
    }

    for (int i = 0; i < num_values; i++)
    {
        printf("Value %d: %s\n", i, values[i]);
    }

    if (num_columns == num_values && tables[table_id].num_rows < MAX_ROWS)
    {
        int row = tables[table_id].num_rows;
        for (int i = 0; i < num_columns; i++)
        {
            int col_index = -1;
            for (int j = 0; j < tables[table_id].num_columns; j++)
            {
                if (strcmp(columns[i], tables[table_id].columns[j].name) == 0)
                {
                    col_index = j;
                    break;
                }
            }
            if (col_index != -1)
            {
                strncpy(tables[table_id].data[row][col_index], values[i], MAX_CELL_LENGTH - 1);
                tables[table_id].data[row][col_index][MAX_CELL_LENGTH - 1] = '\0';
            }
        }
        tables[table_id].num_rows++;
        printf("Inserted new row into table %s\n", tables[table_id].name);
    }
    else
    {
        printf("Error: Mismatch between number of columns and values or table is full\n");
    }

    free(test);
    for (int i = 0; i < num_columns; i++)
    {
        free(columns[i]);
    }
    for (int i = 0; i < num_values; i++)
    {
        free(values[i]);
    }
}

void load(char *filename)
{
    FILE *file = NULL;
    char path[MAX_LINE_LENGTH] = "bdd/";
    char line[MAX_LINE_LENGTH];
    char full_create_statement[MAX_LINE_LENGTH * 10] = ""; 

    strcat(path, filename);
    file = fopen(path, "r");
    if (file == NULL)
    {
        printf("File name is unknown. Please add your file in 'bdd/' folder !");
        return;
    }
    printf("Here is the %s database :\n\n", filename);

    int in_create_statement = 0;
    while (fgets(line, sizeof(line), file))
    {
        trim(line);
        if (strlen(line) > 0)
        {
            if (strncmp(line, "CREATE TABLE", 12) == 0)
            {
                in_create_statement = 1;
                strcpy(full_create_statement, line);
            }
            else if (in_create_statement)
            {
                strcat(full_create_statement, " ");
                strcat(full_create_statement, line);
                if (strchr(line, ';') != NULL)
                {
                    create_table(full_create_statement);
                    in_create_statement = 0;
                }
            }

            if (strncmp(line, "INSERT INTO", 11) == 0)
            {
                insert_into(line);
            }
            

        }
    }

    fclose(file);
}

