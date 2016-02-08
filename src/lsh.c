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
        if(strcmp(token, "&") != 0){
            tokens[pos] = token;
            pos++;
        }

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
 * very short function to get a flag for a
 * background process
 */
int isBackground(char *line, int len){

    char lastChar = line[len-2];
    printf("test: %c\n", line[0]);
    printf("Last Char: %c in len %i\n", lastChar, len);

    if(strcmp(&line[len-2], "&")== 0){
        printf("Background\n");
        return 1;
    }else{
        return 0;
    }


}

/*
 * Our main run loop.
 * get, parse, exe
 */
void shRunLoop(){

    //TODO: we still need to init SIG
    int status = 1;
    int charCount;
    int bg;
    char *line = NULL;
    char **args;
    do{
        printf("->");
        charCount = readline(&line);

        bg = isBackground(line, charCount);
        args = tokenizeArgs(charCount, line);
        printf("-%s-\n", args[0]);
        status = parseLine(args, bg);

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
