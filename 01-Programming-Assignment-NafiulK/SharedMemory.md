# COP 4610 Project 1

**Name:** Nafiul Khalid (nkhal014@fiu.edu)  
**PID:** 6436457  
**Username:** nkhal014

## Description

This program creates four child processes that share the same variable, `total`, using shared memory.

Each child process increments the shared variable a different number of times:

- Process 1: 100,000 times
- Process 2: 200,000 times
- Process 3: 300,000 times
- Process 4: 500,000 times

The parent process waits for all four children to finish, prints the process IDs as they exit, detaches from the shared memory, and removes the shared memory segment.

> [!NOTE] 
> Because the processes access the same shared variable without synchronization, the counter values can be different each time the program is run.

## Environment

The program was compiled and tested on the COP 4610 VM:

`[username]@ocelot.aul.fiu.edu`

## Source File

`Shared_memory.c`

## Compile

Using GCC to compile the program:

```bash
gcc Shared_memory.c -o shared_memory
```

This command will generate an output file for the source file. 

## Run

Run the program with:

```bash
./shared_memory
```

The program will display the counter value printed by each child process, followed by the process IDs of the children that exited.

## Store

Although the 'Run' instruction will execute the source code once, we will store the result of 10 performances in a file, automatically, using the command:

```bash
repeat 10 ./shared_memory > results.txt
```

