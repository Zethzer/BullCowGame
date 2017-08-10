#pragma once

#include "FBullCowGame.h"

#include <map>
#include <cctype>
#include <time.h>

#define TMap std::map // To make syntax Unreal friendly

FBullCowGame::FBullCowGame() : MyCurrentTry(FIRST_TURN_NUMBER), bIsGameWon(false)
{
	Reset(DEFAULT_LENGTH);
}

int32 FBullCowGame::GetMaxTries() const 
{
	TMap<int32, int32> WordLengthToMaxTries{ {3,4}, {4,7}, {5,10}, {6,16} };

	return WordLengthToMaxTries[MyHiddenWord.length()];
}

int32 FBullCowGame::GetCurrentTry() const { return MyCurrentTry; }
int32 FBullCowGame::GetHiddenWordLength() const { return MyHiddenWord.length(); }
bool FBullCowGame::IsGameWon() const { return bIsGameWon; }

EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const
{
	if (!IsIsogram(Guess))
	{
		return EGuessStatus::NOT_ISOGRAM;
	}
	else if (!IsAlphaOnly(Guess))
	{
		return EGuessStatus::NOT_ONLY_LETTERS;
	}
	else if (!IsLowerCase(Guess))
	{
		return EGuessStatus::NO_LOWERCASE;
	}
	else if (Guess.length() != MyHiddenWord.length())
	{
		return EGuessStatus::WRONG_LENGTH;
	}
	else
	{
		return EGuessStatus::OK;
	}
}

void FBullCowGame::Reset()
{
	MyCurrentTry = FIRST_TURN_NUMBER;
	bIsGameWon = false;
}

void FBullCowGame::Reset(int32 WordLength)
{
	MyCurrentTry = FIRST_TURN_NUMBER;
	MyWordLength = WordLength;
	chooseAnHiddenWord(MyWordLength);
	bIsGameWon = false;
}

/** 
 *	Receive a valid guess. Increase turn.
 *	@return Count of bulls and cows
 */
FBullCowCount FBullCowGame::SubmitValidGuess(FString Guess)
{
	FBullCowCount BullCowCount;
	int32 WordLength = MyHiddenWord.length(); // Assume same length as guess

	++MyCurrentTry;

	// Loop through all letters in the guess
	for (int GChar = 0; GChar < WordLength; ++GChar)
	{
		// Compare each letter againt all the letters of the hidden word
		for (int MHWChar = 0; MHWChar < WordLength; ++MHWChar)
		{
			// Letters match ?
			if (Guess[GChar] == MyHiddenWord[MHWChar])
			{
				// Letters at the same place ?
				if (GChar == MHWChar)
				{
					++BullCowCount.Bulls;
				}
				else
				{
					++BullCowCount.Cows;
				}
			}		
		}
	}

	if (BullCowCount.Bulls == WordLength)
	{
		bIsGameWon = true;
	}

	return BullCowCount;
}

bool FBullCowGame::IsIsogram(FString Word) const
{
	// None or one letter in the word ?
	if (Word.length() <= 1)
	{
		return true;
	}

	TMap<char, bool> LetterSeen;

	// Loop other the word
	for (auto Letter : Word)
	{
		// If we the letter is in the map
		if (LetterSeen[Letter])
		{
			return false;
		}
		else
		{
			// See it for the first time
			LetterSeen[Letter] = true;
		}
	}

	return true;
}

bool FBullCowGame::IsLowerCase(FString Word) const
{
	for (auto Letter : Word)
	{
		if (!std::islower(Letter))
		{
			return false;
		}
	}

	return true;
}

bool FBullCowGame::IsAlphaOnly(FString Word) const
{
	for (auto Letter : Word)
	{
		if (!std::isalpha(Letter))
		{
			return false;
		}
	}

	return true;
}

void FBullCowGame::chooseAnHiddenWord(int32 WordLength)
{
	// 5 Word by map
	// Map of words : length 3
	TMap<int32, FString> WordLengthThree{ { 0,"ant" }, { 1,"job" }, { 2,"box" }, { 3,"jar" }, { 4,"gym" } };
	// Map of words : length 4
	TMap<int32, FString> WordLengthFour { { 0,"rock" },{ 1,"quiz" },{ 2,"hard" },{ 3,"joke" },{ 4,"code" } };
	// Map of words : length 5
	TMap<int32, FString> WordLengthFive { { 0,"zombi" },{ 1,"crazy" },{ 2,"quake" },{ 3,"cozey" },{ 4,"punky" } };
	// Map of words : length 6
	TMap<int32, FString> WordLengthSix{ { 0,"planet" },{ 1,"object" },{ 2,"jigsaw" },{ 3,"glitch" },{ 4,"backup" } };

	// Random between 0 and 4 (inclusive)
	srand((unsigned int)time(NULL));
	int32 RandomKey = rand() % 5;

	switch (WordLength)
	{
		case 3:
			MyHiddenWord = WordLengthThree[RandomKey];
			break;
		case 4:
			MyHiddenWord = WordLengthFour[RandomKey];
			break;
		case 5:
			MyHiddenWord = WordLengthFive[RandomKey];
			break;
		case 6:
			MyHiddenWord = WordLengthSix[RandomKey];
			break;
		default:
			break;
	}
}
