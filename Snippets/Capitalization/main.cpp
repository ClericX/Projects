#include <iostream>
#include <stdio.h>

using namespace std;

bool isValidLetter(char x)
{
	return ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z'));
}

void capitalize(char *str)
{
	bool lookingToCapitalize = true;

	for (int i = 0; i < strlen(str); i++)
	{
		if (lookingToCapitalize && isValidLetter(str[i]))
		{
			str[i] = toupper(str[i]);
			lookingToCapitalize = false;
		}
		else if (str[i] == '.')
		{
			lookingToCapitalize = true;
		}
	}
}

int main()
{
	char fix[200];
	cout << "Enter a sentence to fix: " << endl;
	gets(fix);
	capitalize(fix);
	cout << "Fixed: " << fix << endl;
	system("pause");
	return 0; 
}