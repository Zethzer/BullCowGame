/*
 *	This is the console executable, that makes use of the BullCow class
 *	This acts as the view in a MVC pattern, and is responsible for all
 *	user interaction. For game logic see the FBullCowGame class.
 */
#include <iostream>
#include <string>
#include <time.h>

#include "FBullCowGame.h"

 // To make syntax Unreal friendly
using FText = std::string;
using int32 = int;

// Declarations
void PrintIntro();
void PlayGame();
void PrintGameSummary();
FText GetValidGuess();
bool AskToPlayAgain();

FBullCowGame BCGame;

/**
 *	Main function
 */
int main()
{
	do
	{
		BCGame.Reset();
		PrintIntro();
		PlayGame();
		PrintGameSummary();
	} while (AskToPlayAgain());

	// Exit application
	return 0;
}


/**
 *	Print the intro of the game
 */
void PrintIntro()
{
	constexpr int32 MIN_LENGTH = 3;
	constexpr int32 MAX_LENGTH = 6;

	FText Response = "";
	int32 Length = 0;

	std::cout << "\n\nWelcome to Bulls and Cows !\n";
	std::cout << std::endl;
	std::cout << "          }  {              ___" << std::endl;
	std::cout << "          (o o)            (o o)" << std::endl;
	std::cout << "   /-------\\ /              \\ /-------\\ " << std::endl;
	std::cout << "  / | BULL |O                O| COW  | \\ " << std::endl;
	std::cout << " *  |-,----|                  |------|  *" << std::endl;
	std::cout << "    ^      ^                  ^      ^" << std::endl;
	std::cout << std::endl;
	
	std::cout << "Would you choose the length of word (y/n) ? ";
	std::getline(std::cin, Response);
	if (Response[0] == 'y' || Response[0] == 'Y')
	{
		do
		{
			std::cout << "What is the length you want (3-6) ? ";
			std::cin >> Length;
		} while (Length < 3 && Length > 6);
	}
	else
	{
		// Random between 3 and 6 (inclusive)
		srand((unsigned int)time(NULL));
		Length = rand() % (MAX_LENGTH - MIN_LENGTH + 1) + MIN_LENGTH;
	}

	BCGame.Reset(Length);

	std::cout << "The word will have a length of " << BCGame.GetHiddenWordLength() << std::endl;
}

/**
*	Play the game
*/
void PlayGame()
{
	FText Guess = "";
	FBullCowCount BullsCowsCount;

	while (!BCGame.IsGameWon() && BCGame.GetCurrentTry() <= BCGame.GetMaxTries())
	{
		// Get valid guess
		Guess = GetValidGuess();

		// Get Bulls and Cows count
		BullsCowsCount = BCGame.SubmitValidGuess(Guess);

		// Print it
		std::cout << "Bulls = " << BullsCowsCount.Bulls;
		std::cout << "\tCows = " << BullsCowsCount.Cows << "\n\n";
	}
}

void PrintGameSummary()
{
	if (BCGame.IsGameWon())
	{
		std::cout << "CONGRATS ! YOU WON THE GAME !\n\n";
	}
	else
	{
		std::cout << "Oowww :( Bad luck... You lose. Maybe next time ;)\n\n";
	}
}

/**
 *	Get the guess of the player
 *	@return The guess of the player
 */
FText GetValidGuess()
{
	FText Guess = "";
	EGuessStatus Status = EGuessStatus::WRONG_LENGTH;

	// Get a valid guess : loop if needed
	do
	{
		std::cout << "Try " << BCGame.GetCurrentTry() << " of " << BCGame.GetMaxTries();
		std::cout << " | Enter a guess : ";
		std::getline(std::cin >> std::ws, Guess);

		Status = BCGame.CheckGuessValidity(Guess);

		// Print errors if needed
		switch (Status)
		{
		case EGuessStatus::WRONG_LENGTH:
			std::cout << "Guess not valid ! He must contains " << BCGame.GetHiddenWordLength();
			std::cout << " letters.\n\n";
			break;
		case EGuessStatus::NOT_ISOGRAM:
			std::cout << "Guess not valid ! He must be an isogram.\n\n";
			break;
		case EGuessStatus::NO_LOWERCASE:
			std::cout << "Guess not valid ! He must be in lowercase.\n\n";
			break;
		case EGuessStatus::NOT_ONLY_LETTERS:
			std::cout << "Guess not valid ! He must contains only alpha letters.\n\n";
			break;
		default:
			break;
		}
	} while (Status != EGuessStatus::OK);

	return Guess;
}

bool AskToPlayAgain()
{
	FText Response = "";

	std::cout << "Do you want to play again (y/n) ? ";
	std::getline(std::cin, Response);

	return (Response[0] == 'y' || Response[0] == 'Y');
}
