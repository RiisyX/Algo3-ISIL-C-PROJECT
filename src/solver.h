/**
 * @file solver.h
 * @brief Header file for Wordle Solver
 * 
 * Contains type definitions and function prototypes for the
 * automated Wordle solving algorithm.
 * 
 * Course: ALGO3 - Algorithms & Data Structures in C
 */

#ifndef SOLVER_H
#define SOLVER_H

#include <stdbool.h>
#include "wordle.h"

/*============================================================
 * DATA STRUCTURES
 *============================================================*/

/**
 * @brief Solver state structure
 * 
 * Maintains the solver's knowledge about possible words.
 * Uses a boolean mask array for efficient candidate tracking
 * without copying/moving word pointers.
 * 
 * Space Complexity: O(n) where n = total words
 */
typedef struct {
    char** all_words;       /**< Pointer to master word list (not owned) */
    int total_words;        /**< Total count of words in master list */
    bool* possible_mask;    /**< Boolean array: true if word is still possible */
    int possible_count;     /**< Number of remaining candidates */
} SolverState;

/*============================================================
 * FUNCTION PROTOTYPES
 *============================================================*/

/**
 * @brief Initializes the solver state
 * 
 * Allocates and initializes the solver with all words as candidates.
 * Time Complexity: O(n)
 * Space Complexity: O(n) for the boolean mask
 * 
 * @param all_words Pointer to the master word list
 * @param total_count Number of words in the list
 * @return Pointer to initialized SolverState, or NULL on failure
 */
SolverState* init_solver(char** all_words, int total_count);

/**
 * @brief Frees solver state memory
 * 
 * @param state Pointer to SolverState to free
 */
void free_solver(SolverState* state);

/**
 * @brief Filters candidate words based on feedback
 * 
 * Removes words that could not have produced the observed feedback.
 * Uses consistency checking: for each candidate, simulate what
 * feedback it would give and compare to actual feedback.
 * 
 * Time Complexity: O(n * WORD_LENGTH^2) where n = remaining candidates
 * 
 * @param state Pointer to SolverState
 * @param guess The word that was guessed
 * @param result The feedback received (G/Y/X string)
 */
void filter_candidates(SolverState* state, const char* guess, const char* result);

/**
 * @brief Determines the best next guess
 * 
 * Strategy: Letter Frequency Scoring
 * 1. Count frequency of each letter across all remaining candidates
 * 2. Score each candidate by sum of its unique letter frequencies
 * 3. Choose the word with highest score
 * 
 * This heuristic maximizes information gain by preferring words
 * with commonly occurring letters.
 * 
 * Time Complexity: O(n * WORD_LENGTH) where n = remaining candidates
 * 
 * @param state Pointer to SolverState
 * @return Best guess word, or NULL if no candidates remain
 */
char* get_best_guess(SolverState* state);

#endif /* SOLVER_H */
