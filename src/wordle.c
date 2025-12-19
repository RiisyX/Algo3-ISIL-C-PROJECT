/**
 * @file wordle.c
 * @brief Implementation of Wordle game logic
 * 
 * Contains functions for dictionary loading, word validation,
 * and feedback generation.
 * 
 * Course: ALGO3 - Algorithms & Data Structures in C
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "wordle.h"

/**
 * @brief Loads words from a dictionary file
 * 
 * Implementation uses two-pass approach:
 * 1. First pass: Count valid words to allocate exact memory
 * 2. Second pass: Read and store words
 * 
 * This approach minimizes memory fragmentation compared to
 * dynamic resizing (realloc).
 */
char** load_words(const char* filename, int* count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return NULL;
    }

    /* First pass: count valid 5-letter words */
    int lines = 0;
    char buffer[100];
    
    while (fgets(buffer, sizeof(buffer), file)) {
        /* Remove newline characters */
        size_t len = strcspn(buffer, "\r\n");
        if (len == WORD_LENGTH) {
            lines++;
        }
    }

    if (lines == 0) {
        fclose(file);
        return NULL;
    }

    /* Allocate array of pointers */
    char** words = (char**)malloc(lines * sizeof(char*));
    if (!words) {
        fclose(file);
        return NULL;
    }

    /* Second pass: read and store words */
    rewind(file);
    int i = 0;
    
    while (fgets(buffer, sizeof(buffer), file) && i < lines) {
        buffer[strcspn(buffer, "\r\n")] = '\0';  /* Trim newline */
        
        if (strlen(buffer) == WORD_LENGTH) {
            /* Normalize to uppercase */
            for (int j = 0; j < WORD_LENGTH; j++) {
                buffer[j] = toupper(buffer[j]);
            }
            
            /* Allocate and copy word */
            words[i] = (char*)malloc((WORD_LENGTH + 1) * sizeof(char));
            if (words[i]) {
                strcpy(words[i], buffer);
                i++;
            }
        }
    }

    *count = i;
    fclose(file);
    return words;
}

/**
 * @brief Frees all memory allocated for the word list
 */
void free_word_list(char** word_list, int count) {
    if (!word_list) return;
    
    for (int i = 0; i < count; i++) {
        free(word_list[i]);
    }
    free(word_list);
}

/**
 * @brief Linear search to check if word exists in dictionary
 */
bool is_valid_word(const char* guess, char** word_list, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(guess, word_list[i]) == 0) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Generates Wordle-style feedback for a guess
 * 
 * Algorithm:
 * 1. Initialize all positions as 'X' (absent)
 * 2. Pass 1: Mark exact matches as 'G' and mark them as used
 * 3. Pass 2: For non-matched letters, check if they appear
 *    elsewhere in the target (mark as 'Y')
 * 
 * The two-pass approach correctly handles duplicate letters.
 */
void get_feedback(const char* target, const char* guess, char* result) {
    /* Create working copy of target to track used letters */
    char temp_target[WORD_LENGTH + 1];
    strcpy(temp_target, target);
    
    /* Initialize all positions as absent */
    for (int i = 0; i < WORD_LENGTH; i++) {
        result[i] = RESULT_ABSENT;
    }
    result[WORD_LENGTH] = '\0';

    /* Pass 1: Find exact matches (Green) */
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (guess[i] == temp_target[i]) {
            result[i] = RESULT_CORRECT;
            temp_target[i] = '#';  /* Mark as used */
        }
    }

    /* Pass 2: Find present but misplaced letters (Yellow) */
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (result[i] == RESULT_CORRECT) {
            continue;  /* Skip already matched positions */
        }

        for (int j = 0; j < WORD_LENGTH; j++) {
            if (guess[i] == temp_target[j]) {
                result[i] = RESULT_PRESENT;
                temp_target[j] = '#';  /* Mark as used */
                break;
            }
        }
    }
}
