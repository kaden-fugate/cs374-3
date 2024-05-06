To run my implementation of smallsh, you will need to follow these steps:

1. unzip the file (im assuming youve done this already as you are reading this file
that was contained within the zip file lol)

2. cd into your newly 'fugateka_program3' directory (or whatever the name is)

3. run 'make'. you should see the following output: 

"
gcc --std=c99 -g -D_POSIX_C_SOURCE -c globals.c
gcc --std=c99 -g -D_POSIX_C_SOURCE -c command.c
gcc --std=c99 -g -D_POSIX_C_SOURCE -c functions.c
gcc --std=c99 -g -D_POSIX_C_SOURCE -c exec.c
gcc --std=c99 -g -D_POSIX_C_SOURCE -c signals.c
gcc --std=c99 -g -D_POSIX_C_SOURCE -c main.c
gcc --std=c99 -g -D_POSIX_C_SOURCE globals.o command.o functions.o exec.o signals.o main.o -o smallsh
"

4. once the executable has been compiled, run './smallsh' to run the executable. once running,
you should be prompted with ': '. this means that you have properly started the program!
great job! and thank you in advance for taking the time to grade my assignment!!!!!
as a ULA myself, i know how much time and effort it takes.

NOTE:

For some reason, when I run the p3testscript with my executeable, theres
some weird stuff going on with the output. The only thing that isnt right
about my output is seen on lines 138 & 141 (im assuming the output for the
grading TA will be the same as mine. im also assuming you use the same testing
script). 

Basically, on these lines, it gives a background process id (line 138)
then says that background process finished (line 141). The weird part about this
is that no time before this is any command called as a background process. Additionally,
when I run the commands in the test script manually, it doesnt have this issue.

Anyways, I figured I'd explain the weird part rather than having you fish it out
yourself. If I end up losing points for that, that's fine.