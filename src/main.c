/**
 * @file main.c
 * @brief Main entry point for the Wordle Game and Solver
 * 
 * This file contains the main game loop that supports two modes:
 * 1. Human player mode: User guesses the word manually
 * 2. Solver mode: Automated AI solves the puzzle
 * 
 * Course: ALGO3 - Algorithms & Data Structures in C
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "wordle.h"
#include "solver.h"

/* ANSI Color Codes for colored terminal output */
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/**
 * @brief Prints the guess with colored feedback
 * 
 * Green = correct position, Yellow = wrong position, White = not in word
 * 
 * @param guess The guessed word
 * @param result The feedback string (G/Y/X)
 */
void print_result_colored(const char* guess, const char* result) {
    printf("Result:  ");
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (result[i] == RESULT_CORRECT) {
            printf(ANSI_COLOR_GREEN "%c" ANSI_COLOR_RESET, guess[i]);
        } else if (result[i] == RESULT_PRESENT) {
            printf(ANSI_COLOR_YELLOW "%c" ANSI_COLOR_RESET, guess[i]);
        } else {
            printf("%c", guess[i]);
        }
    }
    printf("  [%s]\n", result);
}

/**
 * @brief Main function - Entry point of the program
 * 
 * Usage:
 *   ./wordle         - Play the game as a human
 *   ./wordle solver  - Watch the AI solve the puzzle
 */
int main(int argc, char* argv[]) {
    /* Seed random number generator */
    srand((unsigned int)time(NULL));

    /* Load dictionary from file */
    printf("====================================\n");
    printf("   WORDLE - ALGO3 Project\n");
    printf("====================================\n\n");
    printf("Loading dictionary...\n");
    
    int word_count = 0;
    char** words = load_words("words.txt", &word_count);
    
    if (!words) {
        printf("ERROR: Failed to load dictionary.\n");
        printf("Ensure 'words.txt' exists in the current directory.\n");
        system("pause");
        return 1;
    }
    printf("Loaded %d words.\n\n", word_count);

    /* Determine game mode from command line arguments */
    int solver_mode = 0;
    if (argc > 1 && strcmp(argv[1], "solver") == 0) {
        solver_mode = 1;
    }

    /* Select target word randomly */
    int target_idx = rand() % word_count;
    char* target = words[target_idx];

    /* Initialize solver if in solver mode */
    SolverState* solver = NULL;
    if (solver_mode) {
        solver = init_solver(words, word_count);
        if (!solver) {
            printf("ERROR: Failed to initialize solver.\n");
            free_word_list(words, word_count);
            system("pause");
            return 1;
        }
        printf("=== AUTOMATED SOLVER MODE ===\n");
        printf("Watch the AI solve the puzzle!\n\n");
    } else {
        printf("=== HUMAN PLAYER MODE ===\n");
        printf("Guess the 5-letter word in %d tries.\n\n", MAX_ATTEMPTS);
    }

    /* Main game loop */
    char guess[100];
    char result[WORD_LENGTH + 1];
    int won = 0;

    for (int attempt = 1; attempt <= MAX_ATTEMPTS; attempt++) {
        printf("Attempt %d/%d: ", attempt, MAX_ATTEMPTS);
        
        if (solver_mode) {
            /* Solver picks the best guess */
            char* best = get_best_guess(solver);
            if (!best) {
                printf("Solver error: No candidates remaining!\n");
                break;
            }
            strcpy(guess, best);
            printf("%s\n", guess);
        } else {
            /* Human enters guess */
            if (scanf("%99s", guess) != 1) {
                printf("Input error.\n");
                break;
            }
            /* Convert to uppercase for consistency */
            for (int i = 0; guess[i]; i++) {
                guess[i] = toupper(guess[i]);
            }
        }

        /* Validate guess length */
        if (strlen(guess) != WORD_LENGTH) {
            printf("Invalid: Must be exactly %d letters.\n\n", WORD_LENGTH);
            attempt--;  /* Don't count invalid attempts */
            continue;
        }

        /* Validate guess is in dictionary */
        if (!is_valid_word(guess, words, word_count)) {
            printf("Invalid: Word not in dictionary.\n\n");
            attempt--;
            continue;
        }

        /* Generate and display feedback */
        get_feedback(target, guess, result);
        print_result_colored(guess, result);

        /* Update solver's candidate list */
        if (solver_mode) {
            filter_candidates(solver, guess, result);
        }

        /* Check for win condition */
        int all_correct = 1;
        for (int i = 0; i < WORD_LENGTH; i++) {
            if (result[i] != RESULT_CORRECT) {
                all_correct = 0;
                break;
            }
        }

        if (all_correct) {
            printf("\n====================================\n");
            printf("  CONGRATULATIONS! YOU WON!\n");
            printf("  The word was: %s\n", target);
            if (solver_mode) {
                printf("  Solved in %d attempt(s).\n", attempt);
            }
            printf("====================================\n");
            won = 1;
            break;
        }
        printf("\n");
    }

    /* Display loss message if not won */
    if (!won) {
        printf("\n====================================\n");
        printf("  GAME OVER\n");
        printf("  The word was: %s\n", target);
        printf("====================================\n");
    }

    /* Free allocated memory */
    if (solver) {
        free_solver(solver);
    }
    free_word_list(words, word_count);

    /* Pause before exit (Windows) */
    printf("\n");
    system("pause");

    return 0;
}
