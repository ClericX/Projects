#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **guesses;
int patternLen;
int numGuesses;
char *answer;

void calculatePatternCorrectness(char *guess, char *result)
{
	// Fill the result with filler.
	for (int i = 0; i < patternLen; i++)
	{
		result[i] = '.';
	}

	int curResultSlot = 0;

	// Allocate the mask.
	char *answerMask = (char *)malloc(patternLen);
	strcpy(answerMask, answer);
	char *guessMask = (char *)malloc(patternLen);
	strcpy(guessMask, guess);

	// First check for exact matches and remove them from available matches in the mask.
	for (int i = 0; i < patternLen; i++)
	{
		if (answerMask[i] == guessMask[i])
		{
			answerMask[i] = ' ';
			guessMask[i] = ' ';
			result[curResultSlot] = 'b';
			curResultSlot++;
		}
	}

	for (int i = 0; i < patternLen; i++)
	{
		if (answerMask[i] != ' ')
		{
			for (int x = 0; x < patternLen; x++)
			{
				if (answerMask[i] == guessMask[x] && guessMask[x] != ' ')
				{
					answerMask[i] = ' ';
					guessMask[x] = ' ';
					result[curResultSlot] = 'w';
					curResultSlot++;
				}
			}
		}
	}

	// For some reason freeing here crashes the app for me...
	//free(answerMask);
	//free(guessMask);
}

bool printGuess(int theGuess)
{
	printf("%d: ", theGuess+1);

	char *patternCorrectness = (char *)malloc(patternLen * sizeof(char));
	calculatePatternCorrectness(guesses[theGuess], patternCorrectness);

	for (int i = 0; i < patternLen; i++)
	{
		printf("%c", patternCorrectness[i]);
	}

	printf(" ");

	for (int i = 0; i < patternLen; i++)
	{
		printf("%c", guesses[theGuess][i]);
	}

	printf("\n");

	bool guessedRight = true;

	for (int i = 0; i < patternLen; i++)
	{
		if (patternCorrectness[i] != 'b')
		{
			guessedRight = false;
			break;
		}
	}

	free(patternCorrectness);

	return guessedRight;
}

bool printGuesses(int curGuess)
{
	for (int i = 0; i < curGuess; i++)
	{
		if (printGuess(i))
		{
			return true;
		}
	}

	return false;
}

int main()
{
	// Get initial input.
	printf("Enter the pattern length: ");
	scanf("%d", &patternLen);
	printf("Enter the number of guesses: ");
	scanf("%d", &numGuesses);

	// Allocate necessary memory for user input.
	guesses = (char **)malloc(numGuesses * sizeof(char *));
	for (int i = 0; i < numGuesses; i++)
	{
		guesses[i] = (char *)malloc((patternLen+1) * sizeof(char));
	}
	answer = (char *)malloc((patternLen+1) * sizeof(char));

	// Get the correct answer.
	printf("Input the key pattern with no spaces: ");
	scanf("%s", answer);

	bool guessedRight = false;

	// Go through the guessing process.
	for (int i = 0; i < numGuesses; i++)
	{
		printf("Input a guess pattern with no spaces: ");
		scanf("%s", guesses[i]);
		if (printGuesses(i+1))
		{
			guessedRight = true;
			break;
		}
	}

	if (guessedRight)
	{
		printf("You guessed the pattern!");
	}
	else
	{
		printf("You did not guess the pattern!");
	}

	free(answer);
	for (int i = 0; i < numGuesses; i++)
	{
		free(guesses[i]);
	}
	free(guesses);

	return 0;
}