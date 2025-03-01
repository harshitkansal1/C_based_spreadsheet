#ifndef RECALCULATIONS_H
#define RECALCULATIONS_H

typedef struct __attribute__((packed)) {
    unsigned int row:10;
    unsigned int col:15;
} CellState;

typedef struct {
    CellState *states;
    int top;
    int capacity;
} Stack;

Stack *create_stack(int capacity);
void push(Stack *stack, int row, int col);
int pop(Stack *stack, CellState *state);
void free_stack(Stack *stack);

struct __attribute__((packed)) relation_data {
    int operation:6;
    int i1_row :12;
    int i1_column: 16;
    int i2_row :12;
    int i2_column: 16;
    unsigned int error:1;
};

extern struct relation_data** relation;

int has_cycle(int target_row, int target_col, int current_row, int current_col) ;

int range_has_cycle(int target_row, int target_col);

struct __attribute__((packed)) AVLNode {
    int row :12;
    int col : 16;
    int height: 16;
    struct AVLNode* left;
    struct AVLNode* right;
};

struct __attribute__((packed)) AVLTree {
    struct AVLNode* root;
};

extern struct AVLTree** dependencies;

void initialize_dependencies(int rows, int cols);

void add_dependency(int impactor_row, int impactor_col, int impacted_row, int impacted_col);

void delete_dependencies(int impacted_row, int impacted_col);
void add_dependencies(int , int);

int** get_range_cells(int *start_coords, int *end_coords, int *range_count);

void recalculate(int , int , int** );

void free_relation_graph(void);
void free_dependencies(void);


#endif