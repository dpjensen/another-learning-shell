
#define _GNU_SOURCE

int initShell();
int lineExecJob(char**cmd);

pid_t shellPid;
int shell_terminal = STDIN_FILENO;
/*
 * This will track a single process in use by a job
 */
typedef struct process{
    struct process *next;   //next in LL
    char **argv;            //Our string array of args
    pid_t pid;              //process ID
    char completed;         //if process has completed
    char stopped;           //if process has been stopped
    int status;             //reported status
}process;



/*
 *This is a linked list of jobs,
 *we keep track of ids, states, and processes
 */
typedef struct job{
    struct job *next; //next in ll
    char *command; //string of actual cmd, for messages
    process *first_process; //proc used by the job
    pid_t pigd;     //process group id
    char notifed;   //for notifcation status
    struct termios tmode; //for saved terminal modes
    int stdin, stdout, stderr; //io FDs, if needed

}job;
//beginning of our LL
job job_head = NULL;

