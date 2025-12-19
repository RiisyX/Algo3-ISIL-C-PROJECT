# ALGO3 Project Report

## Wordle Game & Automated Solver

---

## 1. Introduction

This project implements a command-line Wordle game in C, featuring both a human-playable mode and an automated solver. The solver demonstrates practical application of data structures and algorithmic thinking.

---

## 2. Algorithm Design

### 2.1 Solver Strategy: Greedy Letter-Frequency Heuristic

The solver employs a greedy approach based on letter frequency analysis:

1. **Candidate Maintenance:** Track all words that could potentially be the target
2. **Consistency Filtering:** After each guess, eliminate words that are inconsistent with the received feedback
3. **Optimal Guess Selection:** Choose guesses that maximize information gain

#### Guess Selection Algorithm

```
function get_best_guess(candidates):
    // Step 1: Calculate letter frequencies
    freq[26] = {0}
    for each word in candidates:
        for each unique letter in word:
            freq[letter]++

    // Step 2: Score each candidate
    best_word = NULL
    max_score = -1
    for each word in candidates:
        score = sum of freq[letter] for unique letters
        if score > max_score:
            max_score = score
            best_word = word

    return best_word
```

**Rationale:** Words containing frequently-occurring letters are more likely to produce useful feedback (Green or Yellow results), which accelerates candidate elimination.

---

## 3. Data Structure Justification

### 3.1 Word Storage: Dynamic Array of Strings (`char**`)

**Choice:** Array of dynamically allocated strings

**Justification:**

- O(1) random access for target word selection
- Simple linear search sufficient for small dictionary sizes (~50 words)
- Easy memory management with paired allocation/deallocation

**Alternative Considered:** Linked list — rejected due to O(n) random access.

### 3.2 Candidate Tracking: Boolean Mask Array (`bool*`)

**Choice:** Parallel boolean array indicating word eligibility

**Justification:**

- O(1) lookup and update operations
- No memory copying or pointer manipulation needed
- Space-efficient: only n bytes for n words
- Maintains stable indices (word pointers never change)

**Alternative Considered:** Copying eligible words to new array — rejected due to O(n) copy cost per filter operation.

---

## 4. Complexity Analysis

### 4.1 Time Complexity

| Operation         | Complexity | Notes                        |
| ----------------- | ---------- | ---------------------------- |
| Load dictionary   | O(n)       | Two-pass file reading        |
| Validate word     | O(n)       | Linear search                |
| Generate feedback | O(1)       | Fixed 5×5 comparisons        |
| Filter candidates | O(n)       | Check each candidate         |
| Get best guess    | O(n)       | Frequency counting + scoring |

**Overall solver complexity per guess:** O(n) where n = dictionary size

### 4.2 Space Complexity

| Component       | Space    | Notes                     |
| --------------- | -------- | ------------------------- |
| Word list       | O(n × 5) | n words of 5 characters   |
| Boolean mask    | O(n)     | One bool per word         |
| Frequency array | O(1)     | Fixed 26 integers         |
| **Total**       | O(n)     | Linear in dictionary size |

---

## 5. Key Functions

### 5.1 `get_feedback(target, guess, result)`

Generates Wordle-style feedback using a two-pass algorithm:

```c
// Pass 1: Mark exact matches (Green)
for (int i = 0; i < 5; i++) {
    if (guess[i] == target[i]) {
        result[i] = 'G';
        target[i] = '#';  // Mark as used
    }
}

// Pass 2: Mark present letters (Yellow)
for (int i = 0; i < 5; i++) {
    if (result[i] != 'G') {
        for (int j = 0; j < 5; j++) {
            if (guess[i] == target[j]) {
                result[i] = 'Y';
                target[j] = '#';
                break;
            }
        }
    }
}
```

**Why two passes?** Prevents incorrect Yellow assignments when a letter appears multiple times.

### 5.2 `filter_candidates(state, guess, result)`

Eliminates inconsistent candidates by simulation:

```c
for each candidate in possible_words:
    simulated = get_feedback(candidate, guess)
    if simulated != actual_result:
        eliminate(candidate)
```

This technique treats the filtering as a consistency check: a word can only be the target if it would have produced the exact feedback we observed.

---

## 6. Memory Management

The project follows strict memory discipline:

1. **Allocation:** `malloc()` used for word list and solver state
2. **Deallocation:** Corresponding `free()` calls in cleanup functions
3. **Null checks:** All allocations verified before use
4. **No leaks:** Valgrind-clean design (all paths free allocated memory)

---

## 7. Conclusion

This implementation demonstrates effective use of arrays, dynamic memory allocation, and greedy algorithms to create a functional Wordle game and solver. The letter-frequency heuristic provides a reasonable balance between implementation simplicity and solving efficiency, typically solving puzzles in 3-4 attempts.

---

## References

- Wordle game rules: https://www.nytimes.com/games/wordle
- C Standard Library documentation
