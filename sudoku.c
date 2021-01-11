#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"
#include "merge_sort.h"

// DIM is the dimension of the Sudoku board
#define DIM 9

// DIMBOX is the dimension of a box
static const int DIMBOX = 3;
// EMPTY is the code for an empty cell
static const int EMPTY = 0;
// PRINT holds the characters for printing out the Sudoku board
static const char PRINT[] = { '_', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
// SUCCESS indicates the successful execution of a function
static const int SUCCESS = 0;
// CONSTRAINT_VIOLATED indicates that a row-, column, or box-constraint is
//   violated
static const int CONSTRAINT_VIOLATED = -1;

struct sudoku {
  // puzzle contains the start state of the puzzle
  int puzzle[DIM * DIM];
  // solution contains is equal to puzzle at the beginning and then stores
  //   all numbers filled in by the player.
  int solution[DIM * DIM];
};

// see sudoku.h for documentation
struct sudoku *sudoku_read(void) {
  struct sudoku *su = malloc(sizeof(struct sudoku));
  assert(su);
  int *pPuz = su->puzzle, *pSol = su->solution;
  for (int cnt = 0; cnt < DIM * DIM; ++cnt, ++pPuz, ++pSol) {
    char c = '\0';
    assert(scanf(" %c", &c) == 1);
    *pPuz = *pSol = (c == PRINT[EMPTY]) ? 0 : c - '0';
  }
  return su;
}

// see sudoku.h for documentation
void sudoku_destroy(struct sudoku *su) {
  assert(su);
  free(su);
}

// solution_print_separator() prints out a separator between boxes.
// produces output
static void solution_print_separator(void) {
  for (int box = 0; box < DIM / DIMBOX; ++box) {
    printf("+");
    for (int i = 0; i < 3 * DIMBOX; ++i) {
      printf("-");
    }
  }
  printf("+\n");
}

// see sudoku.h for documentation
void solution_print(const struct sudoku *su) {
  assert(su);

  for (int r = 0; r < DIM; ++r) {
    if (r % DIMBOX == 0) {
      solution_print_separator();
    }
    for (int c = 0; c < DIM; ++c) {
      if (c % DIMBOX == 0) {
        printf("|");
      }
      printf(" %c ", PRINT[su->solution[r * DIM + c]]);
    }
    printf("|\n");
  }
  solution_print_separator();
  printf("\n");
}

// see sudoku.h for documentation
void solution_reset(struct sudoku *su) {
  assert(su);
  for (int row = 0; row < DIM; ++row) {
    for (int col = 0; col < DIM; ++col) {
      su->solution[row * DIM + col] = su->puzzle[row * DIM + col];
    }
  }  
}

// see sudoku.h for documentation
bool cell_erase(struct sudoku *su, int row, int col) {
  assert(su);
  assert(0 <= row && row <= DIM - 1);
  assert(0 <= col && col <= DIM - 1);

  if (su->puzzle[row * DIM + col] != EMPTY) {
    return false;
  } else {
    su->solution[row * DIM + col] = EMPTY;
    return true;
  }
}

// see sudoku.h for documentation
bool cell_fill(struct sudoku *su, int row, int col, int num) { // check this

  // if we find the same num in the same row, false
  for (int i = 0; i < DIM; ++i) {
    if (su->solution[row * DIM + i] == num && (i + row != col + row) && su->solution[row * DIM + i] != EMPTY) { 
      return false;
    }
  }

  // if we find the same num in the same col, false
  for (int j = 0; j < DIM; ++j) {
    if(su -> solution[j * DIM + col] == num && (j + col != col+row) && su->solution[j * DIM + col] != EMPTY) {
      return false;
    }
  }

  // checking if there is the same num in the 3X3 box
  int box1 = col/3;
  int box2 = row/3;

  for(int i = box2*3; i < box2*3+3; ++i) {
    for (int j = box1*3; j < box1*3+3; ++j) {
      if (((su -> solution [i * DIM + j]) == num) && (i + j != col + row) && su->solution[i * DIM + j] != EMPTY) { 
        return false;
      }
    }
  }

  su -> solution[row * DIM + col] = num;
  return true;
}

// see sudoku.h for documentation
bool puzzle_solved(const struct sudoku *su) {

  // checking to see if each col has exactly one of each number, (1 - 9)
  int row = 0;
  int col = 0;
  int num_counter = 0;
  int num = su -> solution[row * DIM + col];

  for (col = 0; col < DIM; ++col) { 
    for (row = 0; row < DIM; ++ row) {
      if(su -> solution[row * DIM + col] == num && (su -> solution[row * DIM + col] != EMPTY)) {
        num_counter += 1;
      } else if (su -> solution[row * DIM + col] == EMPTY) {
        return false;
      }
    }
  }

  if (num_counter > 1) {
    return CONSTRAINT_VIOLATED;
  }

  // checking to see if each row has exactly one of each number (1 - 9)
  num_counter = 0;
  row = 0;
  col = 0;
  for (row = 0; row < DIM; ++row) {
    for (col = 0; col < DIM; ++col) {
      if (su -> solution[row * DIM + col] == num && (su -> solution[row * DIM + col] != EMPTY)) {
        num_counter += 1;
      } else if (su -> solution[row * DIM + col] == EMPTY) {
        return false;
      }
    }
  }

  if (num_counter > 1) {
    return CONSTRAINT_VIOLATED;
  }

  // checking to see if each 3X3 grid has exactly one of each number 
  int box1 = col / 3;
  int box2 = row / 3;
  num_counter = 0;
  row = 0;
  col = 0;
  for (int row2 = box2 * 3; row2 < box2 * 3 + 3; ++row2) {
    for (int col2 = box1 * 3; col2 < box1 * 3 + 3; ++col2) {
      if ((su -> solution[row2 * DIM + col2] == num) && (su -> solution[row * DIM + col]) != EMPTY && (row2 + col2 != row + col)) {
        num_counter += 1;
      } else if (su -> solution[row * DIM + col] == EMPTY) {
        return false;
      }
    }
  }

  if (num_counter > 1) {
    return CONSTRAINT_VIOLATED;
  }
  return true; 
}

// checks to see if value is in array
bool in_array(int val, int arr[], int arr_len) {
  for (int i = 0; i < arr_len; i++) {
    if (arr[i] == val) {
      return true;
    }
  }
  return false;
}


// see sudoku.h for documentation
int cell_choices(const struct sudoku *su, int row, int col, int choices[]) {
  int options[9] = {1,2,3,4,5,6,7,8,9};
  int missing_val[30]; // array storing missing values in row/col/sq
  int val_row[9]; // array to store values from row
  int val_col[9]; // stores values from col
  int val_square[9]; // stores values from 3X3
  int index_r = 0; // which element is next in row
  int index_c = 0; // which element is next in col
  int index_square = 0; // which element is next in 3X3 square
  int index = 0; // tracking missing value array

  // placing all existing values from row into array
  for (int i = 0; i < DIM; ++i) {
    val_row[index_r++] = su -> solution[row * DIM + i];
  }

  merge_sort(val_row, DIM); // sorts row into asc order

  // check row for missing values between (1 - 9)
  // call compare function
  for (int i = 0; i < DIM; ++i) {
    if (in_array(options[i], val_row, DIM)) { 
      continue;
    } else {
      if (!in_array(options[i], val_row, DIM) && !in_array(options[i], missing_val, DIM)) { // missing val instead of val row
        missing_val[index++] = options[i];
      } else {
        continue;
      }
    }
  }

  // placing all existing values from col into array
  for (int j = 0; j < DIM; ++j) {
    val_col[index_c++] = su -> solution[j * DIM + col];
  }

  merge_sort(val_col, DIM); // sorts values in row into asc order

  for (int i = 0; i < DIM; ++i) {
    if (!in_array(options[i], val_col, DIM) && (!in_array(options[i], missing_val, DIM))) {
      missing_val[index++] = options[i];
    }
  }

  // placing all existing values from 3X3 into array

  int box1 = col/3;
  int box2 = row/3;
  for (int row2 = box2*3; row2 < box2*3+3; ++row2) {
    for (int col2 = box1*3; col2 < box1*3+3; ++col2) {
      val_square[index_square++] = (su -> solution[row2 * DIM + col2]);
    }
  }

  merge_sort(val_square, DIM); // sorts values in square in asc order

  for (int i = 0; i < DIM; ++i) {
    if (!in_array(options[i], val_square, DIM) && (!in_array(options[i], missing_val, DIM))) {
      missing_val[index++] = options[i];
    }
  }

  // if missing value is in val_square, remove it
  for (int i = 0; i < DIM; ++i) {
    if (in_array(missing_val[i], val_square, DIM) || in_array(missing_val[i], val_col, DIM) || in_array(missing_val[i], val_row, DIM)) {
      missing_val[i] = EMPTY;
    } else {
      continue;
    }
  }

  merge_sort(missing_val, 9);

  index = 0;
  int len_count = 0;
  for (int b = 0; b < DIM; ++b) {
    if (missing_val[b] > 0) {
      len_count += 1;
      choices[index++] = missing_val[b];
    }
  }
  return len_count;
}


// see sudoku.h for documentation
bool cell_hint(const struct sudoku *su, int *row, int *col) {
  int choices[9];// = { 0 };
  for (int j = 0; j < DIM; ++j) { // checking down each row
    for (int i = 0; i < DIM; ++i) { // checking each elem in col
      if (su -> solution[j * DIM + i] == EMPTY) {
        if (cell_choices(su, j, i, choices) == 1) {
          *row = j;
          *col = i;
          return true;
        } else {
          continue;
        }
      } else if (su -> solution[j * DIM + i] != EMPTY) {
        continue;
      }
    }
  }
  return false;
}

// finding empty cell
bool finding_empty(struct sudoku *su, int *row, int *col) {
  for (int i = 0; i < DIM; ++i) { // row
    for (int j = 0; j < DIM; ++j) { // col
      if (su -> solution[i * DIM +  j] == EMPTY) {
        *row = i; 
        *col = j; 
        return true;
      } 
    }
  }
  return false;
}

// checks to see if it cell is valid
bool is_ok(struct sudoku *su, int row, int col, int num) {

  // if we find the same num in the same row, false
  for (int i = 0; i < DIM; ++i) {
    if (su->solution[row * DIM + i] == num && (i + row != col + row) && su->solution[row * DIM + i] != EMPTY) { 
      return false;
    }
  }

  // if we find the same num in the same col, false
  for (int j = 0; j < DIM; ++j) {
    if(su -> solution[j * DIM + col] == num && (j + col != col+row) && su->solution[j * DIM + col] != EMPTY) {
      return false; 
    }
  }

  // checking if there is the same num in the 3X3 box
  int row_start = (row/3)*3;
  int col_start = (col/3)*3;

  for(int i = row_start; i < row_start+3; ++i) {
    for (int j = col_start; j < col_start+3; ++j) {
      if (((su -> solution [i * DIM + j]) == num) && su->solution[i * DIM + j] != EMPTY) { 
        return false;
      }
    }
  }
  return true;
}

// see sudoku.h for documentation
bool sudoku_solve(struct sudoku *su) { 
  int row = 0;
  int col = 0;

  if (puzzle_solved(su)) {
    return true;
  }

  (finding_empty(su, &row, &col));
  for (int num = 1; num <= DIM; num++) {
    if (is_ok(su, row, col, num)) { //?
      cell_fill(su, row, col, num);
      if (sudoku_solve(su)) {
        return true;
      } 
      su -> solution[row * DIM + col] = EMPTY;

    }
  }
  return false;
}
