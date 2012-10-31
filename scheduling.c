/*******************************************************************************
*
* CprE 308 Scheduling Lab
*
* scheduling.c
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_PROCESSES 20

struct process
{
  /* Values initialized for each process */
  int arrivaltime;  /* Time process arrives and wishes to start */
  int runtime;      /* Time process requires to complete job */
  int priority;     /* Priority of the process */

  /* Values algorithm may use to track processes */
  int starttime;
  int endtime;
  int flag;
  int remainingtime;
};

/* Forward declarations of Scheduling algorithms */
void first_come_first_served(struct process *proc);
void shortest_remaining_time(struct process *proc);
void round_robin(struct process *proc);
void round_robin_priority(struct process *proc);
void printAverageTime(struct process *proc);

int main()
{
  int i;
  struct process proc[NUM_PROCESSES],      /* List of processes */
                 proc_copy[NUM_PROCESSES]; /* Backup copy of processes */

  /* Seed random number generator */
  /*srand(time(0));*/  /* Use this seed to test different scenarios */
  srand(0xC0FFEE);     /* Used for test to be printed out */

  /* Initialize process structures */
  for(i=0; i<NUM_PROCESSES; i++)
  {
    proc[i].arrivaltime = rand()%100;
    proc[i].runtime = (rand()%30)+10;
    proc[i].priority = rand()%3;
    proc[i].starttime = 0;
    proc[i].endtime = 0;
    proc[i].flag = 0;
    proc[i].remainingtime = 0;
  }

  /* Show process values */
  printf("Process\tarrival\truntime\tpriority\n");
  for(i=0; i<NUM_PROCESSES; i++)
    printf("%d\t%d\t%d\t%d\n", i, proc[i].arrivaltime, proc[i].runtime,
           proc[i].priority);

  /* Run scheduling algorithms */
  printf("\n\nFirst come first served\n");
  memcpy(proc_copy, proc, NUM_PROCESSES * sizeof(struct process));
  first_come_first_served(proc_copy);

  printf("\n\nShortest remaining time\n");
  memcpy(proc_copy, proc, NUM_PROCESSES * sizeof(struct process));
  shortest_remaining_time(proc_copy);

  printf("\n\nRound Robin\n");
  memcpy(proc_copy, proc, NUM_PROCESSES * sizeof(struct process));
  round_robin(proc_copy);

  printf("\n\nRound Robin with priority\n");
  memcpy(proc_copy, proc, NUM_PROCESSES * sizeof(struct process));
  round_robin_priority(proc_copy);

  return 0;
}

void first_come_first_served(struct process *proc)
{
	int time = 0;
	int procsFinished = 0;
	while(procsFinished != NUM_PROCESSES)
	{
		struct process * currentProc = &(proc[0]);
		int currentProcIndex = 0;
		int i;
		for(i = 0; i < NUM_PROCESSES; i++)
		{
			if(proc[i].arrivaltime < currentProc->arrivaltime)
		{
				if(proc[i].flag == 0)
				{
					currentProc = &(proc[i]);
					currentProcIndex = i;
				}
			}
			else
			{
				if(currentProc->flag == 1)
				{
					currentProc = &(proc[i]);
					currentProcIndex = i;
				}
			}
		}
		
		if(currentProc->arrivaltime <= time)
		{
			printf("Process %i started at time %i\n", currentProcIndex, time);
			currentProc->starttime = time;
			currentProc->endtime = time + currentProc->runtime;
			currentProc->flag = 1;
			currentProc->remainingtime = 0;
			time += currentProc->runtime;
			printf("Process %i finished at time %i\n", currentProcIndex, time);
			procsFinished++;
		}
		else
		{
			time++;
		}
	}

	printAverageTime(proc);

}

void shortest_remaining_time(struct process *proc)
{
	int time = 0;
	int procsFinished = 0;

	int i;
	for(i = 0; i < NUM_PROCESSES; i++)
	{
		proc[i].remainingtime = proc[i].runtime;
	}


	while(procsFinished != NUM_PROCESSES)
	{
		int currProcIndex = 0;
		for(i = 0; i < NUM_PROCESSES; i++)
		{
			if(proc[i].remainingtime != 0)
			{
				if(proc[i].remainingtime < proc[currProcIndex].remainingtime && proc[i].arrivaltime <= time)
				{
					currProcIndex = i;
				}
				else if(proc[currProcIndex].remainingtime == 0)
				{
					currProcIndex = i;
				}
			}
		}
		if(proc[currProcIndex].arrivaltime <= time)
		{
			if(proc[currProcIndex].flag == 0)
			{
				proc[currProcIndex].flag = 1;
				printf("Process %i started at time %i\n", currProcIndex, time);
				proc[currProcIndex].starttime = time;
				proc[currProcIndex].remainingtime--;
				time++;
			}
			else
			{
				proc[currProcIndex].remainingtime--;
				time++;
			}

			if(proc[currProcIndex].remainingtime == 0)
			{
				printf("Process %i finished at time %i\n", currProcIndex, time);
				proc[currProcIndex].endtime = time;
				procsFinished++;
			}
		}
		else
		{
			time++;
		}

	}

	printAverageTime(proc);
}

void round_robin(struct process *proc)
{
	int time = 0;
	int procsFinished = 0;
	
	int i;
	for(i = 0; i < NUM_PROCESSES; i++)
	{
		proc[i].remainingtime = proc[i].runtime;
	}
	
	while(procsFinished != NUM_PROCESSES)
	{
		int i;
		char timeIncremented = 0;
		for(i = 0; i < NUM_PROCESSES; i++)
		{
			if(proc[i].arrivaltime <= time && proc[i].remainingtime != 0)
			{
				if(proc[i].remainingtime == proc[i].runtime)
				{
					printf("Process %i started at time %i\n", i, time);
					proc[i].starttime = time;
					time++;
					timeIncremented = 1;
					proc[i].remainingtime--;
				}
				else if(proc[i].remainingtime == 1)
				{
					time++;
					timeIncremented = 1;
					proc[i].remainingtime--;
					printf("Process %i finished at time %i\n", i, time);
					proc[i].endtime = time;
					procsFinished++;
				}
				else
				{
					proc[i].remainingtime--;
					time++;
					timeIncremented = 1;
				}
				
			}
		}
		if(!timeIncremented)
		{
			time++;
		}
	
	}
	printAverageTime(proc);
}

void round_robin_priority(struct process *proc)
{
	int time = 0;
	int procsFinished = 0;
	
	int i;
	for(i = 0; i < NUM_PROCESSES; i++)
	{
		proc[i].remainingtime = proc[i].runtime;
	}
	
	while(procsFinished != NUM_PROCESSES)
	{
		int i;
		char timeIncremented = 0;
		for(i = 0; i < NUM_PROCESSES; i++)//round robin loop for priority 2 processes
		{
			if(proc[i].arrivaltime <= time && proc[i].remainingtime != 0 && proc[i].priority == 2)
			{
				if(proc[i].remainingtime == proc[i].runtime)
				{
					printf("Process %i started at time %i\n", i, time);
					proc[i].starttime = time;
					time++;
					timeIncremented = 1;
					proc[i].remainingtime--;
				}
				else if(proc[i].remainingtime == 1)
				{
					time++;
					timeIncremented = 1;
					proc[i].remainingtime--;
					printf("Process %i finished at time %i\n", i, time);
					proc[i].endtime = time;
					procsFinished++;
				}
				else
				{
					proc[i].remainingtime--;
					time++;
					timeIncremented = 1;
				}
				
			}
		}
		if(!timeIncremented)//only check for priority 1 processes if there were no priority 2's executed
		{
			for(i = 0; i < NUM_PROCESSES; i++)//round robin check for priority 1 processes
			{
				if(proc[i].arrivaltime <= time && proc[i].remainingtime != 0 && proc[i].priority == 1)
				{
					if(proc[i].remainingtime == proc[i].runtime)
					{
						printf("Process %i started at time %i\n", i, time);
						proc[i].starttime = time;
						time++;
						timeIncremented = 1;
						proc[i].remainingtime--;
					}
					else if(proc[i].remainingtime == 1)
					{
						time++;
						timeIncremented = 1;
						proc[i].remainingtime--;
						printf("Process %i finished at time %i\n", i, time);
						proc[i].endtime = time;
						procsFinished++;
					}
					else
					{
						proc[i].remainingtime--;
						time++;
						timeIncremented = 1;
					}
					int j;
					char higherProcessReady = 0;
					for(j = 0; j < NUM_PROCESSES; j++)//check if a priority 2 process has become runnable
					{
						if(proc[j].priority > 1 && proc[j].remainingtime != 0 && proc[j].arrivaltime <= time)
						{
							higherProcessReady = 1;
							break;//no need to continue looping after finding a ready process of higher priority
						}
					}
					if(higherProcessReady)//if a priority 2 is ready then break out of the priority 1 round robin
					{
						break;
					}
				}
			}
		}
		if(!timeIncremented)//only check for priority 0 processes if there were no priority 1's executed
		{
			for(i = 0; i < NUM_PROCESSES; i++)//round robin check for priority 0 processes
			{
				if(proc[i].arrivaltime <= time && proc[i].remainingtime != 0 && proc[i].priority == 0)
				{
					if(proc[i].remainingtime == proc[i].runtime)
					{
						printf("Process %i started at time %i\n", i, time);
						proc[i].starttime = time;
						time++;
						timeIncremented = 1;
						proc[i].remainingtime--;
					}
					else if(proc[i].remainingtime == 1)
					{
						time++;
						timeIncremented = 1;
						proc[i].remainingtime--;
						printf("Process %i finished at time %i\n", i, time);
						proc[i].endtime = time;
						procsFinished++;
					}
					else
					{
						proc[i].remainingtime--;
						time++;
						timeIncremented = 1;
					}
					int j;
					char higherProcessReady = 0;
					for(j = 0; j < NUM_PROCESSES; j++)//check if a priority 1 or 2 process has become runnable
					{
						if(proc[j].priority > 0 && proc[j].remainingtime != 0 && proc[j].arrivaltime <= time)
						{
							higherProcessReady = 1;
							break;//no need to continue looping after finding a ready process of higher priority
						}
					}
					if(higherProcessReady)//if a priority 1 or 2 is ready then break out of the priority 0 round robin
					{
						break;
					}
				}
			}
		}
		if(!timeIncremented)//if none of the processes were ready to run then increment the time, essential a no operation (nop)
		{
			time++;
		}
	}

	printAverageTime(proc);
}

void printAverageTime(struct process *proc)
{
	float averageTime = 0;
	int i;
	for(i = 0; i < NUM_PROCESSES; i++)
	{
		averageTime += proc[i].endtime - proc[i].arrivaltime;
	}

	averageTime /= (float)NUM_PROCESSES;
		printf("Average time from arrival to finish is %4.2f seconds\n", averageTime);

}
