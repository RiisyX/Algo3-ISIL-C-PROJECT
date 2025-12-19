/**
 * @file solver.c
 * @brief Implementation of Wordle Solver Algorithm
 * 
 * Implements an automated Wordle solver using a greedy
 * letter-frequency heuristic for guess selection.
 * 
 * Course: ALGO3 - Algorithms & Data Structures in C
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "solver.h"

/**
 * @brief Initializes solver with all words as candidates
 */
SolverState* init_solver(char** all_words, int total_count) {
    SolverState* state = (SolverState*)malloc(sizeof(SolverState));
    if (!state) {
        return NULL;
    }

    state->all_words = all_words;
    state->total_words = total_count;
    state->possible_count = total_count;
    
    /* Allocate boolean mask array */
    state->possible_mask = (bool*)malloc(total_count * sizeof(bool));
    if (!state->possible_mask) {
        free(state);
        return NULL;
    }

    /* Initialize: all words are possible candidates */
    for (int i = 0; i < total_count; i++) {
        state->possible_mask[i] = true;
    }

    return state;
}

/**
 * @brief Frees solver state memory
 */
void free_solver(SolverState* state) {
    if (state) {
        if (state->possible_mask) {
            free(state->possible_mask);
        }
        free(state);
    }
}

/**
 * @brief Checks if a candidate could produce the observed feedback
 * 
 * Simulates what feedback would be generated if 'candidate_answer'
 * were the target word, and compares to the actual observed feedback.
 * 
 * @param candidate_answer Potential target word to test
 * @param guess The guess that was made
 * @param observed_feedback The actual feedback received
 * @return true if candidate is consistent with feedback
 */
static bool is_consistent(const char* candidate_answer, 
                          const char* guess, 
                          const char* observed_feedback) {
    char simulated_feedback[WORD_LENGTH + 1];
    get_feedback(candidate_answer, guess, simulated_feedback);
    return strcmp(simulated_feedback, observed_feedback) == 0;
}

/**
 * @brief Filters candidates based on guess feedback
 * 
 * Algorithm:
 * For each remaining candidate, check if it could have been the
 * target word given the feedback we received. If not, eliminate it.
 */
void filter_candidates(SolverState* state, const char* guess, const char* result) {
    int new_count = 0;
    
    for (int i = 0; i < state->total_words; i++) {
        if (state->possible_mask[i]) {
            /* Keep word only if it's consistent with the feedback */
            if (is_consistent(state->all_words[i], guess, result)) {
                new_count++;
            } else {
                state->possible_mask[i] = false;
            }
        }
    }
    
    state->possible_count = new_count;
    printf("         [%d candidates remaining]\n", new_count);
}

/**
 * @brief Selects the best guess using letter frequency scoring
 * 
 * Heuristic Strategy:
 * 1. Calculate how often each letter (A-Z) appears in remaining candidates
 * 2. For each candidate, compute a score = sum of frequencies of its unique letters
 * 3. Select the word with the highest score
 * 
 * Rationale:
 * Words with common letters are more likely to produce useful feedback
 * (Green or Yellow), which helps eliminate more candidates.
 */
char* get_best_guess(SolverState* state) {
    /* Handle edge cases */
    if (state->possible_count == 0) {
        return NULL;
    }
    
    /* If only one candidate remains, return it */
    if (state->possible_count == 1) {
        for (int i = 0; i < state->total_words; i++) {
            if (state->possible_mask[i]) {
                return state->all_words[i];
            }
        }
    }

    /*
     * Step 1: Calculate letter frequencies across remaining candidates
     * We count unique letters per word to avoid over-counting
     * repeated letters (e.g., 'E' in "EERIE")
     */
    int freq[26] = {0};
    
    for (int i = 0; i < state->total_words; i++) {
        if (state->possible_mask[i]) {
            const char* word = state->all_words[i];
            bool seen[26] = {false};
            
            for (int k = 0; k < WORD_LENGTH; k++) {
                int letter_idx = word[k] - 'A';
                if (letter_idx >= 0 && letter_idx < 26 && !seen[letter_idx]) {
                    freq[letter_idx]++;
                    seen[letter_idx] = true;
                }
            }
        }
    }

    /*
     * Step 2: Score each candidate and find the best
     * Score = sum of frequencies of unique letters in the word
     */
    char* best_word = NULL;
    int max_score = -1;

    for (int i = 0; i < state->total_words; i++) {
        if (state->possible_mask[i]) {
            const char* word = state->all_words[i];
            int score = 0;
            bool seen[26] = {false};
            
            for (int k = 0; k < WORD_LENGTH; k++) {
                int letter_idx = word[k] - 'A';
                if (letter_idx >= 0 && letter_idx < 26 && !seen[letter_idx]) {
                    score += freq[letter_idx];
                    seen[letter_idx] = true;
                }
            }

            if (score > max_score) {
                max_score = score;
                best_word = state->all_words[i];
            }
        }
    }

    return best_word;
}
