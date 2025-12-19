/**
 * @file wordle.h
 * @brief Header file for Wordle game logic
 * 
 * Contains constants, type definitions, and function prototypes
 * for the core Wordle game mechanics.
 * 
 * Course: ALGO3 - Algorithms & Data Structures in C
 */

#ifndef WORDLE_H
#define WORDLE_H

#include <stdbool.h>

/*============================================================
 * CONSTANTS
 *============================================================*/

/** Length of each word in the game */
#define WORD_LENGTH 5

/** Maximum number of guessing attempts */
#define MAX_ATTEMPTS 6

/** Feedback character: Correct letter in correct position (Green) */
#define RESULT_CORRECT 'G'

/** Feedback character: Correct letter in wrong position (Yellow) */
#define RESULT_PRESENT 'Y'

/** Feedback character: Letter not in word (Gray) */
#define RESULT_ABSENT  'X'

/*============================================================
 * FUNCTION PROTOTYPES
 *============================================================*/

/**
 * @brief Loads words from a dictionary file
 * 
 * Reads a text file containing one word per line and stores
 * valid 5-letter words in a dynamically allocated array.
 * Words are normalized to uppercase.
 * 
 * Time Complexity: O(n) where n = number of lines in file
 * Space Complexity: O(n * WORD_LENGTH) for storing words
 * 
 * @param filename Path to the dictionary file
 * @param count Pointer to store the number of words loaded
 * @return Pointer to array of word strings, or NULL on failure
 */
char** load_words(const char* filename, int* count);

/**
 * @brief Frees memory allocated for the word list
 * 
 * @param word_list Array of word strings to free
 * @param count Number of words in the array
 */
void free_word_list(char** word_list, int count);

/**
 * @brief Checks if a word exists in the dictionary
 * 
 * Performs linear search through the word list.
 * Time Complexity: O(n) where n = word count
 * 
 * @param guess The word to search for
 * @param word_list Array of valid words
 * @param count Number of words in the list
 * @return true if word exists, false otherwise
 */
bool is_valid_word(const char* guess, char** word_list, int count);

/**
 * @brief Generates feedback for a guess against the target
 * 
 * Compares guess to target and produces feedback string:
 * - 'G' (Green): Letter is correct and in correct position
 * - 'Y' (Yellow): Letter is in word but wrong position
 * - 'X' (Gray): Letter is not in the word
 * 
 * Algorithm uses two passes:
 * 1. First pass marks exact matches (Green)
 * 2. Second pass marks present letters (Yellow)
 * 
 * Time Complexity: O(WORD_LENGTH^2) = O(25) = O(1)
 * 
 * @param target The correct target word
 * @param guess The player's guess
 * @param result Buffer to store feedback (min WORD_LENGTH+1 bytes)
 */
void get_feedback(const char* target, const char* guess, char* result);

#endif /* WORDLE_H */
