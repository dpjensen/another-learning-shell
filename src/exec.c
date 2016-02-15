#include "exec.h"
#include "history.h"
#include "lineHandle.h"

/*
 * What follows is our functions defs and struct used for builtins
 * builtins are held in array of structs, corrisponding to the string + func pointer
 *
 */
int builtinCd(char **args);
int builtinExit();
int builtinHistory();
int builtinRunHist(char **args);

struct builtin builtinList[NUM_BUILTINS] = {

    { .builtinStr = "cd",       .builtinPtr = &builtinCd},
    { .builtinStr = "exit",     .builtinPtr = &builtinExit},
    { .builtinStr = "history",  .builtinPtr = &builtinHistory},
    { .builtinStr = "!",        .builtinPtr = &builtinRunHist},
    { .builtinStr = "!!",       .builtinPtr = &builtinRunHist}

};


/*
 * Job control and execution will start here,
 * after we go through builtin process
 *
 * ARGS:
 *      string array of commands, passed to exec
 *      bool, if job is background
 *
 * RETURN: 1 for success
 *      
 */
int lineExecJob(char**cmd, int isBackground){
   
    pid_t childPid;
    childPid =  fork();
    int status;

    if(childPid == 0){ //child proc

        //reset those signals we ignored earlier
        signal (SIGINT, SIG_DFL);
        signal (SIGQUIT, SIG_DFL);
        signal (SIGTSTP, SIG_DFL);
        signal (SIGTTIN, SIG_DFL);
        signal (SIGTTOU, SIG_DFL);
        signal (SIGCHLD, SIG_DFL);

        if(execvp(cmd[0], cmd) == -1){
            perror("Could not run command");
        }
        exit(1);
    } else if (childPid < 0){
        perror("Could not fork");
    }else{
        //parent
        if(isBackground == 0){ 
            do{//only wait if we're not running as background
                waitpid(childPid, &status, WUNTRACED);

            //this is how we check for status: exit or sig from child
            }while(!WIFEXITED(status) && !WIFSIGNALED(status));
        }
    }


    return 1;

}


/*
 * handful of builtin functions
 *
 */


/*
 * Handles commands to run from history
 *
 */
int builtinRunHist(char **args){

    if(histCount == 1){
        printf("No History to run!\n");
        return 1;
    }


    if( (args[1] == NULL) && (strcmp(args[0], "!!") != 0) ){
        printf("Enter a number from the history file, with spaces\n");
        return 1;
    }
    //process history count number, and run
    int toRun;
    char *cmdLine;
    char **histArgs;
    char **runArgs;
    int lineCount;

    if(args[1] != NULL){
        sscanf(args[1], "%d", &toRun);
        if(toRun > histCount){
            printf("Invalid history count.\n");
            return 1;
        }
    }else{
        //this is for !!
        toRun = histCount;
    }
    lineCount = getHistItem(toRun, &cmdLine);
    //remove any trailing newline
    cmdLine[strcspn(cmdLine, "\n")] = 0;
    
    histArgs = tokenizeArgs(lineCount, cmdLine);

    if((strcmp(histArgs[1], "!!") == 0)){
        printf("Saved from infinate loop.\n");
        free(histArgs);
        free(cmdLine);
        return 1;
    }

    //our super hacky way of getting rid of the history number
    runArgs = histArgs +1;
    parseLine(runArgs, 0);
   
    free(histArgs);
    free(cmdLine);
    return 1;
}


int builtinCd(char ** cmd){
    printf("CD: %s\n", cmd[1]);
    if(cmd[1] == NULL){
        return 1;
    }else{
        chdir(cmd[1]);
    }

    return 1;
}

int builtinExit(){
    return 0;
}

int builtinHistory(){
    printHist();
    return 1;
}

/*
 * check to see if you're running a builtin,
 * or sending something to exec
 *
 * ARGS:
 *      string array of commands
 *      bool, if cmd is running in background
 *
 * RETURNS:
 *      1 on normal process, 0 to exit shell    
 *  
 */
int parseLine( char** cmd, int isBackground){

    for(int32_t i = 0; i < NUM_BUILTINS; i++){
        if(strcmp(cmd[0], builtinList[i].builtinStr)==0){
            return (*builtinList[i].builtinPtr)(cmd);
        }
    }

    return lineExecJob(cmd, isBackground);

}
