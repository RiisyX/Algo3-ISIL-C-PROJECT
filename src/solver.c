#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "solver.h"

SolverState* init_solver(char** all_words, int total_count) {
    SolverState* state = (SolverState*)malloc(sizeof(SolverState));
    if (!state) return NULL;

    state->all_words = all_words;
    state->total_words = total_count;
    state->possible_count = total_count;
    
    state->possible_mask = (bool*)malloc(total_count * sizeof(bool));
    if (!state->possible_mask) {
        free(state);
        return NULL;
    }

    // Initially, all words are possible
    for (int i = 0; i < total_count; i++) {
        state->possible_mask[i] = true;
    }

    return state;
}

void free_solver(SolverState* state) {
    if (state) {
        if (state->possible_mask) free(state->possible_mask);
        free(state);
    }
}

// Helper to check if a specific word would produce the observed feedback given a candidate answer
static bool is_consistent(const char* candidate_answer, const char* guess, const char* observed_feedback) {
    char simulated_feedback[WORD_LENGTH + 1];
    get_feedback(candidate_answer, guess, simulated_feedback);
    return strcmp(simulated_feedback, observed_feedback) == 0;
}

void filter_candidates(SolverState* state, const char* guess, const char* result) {
    int new_count = 0;
    for (int i = 0; i < state->total_words; i++) {
        if (state->possible_mask[i]) {
            // Keep this word only if it could be the target
            // i.e., if we treated THIS word as the target, would we get the same feedback?
            if (is_consistent(state->all_words[i], guess, result)) {
                new_count++;
            } else {
                state->possible_mask[i] = false;
            }
        }
    }
    state->possible_count = new_count;
    printf("DEBUG: Remaining candidates: %d\n", new_count);
}

// Calculate score based on letter frequency in remaining candidates
// We want to pick a word that uses the most "popular" letters among the remaining set
// to maximize the chance of getting a Green or Yellow hit, thus splitting the set.
char* get_best_guess(SolverState* state) {
    if (state->possible_count == 0) return NULL;
    if (state->possible_count == 1) {
        // Only one left, return it
        for (int i = 0; i < state->total_words; i++) {
            if (state->possible_mask[i]) return state->all_words[i];
        }
    }

    // 1. Calculate frequency of each letter A-Z in all REMAINING candidates
    int freq[26] = {0};
    for (int i = 0; i < state->total_words; i++) {
        if (state->possible_mask[i]) {
            const char* w = state->all_words[i];
            // Count unique letters per word to avoid double counting 'E' in "EERIE" for popularity
            bool seen[26] = {false}; 
            for (int k = 0; k < WORD_LENGTH; k++) {
                int idx = w[k] - 'A';
                if (idx >= 0 && idx < 26 && !seen[idx]) {
                    freq[idx]++;
                    seen[idx] = true;
                }
            }
        }
    }

    // 2. Score every valid word (even those not possible?) to find the best discriminator.
    // Optimization: For this assignment, we strictly pick from ANY valid word in the dictionary
    // because sometimes a word that is known to be wrong is the best probe.
    // However, the rule "Maintains a list of candidate words" implies we might want to pick a candidate.
    // A safe efficient strategy for 2nd year: pick the best scoring word from the CANDIDATES first.
    
    char* best_word = NULL;
    int max_score = -1;

    for (int i = 0; i < state->total_words; i++) {
        // We only consider possible candidates as guesses to keep it simple ("Hard Mode" implicitly)
        // unless we want to implement the more complex "guess impossible word to reduce set" strategy.
        if (state->possible_mask[i]) {
            int current_score = 0;
            const char* w = state->all_words[i];
            bool seen[26] = {false};
            for (int k = 0; k < WORD_LENGTH; k++) {
                int idx = w[k] - 'A';
                if (idx >= 0 && idx < 26 && !seen[idx]) {
                    current_score += freq[idx];
                    seen[idx] = true;
                }
            }

            if (current_score > max_score) {
                max_score = current_score;
                best_word = state->all_words[i];
            }
        }
    }

    return best_word;
}
