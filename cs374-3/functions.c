#include "functions.h"

// this function will exit all running child processes
void exitProcess(int child_processes[2048]){

    for(int i = 0; child_processes[i] != -255; i++){ kill(child_processes[i], SIGTERM); }

}

// This function will go to the users desired dir
void cd(char **arguments){
    
    // do nothing if cd command invalid
    if( arguments[2] ){ return; } 

    // go home if no arguments
    if( !arguments[1] ){ chdir( getenv("HOME") ); }

    // arg[1] is desired dir
    else{ chdir( arguments[1] ); }

}

/* this function will take an array of child pids (ints) and will determine if the
proccess has exited. If the process has exited, it will remove the PID from the list
of PIDs and will set the exit status to the childs respective status. */
int* checkChildPids(int *child_pids, char **exit_status){

    int new_index = 0;
    int *new_child_pids = malloc( sizeof(int) * 2048 );

    // make new array of child pids with finished signal gone  
    for(int j = 0; j < 2048; j++){ new_child_pids[j] = -255; }

    // check all PIDs in the arr
    for(int i = 0; child_pids[i] != -255; i++){

        int status;

        // if child pid has exited (WNOHANG flag tells waitpid not to wait for child to exit)
        if( waitpid(child_pids[i], &status, WNOHANG) == child_pids[i] ){ 

            char *str = malloc(256);

            // if child exited
            if( WIFEXITED( status ) ){ 
                printf("Background Process #%d Finished: Exit Value %d\n", child_pids[i], WEXITSTATUS(status));
                sprintf(str, "Exit Value %d\0", WEXITSTATUS(status)); 
            }

            // if child was terminated
            else if( WIFSIGNALED( status ) ){ 
                printf("Background Process #%d Finished: Terminated by Signal %d\n", child_pids[i], WTERMSIG(status));
                sprintf(str, "Terminated by Signal %d\0", WTERMSIG(status)); 
            }

            // set exit status variable to hold latest childs exit status
            *exit_status = str;

        }
        else{ new_child_pids[new_index] = child_pids[i]; new_index++; } // shift arr by one if child found

    }

    return new_child_pids;

}

// given an array of integers, this function will return its length
int getLen(int *array){
    int i = 0;
    for(i; array[i] != -255 && i < 2048; i++){ }
    return i;
}

/* given a pointer to an array of integers, this function will set them all
to -255, the value that I am using to define a NULL child PID */
void setNull(int **array){ for(int i = 0; i < 2048; i++){ (*array)[i] = -255; } }