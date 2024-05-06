#ifndef SIGNALS_H
#define SIGNALS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "globals.h"

void disableSIGINT();
void enableSIGINT();
void handleSIGTSTP(int);
void disableSIGTSTP();

#endif