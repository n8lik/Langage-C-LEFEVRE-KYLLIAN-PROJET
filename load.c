#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "load.h"

struct Table tables[MAX_TABLES];
TreeNode* firstNode = NULL;

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
    memmove(str, start, end - start + 2); // Move the string to the beginning
}

void display_tree() {
    printf("Structure de l'arbre :\n");
    print_tree(firstNode, 5);
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

    if (!firstNode) {    
        firstNode = create_node(TABLE_NODE, &tables[num_tables]);
    } else {
        insert(firstNode, TABLE_NODE, &tables[num_tables]);
    }
    token = strtok(NULL, "(,"); // Tokenize the next token to extract the columns
   while (token != NULL && tables[num_tables].num_columns < MAX_COLUMNS) // While the token is not NULL and the number of columns is less than the maximum number of columns
    {

            strcpy(tables[num_tables].columns[tables[num_tables].num_columns].name, token); // Copy the token to the column name
            tables[num_tables].columns[tables[num_tables].num_columns].id = tables[num_tables].num_columns; // Assign the column ID
            insert(firstNode, COLUMN_NODE, &tables[num_tables].columns[tables[num_tables].num_columns]);

            tables[num_tables].num_columns++; // Increment the number of columns
            
        
        token = strtok(NULL, ","); // Tokenize the next token
    } 

    num_tables++; // Increment the number of tables
}

void insert_into(char *full_statement) 
{
    char *secondstatement = malloc(strlen(full_statement) + 1); // Allocate memory for the second statement
    strcpy(secondstatement, full_statement); // Copy the full statement to the second statement
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


    char *values_start = strstr(secondstatement, "VALUES"); // Find the VALUES keyword
    if (values_start == NULL) { // If the values start is NULL
        printf("Error: Unable to find VALUES keyword\n");  // Print an error message
        return;
    }
    values_start += 6; // Move the values start pointer to the right

    while (*values_start == ' ' || *values_start == '\n') {  // While the values start is a space or a new line
        values_start++; // Move the values start pointer to the right
    }

    char *value_set = values_start; // Assign the values start to the value set
    while (*value_set != '\0') { // While the value set is not null
        char *end_set = strchr(value_set, ')'); // Find the end set
        if (end_set == NULL) { // If the end set is NULL
            printf("Error: Malformed VALUES clause\n");  // Print an error message
            return;
        }
        end_set++; // Move the end set pointer to the right
        char *values[MAX_COLUMNS]; // Declare an array of values
        int num_values = 0;  // Initialize the number of values to 0
        char *value_start = strchr(value_set, '('); // Find the value start
        if (value_start == NULL) { // If the value start is NULL
            printf("Error: Malformed VALUE set\n"); // Print an error message
            return;
        }
        value_start++; // Move the value start pointer to the right
 
        char *value_end = value_start; // Assign the value start to the value end
        while (value_end < end_set && num_values < MAX_COLUMNS) { // While the value end is less than the end set and the number of values is less than the maximum number of columns
            if (*value_end == ',' || *value_end == ')') { // If the value end is a comma or a parenthesis
                *value_end = '\0'; // Null-terminate the value end
                trim(value_start); // Trim the value start
                if (value_start[0] == '\'' && value_start[strlen(value_start)-1] == '\'') { // If the value start is a single quote
                    value_start[strlen(value_start)-1] = '\0'; // Null-terminate the value start
                    value_start++; // Move the value start pointer to the right
                } 
                values[num_values] = value_start; // Assign the value start to the values
                num_values++; // Increment the number of values
                value_start = value_end + 1; // Move the value start pointer to the right
            }
            value_end++; // Move the value end pointer to the right
        } 

        if (num_values != num_columns) { // If the number of values is not equal to the number of columns
            printf("Error: Number of values (%d) does not match number of columns (%d)\n", num_values, num_columns); // Print an error message
        } else {
            printf("Inserting values:\n"); // Print a message
            for (int i = 0; i < num_columns; i++) {
                printf("  %s: %s\n", columns[i], values[i]);
                

                
            }

            if (tables[table_id].num_rows < MAX_ROWS) { // If the number of rows is less than the maximum number of rows
                for (int i = 0; i < num_columns; i++) { // For each column
                    strncpy(tables[table_id].data[tables[table_id].num_rows][i], values[i], MAX_CELL_LENGTH - 1); // Copy the value to the data
                    
                    insert(firstNode, VALUE_NODE, values[i]);

                    tables[table_id].data[tables[table_id].num_rows][i][MAX_CELL_LENGTH - 1] = '\0'; // Null-terminate the data
                }
                tables[table_id].num_rows++; // Increment the number of rows
                printf("Row inserted successfully.\n"); // Print a message
            } else {
                printf("Error: Table is full, cannot insert more rows.\n"); // Print an error message
            }
        }

        value_set = end_set; // Assign the end set to the value set
        while (*value_set == ' ' || *value_set == ',' || *value_set == '\n') { // While the value set is a space, a comma, or a new line
            value_set++; // Move the value set pointer to the right
        } 
    }

    free(secondstatement);
    free(test);
    for (int i = 0; i < num_columns; i++) {
        free(columns[i]);
    }
}

void load(char *filename)
{
    FILE *file = NULL; // Declare a file pointer
    char path[MAX_LINE_LENGTH] = "bdd/"; // Declare a path
    char line[MAX_LINE_LENGTH]; // Declare a array of  line
    char full_create_statement[MAX_LINE_LENGTH * 10] = "";  // Declare a full create statement
    char full_statement[MAX_LINE_LENGTH * 10] = ""; // Declare a full statement

    strcat(path, filename); // Concatenate the path with the filename
    file = fopen(path, "r"); // Open the file
    if (file == NULL) // If the file is not found
    {
        printf("File name is unknown. Please add your file in 'bdd/' folder !"); // Print an error message
        return;
    }
    printf("Here is the %s database :\n\n", filename); // Print a message

    int in_create_statement = 0; // Initialize the create statement to 0
      int in_insert_statement = 0;
    trim(line);
    
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

             if (strncmp(line, "INSERT INTO", 11) == 0)
            {
                in_insert_statement = 1;
                strcpy(full_statement, line);
            }
            else if (in_insert_statement)
            {
                strcat(full_statement, " ");
                strcat(full_statement, line);
            }

            if (strchr(line, ';') != NULL && in_insert_statement)
            {
                insert_into(full_statement);
                in_insert_statement = 0;
                full_statement[0] = '\0'; // Réinitialiser full_statement
            }
            
        

        }
    }

    display_tree(); // Display the tree
    fclose(file);
}

