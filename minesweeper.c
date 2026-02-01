#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define SIZE 9
#define NUM_MINES 10
#define CLOSED_CELL '#' 

// ANSI Color Codes for Terminal
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_BG_PURPLE     "\x1b[45m"
#define ANSI_BG_WHITE      "\x1b[47m"
#define ANSI_FG_RED        "\x1b[31m"
#define ANSI_FG_WHITE      "\x1b[37m"

char board[SIZE][SIZE];
char display[SIZE][SIZE];

void initializeBoard();
void placeMines();
void calculateNeighborCounts();
void displayBoard();
void uncoverCell(int row, int col);
bool endGame(int row, int col);

int main() {
	printf("--- MINESWEEPER IN TERMINAL ---\n");
    int row, col;
    char action;
    bool gameContinues = true;

    srand((unsigned int)time(NULL));
    initializeBoard();
    placeMines();
    calculateNeighborCounts();

    while (gameContinues) {
        displayBoard();
        printf("Enter Action: (U/F), Row, Col: ");
        if (scanf(" %c %d %d", &action, &row, &col) != 3) {
            while (getchar() != '\n');
            continue;
        }

        row--;
        col--;

        if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) continue;

        if (action == 'F' || action == 'f') {
            if (display[row][col] == CLOSED_CELL) {
                display[row][col] = '.';
            }
            else if (display[row][col] == '.') {
                display[row][col] = CLOSED_CELL;
            }
        }
        else if (action == 'U' || action == 'u') {

            if (display[row][col] != CLOSED_CELL) {
                continue;
            }

            if (board[row][col] == 'M') {
                gameContinues = false;
            }
            else {
                uncoverCell(row, col);
                int rem = 0;

                for (int i = 0; i < SIZE; i++) {
                    for (int j = 0; j < SIZE; j++) {
                        if (display[i][j] == CLOSED_CELL || display[i][j] == '.') {
                            rem++;
                        }
                    }
                }
                if (rem == NUM_MINES) {
                    gameContinues = false; printf("\nWINNER!\n");
                }
            }
        }
    }
    endGame(row, col);
    return 0;
}


// Board Initialization with Assembly

void initializeBoard() {
    short r, c;
    short s_size = SIZE;
    char char_zero = '0';
    char char_closed = CLOSED_CELL;

    for (r = 0; r < s_size; r++) {
        for (c = 0; c < s_size; c++) {
            short final_index = 0;

            __asm {
                mov ax, r
                mov bx, s_size
                imul bx; AX = r * 5 (veya 8)
                add ax, c; AX = indis
                mov final_index, ax;
            }

            board[0][final_index] = char_zero;
            display[0][final_index] = char_closed;
        }
    }
}


// Neighbor Count Calculation with Assembly

void calculateNeighborCounts() {
    short r, c, dr, dc, nr, nc;
    short count;
    short s_size = SIZE;
    short index;

    for (r = 0; r < SIZE; r++) {
        for (c = 0; c < SIZE; c++) {
            if (board[r][c] != 'M') {
                count = 0;
                __asm {
                    mov ax, -1
                    mov dr, ax

                    DR_LOOP :
					    cmp dr, 1
                        jg DR_END

                        mov ax, -1
                        mov dc, ax
                        
                        DC_LOOP :
                            cmp dc, 1
                            jg DC_END

                            mov ax, dr
                            or ax, dc
                            jz DC_INC

							; Calculate neighbor row and column
                            mov ax, r
                            add ax, dr
                            mov nr, ax

                            mov ax, c
                            add ax, dc
                            mov nc, ax

							; Boundary check for neighbor cell
                            cmp nr, 0
                            jl DC_INC
                            cmp nc, 0
                            jl DC_INC

                            mov ax, s_size
                            cmp nr, ax
                            jge DC_INC
                            cmp nc, ax
                            jge DC_INC

						    ; Calculate index of neighbor cell
                            mov ax, nr
                            mov bx, s_size
                            imul bx
                            add ax, nc
                            mov index, ax
                }
                if (board[0][index] == 'M') {
                    count++;
                }
                __asm {
                    DC_INC:
                        inc dc
                        jmp DC_LOOP

                    DC_END :
                        inc dr
                        jmp DR_LOOP

                    DR_END :
                }
                board[r][c] = (char)(count + '0');
            }
        }
    }
}


// Boundary Check Logic with Assembly

void uncoverCell(int r, int c) {
    short row = (short)r;
    short col = (short)c;
    short s_size = SIZE;
    short is_invalid = 0;

    __asm {
		; Row check
        mov ax, row
        cmp ax, 0
        jl INVALID
        cmp ax, s_size
        jge INVALID

		; Column check
        mov ax, col
        cmp ax, 0
        jl INVALID
        cmp ax, s_size
        jge INVALID

        jmp VALID

        INVALID :
        mov is_invalid, 1
        
        VALID :
    }

    if (is_invalid || display[r][c] != CLOSED_CELL) return;

    display[r][c] = board[r][c];

    if (board[r][c] == '0') {
        display[r][c] = ' ';
        for (int i = -1; i <= 1; i++)
            for (int j = -1; j <= 1; j++)
                if (i != 0 || j != 0) {
                    uncoverCell(r + i, c + j);
                }
    }
}


// Other Functions

void displayBoard() {
    printf("\n     ");
    for (int c = 0; c < SIZE; c++) {
        printf("%d ", c + 1);
    }
    printf("\n   +-");
    for (int c = 0; c < SIZE; c++) {
        printf("--");
    }
    printf("+\n");

    for (int r = 0; r < SIZE; r++) {
        if (r + 1 < 10) {
            printf(" %d |", r + 1);
        } else {
            printf("%d |", r + 1);
		}

		printf(" ");
        for (int c = 0; c < SIZE; c++) {
            char cell = display[r][c];
            if (cell == '.') {
                printf(ANSI_BG_PURPLE "%c" ANSI_COLOR_RESET, cell);
                printf(" ");
            }
            else if (cell >= '1' && cell <= '8') {
                printf(ANSI_FG_WHITE "%c " ANSI_COLOR_RESET, cell);
            }
            else {
                printf("%c ", cell);
            }
        }
        printf("|\n");
    }
    printf("   +-");
    for (int c = 0; c < SIZE; c++) {
        printf("--");
    }
    printf("+\n");
}


bool endGame(int last_row, int last_col) {
    printf("\n--- GAME OVER ---\n");

    printf("\n     ");
    for (int c = 0; c < SIZE; c++) {
        printf("%d ", c + 1);
    }
    printf("\n   +-");
    for (int c = 0; c < SIZE; c++) {
        printf("--");
    }
    printf("+\n");
    for (int r = 0; r < SIZE; r++) {
        if (r + 1 < 10) {
            printf(" %d |", r + 1);
		}
        else {
            printf("%d |", r + 1);
        }
		printf(" ");
        for (int c = 0; c < SIZE; c++) {
            char final_char = board[r][c];
			bool isFlagged = (display[r][c] == '.');
            if (isFlagged) {
			    printf(ANSI_BG_PURPLE);
            }

            if (r == last_row && c == last_col && board[r][c] == 'M') {
                printf(ANSI_BG_WHITE ANSI_FG_RED "X" ANSI_COLOR_RESET);
                printf(" ");
            }
            else if (board[r][c] == 'M') {
                printf(ANSI_FG_RED "M" ANSI_COLOR_RESET);
				printf(" ");
            }
            else {
                if (final_char == '0') {
                    printf(" " ANSI_COLOR_RESET);
					printf(" ");
                }
                else {
                    printf("%c" ANSI_COLOR_RESET, final_char);
					printf(" ");
                }
            }
        }
        printf("|\n");
    }
    printf("   +-");
    for (int c = 0; c < SIZE; c++) {
        printf("--");
    }
    printf("+\n");

    if (board[last_row][last_col] == 'M') {
        printf("You hit a mine! You Lose!\n");
        return false;
    }
    else {
        printf("You won the game!\n");
        return true;
    }
}


void placeMines() {
    int m = 0;
    while (m < NUM_MINES) {
        int r = rand() % SIZE, c = rand() % SIZE;

        if (board[r][c] != 'M') {
            board[r][c] = 'M';
            m++;
        }
    }
}