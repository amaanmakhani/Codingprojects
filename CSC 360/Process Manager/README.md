# CSC360 A1

- To build type 
    make

- To run PMan
    ./PMan

- To use inf, an example is shown below as it was changed slightly
    bg ./inf 2 20

- To use args:
    bg ./args 2 20

- To test utime use:
    bg ./utimeTest 2000

- The program handles many errors very well, this includes:

    1. bg execvp failure
    2. bglist given too many commands 
    3. bgkill, bgstart, bgstop called with invalid PID
    - Note: User is notified of all process status changes (i.e. started, stopped, not only terminated and killed)
    4. pstat called with an invalid PID
    5. Invalid commands
    6. All system call return values are checked