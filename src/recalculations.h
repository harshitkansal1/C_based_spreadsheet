
struct relation_data {
    int operation;
    int i1_row;
    int i1_column;
    int i2_row;
    int i2_column;
    int error;
};

extern struct relation_data** relation;

int has_cycle(int target_row, int target_col, int current_row, int current_col) ;

int range_has_cycle(int target_row, int target_col);

struct AVLNode {
    int row;
    int col;
    int height;
    struct AVLNode* left;
    struct AVLNode* right;
};

struct AVLTree {
    struct AVLNode* root;
};

extern struct AVLTree** dependencies;

void initialize_dependencies(int rows, int cols);

void add_dependency(int impactor_row, int impactor_col, int impacted_row, int impacted_col);

void delete_dependencies(int impacted_row, int impacted_col);
void add_dependencies(int , int);

int** get_range_cells(int *start_coords, int *end_coords, int *range_count);

void recalculate(int , int , int** );
