// programmed by ali tabaraei

#include <iostream>
#include <vector>

using namespace std;

struct process {
	int id,
		arrival,
		burst,
		io_start,
		io_end,
		waiting = 0,
		turnaround,
		start,
		end,
		firstArrival;
};

// process information & ready queue
vector<process> input, ready, showGaunt, showInfo;

// check if ready queue contains a progress
bool contain(int id)
{
	for (int i = 0; i < ready.size(); i++)
		if (ready[i].id == id)
			return true;
	return false;
}

void sortProcesses(int globalTime)
{
	// for any remaining process in input
	for (int i = 0; i < input.size(); i++)
	{
		// if it is ready to go inside queue
		if (!contain(input[i].id) && input[i].arrival <= globalTime)
		{
			// if there is no process in ready queue, insert process at first
			// else, find the right place by "shortest job first"
			int position = 0;
			while (position < ready.size() && input[i].burst >= ready[position].burst) { position++; }
			ready.insert(ready.begin() + position, input[i]);
		}
	}
}

int main()
{
	system("color 3b");

	// get process information
	int n;
	cout << "Enter number of processes: ";
	cin >> n;

	system("cls");
	// get context switch value
	int CS;
	cout << "Enter value of context switch: ";
	cin >> CS;

	system("cls");
	cout << "Enter 'arrival time' - 'burst time' - 'i/o interrupt start' -  'i/o interrupt end'" << endl;
	for (int i = 0; i < n; i++)
	{
		process p;
		p.id = i;
		cout << "P" << i << ":" << endl;
		cin >> p.arrival >> p.burst >> p.io_start >> p.io_end;
		input.push_back(p);
	}

	// set showInfo default values
	showInfo = input;
	for (int i = 0; i < n; i++)
	{
		showInfo[i].firstArrival = input[i].arrival;
	}

	// scheduling shortest job first
	int globalTime = 0;

	while (input.size() > 0)
	{
		// getting ready queue
		sortProcesses(globalTime);

		// if there are no processes in ready queue
		if (ready.size() == 0) globalTime++;
		// else
		else
		{
			// getting start time of the process
			ready[0].start = globalTime;

			// calculating waiting time
			ready[0].waiting += globalTime - ready[0].arrival;

			// if it has no io anymore
			if (ready[0].io_start == -1)
			{
				// procces starts till the end
				globalTime += ready[0].burst;
				ready[0].burst = 0;

				// calculating turnaround time
				ready[0].turnaround = globalTime - showInfo[ready[0].id].firstArrival;
			}
			// if it has io
			else
			{
				// process starts up to io interrupt
				globalTime += ready[0].io_start;
				// after interrupt, it stops and updates arrival and burst for next time
				ready[0].burst -= ready[0].io_start;
				ready[0].arrival = globalTime + (ready[0].io_end - ready[0].io_start);
				// no longer it has io
				ready[0].io_start = ready[0].io_end = -1;
				// save first arrival time for turnaround time
				ready[0].firstArrival = showInfo[ready[0].id].firstArrival;
			}

			// getting end time of the process
			ready[0].end = globalTime;

			// update input processes
			input[ready[0].id] = ready[0];
			showInfo[ready[0].id] = ready[0];

			// save information
			showGaunt.push_back(ready[0]);

			// if process has ended, should delete it from input list
			if (ready[0].burst == 0) input.erase(input.begin() + ready[0].id);

			// delete process from ready queue
			ready.erase(ready.begin());

			// calculate context swtich
			globalTime += CS;
		}

	}

	system("cls");
	// print cpu gaunt
	cout << "CPU Gaunt: [start] process [end]" << endl;
	for (int i = 0; i < showGaunt.size(); i++)
	{
		cout << "[" << showGaunt[i].start << "] P" << showGaunt[i].id << " [" << showGaunt[i].end << "]" << endl;
	}

	// waiting time and turnaround time for each process

	float avgWait = 0, avgTurn = 0;
	cout << endl << endl << endl << "Waiting time & Turnaround time details for each process" << endl;
	for (int i = 0; i < n; i++)
	{
		cout << "P" << showInfo[i].id << ":" << endl;
		cout << "Waiting time = " << showInfo[i].waiting << "\t Turnaround time = " << showInfo[i].turnaround << endl << endl;

		// calculate avg time
		avgWait += showInfo[i].waiting;
		avgTurn += showInfo[i].turnaround;
	}

	// average details
	cout << endl << endl;
	cout << "Average waiting time = " << avgWait / n << endl;
	cout << "Average Turnaround time = " << avgTurn / n << endl;

	system("pause");
	return 0;
}