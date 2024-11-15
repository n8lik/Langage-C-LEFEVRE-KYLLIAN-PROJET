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
    memmove(str, start, end - start + 2); // 
}


void create_table(char *line)
{
    
    if (num_tables < MAX_TABLES) {
        char *token = strtok(line, " ");
         while (token != NULL && strcmp(token, "TABLE") != 0) {
            token = strtok(NULL, " ");
        }
        token = strtok(NULL, " (");
        strcpy(tables[num_tables].name, token);
        tables[num_tables].num_columns = 0;
        tables[num_tables].num_rows = 0;
        tables[num_tables].id = num_tables;
        printf("Table %s created\n", tables[num_tables].name);
        num_tables++;
    }
    else
    {
        printf("You have reached the maximum number of tables\n");
    }
}


void load(char *filename)
{

    FILE *file = NULL;
    char path[MAX_LINE_LENGTH] = "bdd/";
    char line[MAX_LINE_LENGTH]; 

    strcat(path, filename);
    file = fopen(path, "r");
    if (file == NULL)
    {
        printf("File name is unknowed. Please add your file in '/BDD/' folder !");
    }
    printf("Here is the %s database :\n\n", filename);

    while (fgets(line, sizeof(line), file))
    {
        trim(line);
        if (strlen(line) > 0)
        {
            if (strncmp(line, "CREATE TABLE", 12) == 0)
            {
                
                create_table(line);
                

            }

            /* else if (strncmp(line, "INSERT INTO", 11) == 0)
            {
               
            }
            else if (strncmp(line, "SELECT", 6) == 0)
            {
            }
            else if (strncmp(line, "UPDATE", 6) == 0)
            {
            }
            else if (strncmp(line, "DELETE", 6) == 0)
            {
            }
            else if (strncmp(line, "DROP TABLE", 10) == 0)
            {
            }
            else if (strncmp(line, "ALTER TABLE", 11) == 0)
            {
            }
           
 */

        }
    }

    fclose(file);
}

