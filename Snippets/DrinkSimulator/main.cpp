#include <iostream>
#include <stdio.h>

struct drink
{
	char *name;
	float cost;
	int quantity;
};

int main()
{
	// Initialize drinks
	drink drinks[5] = { {"Cola", 0.75f, 20}, {"Root Beer", 0.75f, 20}, {"Lemon-Lime", 0.75f, 20}, {"Grape Soda", 0.80f, 20}, {"Cream Soda", 0.80f, 20} };

	bool quit = false;
	float machineProfit = 0.0f;

	while (!quit)
	{
		// Print drinks
		printf("%3s%10s\t\t%5s\t\t%12s\n", "ID", "Name", "Price", "# in machine");
		printf("------------------------------------------------------\n");
		for (int i = 0; i < 5; i++)
		{
			printf("%d. %10s\t\t%.2f\t\t%d\n", i+1, drinks[i].name, drinks[i].cost, drinks[i].quantity);
		}

		// Get user input.
		printf("Enter either a number for a drink ID or 'q' to quit.\n");
		char in[2];
		scanf("%s", &in);
		
		// Act on user input.
		if (in[0] == 'q')
		{
			printf("You have chosen to quit.\n");
			printf("The machine has earned $%.2f\n", machineProfit);
			quit = true;
		}
		else
		{
			// Calculate drink index.
			int theDrink = in[0] - '0' - 1;
			
			if (drinks[theDrink].quantity > 0)
			{
				// Ask for money input.
				printf("Enter the amount of money you are inserting: ");
				float moneyIn = -5.0f;
				while (moneyIn < 0.0f || moneyIn > 1.0f)
				{
					scanf("%f", &moneyIn);

					if (moneyIn < 0.0f || moneyIn > 1.0f)
					{
						printf("Please enter a positive amount which is $1 or less.\n");
					}
				}

				// Calculate change and release drink.
				printf("Change: %.2f\n", moneyIn - drinks[theDrink].cost);
				drinks[theDrink].quantity--;
				machineProfit += drinks[theDrink].cost;
				printf("BEEP! Out popped a %s!\n", drinks[theDrink].name);
			}
			else
			{
				printf("%s is sold out! Please select another drink.\n", drinks[theDrink].name);
			}
		}

		printf("\n");
	}

	system("pause");
}