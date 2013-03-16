#include <iostream>
#include <stdio.h>

using namespace std;

bool isLower(char x)
{
	return (x >= 'a' && x <= 'z');
}

bool isUpper(char x)
{
	return (x >= 'A' && x <= 'Z');
}

bool isDigit(char x)
{
	return (x >= '0' && x <= '9');
}

void verifyPassword(char *str)
{
	bool longEnough = strlen(str) >= 6;
	bool foundLower = false;
	bool foundUpper = false;
	bool foundDigit = false;

	for (int i = 0; i < strlen(str); i++)
	{
		if (isLower(str[i]))
		{
			foundLower = true;
		}
		else if (isUpper(str[i]))
		{
			foundUpper = true;
		}
		else if (isDigit(str[i]))
		{
			foundDigit = true;
		}

		if (foundLower && foundUpper && foundDigit)
		{
			break;
		}
	}

	if (!longEnough)
	{
		cout << "You need to have 6 or more characters." << endl;
	}

	if (!foundLower)
	{
		cout << "You need at least 1 lowercase character." << endl;
	}

	if (!foundUpper)
	{
		cout << "You need at least 1 uppercase character." << endl;
	}

	if (!foundDigit)
	{
		cout << "You need at least 1 digit." << endl;
	}
}

int main()
{
	char fix[200];
	cout << "Enter your password: " << endl;
	gets(fix);
	verifyPassword(fix);
	system("pause");
	return 0; 
}