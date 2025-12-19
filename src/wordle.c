#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "wordle.h"

char** load_words(const char* filename, int* count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return NULL;
    }

    // First pass: count words
    int lines = 0;
    char buffer[100];
    while (fgets(buffer, sizeof(buffer), file)) {
        // Remove whitespace/newline
        size_t len = strcspn(buffer, "\r\n");
        if (len == WORD_LENGTH) {
            lines++;
        }
    }

    if (lines == 0) {
        printf("Error: No valid words found in %s\n", filename);
        fclose(file);
        return NULL;
    }

    // Allocate memory for pointers
    char** words = (char**)malloc(lines * sizeof(char*));
    if (!words) {
        printf("Error: Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    // Second pass: read words
    rewind(file);
    int i = 0;
    while (fgets(buffer, sizeof(buffer), file) && i < lines) {
        buffer[strcspn(buffer, "\r\n")] = 0; // trim newline
        
        if (strlen(buffer) == WORD_LENGTH) {
            // Normalize to uppercase
            for(int j=0; j<WORD_LENGTH; j++) buffer[j] = toupper(buffer[j]);
            
            words[i] = (char*)malloc((WORD_LENGTH + 1) * sizeof(char));
            strcpy(words[i], buffer);
            i++;
        }
    }

    *count = i;
    fclose(file);
    return words;
}

void free_word_list(char** word_list, int count) {
    if (!word_list) return;
    for (int i = 0; i < count; i++) {
        free(word_list[i]);
    }
    free(word_list);
}

bool is_valid_word(const char* guess, char** word_list, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(guess, word_list[i]) == 0) {
            return true;
        }
    }
    return false;
}

void get_feedback(const char* target, const char* guess, char* result) {
    // Make a copy of target to mark matched letters so we don't count them twice for yellow
    char temp_target[WORD_LENGTH + 1];
    strcpy(temp_target, target);
    
    // Initialize result with 'X'
    for (int i = 0; i < WORD_LENGTH; i++) {
        result[i] = RESULT_ABSENT;
    }
    result[WORD_LENGTH] = '\0';

    // Pass 1: Check for exact matches (Green)
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (guess[i] == temp_target[i]) {
            result[i] = RESULT_CORRECT;
            temp_target[i] = '#'; // Mark as used
        }
    }

    // Pass 2: Check for presence in wrong spot (Yellow)
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (result[i] == RESULT_CORRECT) continue; // Skip already matched

        for (int j = 0; j < WORD_LENGTH; j++) {
            // If we find the letter elsewhere and it hasn't been used up
            if (guess[i] == temp_target[j]) {
                result[i] = RESULT_PRESENT;
                temp_target[j] = '#'; // Mark as used
                break;
            }
        }
    }
}
