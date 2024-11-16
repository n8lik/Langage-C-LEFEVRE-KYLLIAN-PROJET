#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "load.h"

struct Table tables[MAX_TABLES];

TreeNode* newNode = NULL;

int num_tables = 0;

void trim(char *str) 
{
    char *start = str; // Pointer to the beginning of the string
    char *end = str + strlen(str) - 1; // Pointer to the end of the string

    while (*start && (*start == ' ' || *start == '\t' || *start == '\n')) // Skip leading white spaces
        start++; // Move the pointer to the right
    while (end > start && (*end == ' ' || *end == '\t' || *end == '\n')) // Skip trailing white spaces
        end--; // Move the pointer to the left

    *(end + 1) = '\0'; // Null-terminate the string
}


void create_table(char *full_statement) // Create a table
{
    
    if (num_tables >= MAX_TABLES) // If the maximum number of tables has been reached
    {
        printf("You have reached the maximum number of tables\n"); // Print an error message
        return; // Exit the function
    } 

    char *token = strtok(full_statement, " "); // Tokenize the full statement
    while (token != NULL && strcmp(token, "TABLE") != 0) // While the token is not NULL and is not "TABLE"
    {
        token = strtok(NULL, " "); // Move to the next token
    }
    token = strtok(NULL, " ("); // Tokenize the next token
    if (token == NULL) // if the token is null
    {
        printf("Error: Unable to extract table name\n"); // Print an error message
        return; // Exit the function
    }
//else
    strcpy(tables[num_tables].name, token); // Copy the token to the table name
    tables[num_tables].num_columns = 0;  // Initialize the number of columns to 0
    tables[num_tables].num_rows = 0; // Initialize the number of rows to 0
    tables[num_tables].id = num_tables; // Assign the table ID
    printf("Table %s created\n", tables[num_tables].name); // Print a message
    TreeNode* firstnode = initialize_tree(&tables[num_tables].name); // Initialize the tree


    token = strtok(NULL, "(,"); // Tokenize the next token to extract the columns
   while (token != NULL && tables[num_tables].num_columns < MAX_COLUMNS) // While the token is not NULL and the number of columns is less than the maximum number of columns
    {
                      
            strcpy(tables[num_tables].columns[tables[num_tables].num_columns].name, token); // Copy the token to the column name
            newNode = insert(firstnode, &tables[num_tables]); // Insert the table into the tree

            tables[num_tables].num_columns++; // Increment the number of columns
            
        
        token = strtok(NULL, ","); // Tokenize the next token
    } 

    printf("Table %s created with %d columns:\n", tables[num_tables].name, tables[num_tables].num_columns); // Print a message
    for (int i = 0; i < tables[num_tables].num_columns; i++)  // For each column
    {
        printf("  - %s %s\n", tables[num_tables].columns[i].name, tables[num_tables].columns[i].type); // Print the column name and type
    }


    num_tables++; // Increment the number of tables
}

void insert_into(char *full_statement) 
{
    char *token = strtok(full_statement, " "); // Tokenize the full statement
    while (token != NULL && strcmp(token, "INTO") != 0) // While the token is not NULL and is not "INTO"
    {
        token = strtok(NULL, " ");  // Move to the next token
    }

    token = strtok(NULL, " ("); // Tokenize the next token
    if (token == NULL) // If the token is NULL
    { 
        printf("Error: Unable to extract table name\n"); // Print an error message
        return;
    }
    
    int table_id = -1; // Initialize the table ID to -1
    num_tables = MAX_TABLES; // Set the number of tables to the maximum number of tables
 
    for (int i = 0; i < num_tables; i++) // For each table
    {
        if (strcmp(token, tables[i].name) == 0) // If the token is equal to the table name
        {
            table_id = i;  // Assign the table ID
            break;
        }
    }

    if (table_id == -1) // If the table ID is -1
    {
        printf("Error: Table %s not found\n", token); // Print an error message
        return;
    }

    token = strtok(NULL, "("); // Tokenize the next token
    char *test = malloc(strlen(token) + 1); // Allocate memory for the test
    strcpy(test, token); // Copy the token to the test
    token = strtok(test, ")"); // Tokenize the test
    if (token == NULL) // If the token is NULL
    {
        printf("Error: Unable to extract column names\n"); // Print an error message
        free(test); // Free the memory
        return;
    }

    char *columns[MAX_COLUMNS]; // Declare an array of columns
    int num_columns = 0; // Initialize the number of columns to 0
    token = strtok(token, ","); // Tokenize the token
    
    while (token != NULL && num_columns < MAX_COLUMNS) // While the token is not NULL and the number of columns is less than the maximum number of columns
    {
        trim(token); // Trim the token
        columns[num_columns] = malloc(strlen(token) + 1); // Allocate memory for the column
        strcpy(columns[num_columns], token); // Copy the token to the column
        num_columns++; // Increment the number of columns
        token = strtok(NULL, ","); // Tokenize the next token
    } 

    for (int i = 0; i < num_columns; i++) // For each column
    {
        printf("Column %d: %s\n", i, columns[i]); // Print the column
    }

    char *values[MAX_COLUMNS]; // Declare an array of values
    int num_values = 0; // Initialize the number of values to 0
    token = strtok(NULL, "("); // Tokenize the next token
    token = strtok(NULL, ")"); // Tokenize the next token
    if (token != NULL)
    {
        char *value = strtok(token, ","); // Tokenize the token
        while (value != NULL && num_values < MAX_COLUMNS) // While the value is not NULL and the number of values is less than the maximum number of columns
        {
            trim(value);
            if (value[0] == '\'' && value[strlen(value)-1] == '\'') // If the value is enclosed in single quotes
            {
                value[strlen(value)-1] = '\0'; // Null-terminate the value
                value++; // Move the pointer to the right
            }
            values[num_values] = malloc(strlen(value) + 1); // Allocate memory for the value
            strcpy(values[num_values], value); // Copy the value to the value
            num_values++; // Increment the number of values
            value = strtok(NULL, ","); // Tokenize the next value
        }
    }

    for (int i = 0; i < num_values; i++) // For each value
    {
        printf("Value %d: %s\n", i, values[i]); // Print the value
    }

    if (num_columns == num_values && tables[table_id].num_rows < MAX_ROWS) // If the number of columns is equal to the number of values and the number of rows is less than the maximum number of rows
    {
        int row = tables[table_id].num_rows; // Assign the row
        for (int i = 0; i < num_columns; i++) // For each column
        {
            int col_index = -1;
            for (int j = 0; j < tables[table_id].num_columns; j++) // For each column
            {
                if (strcmp(columns[i], tables[table_id].columns[j].name) == 0) // If the column is equal to the column name
                {
                    col_index = j; // Assign the column index
                    break; // Break the loop
                }
            }
            if (col_index != -1) // If the column index is not -1
            {
                strncpy(tables[table_id].data[row][col_index], values[i], MAX_CELL_LENGTH - 1); // Copy the value to the data
                tables[table_id].data[row][col_index][MAX_CELL_LENGTH - 1] = '\0'; // Null-terminate the data
                insert(newNode, &tables[table_id]); // Insert the table into the tree
            }
        }
        tables[table_id].num_rows++; // Increment the number of rows
        printf("Inserted new row into table %s\n", tables[table_id].name); // Print a message
    }
    else
    {
        printf("Error: Mismatch between number of columns and values or table is full\n");
    }

    free(test); // Free the memory
    for (int i = 0; i < num_columns; i++) // For each column
    {
        free(columns[i]); // Free the memory
    }
    for (int i = 0; i < num_values; i++) // For each value
    {
        free(values[i]); // Free the memory
    }
}

void load(char *filename)
{
    FILE *file = NULL; // Declare a file pointer
    char path[MAX_LINE_LENGTH] = "bdd/"; // Declare a path
    char line[MAX_LINE_LENGTH]; // Declare a array of  line
    char full_create_statement[MAX_LINE_LENGTH * 10] = "";  // Declare a full create statement

    strcat(path, filename); // Concatenate the path with the filename
    file = fopen(path, "r"); // Open the file
    if (file == NULL) // If the file is not found
    {
        printf("File name is unknown. Please add your file in 'bdd/' folder !"); // Print an error message
        return;
    }
    printf("Here is the %s database :\n\n", filename); // Print a message

    int in_create_statement = 0; // Initialize the create statement to 0
    while (fgets(line, sizeof(line), file)) // While there are lines in the file
    {
        trim(line);
        if (strlen(line) > 0) // If the line is not empty
        {

            // iTS ONly to LOAD the database from a file and not to create a new one 

            if (strncmp(line, "CREATE TABLE", 12) == 0) // If the line starts with "CREATE TABLE"
            {
                in_create_statement = 1; // Set the create statement to 1
                strcpy(full_create_statement, line); // Copy the line to the full create statement
            }
            else if (in_create_statement) // If the create statement is true
            {
                strcat(full_create_statement, " "); // Concatenate the full create statement with a space
                strcat(full_create_statement, line); // Concatenate the full create statement with the line
                if (strchr(line, ';') != NULL) // If the line contains a semicolon
                {
                    create_table(full_create_statement); // Create a table
                    in_create_statement = 0;  // Set the create statement to 0
                }  
            }

            if (strncmp(line, "INSERT INTO", 11) == 0) // If the line starts with "INSERT INTO"
            {
                insert_into(line);  // Insert into the table
            }
            

        }
    }

    fclose(file);
}

