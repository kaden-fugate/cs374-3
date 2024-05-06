#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "command.h"
#include "globals.h"

struct command;

// will make a command struct given a command line
struct command * makeCommand(char line[2048]){

    struct command * user_command = malloc( sizeof( struct command ) );
    user_command->command = strtok(line, " ");

    char *token = strtok(NULL, " ");

    /* init counter to count current iteration of arguments. additionally, init
    mem for possible 512 arguments. */
    int arg_counter = 1;
    user_command->arguments = malloc( sizeof( char* ) * 512 );
    user_command->arguments[0] = user_command->command;

    // loop until cur token is null or indicates an I/O redirection or & is found
    while( token != NULL && strcmp(token, "<") && strcmp(token, ">") && strcmp(token, "&")){

        user_command->arguments[arg_counter] = token;
        arg_counter++;

        token = strtok(NULL, " ");

    }

    // check if user redirected input
    if(token == NULL){ user_command->input_file = NULL; }
    else if( !strcmp(token, "<") ){

        // if true, set new input
        token = strtok(NULL, " ");
        user_command->input_file = token;

        // move pointer to next token after input file name
        token = strtok(NULL, " ");
    }
    else{ user_command->input_file = NULL; }

    // check if user redirected output
    if(token == NULL){ user_command->output_file = NULL; }
    else if( !strcmp(token, ">") ){

        // if true, set new output
        token = strtok(NULL, " ");
        user_command->output_file = token;

        // move pointer to next token after output filename
        token = strtok(NULL, " ");
    }
    else{ user_command->output_file = NULL; }

    // if token is &, set is_background to true, if not, set false
    if(token == NULL){ user_command->is_background = 0; }
    else if( !strcmp(token, "&") ){ 

        if(!foregroundOnly){ user_command->is_background = 1;  }
        else{ user_command->is_background = 0; }

        // if input or output not specified for background process, redirect to /dev/null
        if(!user_command->input_file){ user_command->input_file = "/dev/null"; }
        if(!user_command->output_file){ user_command->output_file = "/dev/null"; }
    }
    else{ user_command->is_background = 0; }

    return user_command;

}

/* this function will expand the users command. if the user command
contains '$$', it will be converted to the prcoesses ID. i.e. 
'comma$$nd $$ pid:$$ ---> comma302nd 302 pid:302 given the current
processes ID is 302. */
void expandCommand(char userInput[2048]){

    // remove trailing \n caused by fgets if present
    if (userInput[strlen(userInput) - 1] == '\n') { userInput[strlen(userInput) - 1] = '\0'; }

    char process_id_str[64];
    int process_id = getpid();
    sprintf(process_id_str, "%d", process_id); //convert pid to str
    int process_id_len = strlen(process_id_str);

    char* cur_pos = userInput;

    while ((cur_pos = strstr(cur_pos, "$$")) != NULL) {

        // move memory where $$ is ahead the length of the proccess id
        memmove(cur_pos + process_id_len, cur_pos + 2, strlen(cur_pos + 2) + 1);

        // copy process id into new place in memory
        memcpy(cur_pos, process_id_str, process_id_len);
        
        // check rest of string for $$'s
        cur_pos += process_id_len;

    }

}