#include "exec.h"


int builtinCd(char **args);
int builtinExit();
int builtinHistory();

struct builtin builtinList[NUM_BUILTINS] = {

    { .builtinStr = "cd", .builtinPtr = &builtinCd},
    { .builtinStr = "exit", .builtinPtr = &builtinExit},
    { .builtinStr = "history", .builtinPtr = &builtinHistory}

};
/*
 * Job control and execution will start here,
 * after we go through builtin process
 */
int lineExecJob(char**cmd){
   
    pid_t childPid;
    childPid =  fork();
    int status;

    if(childPid == 0){
        if(execvp(cmd[0], cmd) == -1){
            perror("Could not run command");
        }
        exit(1);
    } else if (childPid < 0){
        perror("Could not fork");
    }else{
        //parent
        do{
            waitpid(childPid, &status, WUNTRACED);
        }while(!WIFEXITED(status) && WIFSIGNALED(status));
    }


    return 1;

}


/*
 * Two builtin functions for cd
 * and exit
 */
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

    return 1;
}

/*
 * check to see if you're running a builtin,
 * or sending something to exec
 */
int parseLine( char** cmd){

    for(int32_t i = 0; i < NUM_BUILTINS; i++){
        if(strcmp(cmd[0], builtinList[i].builtinStr)==0){
            return (*builtinList[i].builtinPtr)(cmd);
        }
    }

    return lineExecJob(cmd);

}
