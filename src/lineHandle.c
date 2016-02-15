#include "history.h"

/*
 * A small set of functions 
 * for handling, manipulation of line inputs for the shell
 */



/*
 * Small function to get a line from stdin,
 * get rid of newline, and tokenize
 *
 * getline() is a POSIX extension in C
 *
 * ARGS:
 *      pointer to character array
 *
 * RETURNS: Num of characters read in
 */
int readline(char ** linebuf){
    char *buffer = NULL;
    ssize_t readin;
    size_t readLn = 0;
    
    readin = getline(&buffer,&readLn, stdin);
    if(readin == -1){
        perror("Error reading from in");
    }

    //blank line?
    /*
    if(readin == 1){
        printf("Empty arg!\n");
    }
    */

    //printf("Read len %zd of in %zu \n", readin, readLn);

    //strip out newline as needed
    buffer[strcspn(buffer, "\n")] = 0;

    *linebuf = buffer;

    return readin;

}



/*
 * take the char buffer, return string array
 *
 * ARGS:
 *      length of chars in array
 *      char array
 *
 * RETURNS: string array of args to run
 */
char **tokenizeArgs(int inLen, char* buffer){


    //we can't have a token array
    //that's longer than the # of chars, right?
    char *token;
    char **tokens = calloc(inLen, sizeof(char*));
    int pos = 0;
    token = strtok(buffer, " ");
    while( token != NULL){

        //we handle background proc given by &, so don't pass that on
        if(strcmp(token, "&") != 0){
            tokens[pos] = token;
            pos++;
        }

        token = strtok(NULL, " ");
        
    }
    tokens[pos] = NULL;


    return tokens;

}
