//Thomas Zaorski
//RIN: 660747712
//EMAIL: zaorst@rpi.edu

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <string>
#include <fstream>
#include <cstring>
#include <algorithm>
using namespace std;

//Declares various important variables. Vector of memory serves as main memory
vector <char> memory;
int timer = 0;
int used_mem = 0;
int last_spot = 80;
int time_stop;

//Creates a class called process which contains the process' char representation, size, and arrival and exit times
class process 
{
public:
	vector <int> arrival_times;
	vector <int> exit_times;
	char letter;
        int size;	
};

//vector containing the processes
vector <process> processes;

//Prototypes the various functions to be used
void display();
int defrag();
void free_memory(process p);
bool first_fit(process p);
bool next_fit(process p);
bool best_fit(process p);
bool worst_fit(process p);
bool noncontig(process p);
bool arrived(process p);
bool exited(process p);

int main(int argc, char *argv[])
{

//Prints correct usage and exits if not enough command line arguments are supplied
if (argc < 3)
{
      cout << "USAGE: " << argv[0] << " <input-file> { noncontig | first | best | next | worst }" << endl;
      exit(1);
}

//Initiliazes all 2400 spots of main memory to '.'
for (unsigned int i= 0; i < 2400; i++)
	memory.push_back('.');

string str;
bool placed;
string alg = argv[2];
int size_opened = 0;


//Opens provided file
ifstream infile;
infile.open(argv[1]);
if (!infile) 
{
cout << "Unable to open file" << endl;
exit(1); // terminate with error if file can not be opened
}

//Deals with  the odd first line of the txt file
getline(infile, str);
//num_processes = atoi(str.c_str());


//Handles parsing the file and storing information
while (!infile.eof())
{
	vector <string> input;
	getline(infile, str);
	char* pch;
	char * dup = strdup(str.c_str());
	if (!infile.eof())
	{
	pch = strtok(dup, " ");
	while (pch != NULL) 
	{	
		string current = pch;
		input.push_back(current);
		pch = strtok(NULL, ", ");
	}
	process temp;
        for (unsigned int i =0; i < input.size(); i++)
	{
		if (i == 0)
 		   temp.letter = input[i][0];
		else if (i == 1)
		   temp.size = atoi(input[i].c_str());
		else
		{
		   if (i == 2)
		   	temp.arrival_times.push_back(atoi(input[i].c_str()));
		   else if ((i %2) == 0)
			temp.arrival_times.push_back(atoi(input[i].c_str()));
		   else
			temp.exit_times.push_back(atoi(input[i].c_str()));
		}
	}
	processes.push_back(temp);
	}

}


//Loads OS into main memory
for (unsigned int i = 0; i < 80; i++)
{
	memory[i] = '#';
	used_mem++;
}


//Loops until user enters 0. Runs the simulation
while(1)
{

//Checks if any processes are ready to exit, frees their memory if they are
for (unsigned int i = 0; i < processes.size(); i++)
{
	if (exited(processes[i]))
           free_memory(processes[i]);
}

//Checks if any process is ready to enter memory, enters based on provided algorithm 
for (unsigned int i = 0; i < processes.size(); i++)
{
	if (arrived(processes[i]))
	{
		if (alg == "first")
		{
		placed = first_fit(processes[i]);
		if (!placed) //If process couldn't be placed in memory
		{
			size_opened = defrag(); //Defrags
			if (size_opened < processes[i].size) //If defrag didnt free up enough memory, print out of memory
			{
				cout << "OUT-OF-MEMORY" << endl;
				exit(1);
			}
			else //Enter into memory
			{
				placed = first_fit(processes[i]);
				if (!placed) //If it somehow still fails to enter into memory, exit with an error
				{
					cout << "ERROR!" << endl;
					exit(1);
				}
			}
		}
		}
		else if (alg == "next")
		{
		placed = next_fit(processes[i]);
		if (!placed)
		{
			size_opened = defrag();
			if (size_opened < processes[i].size)
			{
				cout << "OUT-OF-MEMORY" << endl;
				exit(1);
			}
			else
			{
				placed = next_fit(processes[i]);
				if (!placed)
				{
					cout << "ERROR!" << endl;
					exit(1);
				}
			}
		}
		}
		else if (alg == "best")
		{
		placed = best_fit(processes[i]);
		if (!placed)
		{
			size_opened = defrag();
			if (size_opened < processes[i].size)
			{
				cout << "OUT-OF-MEMORY" << endl;
				exit(1);
			}
			else
			{
				placed = best_fit(processes[i]);
				if (!placed)
				{
					cout << "ERROR!" << endl;
					exit(1);
				}
			}
		}
		}
		else if (alg == "worst")
		{
		placed = worst_fit(processes[i]);
		if (!placed)
		{
			size_opened = defrag();
			if (size_opened < processes[i].size)
			{
				cout << "OUT-OF-MEMORY" << endl;
				exit(1);
			}
			else
			{
				placed = worst_fit(processes[i]);
				if (!placed)
				{
					cout << "ERROR!" << endl;
					exit(1);
				}
			}
		}
		}
		else if (alg == "noncontig")
		{
		placed = noncontig(processes[i]);
		if (!placed)
		{
			cout << "OUT-OF-MEMORY" << endl;
			exit(1);
		}
		}
		else //Prints invalid algorithm if you enter an algorithm wrong
		{
			cout << "Invalid algorithm requested" << endl;
			cout << "USAGE: " << argv[0] << " <input-file> { noncontig | first | best | next | worst }" << endl;
			exit(1);
		}
		
	}
}

if (timer == 0) //Special case for time zero
{
	display();
	cout << "Enter time to stop: ";
	cin >> time_stop;
	if (time_stop == 0)
	{
		break;
	}
	cout << endl;
}

if (timer >= time_stop) //Checks if simulation arrived at (or passed) user provided time
{
	display();
	cout << "Enter time to stop: ";
	cin >> time_stop;
	if (time_stop == 0)
	{
		break;
	}
	cout << endl;
}

//Increments timer
timer++;

}

cout << endl;
return 0;
}

//Displays the memory
void display()
{
int counter = 0;
cout << "Memory at time " << timer << ":" << endl;
for (unsigned int i = 0; i < memory.size(); i++)
{
	//int counter = 0;
	if (counter < 80)
	{
	   cout << memory[i] << " ";
	   counter++;
        }
	else 
	{
	   cout << "\n";
	   counter = 0;
	   i--;
	}
}
cout << endl;

}

//Checks if the process has any arrival times at the current timer
bool arrived(process p)
{
	for (unsigned int i = 0; i < p.arrival_times.size(); i++)
	{
		if (p.arrival_times[i] == timer)
		   return true;
	}

return false;
}

//Checks if the process has any exit times at the current timer
bool exited(process p)
{
	for (unsigned int i = 0; i < p.exit_times.size(); i++)
	{
		if (p.exit_times[i] == timer)
		   return true;
	}

return false;
}


//First fit
bool first_fit(process p)
{
	int first_spot = 0; 
	int size_slot = 0;
	bool found_empty = false;
	bool found_slot = false;
	for (unsigned int i = 0; i < memory.size(); i++)
	{
		if (memory[i] == '.' && !found_empty)
		{
			found_empty = true;
			first_spot = i;
			size_slot++;
		}
		else if (memory[i] == '.' && found_empty)
		{
			size_slot++;
		}
		else if (memory[i] != '.' && found_empty)
		{
			found_empty = false;
			size_slot = 0;
		}

		if (size_slot == p.size)
		{
		   found_slot = true;
		   for (int i = 0; i < p.size; i++)
		   {
			memory[first_spot + i] = p.letter;
			used_mem++;
	           }

		   break;
		}
	}

	
return found_slot;
}


//Next fit
bool next_fit(process p)
{
	int first_spot = 0; 
	int size_slot = 0;
	bool found_empty = false;
	bool found_slot = false;
	for (unsigned int i = 0; i < memory.size(); i++)
	{
		if (memory[last_spot] == '.' && !found_empty)
		{
			found_empty = true;
			first_spot = last_spot;
			size_slot++;
		}
		else if (memory[last_spot] == '.' && found_empty)
		{
			size_slot++;
		}
		else if (memory[last_spot] != '.' && found_empty)
		{
			found_empty = false;
			size_slot = 0;
		}

		if (size_slot == p.size)
		{
		   found_slot = true;
		   for (int i = 0; i < p.size; i++)
		   {
			memory[first_spot + i] = p.letter;
			used_mem++;
	           }

		   
		}

		if (last_spot == 2399)
		{
			last_spot = 0;
			size_slot = 0;
			found_empty = false;
			if (found_slot)
				break;
		}
		else
		{	
			if (found_slot)
				break;
			last_spot++;			
		}

	}

	
return found_slot;
}


//Best fit
bool best_fit(process p)
{
	int first_spot = 0; 
	int first_spot_best = 0;
	int size_slot = 0;
	int last_size = 2401;
	bool found_empty = false;
	bool found_slot = false;
	for (unsigned int i = 0; i < memory.size(); i++)
	{
		if (memory[i] == '.' && !found_empty)
		{
			found_empty = true;
			first_spot = i;
			size_slot++;
		}
		else if (memory[i] == '.' && found_empty)
		{
			size_slot++;
			if (i == 2399)
			{
			if (size_slot >= p.size)
			{
				found_slot = true;
				if (size_slot < last_size)
				{
					last_size = size_slot;
					first_spot_best = first_spot;
				}
			}
			}
		}
		else if (memory[i] != '.' && found_empty)
		{
			found_empty = false;
			if (size_slot >= p.size)
			{
				found_slot = true;
				if (size_slot < last_size)
				{
					last_size = size_slot;
					first_spot_best = first_spot;
				}
			}
			size_slot = 0;
		}

		
	}

	if (found_slot)
	{
		   for (int i = 0; i < p.size; i++)
		   {
			memory[first_spot_best + i] = p.letter;
			used_mem++;
	           }

	}

return found_slot;
}


//Worst fit
bool worst_fit(process p)
{
	int first_spot = 0; 
	int first_spot_best = 0;
	int size_slot = 0;
	int last_size = 0;
	bool found_empty = false;
	bool found_slot = false;
	for (unsigned int i = 0; i < memory.size(); i++)
	{
		if (memory[i] == '.' && !found_empty)
		{
			found_empty = true;
			first_spot = i;
			size_slot++;
		}
		else if (memory[i] == '.' && found_empty)
		{
			size_slot++;
			if (i == 2399)
			{
			if (size_slot >= p.size)
			{
				found_slot = true;
				if (size_slot > last_size)
				{
					last_size = size_slot;
					first_spot_best = first_spot;
				}
			}
			}
		}
		else if (memory[i] != '.' && found_empty)
		{
			found_empty = false;
			if (size_slot >= p.size)
			{
				found_slot = true;
				if (size_slot > last_size)
				{
					last_size = size_slot;
					first_spot_best = first_spot;
				}
			}
			size_slot = 0;
		}

		
	}

	if (found_slot)
	{
		   for (int i = 0; i < p.size; i++)
		   {
			memory[first_spot_best + i] = p.letter;
			used_mem++;
	           }

	}

return found_slot;
}


//Noncontig
bool noncontig(process p)
{
	int mem_allocated = 0;
	if (2400 - used_mem < p.size)
	{
		return false;
	}
	else
	{
		for (unsigned int i = 0; i < memory.size(); i++)
		{
			if (memory[i] == '.')
			{
				memory[i] = p.letter;
				mem_allocated++;
				used_mem++;
			}

			if (mem_allocated == p.size)
				break;

		}
	}

return true;
}


//Defragger, returns size of block free'ed up
int defrag()
{
vector <char> processes_moved;
bool found_empty = false;
bool seen_before = false;
int index = 0;
int size_gap = 0;

cout << "OUT-OF-MEMORY" << endl;
cout << "Performing defragmentation..." << endl;

for (unsigned int i = 0; i < memory.size(); i++)
{
	if (memory[i] != '.' && !found_empty)
	{
		memory[index] = memory[i];
		index++;
	}
	else if (memory[i] == '.' && !found_empty)
	{
		memory[index] = memory[i];
		size_gap++;
		found_empty = true;
	}
	else if (memory[i] != '.' && found_empty)
	{
		//Check if seen before
		for (unsigned int j = 0; j < processes_moved.size(); j++)
		{
			if (processes_moved[j] == memory[i])
				seen_before = true;
		}
		if (!seen_before)
		{
			processes_moved.push_back(memory[i]);
		}
		seen_before = false;
		memory[index] = memory[i];
		memory[i] = '.';
		index++;
		//size_gap++;
	}
	else if (memory[i] == '.' && found_empty)
	{
		size_gap++;
	}
}

cout << "Defragmentation completed." << endl;

double percent = 0.0;
percent = ((double)size_gap / 2400.0) * 100.0;

//Print info
cout << "Relocated " << processes_moved.size() << " processes to create a free memory block of " << size_gap << " units ("; 
//<< percent << "\% of total memory)." << endl << endl;
//Display memory
printf("%.2f", percent);
cout << "\% of total memory)." << endl << endl;
display();
cout << endl;

return size_gap;
}


//Frees up memory based on the process letter
void free_memory(process p)
{
	for (unsigned int i = 0; i < memory.size(); i++)
	{
		if (memory[i] == p.letter)
		{
			memory[i] = '.';
			used_mem--;
		}
	}
}
