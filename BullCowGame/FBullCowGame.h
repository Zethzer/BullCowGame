/**
 *	The game logic. (no view code or direct user interaction)
 *	The game is a simple guess the word game based on Mastermind
 */

#pragma once

/**
 *	Improves :
 *	- Let the user select the length of the word (3-6) - Done
 *	- Create a dictionnary of 5 hidden words with different length - Done
 *	- Select randomly a word in the dictionnary depends of the length choose by the user or randomly if he select nothing - Done
 */

#include <string>

// To make syntax Unreal friendly
using FString = std::string;
using int32 = int;

struct FBullCowCount
{
	int32 Bulls = 0;
	int32 Cows = 0;
};

enum class EGuessStatus
{
	OK,
	NOT_ISOGRAM,
	WRONG_LENGTH,
	NO_LOWERCASE,
	NOT_ONLY_LETTERS
};

class FBullCowGame
{
public:
	FBullCowGame();

	// Getters
	int32 GetMaxTries() const;
	int32 GetCurrentTry() const;
	int32 GetHiddenWordLength() const;

	// Functions
	bool IsGameWon() const;
	EGuessStatus CheckGuessValidity(FString Guess) const;
	
	void Reset();
	void Reset(int32 WordLength);
	FBullCowCount SubmitValidGuess(FString Guess);

private:
	// Constants
	const	int32	FIRST_TURN_NUMBER	= 1;
	const	int32	DEFAULT_LENGTH		= 3;

	// Members
	int32 MyCurrentTry;
	FString MyHiddenWord;
	bool bIsGameWon;
	int32 MyWordLength;

	// Functions
	bool IsIsogram(FString Word) const;
	bool IsLowerCase(FString Word) const;
	bool IsAlphaOnly(FString Word) const;

	void chooseAnHiddenWord(int32 WordLength);
};