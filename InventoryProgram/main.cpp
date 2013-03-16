#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>

struct inventorydata
{
	inventorydata()
	{
		description = new char[32];
		dateAdded = new char[10];
		encoded = new char[100];
		memset(description, 0, sizeof(description));
		memset(dateAdded, 0, sizeof(dateAdded));
		memset(encoded, 0, sizeof(encoded));
	}
	~inventorydata()
	{
		delete description;
		delete dateAdded;
		delete encoded;
	}

	char *description;
	int quantity;
	float wholesaleCost;
	float retailCost;
	char *dateAdded;

	char *encoded; // Don't use this directly, call encode(). This is just here to prevent memory leaks.

	char *encode()
	{
		char tmp[20];
		strcpy(encoded, description);
		strcat(encoded, "|");
		sprintf(tmp, "%d", quantity);
		strcat(encoded, tmp);
		strcat(encoded, "|");
		sprintf(tmp, "%.2f", wholesaleCost);
		strcat(encoded, tmp);
		strcat(encoded, "|");
		sprintf(tmp, "%.2f", retailCost);
		strcat(encoded, tmp);
		strcat(encoded, "|");
		strcat(encoded, dateAdded);
		return encoded;
	}

	void decode(char *str)
	{
		unsigned int inIdx = 0;
		unsigned int tmpIdx = 0;
		char tmp[100];

		// Read description
		tmpIdx = 0;
		memset(tmp, 0, sizeof(tmp));
		while (str[inIdx] != '|')
		{
			tmp[tmpIdx] = str[inIdx];
			tmpIdx++;
			inIdx++;
		}
		strcpy(description, tmp);
		inIdx++;

		// Read quantity
		tmpIdx = 0;
		memset(tmp, 0, sizeof(tmp));
		while (str[inIdx] != '|')
		{
			tmp[tmpIdx] = str[inIdx];
			tmpIdx++;
			inIdx++;
		}
		quantity = atoi(tmp);
		inIdx++;

		// Read wholesale cost
		tmpIdx = 0;
		memset(tmp, 0, sizeof(tmp));
		while (str[inIdx] != '|')
		{
			tmp[tmpIdx] = str[inIdx];
			tmpIdx++;
			inIdx++;
		}
		wholesaleCost = (float)atof(tmp);
		inIdx++;

		// Read retail cost
		tmpIdx = 0;
		memset(tmp, 0, sizeof(tmp));
		while (str[inIdx] != '|')
		{
			tmp[tmpIdx] = str[inIdx];
			tmpIdx++;
			inIdx++;
		}
		retailCost = (float)atof(tmp);
		inIdx++;

		// Read date added
		tmpIdx = 0;
		memset(tmp, 0, sizeof(tmp));
		while (inIdx < strlen(str))
		{
			tmp[tmpIdx] = str[inIdx];
			tmpIdx++;
			inIdx++;
		}
		strcpy(dateAdded, tmp);
	}
};

inventorydata **items = 0;
unsigned int numItems = 0;

void loadInventory()
{
	// If reloading the inventory, clear the previous data to prevent memory leaks.
	if (items != 0)
	{
		for (unsigned int i = 0; i < numItems; i++)
		{
			delete items[i];
		}
		delete items;
	}

	// Check how many items there are.
	FILE *f = fopen("inventory.txt", "r");
	while (!feof(f))
	{
		char tmp;
		fread(&tmp, 1, 1, f);
		if (tmp == '\n')
		{
			numItems++;
		}
	}
	numItems--; // There should always be a blank line at the end of the file.
	fclose(f);

	// Load each item.
	items = new inventorydata *[numItems];
	f = fopen("inventory.txt", "r");
	for (unsigned int i = 0; i < numItems; i++)
	{
		unsigned int curIdx = 0;
		char encodedEntry[100];
		memset(encodedEntry, 0, sizeof(encodedEntry));
		char tmp = 0;
		while (tmp != '\n' && !feof(f))
		{
			fread(&tmp, 1, 1, f);
			if (tmp != '\r' && tmp != '\n')
			{
				encodedEntry[curIdx] = tmp;
				curIdx++;
			}
		}
		items[i] = new inventorydata;
		items[i]->decode(encodedEntry);
	}
	fclose(f);
}

void saveInventory()
{
	FILE *f = fopen("inventory.txt.", "w");
	for (unsigned int i = 0; i < numItems; i++)
	{
		char *encoded = items[i]->encode();
		fwrite(encoded, strlen(encoded), 1, f);
		fwrite("\n", 1, 1, f);
	}
	fclose(f);
}

int main()
{
	printf("- Inventory Program -\n");
	printf("What do you want to do?\n");
	printf("1. Add a record to the file.\n");
	printf("2. Display all records in the file.\n");
	printf("3. Modify a record.\n");

	char menuOption[2];
	scanf("%s", menuOption);

	if (menuOption[0] == '1')
	{
		inventorydata i;
		printf("Enter an item description: ");
		scanf("%s", i.description);
		printf("Enter the quantity on hand: ");
		scanf("%d", &i.quantity);
		printf("Enter the wholesale cost: ");
		scanf("%f", &i.wholesaleCost);
		printf("Enter the retail cost: ");
		scanf("%f", &i.retailCost);
		printf("Enter the date it was added to the inventory (DD/MM/YYYY): ");
		scanf("%s", i.dateAdded);

		FILE *f = fopen("inventory.txt", "a+");
		char *encoded = i.encode();
		fwrite(encoded, strlen(encoded), 1, f);
		fwrite("\n", 1, 1, f);
		fclose(f);
	}
	else if (menuOption[0] == '2')
	{
		loadInventory();
		for (unsigned int i = 0; i < numItems; i++)
		{
			printf("%d. %32s\t\t%d in stock\n", i+1, items[i]->description, items[i]->quantity);
		}
	}
	else if (menuOption[0] == '3')
	{
		loadInventory();
		printf("Enter the index of the record you want to modify: ");
		int idx = 0;
		scanf("%d", &idx);
		idx--;

		printf("-- RECORD INFO --\n");
		printf("Index: %d\n", idx+1);
		printf("(1) Description: %s\n", items[idx]->description);
		printf("(2) Quantity: %d\n", items[idx]->quantity);
		printf("(3) Wholesale Cost: %.2f\n", items[idx]->wholesaleCost);
		printf("(4) Retail Cost: %.2f\n", items[idx]->retailCost);
		printf("(5) Date Added: %s\n", items[idx]->dateAdded);

		printf("Enter the appropriate number pertaining to the record attribute you want to modify: ");
		int option = 0;
		scanf("%d", &option);

		if (option == 1) { printf("Enter a new description: "); scanf("%s", items[idx]->description); }
		else if (option == 2) { printf("Enter a new quantity: "); scanf("%d", &items[idx]->quantity); }
		else if (option == 3) { printf("Enter a new wholesale cost: "); scanf("%f", &items[idx]->wholesaleCost); }
		else if (option == 4) { printf("Enter a new retail cost: "); scanf("%f", &items[idx]->retailCost); }
		else if (option == 5) { printf("Enter a new date added: "); scanf("%s", items[idx]->dateAdded); }
		saveInventory();
		printf("Changes saved.\n");
	}

	system("pause");
}