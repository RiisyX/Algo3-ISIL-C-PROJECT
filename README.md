# Wordle Game and Solver

## Description

This project is a command-line implementation of the Wordle game written in C. It includes two modes: a human player mode where you try to guess the word yourself, and a solver mode where the program finds the word automatically.

This was developed as part of the ALGO3 course (Algorithms and Data Structures in C).

## Features

- Play Wordle in the terminal
- Dictionary of valid 5-letter words
- Color-coded feedback (Green, Yellow, Gray)
- Automated solver using letter frequency analysis
- Clean separation between game logic and solver logic

## Project Structure

```
wordle-solver/
├── src/
│   ├── main.c        # Main program
│   ├── wordle.c      # Game logic
│   ├── wordle.h      # Game header
│   ├── solver.c      # Solver algorithm
│   └── solver.h      # Solver header
├── words.txt         # Word dictionary
├── README.md
├── report.md         # Technical report (Markdown)
├── Project_Algo3.pdf # Project report (PDF)
└── screenshots/
```

## Compilation

To compile the project, run:

```
gcc src/*.c -o wordle
```

## How to Run

### Human Mode

Play the game yourself:

```
./wordle
```

You will be asked to guess a 5-letter word. You have 6 attempts.

### Solver Mode

Let the program solve the puzzle:

```
./wordle solver
```

The solver will automatically pick words and show how it narrows down the candidates.

## Game Rules

1. A random 5-letter word is selected from the dictionary
2. You have 6 tries to guess it
3. After each guess, you get feedback:
   - G (Green) = correct letter in correct position
   - Y (Yellow) = correct letter but wrong position
   - X (Gray) = letter not in the word
4. You win if you guess the word within 6 attempts

## Solver Strategy

The solver works by eliminating impossible words after each guess.

1. It starts with all words as possible candidates
2. After each guess, it removes words that don't match the feedback
3. To pick the next guess, it scores words based on letter frequency
4. Words with common letters get higher scores
5. It picks the word with the highest score

This approach usually solves the puzzle in 3-4 attempts.

## Screenshots

### Solver Mode (Win)

![Solver winning the game](screenshots/Win%20screen%20+%20Solver.png)

### Human Mode (Game Over)

![Game over screen](screenshots/fail%20Screen.png)

## Author

Rezki Mohamed Riad  
ISIL C Group 1  
242431624912
