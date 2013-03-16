#include <iostream>
#include <stdio.h>

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

int main()
{
	int uppers = 0;
	int lowers = 0;
	int digits = 0;

	FILE *f = fopen("text.txt", "r");

	while (!feof(f) != 0)
	{
		char t;
		fread(&t, 1, 1, f);
		if (isLower(t)) { lowers++; }
		else if (isUpper(t)) { uppers++; }
		else if (isDigit(t)) { digits++; }
	}

	printf("Uppercase letters: %d\n", uppers);
	printf("Lowercase letters: %d\n", lowers);
	printf("Digits: %d\n", digits);
}