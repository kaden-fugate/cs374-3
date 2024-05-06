#ifndef COMMAND_H
#define COMMAND_H

struct command{

    char *command;
    char **arguments;
    char *input_file;
    char *output_file;
    int is_background;

};

struct command * makeCommand(char [2048]);
void expandCommand(char[2048]);

#endif