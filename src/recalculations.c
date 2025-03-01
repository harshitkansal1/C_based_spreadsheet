// 1: constant (cons,cons,1)
/// 2: cell equal(cell,samecell,2)
/// 3: min (r1,r2,3)
/// 4: max (r1,r2,4)
/// 5: avg (r1,r2,5)
/// 6: sum (r1,r2,6)
/// 7: stdev (r1,r2,6)
/// 8: add cell with int: arithmetic (_,_,8)
/// 9: subtract cell with int: arithmetic (_,_,9)
/// 10: multiply cell with int: arithmetic (_,_,10)
/// 11: divide cell with int: arithmetic (_,_,11)
/// 12: add cell with cell: arithmetic (_,_,12)
/// 13: subtract cell with cell: arithmetic (_,_,13)
/// 14: multiply cell with cell: arithmetic (_,_,14)
/// 15: divide cell with cell: arithmetic (_,_15)
/// 16: add int with cell: arithmetic (_,_,16)
/// 17: subtract int with cell: arithmetic (_,_,17)
/// 18: multiply int with cell: arithmetic (_,_,18)
/// 19: divide int with cell: arithmetic (_,_,19)
/// 20: sleep cell
#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
#include "recalculations.h"
#include "sheet.h"

#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))


struct relation_data** relation;
struct AVLTree** dependencies;

// Function to create a relation graph dynamically
void create_relation_graph(void) {
    // Allocate memory for an array of pointers (for rows)
    relation = (struct relation_data**)malloc(ROWS * sizeof(struct relation_data*));
    
    // Allocate memory for each row
    for (int i = 0; i < ROWS; ++i) {
        relation[i] = (struct relation_data*)malloc(COLS * sizeof(struct relation_data));
       
    }

    // Initialize the array elements
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            relation[i][j].operation = 0; // Initialize operation or other fields if needed
            relation[i][j].i1_row = -1;
            relation[i][j].i1_column = -1;
            relation[i][j].i2_row = -1;
            relation[i][j].i2_column = -1;         
            relation[i][j].error = 0;
        }
    }
    initialize_dependencies(ROWS,COLS);

}

int** get_range_cells(int *start_coords, int *end_coords, int *range_count) {
    int start_row = start_coords[0], start_col = start_coords[1];
    int end_row = end_coords[0], end_col = end_coords[1];

    // Ensure the range is valid (normalize coordinates if needed)
    if (start_row > end_row) {
        int temp = start_row;
        start_row = end_row;
        end_row = temp;
    }
    if (start_col > end_col) {
        int temp = start_col;
        start_col = end_col;
        end_col = temp;
    }

    // Calculate the number of cells in the range
    int num_rows = end_row - start_row + 1;
    int num_cols = end_col - start_col + 1;
    *range_count = num_rows * num_cols;

    // Dynamically allocate space for the range of cells
    int **range_cells = (int**)malloc((*range_count) * sizeof(int*));
    for (int i = 0; i < *range_count; i++) {
        range_cells[i] = (int*)malloc(2 * sizeof(int)); // Each cell stores (row, col)
    }

    // Populate the range_cells with the coordinates of the range
    int index = 0;
    for (int i = start_row; i <= end_row; i++) {
        for (int j = start_col; j <= end_col; j++) {
            range_cells[index][0] = i;  // Row index
            range_cells[index][1] = j;  // Column index
            index++;
        }
    }

    return range_cells;
}

int has_cycle(int target_row, int target_col, int current_row, int current_col) {
    if (target_row == current_row && target_col == current_col) {
        // Cycle detected: reached back to the target cell
        return 1;
    }

    struct relation_data current_relation = relation[current_row][current_col];

    if (current_relation.operation == 0 || current_relation.operation == 1  ) {
        // No dependencies for this cell
        return 0;
    }


    if (current_relation.operation == 2 || current_relation.operation == 20) {
         if (has_cycle(target_row, target_col, current_relation.i1_row, current_relation.i1_column)) {
            return 1;
        }
        return 0;
    }

    // Check the first dependency (i1_row, i1_column)
    if(current_relation.operation >= 8 && current_relation.operation <=11)
    {
        if (current_relation.i1_row != -1 && current_relation.i1_column != -1) {
            if (has_cycle(target_row, target_col, current_relation.i1_row, current_relation.i1_column)) {
                return 1;
            }
        }
        return 0;
    }

    // Check the second dependency (i2_row, i2_column)
    if(current_relation.operation >= 16 && current_relation.operation <=19)
    {
    if (current_relation.i2_row != -1 && current_relation.i2_column != -1) {
        if (has_cycle(target_row, target_col, current_relation.i2_row, current_relation.i2_column)) {
            return 1;
        }
    }

    return 0;
    }

     if(current_relation.operation >= 12 && current_relation.operation <=15)
    {
    if (current_relation.i2_row != -1 && current_relation.i2_column != -1) {
        if (has_cycle(target_row, target_col, current_relation.i2_row, current_relation.i2_column)) {
            return 1;
        }
    }

    if (current_relation.i1_row != -1 && current_relation.i1_column != -1) {
            if (has_cycle(target_row, target_col, current_relation.i1_row, current_relation.i1_column)) {
                return 1;
            }
        }

    return 0;
    }

    if(current_relation.operation >= 3 && current_relation.operation <=7)
    {
        int range_count_val;
        int* range_count = &range_count_val;

        int coord1[] = {current_relation.i1_row,current_relation.i1_column};
        int coord2[] = {current_relation.i2_row,current_relation.i2_column};

        int** pointer = get_range_cells(coord1,coord2,range_count);

        if(pointer == NULL)
        {
            printf("Malloc error");
            return -1;
        }

        for(int i = 0; i < *range_count;i++)
        {
            if (has_cycle(target_row, target_col, pointer[i][0], pointer[i][1])) {
                return 1;
            }
        }

        int total_cells = *range_count;
        for (int i = 0; i < total_cells; i++) {
            free(pointer[i]);
        }
        free(pointer);

        return 0;
        
    }

    return 0; // No cycle detected
}

int range_has_cycle(int target_row, int target_col)
{
    struct relation_data current_relation = relation[target_row][target_col];

        int range_count_val_1;
        int* range_count_1 = &range_count_val_1;

        int coord1_1[] = {current_relation.i1_row,current_relation.i1_column};
        int coord2_1[] = {current_relation.i2_row,current_relation.i2_column};

        int** pointer_1 = get_range_cells(coord1_1,coord2_1,range_count_1);

        for(int i = 0; i < *range_count_1;i++)
        {
            if (has_cycle(target_row, target_col, pointer_1[i][0], pointer_1[i][1])) {
                int total_cells = *range_count_1;
                for (int i = 0; i < total_cells; i++) {
                    free(pointer_1[i]);
                }
                free(pointer_1);
                return 1;
            }
        }

        

        int total_cells = *range_count_1;
        for (int i = 0; i < total_cells; i++) {
            free(pointer_1[i]);
        }
        free(pointer_1);

        return 0;

}

struct AVLNode* create_node(int row, int col) {
    struct AVLNode* node = (struct AVLNode*)malloc(sizeof(struct AVLNode));
    if (!node) {
        printf("malloc failed\n");
        return NULL;
    }
    node->row = row;
    node->col = col;
    node->height = 1; // Initial height
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Get the height of a node
int get_height(struct AVLNode* node) {
    return node ? node->height : 0;
}

// Calculate balance factor
int balance_factor(struct AVLNode* node) {
    return node ? get_height(node->left) - get_height(node->right) : 0;
}

// Right rotate
struct AVLNode* rotate_right(struct AVLNode* y) {
    struct AVLNode* x = y->left;
    struct AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = 1 + (get_height(y->left) > get_height(y->right) ? get_height(y->left) : get_height(y->right));
    x->height = 1 + (get_height(x->left) > get_height(x->right) ? get_height(x->left) : get_height(x->right));

    return x;
}

// Left rotate
struct AVLNode* rotate_left(struct AVLNode* x) {
    struct AVLNode* y = x->right;
    struct AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = 1 + (get_height(x->left) > get_height(x->right) ? get_height(x->left) : get_height(x->right));
    y->height = 1 + (get_height(y->left) > get_height(y->right) ? get_height(y->left) : get_height(y->right));

    return y;
}

// Insert a dependency into the AVL tree
struct AVLNode* insert_avl(struct AVLNode* node, int row, int col) {
    if (!node) return create_node(row, col);

    if (row < node->row || (row == node->row && col < node->col)) {
        node->left = insert_avl(node->left, row, col);
    } else if (row > node->row || (row == node->row && col > node->col)) {
        node->right = insert_avl(node->right, row, col);
    } else {
        // Duplicate dependencies are not allowed
        return node;
    }

    // Update height of the ancestor node
    node->height = 1 + (get_height(node->left) > get_height(node->right) ? get_height(node->left) : get_height(node->right));

    // Balance the tree
    int balance = balance_factor(node);

    // Left Left Case
    if (balance > 1 && (row < node->left->row || (row == node->left->row && col < node->left->col))) {
        return rotate_right(node);
    }

    // Right Right Case
    if (balance < -1 && (row > node->right->row || (row == node->right->row && col > node->right->col))) {
        return rotate_left(node);
    }

    // Left Right Case
    if (balance > 1 && (row > node->left->row || (row == node->left->row && col > node->left->col))) {
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }

    // Right Left Case
    if (balance < -1 && (row < node->right->row || (row == node->right->row && col < node->right->col))) {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }

    return node;
}

// Find the node with the smallest value in a subtree
struct AVLNode* min_value_node(struct AVLNode* node) {
    struct AVLNode* current = node;
    while (current && current->left) {
        current = current->left;
    }
    return current;
}

// Delete a dependency from the AVL tree
struct AVLNode* delete_avl(struct AVLNode* root, int row, int col) {
    if (!root) return root;

    if (row < root->row || (row == root->row && col < root->col)) {
        root->left = delete_avl(root->left, row, col);
    } else if (row > root->row || (row == root->row && col > root->col)) {
        root->right = delete_avl(root->right, row, col);
    } else {
        // Node with only one child or no child
        if (!root->left || !root->right) {
            struct AVLNode* temp = root->left ? root->left : root->right;
            free(root);
            return temp;
        }

        // Node with two children: Get the inorder successor
        struct AVLNode* temp = min_value_node(root->right);
        root->row = temp->row;
        root->col = temp->col;
        root->right = delete_avl(root->right, temp->row, temp->col);
    }

    // Update height
    root->height = 1 + (get_height(root->left) > get_height(root->right) ? get_height(root->left) : get_height(root->right));

    // Balance the tree
    int balance = balance_factor(root);

    // Left Left Case
    if (balance > 1 && balance_factor(root->left) >= 0) {
        return rotate_right(root);
    }

    // Left Right Case
    if (balance > 1 && balance_factor(root->left) < 0) {
        root->left = rotate_left(root->left);
        return rotate_right(root);
    }

    // Right Right Case
    if (balance < -1 && balance_factor(root->right) <= 0) {
        return rotate_left(root);
    }

    // Right Left Case
    if (balance < -1 && balance_factor(root->right) > 0) {
        root->right = rotate_right(root->right);
        return rotate_left(root);
    }

    return root;
}
int traverseavl(struct AVLNode* root , struct AVLNode** list)
{
    if(root == NULL)
    {
        return 0;
    }
    **list = *root;
    (*list)++;
    return 1 + traverseavl(root->left , list) + traverseavl(root->right , list);
    ;
}

void initialize_dependencies(int rows, int cols) {
    dependencies = (struct AVLTree**)malloc(rows * sizeof(struct AVLTree*));
    for (int i = 0; i < rows; i++) {
        dependencies[i] = (struct AVLTree*)malloc(cols * sizeof(struct AVLTree));
        for (int j = 0; j < cols; j++) {
            dependencies[i][j].root = NULL;
        }
    }
}

void add_dependency(int impactor_row, int impactor_col, int impacted_row, int impacted_col) {
    dependencies[impactor_row][impactor_col].root = insert_avl(dependencies[impactor_row][impactor_col].root, impacted_row, impacted_col);
}

void clear_dependency(int impactor_row, int impactor_col, int impacted_row, int impacted_col) {
    dependencies[impactor_row][impactor_col].root = delete_avl(dependencies[impactor_row][impactor_col].root, impacted_row, impacted_col);
}

void delete_dependencies(int impacted_row, int impacted_col) {
    struct relation_data current_relation = relation[impacted_row][impacted_col];
    
    // No operation
    if (current_relation.operation == 0 || current_relation.operation == 1) {
        return;
    }

    // Single dependency case
    if (current_relation.operation == 2 || current_relation.operation == 20) {
        int row = current_relation.i2_row;
        int col = current_relation.i2_column;
        clear_dependency(row, col, impacted_row, impacted_col);
    }

    // Single dependency case with i1
    if (current_relation.operation >= 8 && current_relation.operation <= 11) {
        int row = current_relation.i1_row;
        int col = current_relation.i1_column;
        clear_dependency(row, col, impacted_row, impacted_col);
    }

    // Single dependency case with i2
    if (current_relation.operation >= 16 && current_relation.operation <= 19) {
        int row = current_relation.i2_row;
        int col = current_relation.i2_column;
        clear_dependency(row, col, impacted_row, impacted_col);
    }

    // Double dependency case
    if (current_relation.operation >= 12 && current_relation.operation <= 15) {
        int row1 = current_relation.i1_row;
        int col1 = current_relation.i1_column;
        int row2 = current_relation.i2_row;
        int col2 = current_relation.i2_column;
        clear_dependency(row1, col1, impacted_row, impacted_col);
        clear_dependency(row2, col2, impacted_row, impacted_col);
    }

    // Range of dependencies
    if (current_relation.operation >= 3 && current_relation.operation <= 7) {
        int range_count_val;
        int* range_count = &range_count_val;

        int coord1[] = {current_relation.i1_row, current_relation.i1_column};
        int coord2[] = {current_relation.i2_row, current_relation.i2_column};

        int** pointer = get_range_cells(coord1, coord2, range_count);

        if (pointer == NULL) {
            printf("Malloc error\n");
            return;
        }

        for (int i = 0; i < *range_count; i++) {
            clear_dependency(pointer[i][0], pointer[i][1], impacted_row, impacted_col);
        }

        // Free memory used for range coordinates
        int total_cells = *range_count;
        for (int i = 0; i < total_cells; i++) {
            free(pointer[i]);
        }
        free(pointer);
    }
}
void add_dependencies(int impacted_row, int impacted_col){
    struct relation_data current_relation = relation[impacted_row][impacted_col];
    
    // No operation
    if (current_relation.operation == 0 || current_relation.operation == 1) {
        return;
    }

    // Single dependency case
    if (current_relation.operation == 2 || current_relation.operation == 20) {
        int row = current_relation.i2_row;
        int col = current_relation.i2_column;
        add_dependency(row, col, impacted_row, impacted_col);
    }

    // Single dependency case with i1
    if (current_relation.operation >= 8 && current_relation.operation <= 11) {
        int row = current_relation.i1_row;
        int col = current_relation.i1_column;
        add_dependency(row, col, impacted_row, impacted_col);
    }

    // Single dependency case with i2
    if (current_relation.operation >= 16 && current_relation.operation <= 19) {
        int row = current_relation.i2_row;
        int col = current_relation.i2_column;
        add_dependency(row, col, impacted_row, impacted_col);
    }

    // Double dependency case
    if (current_relation.operation >= 12 && current_relation.operation <= 15) {
        int row1 = current_relation.i1_row;
        int col1 = current_relation.i1_column;
        int row2 = current_relation.i2_row;
        int col2 = current_relation.i2_column;
        add_dependency(row1, col1, impacted_row, impacted_col);
        add_dependency(row2, col2, impacted_row, impacted_col);
    }

    // Range of dependencies
    if (current_relation.operation >= 3 && current_relation.operation <= 7) {
        int range_count_val;
        int* range_count = &range_count_val;

        int coord1[] = {current_relation.i1_row, current_relation.i1_column};
        int coord2[] = {current_relation.i2_row, current_relation.i2_column};

        int** pointer = get_range_cells(coord1, coord2, range_count);

        if (pointer == NULL) {
            printf("Malloc error\n");
            return;
        }

        for (int i = 0; i < *range_count; i++) {
            add_dependency(pointer[i][0], pointer[i][1], impacted_row, impacted_col);
        }

        // Free memory used for range coordinates
        int total_cells = *range_count;
        for (int i = 0; i < total_cells; i++) {
            free(pointer[i]);
        }
        free(pointer);
    }
}

void toposort(int row, int col, int* visited, int* stack, int* stack_index) {
    visited[row * COLS + col] = 1;

    struct AVLNode* current = dependencies[row][col].root;
    struct AVLNode* list = (struct AVLNode*) malloc(ROWS * COLS * sizeof(struct AVLNode));
    struct AVLNode* list_head = list;
    int size = traverseavl(current , &list);
    list = list_head;
    for (int i = 0; i<size; i++){
        int next_row = list_head->row;
        int next_col = list_head->col;
        if (!visited[next_row * COLS + next_col]) {
            toposort(next_row, next_col, visited, stack, stack_index);
        }
        list_head++;
    }
    free(list);
    stack[(*stack_index)++] = row * COLS + col;
}

void recalculate(int row , int col , int** sheet){
    int *visited = (int*)malloc(ROWS * COLS * sizeof(int));
    int *stack = (int*)malloc(ROWS * COLS * sizeof(int));
    int *stack_index = (int*)malloc(sizeof(int));;
    *stack_index = 0;
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            visited[i * COLS + j] = 0;
        }
    }
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            stack[i * COLS + j] = -1;
        }
    }
    toposort(row, col, visited, stack, stack_index);
    for (int i = *stack_index -1; i>=0; i--){
        int current_row = stack[i] / COLS;
        int current_col = stack[i] % COLS;
        // printf("Current row: %d, Current col: %d\n", current_row, current_col);
        if (relation[current_row][current_col].operation == 0){
            continue;
        }
        if (relation[current_row][current_col].operation == 1){
            relation[current_row][current_col].error = 0;
            continue;
        }
        if (relation[current_row][current_col].operation == 2){
            int row = relation[current_row][current_col].i2_row;
            int col = relation[current_row][current_col].i2_column;
            sheet[current_row][current_col] = sheet[row][col];
            if (relation[row][col].error == 1){
                relation[current_row][current_col].error = 1;
                continue;
            }else relation[current_row][current_col].error = 0;

            continue;
        }
        if (relation[current_row][current_col].operation >= 3 && relation[current_row][current_col].operation <= 7){
            int coord1[] = {relation[current_row][current_col].i1_row, relation[current_row][current_col].i1_column};
            int coord2[] = {relation[current_row][current_col].i2_row, relation[current_row][current_col].i2_column};
            int cur_coords[] = {current_row, current_col};
            char cell1[10];
            char cell2[10];
            char cur_cell[10];
            coords_to_cell(coord1, cell1);
            coords_to_cell(coord2, cell2);
            coords_to_cell(cur_coords, cur_cell);
            // printf("Current cell: %s\n", cur_cell);
            int err = 0;
            for (int i = coord1[0]; i <= coord2[0]; i++){
                for (int j = coord1[1]; j <= coord2[1]; j++){
                    if (relation[i][j].error == 1){
                        relation[current_row][current_col].error = 1;
                        err = 1;
                    }
                }
            }
            if (err == 1){
                relation[current_row][current_col].error = 1;
                continue;
            }else{
                relation[current_row][current_col].error = 0;
            }
            if (relation[current_row][current_col].operation == 3){
                min_range(sheet , cur_cell, cell1, cell2);
            }
            if (relation[current_row][current_col].operation == 4){
                max_range(sheet , cur_cell, cell1, cell2);
            }
            if (relation[current_row][current_col].operation == 5){
                avg_range(sheet , cur_cell, cell1, cell2);
            }
            if (relation[current_row][current_col].operation == 6){
                sum_range(sheet , cur_cell, cell1, cell2);
            }
            if (relation[current_row][current_col].operation == 7){
                std_dev_range(sheet , cur_cell, cell1, cell2);
            }
            continue;
    }
    if (relation[current_row][current_col].operation >= 8 && relation[current_row][current_col].operation <= 11){
        if (relation[relation[current_row][current_col].i1_row][relation[current_row][current_col].i1_column].error == 1){
            relation[current_row][current_col].error = 1;
            continue;
        }else relation[current_row][current_col].error = 0;

        if (relation[current_row][current_col].operation == 8){
            sheet[current_row][current_col] = sheet[relation[current_row][current_col].i1_row][relation[current_row][current_col].i1_column] + relation[current_row][current_col].i2_row;
        }
        if (relation[current_row][current_col].operation == 9){
            sheet[current_row][current_col] = sheet[relation[current_row][current_col].i1_row][relation[current_row][current_col].i1_column] - relation[current_row][current_col].i2_row;
        }
        if (relation[current_row][current_col].operation == 10){
            sheet[current_row][current_col] = sheet[relation[current_row][current_col].i1_row][relation[current_row][current_col].i1_column] * relation[current_row][current_col].i2_row;
        }
        if (relation[current_row][current_col].operation == 11){
            if (relation[current_row][current_col].i2_row == 0){
                relation[current_row][current_col].error = 1;
                continue;
            }
            sheet[current_row][current_col] = sheet[relation[current_row][current_col].i1_row][relation[current_row][current_col].i1_column] / relation[current_row][current_col].i2_row;
        }
        continue;
    }
    if (relation[current_row][current_col].operation >= 12 && relation[current_row][current_col].operation <= 15){
        if (relation[relation[current_row][current_col].i1_row][relation[current_row][current_col].i1_column].error == 1 || relation[relation[current_row][current_col].i2_row][relation[current_row][current_col].i2_column].error == 1){
            relation[current_row][current_col].error = 1;
            continue;
        }else relation[current_row][current_col].error = 0;

        if (relation[current_row][current_col].operation == 12){
            sheet[current_row][current_col] = sheet[relation[current_row][current_col].i1_row][relation[current_row][current_col].i1_column] + sheet[relation[current_row][current_col].i2_row][relation[current_row][current_col].i2_column];
        }
        if (relation[current_row][current_col].operation == 13){
            sheet[current_row][current_col] = sheet[relation[current_row][current_col].i1_row][relation[current_row][current_col].i1_column] - sheet[relation[current_row][current_col].i2_row][relation[current_row][current_col].i2_column];
        }
        if (relation[current_row][current_col].operation == 14){
            sheet[current_row][current_col] = sheet[relation[current_row][current_col].i1_row][relation[current_row][current_col].i1_column] * sheet[relation[current_row][current_col].i2_row][relation[current_row][current_col].i2_column];
        }
        if (relation[current_row][current_col].operation == 15){
            if (sheet[relation[current_row][current_col].i2_row][relation[current_row][current_col].i2_column] == 0){
                relation[current_row][current_col].error = 1;
                continue;
            }
            sheet[current_row][current_col] = sheet[relation[current_row][current_col].i1_row][relation[current_row][current_col].i1_column] / sheet[relation[current_row][current_col].i2_row][relation[current_row][current_col].i2_column];
        }
        continue;
    }
    if (relation[current_row][current_col].operation >= 16 && relation[current_row][current_col].operation <= 19){
        if (relation[relation[current_row][current_col].i2_row][relation[current_row][current_col].i2_column].error == 1){
            relation[current_row][current_col].error = 1;
            continue;
        }else relation[current_row][current_col].error = 0;
        if (relation[current_row][current_col].operation == 16){
            sheet[current_row][current_col] = relation[current_row][current_col].i1_row + sheet[relation[current_row][current_col].i2_row][relation[current_row][current_col].i2_column];
        }
        if (relation[current_row][current_col].operation == 17){
            sheet[current_row][current_col] = relation[current_row][current_col].i1_row - sheet[relation[current_row][current_col].i2_row][relation[current_row][current_col].i2_column];
        }
        if (relation[current_row][current_col].operation == 18){
            sheet[current_row][current_col] = relation[current_row][current_col].i1_row * sheet[relation[current_row][current_col].i2_row][relation[current_row][current_col].i2_column];
        }
        if (relation[current_row][current_col].operation == 19){
            if (sheet[relation[current_row][current_col].i2_row][relation[current_row][current_col].i2_column] == 0){
                relation[current_row][current_col].error = 1;
                continue;
            }
            sheet[current_row][current_col] = relation[current_row][current_col].i1_row / sheet[relation[current_row][current_col].i2_row][relation[current_row][current_col].i2_column];
        }
        continue;
    }
    if (relation[current_row][current_col].operation == 20){
        int cur_coords[] = {current_row, current_col};
        char cur_cell[10];
        coords_to_cell(cur_coords, cur_cell);
        if (relation[relation[current_row][current_col].i1_row][relation[current_row][current_col].i1_column].error == 1){
            relation[current_row][current_col].error = 1;
            continue;
        }else relation[current_row][current_col].error = 0;
        sleep_value(sheet, cur_cell, sheet[relation[current_row][current_col].i1_row][relation[current_row][current_col].i1_column]);
        continue;
    }
    free(visited);
    free(stack);
}
}

