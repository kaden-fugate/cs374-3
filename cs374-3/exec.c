#include "exec.h"

// NOTE: yes, my exec function is so god damn huge that i made a special file just for it :)

// redirect all STDIN and STDOUT to the given files if init flag is set. else, route STDIN and STDOUT back to their original files
void redirectIO(int stdin_fd, int stdout_fd, char *input_file, char *output_file, int *input, int *output, int pipe_err[2], int init){

    // if init flag is set, do initial dup2 | STD IN/OUT --> IN/OUT file & STD ERR --> err pipe
    if(init){

        // only dup2 if I/O file given
        if(input_file){
            *input = open(input_file, O_RDONLY);
            if( *input != -1 ){ dup2(*input, 0); }
        }
        
        if(output_file){
            *output = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
            if( *output != -1 ){ dup2(*output, 1); }
        }

        // route stderr to err pipe
        dup2(pipe_err[1], 2);

    }

    // if init flag not set, reverse dup2 | STD IN/OUT <-- IN/OUT file
    else{
        
        // route back to original STD IN/OUT
        dup2(stdin_fd, 0);
        dup2(stdout_fd, 1);

        // close all files
        if(*input != -1){ close(*input); }
        if(*output != -1){ close(*output); }
        close(stdin_fd);
        close(stdout_fd);

    }

}

/* this function will print the error to the user and will
return stderr back to its original file */
void printErr(int stderr_fd, int pipe_err[2]){

    char buffer[2048];

    // close write end of pipe
    close( pipe_err[1] );

    // read error into buffer, set \n to \0
    int numRead = read(pipe_err[0], buffer, 2047);
    buffer[numRead - 1] = '\0';

    // return stderr to orignal file
    dup2(stderr_fd, 2);
    close(stderr_fd);

    printf("%s\n", buffer);
    
}

/* This function will handle the child process. It will write the child
pid to the parent and exec the desired command. */
void execChild(char *command, char **arguments, int is_background, int stderr_fd, int pipe_fd[2], int pipe_err[2]){

    // ------------------         SET SIGTSTP TO IGNORE         -------------------
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGTSTP, &sa, NULL);
    // -----------------------------------------------------------------------------

    // if foreground, set SIGINT back to default
    if(!is_background){ enableSIGINT(); }
    int child_pid = getpid();

    // write the child PID to parent process
    close( pipe_fd[0] );
    write( pipe_fd[1], &child_pid, sizeof( child_pid ) );
    close( pipe_fd[1] );

    // if command failed, print error, exit with status 1
    if( execvp(command, arguments) == -1 ){ 
        perror(command);
        printErr(stderr_fd, pipe_err);
        exit(1); 
    }

}

/* this function will handle the functionality of the parent process when the 
command is exec'd in the background */
int execParentBackground(int pipe_fd[2], int stdin_fd, int stdout_fd, int input, int output){

    // return I/O to original file       
    dup2(stdin_fd, 0);
    dup2(stdout_fd, 1);

    if(input != -1){ close(input); }
    if(output != -1){ close(output); }

    close(stdin_fd);
    close(stdout_fd);

    printf("Background Process ID is %d\n", child_pid);

    return child_pid;

}

/* this function will handle the functionality of the parent process when the 
command is exec'd in the foreground */
void execParentForeground(int child, char **exit_status, int stderr_fd, int pipe_err[2]){

    char *str = malloc(256);
    int status;

    // wait for child to exit
    waitpid(child, &status, 0);

    // if exited, print exit status to exit_status var
    if( WIFEXITED( status ) ){ sprintf(str, "Exit Value %d\0",  WEXITSTATUS(status) ); }

    // if terminated, print exit status to exit_status var
    else if( WIFSIGNALED( status ) ){ sprintf(str, "Terminated by Signal %d\0", WTERMSIG(status) ); }

    *exit_status = str;

}

/* This function will execute a command given in a bash-like style (i.e. command arg1, arg2, ... argN, < input
file > output file &). The function will also take a pointer to the exit status variable to set it in the main
function. This function will execute the command and either wait for it (run in foreground) or it will instantly
return command line control to the user. If a command is exec'd in the foreground, the funciton will simply return
-255 (NULL PID). If the command is run in the background, the function will return the PID of the child being run in
the background. This background PID will when be added to the list. */
int exec(char *command, char **arguments, char *input_file, char *output_file, int is_background, char **exit_status){

    // get original file descriptors for STDIN and STDOUT
    int stdin_fd = dup(0);
    int stdout_fd = dup(1);
    int stderr_fd = dup(2);

    // file descriptors for I/O files
    int input = -1;
    int output = -1;
    int child;
    int pipe_fd[2];
    int pipe_err[2];

    // set up pipes, redirect I/O files
    pipe( pipe_fd );
    pipe( pipe_err );
    redirectIO(stdin_fd, stdout_fd, input_file, output_file, &input, &output, pipe_err, 1);

    /* if input file given and no input file opened, throw error to user, 
    set I/O back to original, return -255 as the child PID */
    if( (input_file && input == -1) ){ 
        perror(input_file); 
        printErr(stderr_fd, pipe_err); 
        redirectIO(stdin_fd, stdout_fd, input_file, output_file, &input, &output, pipe_err, 0); 
        return -255; 
    }
    
    child = fork(); // fork off child process

    // child process - enter function for child process
    if( child == 0 ){ execChild(command, arguments, is_background, stderr_fd, pipe_fd, pipe_err); }
    
    // parent process
    else{ 

        // read child pid from child process
        close( pipe_fd[1] );
        read( pipe_fd[0], &child_pid, sizeof( child_pid ) );
        close( pipe_fd[0] );

        // if command is exec'd in the background, enter background parent function
        if(is_background){ return execParentBackground( pipe_fd, stdin_fd, stdout_fd, input, output ); }

        // if command is exec'd in the foreground, enter foreground function
        else{ execParentForeground( child, exit_status, stderr_fd, pipe_err ); }

        // redirect back to original I/O
        redirectIO(stdin_fd, stdout_fd, input_file, output_file, &input, &output, pipe_err, 0);

        return -255;
        
    }

}