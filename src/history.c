#include "history.h"
/*
 * History functionality
 */


/*
 * We just need to close our file stream
 */
void closeHist(){
   fclose(histFile); 
}


/*
 * build our file path and open the file for use,
 * and get the last count from history
 * We track the number of commands entered 
 * by getting the last number in the file
 */
void initHist(){
    char *homeDir =  getenv("HOME");
    char *path = strncat(homeDir, HISTNAME, 20);
    histFile = fopen(path, "a+");
    //printf("Opened: %s\n",path);
    if(histFile == NULL){
        perror("Could not open history File");
        exit(1);
    }

    //now we need to see how many items are in the history
    fseek(histFile,0, SEEK_END);

    if(histFile != NULL && ftell(histFile) != 0){
        //we have a nonempty hist file, interate & find last line.
        int c;
        fseek(histFile,-3,SEEK_END);
        while( (c = fgetc(histFile)) != '\n'){
            fseek(histFile, -2, SEEK_CUR);
        }
        char *numString = calloc(20, sizeof(char));
        //we just want the first few characters to get an integer value.
        fgets(numString, 20, histFile);
        //now we have our position, turn to int
        int hist;
        int match = sscanf(numString, "%d:", &hist);
        free(numString);
        if(match == 0){
            //we didn't any count
            histCount = 0;
        }else{
            histCount = hist;
        }
    }else{
        //no history file
        histCount = 0;
    }


}

/*
 * Appends a command to the end of the history file
 *  
 * ARGS:
 *      string to write to history
 */
void writeHist(char * line){
    //get to end of file
    if(histFile == NULL){
        return;
    }

    histCount++;
    fseek(histFile, 0, SEEK_END);
    fprintf(histFile, "%i: %s\n", histCount, line);

}

/*
 * Takes a position in the history list, and return string
 *
 * ARGS:
 *      position in the hist file (beginning: 1), pointer to buffer to write to
 *
 * RETURNS:
 *      number of characters read
 */

int getHistItem(int pos, char **buffer){

    //reset to stop, then run through
    fseek(histFile, 0, SEEK_SET);
    size_t readLn = 0;
    int readCount= 0;
    char *buf;
    for(int i = 1; i<=pos; i++){
        readCount = getline(&buf,&readLn, histFile);
    }

    *buffer = buf;
    return readCount;
     
}


/*
 * Print the complete history
 */
void printHist(){
    fseek(histFile, 0, SEEK_SET);

    int c;
    while( (c = getc(histFile)) != EOF ){
        putchar(c);
    }

}
