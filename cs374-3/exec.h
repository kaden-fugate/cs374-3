#ifndef EXEC_H
#define EXEC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#include "globals.h"
#include "signals.h"

int exec(char *, char **, char *, char *, int, char **);

#endif