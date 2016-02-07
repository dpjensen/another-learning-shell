#include "exec.h"
#include "common.h"

/*
 * take the char buffer, return string array
 */
char **tokenizeArgs(int inLen, char* buffer){


    //we can't have a token array
    //that's longer than the # of chars, right?
    char *token;
    char **tokens = calloc(inLen, sizeof(char*));
    int pos = 0;
    token = strtok(buffer, " ");
    while( token != NULL){

        //tokens[pos] = calloc(toksize, sizeof(char));
        //so here we're goning to have to use strcpy
        //strcpy(tokens[pos], token);
        tokens[pos] = token;
        pos++;

        token = strtok(NULL, " ");
        
    }
    tokens[pos] = NULL;
    //free(buffer);


    return tokens;

}

/*
 * Small function to get a line from stdin,
 * get rid of newline, and tokenize
 *
 * getline() is a POSIX extension in C
 */
int readline(char ** linebuf){
    //int bufsize = 1024; //default buffer size
    char *buffer;
    ssize_t readin;
    size_t readLn = 0;
    
    readin = getline(&buffer,&readLn, stdin);
    if(readin == -1){
        perror("Error reading from in");
    }

    //blank line?
    if(readin == 1){
        printf("Empty arg!\n");
    }

    //printf("Read len %zd of in %zu \n", readin, readLn);

    //strip out newline as needed
    buffer[strcspn(buffer, "\n")] = 0;

    *linebuf = buffer;

    return readin;

}


/*
 * Our main run loop.
 * get, parse, exe
 */
void shRunLoop(){

    //TODO: we still need to init SIG
    int status = 1;
    int charCount;
    char *line = NULL;
    char **args;
    do{
        printf("->");
        charCount = readline(&line);
        args = tokenizeArgs(charCount, line);
        printf("-%s-\n", args[0]);
        status = parseLine(args);

        free(args);
        free(line);

    }while(status != 0);

}



/*
 * A learning shell meant to emulate
 * parsing, job control, etc 
 * like an actual shell
 */
int main(int argc, char **argv){

    //perform init functions for job control
    shRunLoop();
}
