#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "globals.h"
#include "exec.h"
#include "signals.h"

void exitProcess(int[2048]);
void cd(char **);
void printErr(int, int[2]);
int* checkChildPids(int[2048], char **);
int getLen(int*);
void setNull(int**);

#endif