#ifndef SOLVER_H
#define SOLVER_H

#include <stdbool.h>
#include "wordle.h"

// Solver state structure
typedef struct {
    char** all_words;       // Pointer to the master word list
    int total_words;        // Total count in master list
    bool* possible_mask;    // Array of booleans: true if word[i] is still a valid candidate
    int possible_count;     // How many candidates remain
} SolverState;

/**
 * Initializes the solver state.
 * @param all_words Master list of words.
 * @param total_count Total number of words.
 * @return Allocated SolverState.
 */
SolverState* init_solver(char** all_words, int total_count);

/**
 * Frees the solver state.
 * @param state Pointer to SolverState.
 */
void free_solver(SolverState* state);

/**
 * Filters the candidate list based on the last guess and its result.
 * @param state Pointer to SolverState.
 * @param guess The word that was guessed.
 * @param result The feedback string received (e.g., "GXYGX").
 */
void filter_candidates(SolverState* state, const char* guess, const char* result);

/**
 * Determines the best next guess.
 * @param state Pointer to SolverState.
 * @return The best guess string (from the master list), or NULL if no candidates remain.
 */
char* get_best_guess(SolverState* state);

#endif // SOLVER_H
