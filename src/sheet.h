#ifndef SHEET_H
#define SHEET_H

int cell_to_coords(char*  , int*);
int** initialize_sheet(int, int);
void print_sheet(int** , int);
void process_control_input(int** , char*);
void process_assign_input(int** , char* , char*);
int process_arith_expr(int** , char* , char* , char* , char*);
int process_functions(int** , char* , char* , char* , char*);
extern int ROWS , COLS , enable;


struct relation_data {
    int operation;
    int i1_row;
    int i1_column;
    int i2_row;
    int i2_column;
};

extern struct relation_data** relation;

int has_cycle(int target_row, int target_col, int current_row, int current_col) ;

int range_has_cycle(int target_row, int target_col);

struct DependencyNode {
    int row;
    int col;
    struct DependencyNode* next;
};

struct ImpactList {
    struct DependencyNode* head;
};


extern struct ImpactList** dependencies;

void initialize_dependencies(int rows, int cols);

void add_dependency(int impactor_row, int impactor_col, int impacted_row, int impacted_col);

void find_and_modify_impactors(int impacted_row, int impacted_col);

int** get_range_cells(int *start_coords, int *end_coords, int *range_count);

#endif