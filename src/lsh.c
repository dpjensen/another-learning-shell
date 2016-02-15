#include "exec.h"
#include "common.h"
#include "history.h"
#include "lineHandle.h"

/*
 * A learning shell that performs basic functions
 * Execution, background, and history
 *
 * Job execution is very stripped down, and will be expanded with time
 */




/*
 * Quick and dirty, to check for a history command
 *
 * ARGS:
 *      line from user input
 *
 * RETURNS:
 *      1 for history || (!) cmd, 0 otherwise
 */

int isHistCmd(char * line){

    //we wanna trim whitespace before we do this
    while(isspace(*line)){
        line++;
    }
    
    if(line[0] == '!'){
        return 1;
    }else{
        return 0;
    }

}


/*
 * very short function to get a flag for a
 * background process
 *
 * ARGS:
 *      User command string
 *      length of string
 * RETURNS:
 *      1 for bg proc, 0 otherwise
 *
 */
int isBackground(char *line, int len){

    //char lastChar = line[len-2];
    //printf("Last Char: %c in len %i\n", lastChar, len);

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
    initHist(); 
  
    /*the parent shell will ignore sigs
     *These will be set back to default in the child
    */
    signal (SIGINT, SIG_IGN);
    signal (SIGQUIT, SIG_IGN);
    signal (SIGTSTP, SIG_IGN);
    signal (SIGTTIN, SIG_IGN);
    signal (SIGTTOU, SIG_IGN);
    signal (SIGCHLD, SIG_IGN);



    int status = 1;
    int charCount;
    int bg;
    char *line = NULL;
    char **args;
    /*
     * The process:
     *
     * print prompt (->)
     * Read line from stdin
     * write to history
     * check if running in the bg
     * tokenize user string, for exec
     * send array to be executed
     */
    do{
        printf("->");
        charCount = readline(&line);
        
        if(isHistCmd(line) == 0){
            writeHist(line);
        }
        
        bg = isBackground(line, charCount);
        args = tokenizeArgs(charCount, line);
        //printf("-%s-\n", args[0]);

        //we don't wanna process a newline
        if(charCount != 1){
            status = parseLine(args, bg);
        }

        free(args);
        free(line);

    }while(status != 0);

    closeHist();
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
