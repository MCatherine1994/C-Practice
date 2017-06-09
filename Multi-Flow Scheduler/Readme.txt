Construct a simulator of a router to schedule the transmission of multiple traffic flows
Transmit flows to the router via its output interface. Each flow is allocated to a dedicated queue
Use threads to simulate the flows arriving and waiting for transmission

Function Description:
1.calTime() : use this function to calculate the arrival time, start time and finish time for each flow.
2.addToQueue() : use this function to store the flow in the right order
3.requestPipe() : this function will tell if the pipe is avaliable or not and this will determine if the thread needs to wait or not
4.releasePipe() : this function will release the pipe after one thread finished, and also send the signal to the threads which is waiting for the singal
5.thrFunction() : this is the function that gives the overall algorithm that how the threads run
6.main() : get input from input files and store the data into the right array lists; create threads for each flow

Run the program:
1. run Makefile, enter make to the command line
2. now run the MFS.c file, enter "./MFS input.txt" to the command line
