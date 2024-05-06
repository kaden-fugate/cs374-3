#include "functions.h"
#include "command.h"
#include "signals.h"

int main(){

    int *child_pids = malloc(sizeof(int) * 2048);
    setNull(&child_pids); // fill child_pids with -255
    char user_command_str[2048];
    char *exit_status = malloc(sizeof(char) * 256);
    exit_status = "Exit Value 0"; // init exit status 0
    FILE * input_file = stdin;
    FILE * output_file = stdout;

    do{
        // disable SIGINT & SIGTSTP
        disableSIGINT();
        disableSIGTSTP();
        
        // prompt user for input
        printf(": ");
        fflush(output_file);
        fgets(user_command_str, 2048, stdin);

        // expand $$'s, make user command
        expandCommand(user_command_str);
        struct command *user_command = makeCommand(user_command_str);

        // if first 2 chars of user_command_str == cd
        if( !strncmp(user_command_str, "cd", 2) ){ cd( user_command->arguments ); }

        // if user_command_str == status
        else if( !strcmp(user_command_str, "status") ){ printf("%s\n", exit_status); }

        // if user_command_str == "" or user_command_str[0] == "#", we know the line is empty or comment
        else if( !strcmp(user_command_str, "") || !strncmp(user_command_str, "#", 1) ){ }

        /* if user_command_str != exit, fork child, exec
        command. */ 
        else if( strcmp(user_command_str, "exit") ){ 

            /* exec function will return the child processes id if command is run in the background.
            if executed in the foreground, exec will return -255 */
            child_pid = exec(user_command->command, user_command->arguments, user_command->input_file, user_command->output_file, user_command->is_background, &exit_status); 

            // if child process running, add pid to array
            if( child_pid != -255 ){ child_pids[ getLen( child_pids ) ] = child_pid; child_pid = -255; }
            
        }

        /* if child process running in the background, check if it is finished running */
        if( getLen( child_pids ) > 0 ){ child_pids = checkChildPids(child_pids, &exit_status); }

    }

    // repeat while user_command_str not exit
    while( strcmp(user_command_str, "exit") );

    // kill all children (LOL)
    exitProcess(child_pids);

    return 0;
}