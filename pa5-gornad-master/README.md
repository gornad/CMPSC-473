#######################################################################################
PA5: Concurrent Hard Disk Emulation					       12/10/17
---------------------------------------------------------------------------------------
README

(1) Quick Overview
(2) Running the program - how to use the timer!
---------------------------------------------------------------------------------------
(1)	This program attempts to emulate a hard-disk drive (HDD) using a multi-threaded 
	device driver that we have implemented. Our main tasks were to: 
		*Make the shared buffer between the device driver and the disk 
			“thread friendly” and ensure concurrency 
		*Implement the elevator algorithm for disk head scheduling
		*Introduce a timer to ensure disk buffer responsiveness
---------------------------------------------------------------------------------------
(2)	* Use the included Makefile to compile and clean *

	(a) Running with FCFS:
		Set algo = 0 in driver.c (line 21)

	(b) Running with Elevator:
		Set algo = 1 in driver.c (line 21)

	(c) Changing the Buffer Timer
		Change #define TIME 50 in driver.h (line 15) to the time value needed
		NOTE: Timer is set to 50 for testing purposes. Default value is 5 (seconds)

	Run the file by inputting ./disk_simulation < sample_input.txt into the terminal. 
	sample_input.txt is the input passed into the simulation.

	Input file:
		The first line designates the number of IO calls to be made. 
		Afterwards, each line has these three components respectively: 
		*op name: IO call which is either ”read” or ”write”
		*sector number: argument to the above call
		*arrival time: describes when this request is to be issued by the device 
			driver thread (real time) as an offset from the start time of the program.


=======================================================================================
AUTHORS

	Jonathon Dahl
	Georges Junior Naddaf

---------------------------------------------------------------------------------------
Supporting Authors: 

	Bhuvan Urgaonkar
	Ata Fatahi Baarzi
	Aman Jain
	Diman Tootaghaj

=======================================================================================
BUGS

	* 12/10/17 * Some negative finishing and servicing times were observed. A
		hot fix should address these issues in newer releases.

#######################################################################################

