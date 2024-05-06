#include "signals.h"

// this function will disable SIGINT's functionality
void disableSIGINT(){
    // ------------------         SET SIGINT TO IGNORE         -------------------
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    // ---------------------------------------------------------------------------
}

// this function will set SIGINTS functionality back to default
void enableSIGINT(){
    // ------------------         SET SIGINT TO IGNORE         -------------------
    struct sigaction sa;
    sa.sa_handler = SIG_DFL;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    // ---------------------------------------------------------------------------
}

/* this function will handle SIGTSTP. It will switch between foreground only mode when
signaled. It will achieve this by using global variables */
void handleSIGTSTP(int signal){ 

    int status;
    char str[2048];
    waitpid(child_pid, &status, 0); // wait until child proccess exits

    // SIGTSTP was signaled to turn on foreground only
    if( !signaledTSTP ){
        signaledTSTP = 1;
        write(STDOUT_FILENO, "\nEntering Foreground Only Mode\n", 31);
        foregroundOnly = 1;
    }

    // SIGTSTP was signaled to turn off foreground only
    else{ 
        signaledTSTP = 0; 
        write(STDOUT_FILENO, "\nExiting Foreground Only Mode\n", 30); 
        foregroundOnly = 0;
    }

    child_pid = -255;

}

// this function will set SIGTSTP's handler to the 'handleSIGTSTP' handler defined above
void disableSIGTSTP(){
    // ------------------         SET SIGTSTP TO HANDLER         -------------------
    struct sigaction sa;
    sa.sa_handler = handleSIGTSTP;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGTSTP, &sa, NULL);
    // -----------------------------------------------------------------------------
}