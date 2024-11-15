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
        //si il y a ")" dans le token on supprime le ")"
        if (strchr(token, ')') != NULL)
        {
            token[strlen(token) - 2] = '\0';
        }
        {
            strcpy(tables[num_tables].columns[tables[num_tables].num_columns], token);
            tables[num_tables].num_columns++;
        }
        token = strtok(NULL, ",");
    } 

    printf("Table %s created with %d columns:\n", tables[num_tables].name, tables[num_tables].num_columns);
    for (int i = 0; i < tables[num_tables].num_columns; i++)
    {
        printf("  - %s\n", tables[num_tables].columns[i]);
    }

    num_tables++;
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
        }
    }

    fclose(file);
}

