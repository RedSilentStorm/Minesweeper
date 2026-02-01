# Minesweeper (Assembly Hybrid)

This project is a terminal-based implementation of the classic Minesweeper game, developed using C and x86 Inline Assembly. It demonstrates low-level programming techniques by offloading critical logic and memory indexing directly to the CPU.

## Features

* Dynamic Game Board: A 9x9 grid featuring 10 randomly placed mines.
* Hybrid Architecture: Critical calculations, including board indexing and boundary checks, are implemented via __asm blocks.
* Recursive Uncovering: Automatically clears adjacent empty cells when a cell with zero neighboring mines is selected.
* Flagging System: Allows players to mark suspected mine locations to prevent accidental detonation.
* Terminal Visualization: Utilizes ANSI escape codes for grid rendering and status updates.

## Technical Implementation

The following core functions are implemented using x86 Assembly to handle low-level operations:

* initializeBoard: Manages memory-mapped indexing for the two-dimensional arrays to set initial cell states.
* calculateNeighborCounts: Executes nested loops and conditional jumps at the assembly level to scan the 8-cell perimeter of every tile.
* uncoverCell: Performs hardware-level boundary validation to ensure player inputs remain within the array limits.

## Instructions

1. The game displays a grid of closed cells represented by #.
2. Input your moves using the format: [Action] [Row] [Column] (e.g., u 5 5 or f 2 3).
    * Action 'u' (Uncover): Opens the selected cell.
    * Action 'f' (Flag): Places or removes a marker on a cell.
3. Winning: Successfully uncover all cells that do not contain mines.
4. Losing: Detonating a single mine ends the game and reveals all hidden positions.

## Requirements and Compilation

This project requires a compiler that supports the MSVC __asm syntax and must be targeted for the x86 architecture.

1. Add the minesweeper.c file to a C project in Visual Studio.
2. Set the Solution Platform to x86 (Inline Assembly is generally not supported in x64 mode).
3. Compile and execute the program.


Note: This project was developed for educational purposes to explore the integration of high-level C logic with low-level Assembly instructions.
