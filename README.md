# OS Assignment 3

The following repository provides the solutions to Assignment 3 given during the course Operating Systems (CSE-231) at IIITD during the Monsoon'22 semester. The description is as follows:

## Question 1 (Dining Philospher Problem)
### Part A 
This required us to implement the Dining Philospher's Problem (a classic synchronisation problem) using the following two techniques:
- Strict Ordering of Resource Requests
- Synchronisation Primitives such as Semaphores

### Part B
This required us to implement a modified version of the Dining Philospher's Problem wherein we have 2 soup bowls now. We needed to write two codes for this part again solving the deadlock in this problem using the two above mentioned techniques.

## Question 2 (Interprocess Communication - IPC)
This required us to generate 50 random strings using program P1 and send them in batches of 5 to program P2. Upon receiving the 5 strings, program P2 will print the 5 strings and send back the highest ID which it received back to P1. This process continues till process P1 sends all the 50 strings to P2. At the end of this, we need to print the time taken by the process P1 to perform this operation. The given problem had to be implemented using the given three IPC tools:
- FIFOs
- UNIX Domain Sockets
- Shared Memory

## Question 3 (Kernel Module)
In this we were supposed to implement a Linux Kernel Module which takes the PID of a running process as input, reads its task struct and prints out the following details of the process:
- PPID
- UID
- GPID
- State
- Path
The module should only run when its loaded and should be removed when it is unloaded.

## End
All the program logics and explanations are given with the code in the respective folders and can be referenced accordingly.
