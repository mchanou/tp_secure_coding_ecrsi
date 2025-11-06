/*
* TD Secure Coding UBO 2024
* Exercice 1
* Fichier ex1.c
* Compilation: cc ex1.c -o ex1
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char *argv[]) {
    int error = EXIT_SUCCESS;
    char command[100];
    char filename[50];

    /* Get the filename to read */
    printf("Enter a file name to display:");
    fgets(filename,sizeof(filename),stdin);/*Buffet overflow"*/

    /* Remove the newline character from the input */
    filename[strcspn(filename, "\n")] = 0;

    /* Construct the command */
    sprintf(command, "/bin/cat %s", filename);

    /* Execute the command */
    system(command);
    
    return error;

}