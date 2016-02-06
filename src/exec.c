#include "exec.h"

/*
 * performs a series of init functions
 * that will be used to manage
 * processes and job
 */
void initShell(){

    //get the file descriptor of the shell
    int32_t shellFD = STDIN_FILENO;

    //ignore the signals, so we don't kill the shell by accident
    signal (SIGINT, SIG_IGN);
    signal (SIGQUIT, SIG_IGN);
    signal (SIGTSTP, SIG_IGN);
    signal (SIGTTIN, SIG_IGN);
    signal (SIGTTOU, SIG_IGN);
    signal (SIGCHLD, SIG_IGN);

    //put ourselves in a dedicated process group
    //This will allow job control
    shellPid = getpid();
    int32_t pidStat;
    pidStat = setpgid(shellPid, shellPid);
    if(pidStat < 0){
        perror("Could not set process group");
        exit(1);
    }

    //give our process control of the terminal
    int32_t tcStat;
    tcStat = tcsetpgrp(shellFD, shellPid);
    if(tcStat < 0){
        perror("Could not set forground process\n");
        exit(1);
    }

    //We may want to save off terminal attrs?

}
void placeInForeground(job* toStart, int cont){

    //set foreground terminal process
    tcsetpgrp(shell_terminal, j->pgid);

    //send continue signal here

    //wait for job
    


}

/*
 * Create and job and process, 
 * prepare to run.
 */
int lineExecJob(char**cmd, char * line){

    job *newCurrentJob = calloc(1, sizeof(job));

    //setup our job
    newCurrentJob->command = line;
    newCurrentJob->stdin = STDIN_FILENO;
    newCurrentJob->stdout = STDOUT_FILENO;
    newCurrentJob->stderr = STDERR_FILENO;

    //set up our process for execution 
    newCurrentJob->first = calloc(1, sizeof(process));
    newCurrentJob->first->argv = cmd;

    startJob(newCurrentJob, 1);
    
}

/*
 * Exec our process,
 * put it in foreground or background
 */
void runProc(process *proc, pid_t pgid, int inFD, int outFD, int errFD, int isForeground){
   
    pid_t pid;
    if(shell_is_interactive){
        //give the process group the terminal
        //we do this to parent & child
        pid = getpid();
        if(pgid == 0){
            pgid = pid;
        }
        setpgid(pid, pgid);
        if(isForeground){
            tcsetpgrp(shell_terminal, pgid);
        }
        //reset signal handling
        signal (SIGINT, SIG_DFL);
        signal (SIGQUIT, SIG_DFL);
        signal (SIGTSTP, SIG_DFL);
        signal (SIGTTIN, SIG_DFL);
        signal (SIGTTOU, SIG_DFL);
        signal (SIGCHLD, SIG_DFL);
    }

    if(inFD != STDIN_FILENO){
        dup2(inFD, STDIN_FILENO);
        close(inFD);
    }

    if(outFD != STDOUT_FILENO){
        dup2(outFD, STDOUT_FILENO);
        close(outFD);
    }

    if(errFD != STDERR_FILENO){
        dup2(errFD, STDERR_FILENO);
        close(errFD);
    }

    //exec our new process
    execcp(proc->argv[0], proc->argv);
    perror("exec");
    exit(1);

}



/*
 * prepare to startup a given job
 */
void startJob(job *toStart, int isForeground){

    process *proc;
    pid_t pid;
    int mypipe[2], inFD, outFD;

    inFD = toStart->stdin;
    //loop over all available procs
    for(proc = toStart->first; proc; proc = proc->next){
    
        //are we setting up multiple procs for pipe()
        if(proc->next){
            if(pipe(mypipe)){
                perror("Pipe error");
                exit(1);
            }
            outFD = mypipe[1];
        }else{
            outFD = toStart->stdout;
        }

        //perform our fork operations
        pid = fork();
        if(pid == 0){
            //launch child
            runProc(proc, toStart->pgid, inFD, outFD, toStart->stderr, isForeground);
        }else{
            //nonzero pid, parent
            proc->pid = pid;
            if(shell_is_interactive){
                if(!toStart->pgid){
                    toStart->pgid = pid;
                    setpgid(pid, j->pgid);
                }
            }
        }

        //clean up after pipes
        if(inFD != toStart->stdin){
            close(infile);
        }

        if(outFD != toStart->stdout){
            close(outfile);
        }
        infile = mypipe[0];
    }
    //FORMAT/PRINT JOB INFO
    if(!shell_is_interactive){
        wait_for_job(toStart);
    }else if(isForeground){
        placeInForeground(toStart, 0);
    }else{
        //put in background
        placeInForeground(toStart, 0);
    }

}
