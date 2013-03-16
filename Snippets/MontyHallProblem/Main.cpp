#include <stdio.h>
#include <Windows.h>
#include <time.h>

bool *DoorValues = 0;
bool *RevealedDoors = 0;
int PickedDoor = 0;
int NumDoors = 10;

void InitDoors()
{
	if (DoorValues)
	{
		free(DoorValues);
	}

	if (RevealedDoors)
	{
		free(RevealedDoors);
	}

	DoorValues = new bool[NumDoors];
	memset(DoorValues, 0, sizeof(DoorValues));
	RevealedDoors = new bool[NumDoors];
	memset(RevealedDoors, 0, sizeof(RevealedDoors));

	bool HaveWinningDoor = false;

	for (int i = 0; i < NumDoors; i++)
	{
		DoorValues[i] = rand() > 15000 && !HaveWinningDoor ? true : false;
		if (DoorValues[i] == true)
		{
			HaveWinningDoor = true;
		}
	}

	if (!HaveWinningDoor)
	{
		DoorValues[rand() % (NumDoors - 1)] = true;
	}

	for (int i = 0; i < NumDoors; i++)
	{
		printf("D%d:%d|", i, DoorValues[i]);
	}
}

void PickDoor()
{
	PickedDoor = rand() % (NumDoors - 1);
	printf("Picked:%d|", PickedDoor);
}

void RevealBadDoor()
{
	int RevealedDoorCount = 0;

	while (RevealedDoorCount < (NumDoors - 2))
	{
		int idx = rand() % (NumDoors - 1);
		if (DoorValues[idx] == false && idx != PickedDoor)
		{
			RevealedDoors[idx] = true;
			RevealedDoorCount++;
		}
	}

	printf("Revealed all but 2|");
}

void SwitchDoors()
{
	for (int i = 0; i < NumDoors; i++)
	{
		if (RevealedDoors[i] == false && i != PickedDoor)
		{
			PickedDoor = i;
			break;
		}
	}
	printf("Switched:%d|", PickedDoor);
}

int CheckResult()
{
	return (DoorValues[PickedDoor] == 1) ? 1 : 0;
}

int DoIteration()
{
	InitDoors();
	PickDoor();
	RevealBadDoor();
	SwitchDoors();
	return CheckResult();
}

int main()
{
	srand(time(0));

	int GoodSwitches = 0;

	for (int i = 0; i < 100; i++)
	{
		if (DoIteration() == 1)
		{
			printf("Benefitial\n");
			GoodSwitches++;
		}
		else
		{
			printf("Lossy\n");
		}
	}

	printf("Switching doors resulted in a winning outcome %d / %d times.\n", GoodSwitches, 100);

	system("PAUSE");
}