#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "array_tools.h"
#include "sudoku.h"

// read_int_n(input, n, err_msg) attempts to read n integers from console and
//   store them in input. If unsuccessful, the function prints err_msg and exits
//   the program.
// effects:  reads from input
//           may modify input
//           may terminate program
// requires: input must be at least of length n
static void read_int_n(int *input, int n, char *error_message) {
  assert(input);
  assert(n > 0);
  for (int i = 0; i < n; ++i, ++input) {
    if (scanf("%d", input) != 1) {
      printf("%s", error_message);
      exit(EXIT_SUCCESS);
    }
  }
}

int main(void) {
  struct sudoku *su = sudoku_read();
  solution_print(su);

  const int CMD_QUIT    = lookup_symbol("quit");
  const int CMD_RESET   = lookup_symbol("reset");
  const int CMD_PRINT   = lookup_symbol("print");
  const int CMD_FILL    = lookup_symbol("fill");
  const int CMD_ERASE   = lookup_symbol("erase");
  const int CMD_CHOICES = lookup_symbol("choices");
  const int CMD_HINT    = lookup_symbol("hint");
  const int CMD_SOLVE   = lookup_symbol("solve");

  int cmd = INVALID_SYMBOL;
  while ((cmd = read_symbol()) != INVALID_SYMBOL) {
    if (cmd == CMD_QUIT) {
      printf("Thanks for playing. Goodbye!\n");
      break;
    } else if (cmd == CMD_RESET) {
      solution_reset(su);
    } else if (cmd == CMD_PRINT) {
      solution_print(su);
    } else if (cmd == CMD_FILL) {
      int input[3] = { 0 }; // input[0] => row, input[1] => col, input[2] => num
      read_int_n(input, 3, "ERROR: cannot read parameters for command fill\n");
      if (!cell_fill(su, input[0], input[1], input[2])) {
        printf("Cannot put %d in cell (%d,%d).\n", input[2], input[0], input[1]);
      }
    } else if (cmd == CMD_ERASE) {
      int input[2] = { 0 }; // input[0] => row, input[1] => col
      read_int_n(input, 2, "ERROR: cannot read parameters for command erase\n");
      if (!cell_erase(su, input[0], input[1])) {
        printf("Cannot erase cell (%d,%d).\n", input[0], input[1]);
      }
    } else if (cmd == CMD_CHOICES) {
      int input[2] = { 0 }; // input[0] => row, input[1] => col
      read_int_n(input, 2, "ERROR: cannot read parameters for command choices\n");
      int choices[9];
      int choices_len = cell_choices(su, input[0], input[1], choices);
      if (!choices_len) {
        printf("No possible value for cell (%d,%d)\n", input[0], input[1]);
      } else {
        printf("Possible values for cell (%d,%d) are ", input[0], input[1]);
        array_print(choices, choices_len);
      }
    } else if (cmd == CMD_HINT) {
      int row = 0, col = 0;
      if (!cell_hint(su, &row, &col)) {
        printf("Could not find a cell with only one possible value.\n");
      } else {
        printf("Try filling cell (%d,%d)\n", row, col);
      }
    } else if (cmd == CMD_SOLVE) {
      if (!sudoku_solve(su)) {
        printf("Could not solve the puzzle using search and backtracking :(\n");
      } else {
        solution_print(su);
      }
    } else {
      printf("Invalid Command: ");
      print_symbol(cmd);
      printf("\n");
      break;
    }
    // stop if puzzle is solved.
    if (puzzle_solved(su)) {
      printf("Congratulations! You solved the puzzle!\n");
      break;
    }
  }
  sudoku_destroy(su);
}
