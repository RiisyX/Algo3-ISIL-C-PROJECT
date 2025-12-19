# Wordle Game & Automated Solver

A command-line implementation of the popular Wordle word-guessing game with an integrated automated solver.

## Course Information

**Course:** ALGO3 – Algorithms & Data Structures in C  
**Project:** Wordle Game + Automated Solver  
**Language:** C (Standard C99)

---

## Project Structure

```
wordle-solver/
├── src/
│   ├── main.c          # Main program entry point
│   ├── wordle.c        # Game logic implementation
│   ├── wordle.h        # Game logic header
│   ├── solver.c        # Solver algorithm implementation
│   └── solver.h        # Solver algorithm header
├── words.txt           # Dictionary of valid 5-letter words
├── README.md           # This file
├── report.md           # Technical report
├── screenshots/        # Screenshots of gameplay
└── .gitignore          # Git ignore rules
```

---

## Compilation

Compile the project using GCC:

```bash
gcc src/*.c -o wordle
```

---

## Usage

### Human Player Mode

Play the game yourself by guessing the secret 5-letter word:

```bash
./wordle
```

### Automated Solver Mode

Watch the AI solve the puzzle automatically:

```bash
./wordle solver
```

---

## Game Rules

1. The game selects a random 5-letter word from the dictionary
2. You have **6 attempts** to guess the word
3. After each guess, you receive feedback:
   - **G (Green):** Correct letter in correct position
   - **Y (Yellow):** Correct letter in wrong position
   - **X (Gray):** Letter not in the word
4. Win by guessing the word within 6 attempts

---

## Example Output

### Human Player Mode

```
====================================
   WORDLE - ALGO3 Project
====================================

Loading dictionary...
Loaded 45 words.

=== HUMAN PLAYER MODE ===
Guess the 5-letter word in 6 tries.

Attempt 1/6: CRANE
Result:  CRANE  [XXYXX]

Attempt 2/6: PLANT
Result:  PLANT  [GGGGG]

====================================
  CONGRATULATIONS! YOU WON!
  The word was: PLANT
====================================
```

### Solver Mode

```
====================================
   WORDLE - ALGO3 Project
====================================

Loading dictionary...
Loaded 45 words.

=== AUTOMATED SOLVER MODE ===
Watch the AI solve the puzzle!

Attempt 1/6: CRANE
Result:  CRANE  [XXYXX]
         [12 candidates remaining]

Attempt 2/6: PLANT
Result:  PLANT  [GGGGG]
         [1 candidates remaining]

====================================
  CONGRATULATIONS! YOU WON!
  The word was: PLANT
  Solved in 2 attempt(s).
====================================
```

---

## Solver Strategy

The automated solver uses a **Greedy Letter-Frequency Heuristic**:

1. **Initialization:** All dictionary words start as candidates
2. **Filtering:** After each guess, eliminate words inconsistent with feedback
3. **Guess Selection:** Choose the word that maximizes letter coverage:
   - Count frequency of each letter across remaining candidates
   - Score each word by sum of its unique letter frequencies
   - Select the highest-scoring word

This strategy efficiently narrows down candidates by prioritizing commonly-occurring letters.

---

## Data Structures

| Structure             | Purpose                            | Complexity |
| --------------------- | ---------------------------------- | ---------- |
| `char** words`        | Dynamic array of word strings      | O(n) space |
| `bool* possible_mask` | Candidate tracking without copying | O(n) space |
| `SolverState`         | Encapsulates solver knowledge      | O(1) space |

---

## Screenshots

_Screenshots of gameplay will be added here_

---

## Author

ALGO3 Project Submission
