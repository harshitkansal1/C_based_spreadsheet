// 1: constant (cons,cons,1)
// 2: cell equal(cell,samecell,2)
// 3: min (r1,r2,3)
// 4: max (r1,r2,4)
// 5: avg (r1,r2,5)
// 6: sum (r1,r2,6)
// 7: stdev (r1,r2,7)
// 8: add cell with int: arithmetic (_,_,8)
// 9: subtract cell with int: arithmetic (_,_,9)
// 10: multiply cell with int: arithmetic (_,_,10)
// 11: divide cell with int: arithmetic (_,_,11)
// 12: add cell with cell: arithmetic (_,_,12)
// 13: subtract cell with cell: arithmetic (_,_,13)
// 14: multiply cell with cell: arithmetic (_,_,14)
// 15: divide cell with cell: arithmetic (_,_,15)
// 16: add int with cell: arithmetic (_,_,16)
// 17: subtract int with cell: arithmetic (_,_,17)
// 18: multiply int with cell: arithmetic (_,_,18)
// 19: divide int with cell: arithmetic (_,_,19)
// 20: sleep cell
#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
#include "recalculations.h"
#include "sheet.h"

#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))

struct relation_data** relation;
struct DependencyList** dependencies;

void create_relation_graph(void) {
    relation = (struct relation_data**)malloc(ROWS * sizeof(struct relation_data*));
    for (int i = 0; i < ROWS; ++i) {
        relation[i] = (struct relation_data*)malloc(COLS * sizeof(struct relation_data));
    }

    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            relation[i][j].operation = 0;
            relation[i][j].i1_row = -1;
            relation[i][j].i1_column = -1;
            relation[i][j].i2_row = -1;
            relation[i][j].i2_column = -1;
            relation[i][j].error = 0;
        }
    }
    initialize_dependencies(ROWS, COLS);
}

int** get_range_cells(int *start_coords, int *end_coords, int *range_count) {
    int start_row = start_coords[0], start_col = start_coords[1];
    int end_row = end_coords[0], end_col = end_coords[1];

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

    int num_rows = end_row - start_row + 1;
    int num_cols = end_col - start_col + 1;
    *range_count = num_rows * num_cols;

    int **range_cells = (int**)malloc((*range_count) * sizeof(int*));
    for (int i = 0; i < *range_count; i++) {
        range_cells[i] = (int*)malloc(2 * sizeof(int));
    }

    int index = 0;
    for (int i = start_row; i <= end_row; i++) {
        for (int j = start_col; j <= end_col; j++) {
            range_cells[index][0] = i;
            range_cells[index][1] = j;
            index++;
        }
    }

    return range_cells;
}

int has_cycle(int target_row, int target_col, int current_row, int current_col) {
    Stack *stack = create_stack(ROWS * COLS);
    if (!stack) return -1;
    int *visited = calloc(ROWS * COLS, sizeof(int));
    if (!visited) { free_stack(stack); return -1; }

    push(stack, current_row, current_col);

    while (stack->top >= 0) {
        CellState state;
        if (!pop(stack, &state)) break;

        int row = state.row, col = state.col;
        if (row == target_row && col == target_col) {
            free_stack(stack);
            free(visited);
            return 1;
        }

        int index = row * COLS + col;
        if (visited[index]) continue;
        visited[index] = 1;

        struct relation_data rel = relation[row][col];
        if (rel.operation == 0 || rel.operation == 1) continue;

        int coords1[2] = {rel.i1_row, rel.i1_column};
        int coords2[2] = {rel.i2_row, rel.i2_column};

        if (rel.operation == 2 || rel.operation == 20) {
            if (coords1[0] != -1 && coords1[1] != -1 && !visited[coords1[0] * COLS + coords1[1]])
                push(stack, coords1[0], coords1[1]);
        } else if (rel.operation >= 8 && rel.operation <= 11) {
            if (coords1[0] != -1 && coords1[1] != -1 && !visited[coords1[0] * COLS + coords1[1]])
                push(stack, coords1[0], coords1[1]);
        } else if (rel.operation >= 16 && rel.operation <= 19) {
            if (coords2[0] != -1 && coords2[1] != -1 && !visited[coords2[0] * COLS + coords2[1]])
                push(stack, coords2[0], coords2[1]);
        } else if (rel.operation >= 12 && rel.operation <= 15) {
            if (coords1[0] != -1 && coords1[1] != -1 && !visited[coords1[0] * COLS + coords1[1]])
                push(stack, coords1[0], coords1[1]);
            if (coords2[0] != -1 && coords2[1] != -1 && !visited[coords2[0] * COLS + coords2[1]])
                push(stack, coords2[0], coords2[1]);
        } else if (rel.operation >= 3 && rel.operation <= 7) {
            int range_count;
            int **range_cells = get_range_cells(coords1, coords2, &range_count);
            if (!range_cells) { free_stack(stack); free(visited); return -1; }
            for (int i = 0; i < range_count; i++) {
                int r = range_cells[i][0], c = range_cells[i][1];
                if (!visited[r * COLS + c]) push(stack, r, c);
            }
            for (int i = 0; i < range_count; i++) free(range_cells[i]);
            free(range_cells);
        }
    }

    free_stack(stack);
    free(visited);
    return 0;
}

int range_has_cycle(int target_row, int target_col) {
    Stack *stack = create_stack(ROWS * COLS);
    if (!stack) return -1;
    int *visited = calloc(ROWS * COLS, sizeof(int));
    if (!visited) { free_stack(stack); return -1; }

    struct relation_data rel = relation[target_row][target_col];
    int range_count;
    int coord1[] = {rel.i1_row, rel.i1_column};
    int coord2[] = {rel.i2_row, rel.i2_column};
    int **range_cells = get_range_cells(coord1, coord2, &range_count);
    if (!range_cells) { free_stack(stack); free(visited); return -1; }

    for (int i = 0; i < range_count; i++)
        push(stack, range_cells[i][0], range_cells[i][1]);

    while (stack->top >= 0) {
        CellState state;
        if (!pop(stack, &state)) break;

        int row = state.row, col = state.col;
        if (row == target_row && col == target_col) {
            for (int i = 0; i < range_count; i++) free(range_cells[i]);
            free(range_cells);
            free_stack(stack);
            free(visited);
            return 1;
        }

        int index = row * COLS + col;
        if (visited[index]) continue;
        visited[index] = 1;

        rel = relation[row][col];
        if (rel.operation == 0 || rel.operation == 1) continue;

        int coords1[2] = {rel.i1_row, rel.i1_column};
        int coords2[2] = {rel.i2_row, rel.i2_column};

        if (rel.operation == 2 || rel.operation == 20) {
            if (coords1[0] != -1 && coords1[1] != -1 && !visited[coords1[0] * COLS + coords1[1]])
                push(stack, coords1[0], coords1[1]);
        } else if (rel.operation >= 8 && rel.operation <= 11) {
            if (coords1[0] != -1 && coords1[1] != -1 && !visited[coords1[0] * COLS + coords1[1]])
                push(stack, coords1[0], coords1[1]);
        } else if (rel.operation >= 16 && rel.operation <= 19) {
            if (coords2[0] != -1 && coords2[1] != -1 && !visited[coords2[0] * COLS + coords2[1]])
                push(stack, coords2[0], coords2[1]);
        } else if (rel.operation >= 12 && rel.operation <= 15) {
            if (coords1[0] != -1 && coords1[1] != -1 && !visited[coords1[0] * COLS + coords1[1]])
                push(stack, coords1[0], coords1[1]);
            if (coords2[0] != -1 && coords2[1] != -1 && !visited[coords2[0] * COLS + coords2[1]])
                push(stack, coords2[0], coords2[1]);
        } else if (rel.operation >= 3 && rel.operation <= 7) {
            int sub_range_count;
            int **sub_cells = get_range_cells(coords1, coords2, &sub_range_count);
            if (!sub_cells) { free_stack(stack); free(visited); return -1; }
            for (int i = 0; i < sub_range_count; i++) {
                int r = sub_cells[i][0], c = sub_cells[i][1];
                if (!visited[r * COLS + c]) push(stack, r, c);
            }
            for (int i = 0; i < sub_range_count; i++) free(sub_cells[i]);
            free(sub_cells);
        }
    }

    for (int i = 0; i < range_count; i++) free(range_cells[i]);
    free(range_cells);
    free_stack(stack);
    free(visited);
    return 0;
}

// Create a new dependency node
struct DependencyNode* create_node(int row, int col) {
    struct DependencyNode* node = (struct DependencyNode*)malloc(sizeof(struct DependencyNode));
    if (!node) {
        printf("malloc failed\n");
        return NULL;
    }
    node->row = row;
    node->col = col;
    node->next = NULL;
    return node;
}

// Insert a dependency into the linked list
void insert_dependency(struct DependencyNode** head, int row, int col) {
    struct DependencyNode* new_node = create_node(row, col);
    if (!new_node) return;

    // Check for duplicates
    struct DependencyNode* current = *head;
    while (current) {
        if (current->row == row && current->col == col) {
            free(new_node);
            return; // Duplicate found, no insertion
        }
        current = current->next;
    }

    new_node->next = *head;
    *head = new_node;
}

// Delete a specific dependency from the linked list
void delete_dependency(struct DependencyNode** head, int row, int col) {
    struct DependencyNode *current = *head, *prev = NULL;
    while (current) {
        if (current->row == row && current->col == col) {
            if (prev) {
                prev->next = current->next;
            } else {
                *head = current->next;
            }
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

// Traverse the linked list and count nodes (or collect them if needed)
int traverse_list(struct DependencyNode* head, struct DependencyNode** list) {
    int count = 0;
    struct DependencyNode* current = head;
    while (current) {
        if (list) {
            **list = *current;
            (*list)++;
        }
        count++;
        current = current->next;
    }
    return count;
}

void initialize_dependencies(int rows, int cols) {
    dependencies = (struct DependencyList**)malloc(rows * sizeof(struct DependencyList*));
    for (int i = 0; i < rows; i++) {
        dependencies[i] = (struct DependencyList*)malloc(cols * sizeof(struct DependencyList));
        for (int j = 0; j < cols; j++) {
            dependencies[i][j].head = NULL;
        }
    }
}

void add_dependency(int impactor_row, int impactor_col, int impacted_row, int impacted_col) {
    insert_dependency(&dependencies[impactor_row][impactor_col].head, impacted_row, impacted_col);
}

void clear_dependency(int impactor_row, int impactor_col, int impacted_row, int impacted_col) {
    delete_dependency(&dependencies[impactor_row][impactor_col].head, impacted_row, impacted_col);
}

void delete_dependencies(int impacted_row, int impacted_col) {
    struct relation_data current_relation = relation[impacted_row][impacted_col];
    
    if (current_relation.operation == 0 || current_relation.operation == 1) {
        return;
    }

    if (current_relation.operation == 2 || current_relation.operation == 20) {
        int row = current_relation.i2_row;
        int col = current_relation.i2_column;
        clear_dependency(row, col, impacted_row, impacted_col);
    }

    if (current_relation.operation >= 8 && current_relation.operation <= 11) {
        int row = current_relation.i1_row;
        int col = current_relation.i1_column;
        clear_dependency(row, col, impacted_row, impacted_col);
    }

    if (current_relation.operation >= 16 && current_relation.operation <= 19) {
        int row = current_relation.i2_row;
        int col = current_relation.i2_column;
        clear_dependency(row, col, impacted_row, impacted_col);
    }

    if (current_relation.operation >= 12 && current_relation.operation <= 15) {
        int row1 = current_relation.i1_row;
        int col1 = current_relation.i1_column;
        int row2 = current_relation.i2_row;
        int col2 = current_relation.i2_column;
        clear_dependency(row1, col1, impacted_row, impacted_col);
        clear_dependency(row2, col2, impacted_row, impacted_col);
    }

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

        int total_cells = *range_count;
        for (int i = 0; i < total_cells; i++) {
            free(pointer[i]);
        }
        free(pointer);
    }
}

void add_dependencies(int impacted_row, int impacted_col) {
    struct relation_data current_relation = relation[impacted_row][impacted_col];
    
    if (current_relation.operation == 0 || current_relation.operation == 1) {
        return;
    }

    if (current_relation.operation == 2 || current_relation.operation == 20) {
        int row = current_relation.i2_row;
        int col = current_relation.i2_column;
        add_dependency(row, col, impacted_row, impacted_col);
    }

    if (current_relation.operation >= 8 && current_relation.operation <= 11) {
        int row = current_relation.i1_row;
        int col = current_relation.i1_column;
        add_dependency(row, col, impacted_row, impacted_col);
    }

    if (current_relation.operation >= 16 && current_relation.operation <= 19) {
        int row = current_relation.i2_row;
        int col = current_relation.i2_column;
        add_dependency(row, col, impacted_row, impacted_col);
    }

    if (current_relation.operation >= 12 && current_relation.operation <= 15) {
        int row1 = current_relation.i1_row;
        int col1 = current_relation.i1_column;
        int row2 = current_relation.i2_row;
        int col2 = current_relation.i2_column;
        add_dependency(row1, col1, impacted_row, impacted_col);
        add_dependency(row2, col2, impacted_row, impacted_col);
    }

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

        int total_cells = *range_count;
        for (int i = 0; i < total_cells; i++) {
            free(pointer[i]);
        }
        free(pointer);
    }
}

void toposort(int row, int col, int* visited, int* stack, int* stack_index) {
    Stack *call_stack = create_stack(ROWS * COLS);
    if (!call_stack) return;

    push(call_stack, row, col);

    while (call_stack->top >= 0) {
        CellState state;
        if (!pop(call_stack, &state)) break;

        int current_row = state.row, current_col = state.col;
        int index = current_row * COLS + current_col;

        if (visited[index] == 1) {
            stack[(*stack_index)++] = index;
            continue;
        }

        visited[index] = 1;

        push(call_stack, current_row, current_col);

        struct DependencyNode *list = malloc(ROWS * COLS * sizeof(struct DependencyNode));
        if (!list) { free_stack(call_stack); return; }
        struct DependencyNode *list_head = list;
        int size = traverse_list(dependencies[current_row][current_col].head, &list);
        list = list_head;
        for (int i = size - 1; i >= 0; i--) {
            int next_row = list[i].row, next_col = list[i].col;
            if (!visited[next_row * COLS + next_col])
                push(call_stack, next_row, next_col);
        }
        free(list);
    }

    free_stack(call_stack);
}

void recalculate(int row, int col, int** sheet) {
    int *visited = (int*)malloc(ROWS * COLS * sizeof(int));
    if (!visited) return;

    int *stack = (int*)malloc(ROWS * COLS * sizeof(int));
    if (!stack) {
        free(visited);
        return;
    }

    int *stack_index = (int*)malloc(sizeof(int));
    if (!stack_index) {
        free(stack);
        free(visited);
        return;
    }

    *stack_index = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            visited[i * COLS + j] = 0;
        }
    }
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            stack[i * COLS + j] = -1;
        }
    }
    toposort(row, col, visited, stack, stack_index);
    for (int i = *stack_index - 1; i >= 0; i--) {
        int current_row = stack[i] / COLS;
        int current_col = stack[i] % COLS;
        if (relation[current_row][current_col].operation == 0) {
            continue;
        }
        if (relation[current_row][current_col].operation == 1) {
            continue;
        }
        if (relation[current_row][current_col].operation == 2) {
            int row_dep = relation[current_row][current_col].i2_row;
            int col_dep = relation[current_row][current_col].i2_column;
            sheet[current_row][current_col] = sheet[row_dep][col_dep];
            if (relation[row_dep][col_dep].error == 1) {
                relation[current_row][current_col].error = 1;
                continue;
            } else {
                relation[current_row][current_col].error = 0;
            }
            continue;
        }
        if (relation[current_row][current_col].operation >= 3 && relation[current_row][current_col].operation <= 7) {
            int coord1[] = {relation[current_row][current_col].i1_row, relation[current_row][current_col].i1_column};
            int coord2[] = {relation[current_row][current_col].i2_row, relation[current_row][current_col].i2_column};
            int cur_coords[] = {current_row, current_col};
            char cell1[10];
            char cell2[10];
            char cur_cell[10];
            coords_to_cell(coord1, cell1);
            coords_to_cell(coord2, cell2);
            coords_to_cell(cur_coords, cur_cell);
            int err = 0;
            for (int i = coord1[0]; i <= coord2[0]; i++) {
                for (int j = coord1[1]; j <= coord2[1]; j++) {
                    if (relation[i][j].error == 1) {
                        relation[current_row][current_col].error = 1;
                        err = 1;
                    }
                }
            }
            if (err == 1) {
                relation[current_row][current_col].error = 1;
                continue;
            } else {
                relation[current_row][current_col].error = 0;
            }
            if (relation[current_row][current_col].operation == 3) {
                min_range(sheet, cur_cell, cell1, cell2);
            }
            if (relation[current_row][current_col].operation == 4) {
                max_range(sheet, cur_cell, cell1, cell2);
            }
            if (relation[current_row][current_col].operation == 5) {
                avg_range(sheet, cur_cell, cell1, cell2);
            }
            if (relation[current_row][current_col].operation == 6) {
                sum_range(sheet, cur_cell, cell1, cell2);
            }
            if (relation[current_row][current_col].operation == 7) {
                std_dev_range(sheet, cur_cell, cell1, cell2);
            }
            continue;
        }
        if (relation[current_row][current_col].operation >= 8 && relation[current_row][current_col].operation <= 11) {
            if (relation[relation[current_row][current_col].i1_row][relation[current_row][current_col].i1_column].error == 1) {
                relation[current_row][current_col].error = 1;
                continue;
            } else {
                relation[current_row][current_col].error = 0;
            }
            if (relation[current_row][current_col].operation == 8) {
                sheet[current_row][current_col] = sheet[relation[current_row][current_col].i1_row][relation[current_row][current_col].i1_column] + relation[current_row][current_col].i2_row;
            }
            if (relation[current_row][current_col].operation == 9) {
                sheet[current_row][current_col] = sheet[relation[current_row][current_col].i1_row][relation[current_row][current_col].i1_column] - relation[current_row][current_col].i2_row;
            }
            if (relation[current_row][current_col].operation == 10) {
                sheet[current_row][current_col] = sheet[relation[current_row][current_col].i1_row][relation[current_row][current_col].i1_column] * relation[current_row][current_col].i2_row;
            }
            if (relation[current_row][current_col].operation == 11) {
                if (relation[current_row][current_col].i2_row == 0) {
                    relation[current_row][current_col].error = 1;
                    continue;
                }
                sheet[current_row][current_col] = sheet[relation[current_row][current_col].i1_row][relation[current_row][current_col].i1_column] / relation[current_row][current_col].i2_row;
            }
            continue;
        }
        if (relation[current_row][current_col].operation >= 12 && relation[current_row][current_col].operation <= 15) {
            if (relation[relation[current_row][current_col].i1_row][relation[current_row][current_col].i1_column].error == 1 || 
                relation[relation[current_row][current_col].i2_row][relation[current_row][current_col].i2_column].error == 1) {
                relation[current_row][current_col].error = 1;
                continue;
            } else {
                relation[current_row][current_col].error = 0;
            }
            if (relation[current_row][current_col].operation == 12) {
                sheet[current_row][current_col] = sheet[relation[current_row][current_col].i1_row][relation[current_row][current_col].i1_column] + 
                                                  sheet[relation[current_row][current_col].i2_row][relation[current_row][current_col].i2_column];
            }
            if (relation[current_row][current_col].operation == 13) {
                sheet[current_row][current_col] = sheet[relation[current_row][current_col].i1_row][relation[current_row][current_col].i1_column] - 
                                                  sheet[relation[current_row][current_col].i2_row][relation[current_row][current_col].i2_column];
            }
            if (relation[current_row][current_col].operation == 14) {
                sheet[current_row][current_col] = sheet[relation[current_row][current_col].i1_row][relation[current_row][current_col].i1_column] * 
                                                  sheet[relation[current_row][current_col].i2_row][relation[current_row][current_col].i2_column];
            }
            if (relation[current_row][current_col].operation == 15) {
                if (sheet[relation[current_row][current_col].i2_row][relation[current_row][current_col].i2_column] == 0) {
                    relation[current_row][current_col].error = 1;
                    continue;
                }
                sheet[current_row][current_col] = sheet[relation[current_row][current_col].i1_row][relation[current_row][current_col].i1_column] / 
                                                  sheet[relation[current_row][current_col].i2_row][relation[current_row][current_col].i2_column];
            }
            continue;
        }
        if (relation[current_row][current_col].operation >= 16 && relation[current_row][current_col].operation <= 19) {
            if (relation[relation[current_row][current_col].i2_row][relation[current_row][current_col].i2_column].error == 1) {
                relation[current_row][current_col].error = 1;
                continue;
            } else {
                relation[current_row][current_col].error = 0;
            }
            if (relation[current_row][current_col].operation == 16) {
                sheet[current_row][current_col] = relation[current_row][current_col].i1_row + 
                                                  sheet[relation[current_row][current_col].i2_row][relation[current_row][current_col].i2_column];
            }
            if (relation[current_row][current_col].operation == 17) {
                sheet[current_row][current_col] = relation[current_row][current_col].i1_row - 
                                                  sheet[relation[current_row][current_col].i2_row][relation[current_row][current_col].i2_column];
            }
            if (relation[current_row][current_col].operation == 18) {
                sheet[current_row][current_col] = relation[current_row][current_col].i1_row * 
                                                  sheet[relation[current_row][current_col].i2_row][relation[current_row][current_col].i2_column];
            }
            if (relation[current_row][current_col].operation == 19) {
                if (sheet[relation[current_row][current_col].i2_row][relation[current_row][current_col].i2_column] == 0) {
                    relation[current_row][current_col].error = 1;
                    continue;
                }
                sheet[current_row][current_col] = relation[current_row][current_col].i1_row / 
                                                  sheet[relation[current_row][current_col].i2_row][relation[current_row][current_col].i2_column];
            }
            continue;
        }
        if (relation[current_row][current_col].operation == 20) {
            int cur_coords[] = {current_row, current_col};
            char cur_cell[10];
            coords_to_cell(cur_coords, cur_cell);
            if (relation[relation[current_row][current_col].i1_row][relation[current_row][current_col].i1_column].error == 1) {
                relation[current_row][current_col].error = 1;
                continue;
            } else {
                relation[current_row][current_col].error = 0;
            }
            sleep_value(sheet, cur_cell, sheet[relation[current_row][current_col].i1_row][relation[current_row][current_col].i1_column]);
            continue;
        }
    }
    free(visited);
    free(stack);
    free(stack_index);
}

void free_relation_graph(void) {
    if (!relation) return;
    for (int i = 0; i < ROWS; i++) free(relation[i]);
    free(relation);
    relation = NULL;
}

void free_dependencies(void) {
    if (!dependencies) return;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            struct DependencyNode* current = dependencies[i][j].head;
            while (current) {
                struct DependencyNode* next = current->next;
                free(current);
                current = next;
            }
            dependencies[i][j].head = NULL;
        }
        free(dependencies[i]);
    }
    free(dependencies);
    dependencies = NULL;
}

Stack *create_stack(int capacity) {
    Stack *stack = malloc(sizeof(Stack));
    if (!stack) return NULL;
    stack->states = malloc(capacity * sizeof(CellState));
    if (!stack->states) { free(stack); return NULL; }
    stack->top = -1;
    stack->capacity = capacity;
    return stack;
}

void push(Stack *stack, int row, int col) {
    if (stack->top + 1 >= stack->capacity) {
        stack->capacity *= 2;
        CellState *new_states = realloc(stack->states, stack->capacity * sizeof(CellState));
        if (!new_states) return;
        stack->states = new_states;
    }
    stack->top++;
    stack->states[stack->top].row = row;
    stack->states[stack->top].col = col;
}

int pop(Stack *stack, CellState *state) {
    if (stack->top < 0) return 0;
    *state = stack->states[stack->top--];
    return 1;
}

void free_stack(Stack *stack) {
    if (stack) {
        free(stack->states);
        free(stack);
    }
}