# Wordle Solver & Game

An optimized C implementation of the popular Wordle game, featuring a built-in automated solver.
Project for ALGO3 -- Algorithms & Data Structures.

## Project Structure

- `src/`: Source code files (.c, .h)
- `words.txt`: Dictionary of valid 5-letter words
- `README.md`: This file

## Compilation

To compile the project, run:

```bash
gcc src/*.c -o wordle
```

## Usage

### 1. Play Manually

Run the executable to play as a human:

```bash
./wordle
```

Enter your 5-letter guesses when prompted.

### 2. Run Automated Solver

Run the executable with the `solver` argument to watch the computer play:

```bash
./wordle solver
```

## Solver Strategy

The solver uses a **Greedy Frequency analysis** approach:

1. Maintains a list of all matching candidate words.
2. After each guess, filters out candidates that conflict with the feedback (Green/Yellow/Gray information).
3. Selects the next guess by:
   - Calculating letter frequencies of all remaining candidates.
   - Scoring each potential word based on the popularity of its unique letters.
   - Choosing the word with the highest score to maximize the probability of revealing new information (reducing the search space).

## Example Output (Solver)

```
Loading dictionary...
Loaded 45 words.

--- AUTOMATED SOLVER MODE ---

Try 1/6: SLATE (Solver chose)
Attempt: SLATE  (Feedback: XXYXX)
DEBUG: Remaining candidates: 8

Try 2/6: BRAIN (Solver chose)
Attempt: BRAIN  (Feedback: GGGGG)

WINNER! The word was BRAIN
Solver cracked it in 2 tries.
```

## Screenshots

[Placeholder for game screenshots]
