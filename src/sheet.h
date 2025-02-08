#ifndef SHEET_H
#define SHEET_H

void coords_to_cell(int* , char*);
int cell_to_coords(char*  , int*);
int** initialize_sheet(int, int);
void print_sheet(int** , int);
void process_control_input(int** , char*);
void process_assign_input(int** , char* , char*);
int process_arith_expr(int** , char* , char* , char* , char*);
int process_functions(int** , char* , char* , char* , char*);
extern int ROWS , COLS , enable;

#endif