#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "wordle.h"
#include "solver.h"

// ANSI Color Codes for terminal output
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void print_result_colored(const char* guess, const char* result) {
    printf("Attempt: ");
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (result[i] == RESULT_CORRECT) {
            printf(ANSI_COLOR_GREEN "%c" ANSI_COLOR_RESET, guess[i]);
        } else if (result[i] == RESULT_PRESENT) {
            printf(ANSI_COLOR_YELLOW "%c" ANSI_COLOR_RESET, guess[i]);
        } else {
            printf("%c", guess[i]);
        }
    }
    printf("  (Feedback: %s)\n", result);
}

int main(void) {
    srand((unsigned int)time(NULL));

    // Load dictionary
    printf("Loading dictionary...\n");
    int word_count = 0;
    char** words = load_words("wordle-solver/words.txt", &word_count);
    if (!words) words = load_words("../words.txt", &word_count);
    if (!words) words = load_words("words.txt", &word_count);

    if (!words) {
        printf("Failed to load dictionary. Ensure 'words.txt' exists.\n");
        system("pause");
        return 1;
    }
    printf("Loaded %d words.\n", word_count);

    // Pick target
    int target_idx = rand() % word_count;
    char* target = words[target_idx]; 

    // Initialize solver
    SolverState* solver = init_solver(words, word_count);
    if (!solver) {
        printf("Failed to init solver.\n");
        free_word_list(words, word_count);
        system("pause");
        return 1;
    }
    printf("\n--- AUTOMATED SOLVER MODE ---\n");

    // Game loop
    char guess[100];
    char result[WORD_LENGTH + 1];
    bool won = false;

    for (int attempt = 1; attempt <= MAX_ATTEMPTS; attempt++) {
        printf("\nTry %d/%d: ", attempt, MAX_ATTEMPTS);
        
        char* best = get_best_guess(solver);
        if (!best) {
            printf("Solver cannot find more words!\n");
            break;
        }
        strcpy(guess, best);
        printf("%s (Solver chose)\n", guess);

        get_feedback(target, guess, result);
        print_result_colored(guess, result);
        filter_candidates(solver, guess, result);

        // Check win
        bool all_green = true;
        for (int i = 0; i < WORD_LENGTH; i++) {
            if (result[i] != RESULT_CORRECT) all_green = false;
        }

        if (all_green) {
            printf("\n=============================\n");
            printf("WINNER! The word was %s\n", target);
            printf("Solver cracked it in %d tries.\n", attempt);
            printf("=============================\n");
            won = true;
            break;
        }
    }

    if (!won) {
        printf("\nGAME OVER. The word was %s\n", target);
    }

    // Cleanup
    free_solver(solver);
    free_word_list(words, word_count);

    printf("\n");
    system("pause");

    return 0;
}
