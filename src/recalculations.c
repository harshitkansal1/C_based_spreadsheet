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
#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
#include "sheet.h"

struct relation_data** relation;
struct ImpactList** dependencies;


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
            relation[i][j].i1_row = 0;
            relation[i][j].i1_column = 0;
            relation[i][j].i2_row = 0;
            relation[i][j].i2_column = 0;         
        
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

    if (current_relation.operation == 0 || current_relation.operation == 1) {
        // No dependencies for this cell
        return 0;
    }


    if (current_relation.operation == 2) {

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

void initialize_dependencies(int rows, int cols) {
    dependencies = malloc(rows * sizeof(struct ImpactList*));
    for (int i = 0; i < rows; i++) {
        dependencies[i] = malloc(cols * sizeof(struct ImpactList));
        for (int j = 0; j < cols; j++) {
            dependencies[i][j].head = NULL;
        }
    }
}

void add_dependency(int impactor_row, int impactor_col, int impacted_row, int impacted_col) {
    struct DependencyNode* new_node = malloc(sizeof(struct DependencyNode));

    if(new_node == NULL)
    {
        printf("malloc failed");
        return;
    }
    new_node->row = impacted_row;
    new_node->col = impacted_col;
    new_node->next = dependencies[impactor_row][impactor_col].head; // Add to the head of the list
    dependencies[impactor_row][impactor_col].head = new_node;
}

void clear_dependency(int impactor_row, int impactor_col, int impacted_row, int impacted_col) {
    struct DependencyNode* iter = dependencies[impactor_row][impactor_col].head;

    if(iter->row == impacted_row && iter->col ==impacted_col)
    {
        
        dependencies[impactor_row][impactor_col].head = iter->next;
        return;
    }

    iter = iter->next;
    struct DependencyNode* prev = dependencies[impactor_row][impactor_col].head;

    while(iter->row != impacted_row || iter->col != impacted_col)
    {
        if (iter->next == NULL)
        {
            printf("this is the part causing problem");
            return;
        }
        iter = iter->next;
        prev = prev->next;
    }

    prev->next = iter->next;

}

void find_and_modify_impactors(int impacted_row, int impacted_col)
{
    struct relation_data current_relation = relation[impacted_row][impacted_col];

    if(current_relation.operation == 0)
    {
        return;
    }
    if(current_relation.operation == 1)
    {
        return;
    }
    if(current_relation.operation == 2)
    {
        int row = current_relation.i2_row;
        int col = current_relation.i2_column;

        clear_dependency(row,col,impacted_row,impacted_col);
    }
    if(current_relation.operation >= 8 && current_relation.operation <= 11)
    {
        int row = current_relation.i1_row;
        int col = current_relation.i1_column;

        clear_dependency(row,col,impacted_row,impacted_col);
    }

    if(current_relation.operation >= 16 && current_relation.operation <= 19)
    {
        int row = current_relation.i2_row;
        int col = current_relation.i2_column;

        clear_dependency(row,col,impacted_row,impacted_col);
    }

    if(current_relation.operation >= 12 && current_relation.operation <= 15)
    {
        int row1 = current_relation.i1_row;
        int col1 = current_relation.i1_column;

        int row2 = current_relation.i2_row;
        int col2 = current_relation.i2_column;

        clear_dependency(row1,col1,impacted_row,impacted_col);
        clear_dependency(row2,col2,impacted_row,impacted_col);

    }


    if(current_relation.operation >= 3 && current_relation.operation <= 7)
    {
        int range_count_val;
        int* range_count = &range_count_val;

        int coord1[] = {current_relation.i1_row,current_relation.i1_column};
        int coord2[] = {current_relation.i2_row,current_relation.i2_column};

        int** pointer = get_range_cells(coord1,coord2,range_count);

        if(pointer == NULL)
        {
            printf("Malloc error");
            return;
        }

        for(int i = 0; i < *range_count;i++)
        {
            clear_dependency(pointer[i][0],pointer[i][1],impacted_row,impacted_col);
        }

        int total_cells = *range_count;
        for (int i = 0; i < total_cells; i++) {
            free(pointer[i]);
        }
        free(pointer);


    }
    


}