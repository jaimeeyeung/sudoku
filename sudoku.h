#include <stdbool.h>

// sudoku holds all information required to play
struct sudoku;

// === WE HAVE IMPLEMENTED THE FOLLOWING FUNCTIONS FOR YOU ====================

// sudoku_read() reads in a Sudoku puzzle from input and returns a pointer to a
//   new sudoku structure.
// effects: reads from input
//			    allocates memory (client must call sudoku_destroy)
struct sudoku *sudoku_read(void);

// sudoku_destroy(su) frees all memory for su
// effects: su is no longer valid
void sudoku_destroy(struct sudoku *su);

// solution_print(s) prints the current solution.
// effects: produces output
void solution_print(const struct sudoku *su);

// solution_reset(su) resets the solution to be the initial puzzle.
// effects: mutates su
void solution_reset(struct sudoku *su);

// cell_erase(su, row, col) tries to set the value of the cell (row,col) to
//   EMPTY. The function returns true if successful, and false otherwise.
// requires: row and col are valid indices
// effects:  might mutate su
bool cell_erase(struct sudoku *su, int row, int col);

// cell_fill(su, row, col, num) tries to fill num in the cell (row,col). The
//   function returns true if successful, and false if filling (row,col) with
//   num would violate any of the row-, column-, or box-constraints.
// requires: row and col are valid indices. 
//           num is an integer between 1 and 9.
// effects:  might mutate su
bool cell_fill(struct sudoku *su, int row, int col, int num);

// puzzle_solved(su) returns true if su holds a valid solution to the puzzle, 
//   and false otherwise.
bool puzzle_solved(const struct sudoku *su);

// cell_choices(su, row, col, choices) determines all the possible values for
//   (row,col) that do not violate any of the row-, column-, and box-
//   constraints. The function writes all possible values for (row,col) into
//   choices. The function returns the number of choices.
// requires: row and col are valid indices
//           choices is a valid array with length DIM
// effects:  might mutate choices
int cell_choices(const struct sudoku *su, int row, int col, int choices[]);

// cell_hint(su, row, col) tries to find a cell that has only one possible
//   value. If such a cell exists, the function mutates row and col to be the
//   indices of that cell. The function returns true if such a cell exists,
//   and false otherwise.
// requires: row and col are valid pointers.
// effects:  might mutate row and col
bool cell_hint(const struct sudoku *su, int *row, int *col);

// sudoku_solve(su) solves the Sudoku puzzle using search and backtracking. The
//   function returns true if su has a valid solution, and false otherwise.
// effects: might mutate *su
bool sudoku_solve(struct sudoku *su);
