#ifndef WORDLE_H
#define WORDLE_H

#include <stdbool.h>

#define WORD_LENGTH 5
#define MAX_ATTEMPTS 6

// Feedback characters
#define RESULT_CORRECT 'G' // Green
#define RESULT_PRESENT 'Y' // Yellow
#define RESULT_ABSENT  'X' // Gray

// Structure to hold game state (useful if we expand features later)
typedef struct {
    char target_word[WORD_LENGTH + 1];
    int attempts_used;
    bool solved;
} GameState;

/**
 * Loads words from a file into a dynamically allocated array.
 * @param filename Name of the file to load words from.
 * @param count Pointer to an integer where the word count will be stored.
 * @return Pointer to array of strings (char**), or NULL on failure.
 */
char** load_words(const char* filename, int* count);

/**
 * Frees the memory allocated for the word list.
 * @param word_list Array of strings.
 * @param count Number of words in the list.
 */
void free_word_list(char** word_list, int count);

/**
 * Checks if a guessed word is in the valid word list.
 * @param guess The 5-letter guess string.
 * @param word_list Array of valid words.
 * @param count Number of words in the list.
 * @return true if valid, false otherwise.
 */
bool is_valid_word(const char* guess, char** word_list, int count);

/**
 * Generates feedback for a guess against a target word.
 * @param target The correct word.
 * @param guess The player's guess.
 * @param result Buffer to store the result string (must be at least WORD_LENGTH + 1).
 */
void get_feedback(const char* target, const char* guess, char* result);

#endif // WORDLE_H
