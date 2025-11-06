/*
* TD Secure Coding UBO 2024
* Exercice 2
* Fichier ex2.c
* Compilation: cc ex2.c -fno-stack-protector -O0 -ggdb3 -o ex2
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE (!FALSE)
#endif

/* Remove all not printable characters from the user supplied characters chain */
static inline int filter_password_characters(const char *input, char *output) {
    register const char *i = input;
    const char * const limit = input + strlen(input);
    register char *o = output;
    for(;i<limit;i++) {
        if (isprint(*i)) {
            *o = *i;
            o++;
        }
    }   
    *o = '\0';
}
static int check_password(const char* password) {
    const char * const secret = "AzErTy";
    char check[sizeof(int)];
    char user_password[10]; /*Buffer overflow possible*/
    int *is_valid = (int*)check;
    *is_valid = FALSE;
    filter_password_characters(password,user_password);
    if (strncmp(user_password,secret,strlen(user_password)) == 0) {
        *is_valid = TRUE;
    }
return *is_valid;
}


int main(int argc, char *argv[]) {
    int error = EXIT_SUCCESS;
    char user_input[50];

    /* Get the password */
    printf("Enter password: ");
    fgets(user_input, sizeof(user_input), stdin);

    /* Remove the newline character from the input */
    user_input[strcspn(user_input, "\n")] = 0;

    if (check_password(user_input)) {
    printf("Password check: OK\n");
    } else {
    printf("Password check: FAILED\n");
    }
    return error;
}