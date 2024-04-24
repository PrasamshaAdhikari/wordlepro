#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAX_NUM_OF_WORDS 100

void clearScreen()
{
    system("clear || cls");
}

bool processGuess(const char *theAnswer, const char *theGuess)
{
    // Create a clue to give feedback to the player
    char clue[6] = "-----";
    // Keep track of letters used in the guess
    bool answerFlags[5] = {false};

    // Check for correct letters in correct positions
    for (int i = 0; i < 5; i++)
    {
        if (theGuess[i] == theAnswer[i])
        {
            clue[i] = 'G';
            answerFlags[i] = true;
        }
    }

    // Check for correct letters in wrong positions
    for (int i = 0; i < 5; i++)
    {
        if (clue[i] == '-')
        {
            for (int j = 0; j < 5; j++)
            {
                if (theGuess[i] == theAnswer[j] && !answerFlags[j])
                {
                    clue[i] = 'Y';
                    answerFlags[j] = true;
                    break;
                }
            }
        }
    }

    // Print the clue with colors
    for (int i = 0; i < 5; i++)
    {
        if (clue[i] == 'G')
        {
            printf("\033[0;32m%c\033[0m", theGuess[i]); // Green
        }
        else if (clue[i] == 'Y')
        {
            printf("\033[0;34m%c\033[0m", theGuess[i]); // Blue
        }
        else
        {
            printf("\033[0;31m%c\033[0m", theGuess[i]); // Red
        }
    }
    printf("\n");

    // Return true if the guess is correct
    return strcmp(clue, "GGGGG") == 0;
}

int main()
{
    // Clear the screen
    clearScreen();

    // Print color legend
    printf("Color Legend:\n");
    printf("\033[0;32mGreen\033[0m: Correct letter in correct position\n");
    printf("\033[0;34mBlue\033[0m: Correct letter in wrong position\n");
    printf("\033[0;31mRed\033[0m: Incorrect letter\n\n");

    char **wordsList = calloc(MAX_NUM_OF_WORDS, sizeof(char *));
    int wordCount = 0;
    char fiveLetterWord[6];

    FILE *wordsFile = fopen("words.txt", "r");
    if (wordsFile == NULL)
    {
        printf("Failed to open the file.\n");
        return 1;
    }

    // Read words from file
    while (fscanf(wordsFile, "%5s", fiveLetterWord) != EOF && wordCount < MAX_NUM_OF_WORDS)
    {
        wordsList[wordCount] = malloc(6 * sizeof(char));
        strcpy(wordsList[wordCount], fiveLetterWord);
        wordCount++;
    }
    fclose(wordsFile);

    // Choose a random word from the list
    srand(time(NULL));
    char *answer = wordsList[rand() % wordCount];

    int num_of_guesses = 0;
    bool guessed_correctly = false;
    char *guess = malloc(6 * sizeof(char));

    // Game loop
    while (num_of_guesses < 6 && !guessed_correctly)
    {
        printf("\nInput a 5 letter word and press enter: ");
        scanf("%s", guess);
        printf("\nYou guessed: ");

        // Process the guess and display feedback
        guessed_correctly = processGuess(answer, guess);
        if (!guessed_correctly)
        {
            printf("Incorrect guess! Try again.\n");
        }
        else
        {
            printf("Congratulations! You guessed the correct word.\n");
        }

        num_of_guesses++;
    }

    // Display end-of-game message
    if (!guessed_correctly)
    {
        printf("Sorry, you've used all your guesses. The correct word is %s\n", answer);
    }

    // Free allocated memory
    for (int i = 0; i < wordCount; i++)
    {
        free(wordsList[i]);
    }
    free(wordsList);
    free(guess);

    return 0;
}