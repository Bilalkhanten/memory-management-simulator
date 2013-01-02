OWNER: Thomas Zaorski
EMAIL: zaorst@rpi.edu

This program simulates some of the various algorithms used to allocate processes to main memory (both contiguous and noncontiguous) . Defragmentation is also demonstrated if there is no memory slot large enough to fit a large enough slot is not created by the defrag, the simulation ends (not applicable to noncontig).

To run the program the usage is as follows: "./memsim <input-file> { noncontig | first | best | next | worst }"
Example: ./memsim proc.txt concontig

In order to end my simulation you need to enter 0 when you're asked to enter the time to stop. If you enter an invalid time, ie: you enter a lesser time then where you're currently at, my simulation moves forward by 1 second then asks you to enter a new time to stop.

I've included the 3 text files which can be used to run the simulation, if you would like to create your own, the style is as follows:

20
A 45 0 350 400 450 500 750
B 28 0 2650
C 58 0 950 1100 1200
D 86 0 650 1350 1800
E 14 0 1400
F 24 100 480 550 975

The first line is the number of processes to be created. The next row is divided up as follows: "A" is the representation of the process in main memory, "45" is memory size required, "0" is the first entry time for the process with "350" being the first exit time. All subsequent values on the line are additional entry and exit times for this process.
