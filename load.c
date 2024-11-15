#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 300
void trim(char *str)
{
    char *start = str;
    char *end = str + strlen(str) - 1;

    while (*start && (*start == ' ' || *start == '\t' || *start == '\n'))
        start++;
    while (end > start && (*end == ' ' || *end == '\t' || *end == '\n'))
        end--;

    *(end + 1) = '\0';
    memmove(str, start, end - start + 2);
}

void load(char *filename)
{

    FILE *file = NULL;
    char path[MAX_LINE_LENGTH] = "bdd/";
    char line[MAX_LINE_LENGTH]; // Déclaration de la variable line

    strcat(path, filename);
    file = fopen(path, "r");
    if (file == NULL)
    {
        printf("File name is unknowed. Please add your file in '/BDD/' folder !");
    }
    printf("Contenu du fichier SQL :\n\n");

    while (fgets(line, sizeof(line), file))
    {
        trim(line);
        if (strlen(line) > 0)
        {
            if (strncmp(line, "--", 2) == 0)
            {
                printf("\n%s\n", line);
            }
            else
            {
                
                printf("%s", line);
                if (line[strlen(line) - 1] == ';')
                {
                    printf("\n");
                }
            }
        }
    }

    fclose(file);
}