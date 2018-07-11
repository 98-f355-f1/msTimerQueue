#include <stdio.h>
#include <windows.h>

HANDLE gDoneEvent;

VOID CALLBACK TimerRoutine(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
	if (lpParam == NULL)
	{
		printf("TimerRoutine lpParam is NULL\n");
	}
	else
	{
		printf("TimerRoutine called. Parameter is %d\n", *(int*)lpParam);
		if (TimerOrWaitFired)
		{
			printf("Timer timed out\n");
		}
		else
		{
			printf("The event was signaled\n");
		}
	}

	SetEvent(gDoneEvent);
}

int main(void)
{
	HANDLE hTimer = NULL;
	HANDLE hTimerQueue = NULL;
	int arg = 123;

	// Use an event object to track the TimerRoutine execution
	gDoneEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (gDoneEvent == NULL)
	{
		printf("CreateEvent failed (%d)\n", GetLastError());
		return 1;
	}

	// Create timer queue
	hTimerQueue = CreateTimerQueue();
	if (hTimerQueue == NULL)
	{
		printf("CreateTimerQueue failed (%d)\n", GetLastError());
		return 2;
	}

	// Set a timer to call the timer routine in 10 seconds.
	if (!CreateTimerQueueTimer(&hTimer, hTimerQueue, (WAITORTIMERCALLBACK)TimerRoutine, &arg, 10000, 0, 0))
	{
		printf("CreateTimerQueueTimer failed (%d)\n", GetLastError());
		return 3;
	}

	// TODO: Do other useful work here 
	printf("Call timer routine in 10 seconds...\n");

	/* Wait for the timer-queue thread to complete using an event
	object. The thread will signal the event at that time. */
	if (WaitForSingleObject(gDoneEvent, INFINITE) != WAIT_OBJECT_0)
		printf("WaitForSingleObject failed (%d)\n", GetLastError());

	CloseHandle(gDoneEvent);
	getchar();
	return(EXIT_SUCCESS);
}
