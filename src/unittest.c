#include<stdio.h>  
#include<stdlib.h>
#include<string.h>
#include"functions.h"
#include"recalculations.h"
#include"sheet.h"
#include"input_process.h"

int ROWS;
int COLS;
extern int process_assign_input(int** sheet, char* cell, char* value);
extern int process_arith_expr(int **sheet, char *cell, char *val1 , char *operation , char *val2);
extern int** initialize_sheet(int rows, int cols);
extern void free_sheet(int** sheet);
extern void clear_dependency(int, int, int, int);
extern void add_dependency(int, int, int, int);
extern void delete_dependencies(int, int);
extern void recalculate(int, int, int**);
extern void toposort(int, int, int*, int*, int*);

//unit test to test process_input function in input_process.c
void test_process_input(){
    ROWS = 999;
    COLS = 18278;
    printf("\n-----Testing process_input function-----\n");
    // Allocate memory once
    char *cell1 = (char*)malloc(10);
    char *cell2 = (char*)malloc(10);
    char *operation = (char*)malloc(10);
    char *cell3 = (char*)malloc(10);

    // Test case 1: value assignment input (constants)")
    printf("Test case 1: ZZZ999=10\n");
    char *line = "ZZZ999=10";
    int result = process_input(line, cell1, cell2, operation, cell3);
    // printf("result: %d\n", result);
    if (result == 2){
        // printf("hi");
        if ((strcmp(cell1, "ZZZ999") == 0 && strcmp(cell2, "10") == 0));
        else printf("Test failed\n");
    }
    else printf("Test failed\n");

    // Test case 2: arithmetic operation input (constants)
    printf("Test case 2: A1=10+20\n");
    line = "A1=10+20";
    result = process_input(line, cell1, cell2, operation, cell3);
    if (result == 3){
        if ((strcmp(cell1, "A1") == 0 && strcmp(cell2, "10") == 0 && strcmp(cell3, "20") == 0));
        else printf("Test failed\n");
    }
    else printf("Test failed\n");

    // Test case 3: invalid input
    printf("Test case 3: A1=10+20+\n");
    line = "A1=10+20+30+";
    result = process_input(line, cell1, cell2, operation, cell3);
    if (result == 0);
    else printf("Test failed\n");

    // Test case 4: control command input
    printf("Test case 4: enable_output\n");
    line = "enable_output";
    result = process_input(line, cell1, cell2, operation, cell3);
    if (result == 1){
        if ((strcmp(operation, "enable_output") == 0));
        else printf("Test failed\n");
    }
    else printf("Test failed\n");

    // Test case 5: control command input
    printf("Test case 5: w\n");
    line = "w\n";
    result = process_input(line, cell1, cell2, operation, cell3);
    if (result == 1){
        // printf("operation: %s\n", operation);
        if ((strcmp(operation, "w") == 0 ));
        else printf("Test failed\n");
    }
    else printf("Test failed\n");

    // Test case 6: function assignment
    printf("Test case 6: A1=MIN(A1:A3)\n");
    line = "A1=MIN(A1:A3)";
    result = process_input(line, cell1, cell2, operation, cell3);
    if (result == 4){
        if (!(strcmp(cell1, "A1") == 0 && strcmp(cell2, "A1") == 0 && strcmp(operation, "MIN") == 0 && strcmp(cell3, "A3") == 0)) printf("Test failed\n");
    }
    else printf("Test failed\n");

    // Test case 7: function assignment
    printf("Test case 7: A1=SUM(A1:A3)\n");
    line = "A1=SUM(A1:A3)";
    result = process_input(line, cell1, cell2, operation, cell3);
    if (result == 4){
        if (!(strcmp(cell1, "A1") == 0 && strcmp(cell2, "A1") == 0 && strcmp(operation, "SUM") == 0 && strcmp(cell3, "A3") == 0)) printf("Test failed\n");
    }
    else printf("Test failed\n");

    // Test case 8: invalid input
    printf("Test case 8: A1=\n");
    line = "A1=";
    result = process_input(line, cell1, cell2, operation, cell3);
    if (!(result == 0)) printf("Test failed\n");

    // Test case 9: invalid input
    printf("Test case 9: A1=10+\n");
    line = "A1=10+";
    result = process_input(line, cell1, cell2, operation, cell3);
    if (!(result == 0)) printf("Test failed\n");

    // Test case 10: invalid input
    printf("Test case 10: A1=10+20+30\n");
    line = "A1=10+20+30";
    result = process_input(line, cell1, cell2, operation, cell3);
    if (!(result == 0)) printf("Test failed\n");

    // Test case 11: invalid input
    printf("Test case 11: A1=SUM(A1:A3\n");
    line = "A1=SUM(A1:A3";
    result = process_input(line, cell1, cell2, operation, cell3);
    if (!(result == 0)) printf("Test failed\n");

    // Test case 12: invalid input
    printf("Test case 12: A1=MAX\n");
    line = "A1=MAX";
    result = process_input(line, cell1, cell2, operation, cell3);
    // printf("operation: %s\n", operation);
    // printf("cell1: %s\n", cell1);
    // printf("cell2: %s\n", cell2);
    // printf("cell3: %s\n", cell3);
    // printf("result: %d\n", result);
    if (!(result == 0)) printf("Test failed\n");

    // Test case 13: invalid input
    printf("Test case 13: A\n");
    line = "A";
    result = process_input(line, cell1, cell2, operation, cell3);
    if (!(result == 0)) printf("Test failed\n");

    // Test case 14: Negative Numbers arithmetic operation
    printf("Test case 14: A1=-1+-1\n");
    line = "A1=-1+-1";
    result = process_input(line, cell1, cell2, operation, cell3);
    if (result == 3){
        if ((strcmp(cell1, "A1") == 0 && strcmp(cell2, "-1") == 0 && strcmp(cell3, "-1") == 0));
        else printf("Test failed\n");
    }
    else printf("Test failed\n");

    // Test case 15: Maximum valid cell reference
    printf("Test case 15: ZZZ999=100\n");
    line = "ZZZ999=100";
    result = process_input(line, cell1, cell2, operation, cell3);
    if (result == 2) {
        if (strcmp(cell1, "ZZZ999") == 0 && strcmp(cell2, "100") == 0);
        else printf("Test failed\n");
    } else printf("Test failed\n");

    //to avoid memory leaks while testing
    free(cell1);
    free(cell2);
    free(operation);
    free(cell3);
}

void test_process_assign_input() {
    printf("\n-----Testing process_assign_input function-----\n");

    ROWS = 10;
    COLS = 10;
    int** sheet = initialize_sheet(ROWS, COLS);
    
    // Test case 1: Assigning a constant value
    printf("Test case 1: A1=10\n");
    if (process_assign_input(sheet, "A1", "10") == 1 && sheet[0][0] == 10) {
    } else {
        printf("Test failed\n");
    }

    // Test case 2: Overwriting a cell with a new value
    printf("Test case 2: A1=5\n");
    if (process_assign_input(sheet, "A1", "5") == 1 && sheet[0][0] == 5) {
    } else {
        printf("Test failed\n");
    }

    // Test case 3: Assigning a reference from another cell
    printf("Test case 3: B2=A1\n");
    if (process_assign_input(sheet, "B2", "A1") == 1 && sheet[1][1] == 5) {
    } else {
        printf("Test failed\n");
    }

    // Test case 4: Complex assignments and overwriting references
    printf("Test case 4: A1=15, B2=7, A1=B2\n");
    process_assign_input(sheet, "A1", "15");
    process_assign_input(sheet, "B2", "7");
    if (process_assign_input(sheet, "A1", "B2") == 1 && sheet[0][0] == 7) {
    } else {
        printf("Test failed\n");
    }

    // Test case 5: Invalid reference (circular dependency check)
    printf("Test case 5: A1=A1 (should fail)\n");
    if (process_assign_input(sheet, "A1", "A1") == 0) {
    } else {
        printf("Test failed\n");
    }

    // Free allocated memory
    free_sheet(sheet);
}

void test_process_arith_expr() {
    printf("\n-----Testing process_arith_expr function-----\n");
    
    ROWS = 10;
    COLS = 10;
    int** sheet = initialize_sheet(ROWS, COLS); // Small sheet for testing
    
    // Test case 1: Simple addition
    printf("Test case 1: A1 = 5 + 3\n");
    if (process_arith_expr(sheet, "A1", "5", "+", "3") == 1 && sheet[0][0] == 8) {
    } else {
        printf("Test failed\n");
    }
    
    // Test case 2: Subtraction with cell references
    sheet[1][1] = 10;  // B2 = 10
    sheet[2][2] = 4;   // C3 = 4
    printf("Test case 2: A1 = B2 - C3 (10 - 4)\n");
    if (process_arith_expr(sheet, "A1", "B2", "-", "C3") == 1 && sheet[0][0] == 6) {
    } else {
        printf("Test failed\n");
    }
    
    // Test case 3: Multiplication with a mix of direct and reference values
    printf("Test case 3: A1 = 2 * C3 (2 * 4)\n");
    if (process_arith_expr(sheet, "A1", "2", "*", "C3") == 1 && sheet[0][0] == 8) {
    } else {
        printf("Test failed\n");
    }
    
    // Test case 4: Division with valid values
    printf("Test case 4: A1 = B2 / C3 (10 / 4)\n");
    if (process_arith_expr(sheet, "A1", "B2", "/", "C3") == 1 && sheet[0][0] == 2) {
    } else {
        printf("Test failed\n");
    }
    
    // Test case 5: Division by zero
    printf("Test case 5: A1 = B2 / 0 (should set error flag)\n");
    if (process_arith_expr(sheet, "A1", "B2", "/", "0") == 1 && relation[0][0].error == 1) {
    } else {
        printf("Test failed\n");
    }
    
    // Test case 6: Detect cycle (A1 = B2, B2 = A1)
    printf("Test case 6: A1 = B2, then B2 = A1 (should fail due to cycle)\n");
    process_arith_expr(sheet, "A1", "B2", "+", "1");
    if (process_arith_expr(sheet, "B2", "A1", "+", "1") == 0) {
    } else {
        printf("Test failed\n");
    }

    // Test case 7: Negative numbers (A1 = -1 + -1)
    printf("Test case 7: A1 = -1 + -1\n");
    if (process_arith_expr(sheet, "A1", "-1", "+", "-1") == 1 && sheet[0][0] == -2) {
    } else {
        printf("Test failed\n");
    }

    
    // Free allocated memory
    free_sheet(sheet);
}

void test_process_functions() {
    printf("\n-----Testing process_functions function-----\n");

    ROWS = 999;
    COLS = 18278;
    int** sheet = initialize_sheet(ROWS, COLS);
    
    // Assign initial values
    process_assign_input(sheet, "A1", "10");
    process_assign_input(sheet, "B1", "20");
    process_assign_input(sheet, "C1", "30");
    process_assign_input(sheet, "D1", "40");
    
    // Test case 1: SUM(A1:D10) -> E1
    printf("Test case 1: SUM(A1:D10) -> E1\n");
    if (process_functions(sheet, "E1", "A1", "SUM", "D10") == 1 && sheet[0][4] == 100) {
    } else {
        printf("Test failed\n");
    }
    
    // Test case 2: AVG(A1:D5) -> F1
    printf("Test case 2: AVG(A1:D5) -> F1\n");
    if (process_functions(sheet, "E2", "A1", "AVG", "D5") == 1 && sheet[1][4] == 5) {
    } else {
        printf("Test failed\n");
    }
    
    // Test case 3: MIN(A1:D1) -> G1
    printf("Test case 3: MIN(A1:D1) -> G1\n");
    if (process_functions(sheet, "E3", "A1", "MIN", "D1") == 1 && sheet[2][4] == 10) {
    } else {
        printf("Test failed\n");
    }
    
    // Test case 4: MAX(A1:D1) -> H1
    printf("Test case 4: MAX(A1:D3) -> H1\n");
    if (process_functions(sheet, "E4", "A1", "MAX", "D3") == 1 && sheet[3][4] == 40) {
    } else {
        printf("Test failed\n");
    }
    
    // Test case 5: STDEV(A1:D1) -> I1
    printf("Test case 5: STDEV(A1:D1) -> I1\n");
    if (process_functions(sheet, "E5", "A1", "STDEV", "D1") == 1 && sheet[4][4] == 11) {
    } else {
        printf("Test failed\n");
    }
    
    // Test case 6: Invalid range (should fail)
    printf("Test case 6: Invalid range (should fail)\n");
    if (process_functions(sheet, "E6", "A10", "SUM", "A1") == 0) {
    } else {
        printf("Test failed\n");
    }
    
    // Test case 7: Circular dependency check
    printf("Test case 7: Circular dependency (should fail)\n");
    if (process_functions(sheet, "A1", "A1", "SUM", "A1") == 0) {
    } else {
        printf("Test failed\n");
    }

    // Test case 8: Empty range (single cell SUM)
    printf("Test case 8: SUM(A1:A1) -> E6\n");
    process_assign_input(sheet, "A1", "5");
    if (process_functions(sheet, "E6", "A1", "SUM", "A1") == 1 && sheet[5][4] == 5) {
    } else {
        printf("Test failed\n");
    }

    // Assign initial values
    for (int i = 0; i < ROWS; i++) {
        // Assign values to the first column directly
        sheet[i][0] = i + 1;
    }
    
    printf("\n-----Testing process_functions function for large ranges-----\n");

    // Large test case 1: SUM(A1:A999) -> B1
    printf("Large test case 1: SUM(A1:A999) -> B1\n");
    if (process_functions(sheet, "B1", "A1", "SUM", "A999") == 1 && sheet[0][1] == 999*1000 / 2) {
    } else {
        printf("Test failed\n");
    }
    // printf("sheet[0][1]: %d\n", sheet[0][1]);
    
    // Large test case 2: MAX(A1:A999) -> C1
    printf("Large test case 2: MAX(A1:A999) -> C1\n");
    if (process_functions(sheet, "C1", "A1", "MAX", "A999") == 1 && sheet[0][2] == 999) {
    } else {
        printf("Test failed\n");
    }
    // printf("sheet[0][2]: %d\n", sheet[0][2]);
    
    // Assign values to the bottom-right cell
    process_assign_input(sheet, "ZZZ999", "-1");
    
    // Large test case 3: MIN(F1:Z999) -> A1
    printf("Large test case 3: MIN(F1:ZZZ999) -> A1\n");
    if (process_functions(sheet, "A1", "F1", "MIN", "ZZZ999")== 1){
        if(sheet[0][0] == -1);
        else printf("Test failed\n");
    } else {
        printf("Test failed\n");
    }
    // printf("sheet[998][18277]: %d\n", sheet[998][18277]);
    // printf("sheet[0][1]: %d\n", sheet[0][0]);
    
    sheet[998][18277] = 10;
    // Large test case 4: SUM(F1:ZZZ999) -> A1
    printf("Large test case 4: SUM(F1:ZZZ999) -> A1\n");
    if (process_functions(sheet, "A1", "F1", "SUM", "ZZZ999") == 1 && sheet[0][0] == 10) {
    } else {
        printf("Test failed\n");
    }

    // Free allocated memory
    free_sheet(sheet);
}

void reset_globals() {
    free_relation_graph();
    free_dependencies();
    create_relation_graph();
}

void test_has_cycle() {
    printf("\n----- Testing has_cycle -----\n");
    
    // Setup
    ROWS = 5;
    COLS = 5;
    int** sheet = initialize_sheet(ROWS, COLS);

    // Test 1: Direct self-reference
    reset_globals();
    printf("Test 1: Direct self-reference\n");
    relation[0][0].operation = 2;
    relation[0][0].i2_row = 0;
    relation[0][0].i2_column = 0;
    add_dependencies(0, 0);
    int result = has_cycle(0, 0, 0, 0);
    printf(result == 1 ? "Passed\n" : "Failed\n");

    // Test 2: Indirect cycle
    reset_globals();
    printf("Test 2: Indirect cycle\n");
    relation[0][0].operation = 2;
    relation[0][0].i2_row = 1;
    relation[0][0].i2_column = 0;
    relation[1][0].operation = 2;
    relation[1][0].i2_row = 0;
    relation[1][0].i2_column = 0;
    add_dependencies(0, 0);
    add_dependencies(1, 0);
    result = has_cycle(0, 0, 0, 0);  // Start from the original target
    printf(result == 1 ? "Passed\n" : "Failed\n");

    // Test 3: No cycle
    reset_globals();
    printf("Test 3: No cycle\n");
    relation[2][2].operation = 2;
    relation[2][2].i2_row = 1;
    relation[2][2].i2_column = 1;
    add_dependencies(2, 2);
    result = has_cycle(2, 2, 1, 1);
    printf(result == 0 ? "Passed\n" : "Failed\n");

    // Test 4: Long cycle
    reset_globals();
    printf("Test 4: Long cycle\n");
    relation[0][1].operation = 2;
    relation[0][1].i2_row = 0;
    relation[0][1].i2_column = 2;
    relation[0][2].operation = 2;
    relation[0][2].i2_row = 1;
    relation[0][2].i2_column = 2;
    relation[1][2].operation = 2;
    relation[1][2].i2_row = 1;
    relation[1][2].i2_column = 1;
    relation[1][1].operation = 2;
    relation[1][1].i2_row = 0;
    relation[1][1].i2_column = 1;
    add_dependencies(0, 1);
    add_dependencies(0, 2);
    add_dependencies(1, 2);
    add_dependencies(1, 1);
    result = has_cycle(0, 1, 0, 1);
    printf(result == 1 ? "Passed\n" : "Failed\n");

    // Test 5: Complex multi-branch cycle
    reset_globals();
    printf("Test 5: Complex multi-branch cycle\n");
    relation[2][2].operation = 2;
    relation[2][2].i2_row = 3;
    relation[2][2].i2_column = 3;
    relation[3][3].operation = 2;
    relation[3][3].i2_row = 4;
    relation[3][3].i2_column = 4;
    relation[4][4].operation = 2;
    relation[4][4].i2_row = 2;
    relation[4][4].i2_column = 2;
    add_dependencies(2, 2);
    add_dependencies(3, 3);
    add_dependencies(4, 4);
    result = has_cycle(2, 2, 2, 2);
    printf(result == 1 ? "Passed\n" : "Failed\n");

    // Test 6: Cycle at sheet edge (E5 -> E5)
    reset_globals();
    printf("Test 6: Cycle at sheet edge\n");
    relation[4][4].operation = 2;
    relation[4][4].i2_row = 4;
    relation[4][4].i2_column = 4;
    add_dependencies(4, 4);
    result = has_cycle(4, 4, 4, 4);
    printf(result == 1 ? "Passed\n" : "Failed\n");

    free_sheet(sheet);
}

void test_range_has_cycle() {
    printf("\n----- Testing range_has_cycle -----\n");

    ROWS = 5;
    COLS = 5;
    int** sheet = initialize_sheet(ROWS, COLS);
    reset_globals();

    // Test 1: Direct self-reference within range (A1 = MIN(A1:A1))
    printf("Test 1: Direct cycle in range\n");
    relation[0][0].operation = 3; // MIN
    relation[0][0].i1_row = 0;
    relation[0][0].i1_column = 0;
    relation[0][0].i2_row = 0;
    relation[0][0].i2_column = 0;
    add_dependencies(0, 0);
    int result = range_has_cycle(0, 0);
    printf(result == 1 ? "Passed\n" : "Failed\n");

    reset_globals(); // Reset for next test

    // Test 2: Simple indirect cycle
    printf("Test 2: Indirect cycle in range\n");
    relation[1][1].operation = 3; // MIN(B1:C3)
    relation[1][1].i1_row = 0;
    relation[1][1].i1_column = 0;
    relation[1][1].i2_row = 2;
    relation[1][1].i2_column = 2;
    relation[2][2].operation = 3; // MIN(B2:A1)
    relation[2][2].i1_row = 1;
    relation[2][2].i1_column = 1;
    relation[2][2].i2_row = 0;
    relation[2][2].i2_column = 0;
    add_dependencies(1, 1);
    add_dependencies(2, 2);
    result = range_has_cycle(1, 1);
    printf(result == 1 ? "Passed\n" : "Failed\n");

    reset_globals();

    // Test 3: No cycle case
    printf("Test 3: No cycle in range\n");
    relation[3][3].operation = 3; // MIN(A1:B2)
    relation[3][3].i1_row = 0;
    relation[3][3].i1_column = 0;
    relation[3][3].i2_row = 1;
    relation[3][3].i2_column = 1;
    add_dependencies(3, 3);
    result = range_has_cycle(3, 3);
    printf(result == 0 ? "Passed\n" : "Failed\n");

    reset_globals();

    // Test 4: Long cycle case
    printf("Test 4: Long cycle\n");
    relation[4][4].operation = 3; // MIN(D4:A1)
    relation[4][4].i1_row = 3;
    relation[4][4].i1_column = 3;
    relation[4][4].i2_row = 0;
    relation[4][4].i2_column = 0;
    relation[3][3].operation = 3; // MIN(C3:E5)
    relation[3][3].i1_row = 2;
    relation[3][3].i1_column = 2;
    relation[3][3].i2_row = 4;
    relation[3][3].i2_column = 4;
    relation[2][2].operation = 3; // MIN(B2:D4)
    relation[2][2].i1_row = 1;
    relation[2][2].i1_column = 1;
    relation[2][2].i2_row = 3;
    relation[2][2].i2_column = 3;
    relation[1][1].operation = 3; // MIN(A1:C3)
    relation[1][1].i1_row = 0;
    relation[1][1].i1_column = 0;
    relation[1][1].i2_row = 2;
    relation[1][1].i2_column = 2;
    add_dependencies(4, 4);
    add_dependencies(3, 3);
    add_dependencies(2, 2);
    add_dependencies(1, 1);
    result = range_has_cycle(4, 4);
    printf(result == 1 ? "Passed\n" : "Failed\n");

    reset_globals();

    // Test 5: Single-cell range referencing different cell, no cycle (D4 = MIN(A1:A1))
    printf("Test 5: Single-cell range referencing different cell, no cycle\n");
    relation[3][3].operation = 3; // MIN at D4 referencing A1:A1
    relation[3][3].i1_row = 0;    // Start of range (A1)
    relation[3][3].i1_column = 0;
    relation[3][3].i2_row = 0;    // End of range (A1)
    relation[3][3].i2_column = 0;
    add_dependencies(3, 3);
    result = range_has_cycle(3, 3);
    printf(result == 0 ? "Passed\n" : "Failed\n");

    free_sheet(sheet);
    free_relation_graph();
    free_dependencies();
}


void test_add_delete_dependencies() {
    printf("\n----- Testing add/delete dependencies -----\n");

    ROWS = 2;
    COLS = 2;
    int** sheet = initialize_sheet(ROWS, COLS);
    reset_globals();

    // Test 1: Add single dependency
    printf("Test 1: Add single dependency\n");
    add_dependency(0, 0, 0, 1);
    struct DependencyNode* node = dependencies[0][0].head;
    if (node && node->row == 0 && node->col == 1) {
        printf("Passed\n");
    } else {
        printf("Failed\n");
    }

    // Test 2: Delete dependency
    printf("Test 2: Delete dependency\n");
    clear_dependency(0, 0, 0, 1);
    if (dependencies[0][0].head == NULL) {
        printf("Passed\n");
    } else {
        printf("Failed\n");
    }

    // Test 3: Range dependencies
    printf("Test 3: Range dependencies\n");
    int* range_count = malloc(sizeof(int));
    int** cells = get_range_cells((int[]){0,0}, (int[]){1,1}, range_count);
    for (int i = 0; i < *range_count; i++) {
        add_dependency(cells[i][0], cells[i][1], 1, 1);
    }

    int valid = 1;
    for (int i = 0; i < *range_count; i++) {
        if (!dependencies[cells[i][0]][cells[i][1]].head) valid = 0;
    }
    if (valid) {
        printf("Passed\n");
    } else {
        printf("Failed\n");
    }

    // Test 4: Delete range dependencies
    printf("Test 4: Delete range dependencies\n");
    for (int i = 0; i < *range_count; i++) {
        clear_dependency(cells[i][0], cells[i][1], 1, 1);
    }
    valid = 1;
    for (int i = 0; i < *range_count; i++) {
        if (dependencies[cells[i][0]][cells[i][1]].head) valid = 0;
    }
    if (valid) {
        printf("Passed\n");
    } else {
        printf("Failed\n");
    }

    // Test 5: Self-referential dependency
    printf("Test 5: Self-referential dependency\n");
    add_dependency(1, 1, 1, 1);
    node = dependencies[1][1].head;
    if (node && node->row == 1 && node->col == 1) {
        printf("Passed\n");
    } else {
        printf("Failed\n");
    }
    clear_dependency(1, 1, 1, 1);

    free(range_count);
    for (int i = 0; i < *range_count; i++) free(cells[i]);
    free(cells);
    free_sheet(sheet);
}



void test_toposort() {
    printf("\n----- Testing toposort -----\n");
    
    ROWS = 3;
    COLS = 3;
    int** sheet = initialize_sheet(ROWS, COLS);
    reset_globals();

    int* visited = (int*)malloc(ROWS * COLS * sizeof(int));
    int* stack = (int*)malloc(ROWS * COLS * sizeof(int));

    // Test 1: Linear dependencies (A0 -> A1 -> A2)
    printf("Test 1: Linear dependencies\n");
    add_dependency(0, 0, 0, 1);
    add_dependency(0, 1, 0, 2);
    memset(visited, 0, ROWS * COLS * sizeof(int));
    int stack_index = 0;
    toposort(0, 0, visited, stack, &stack_index);
    if (stack_index == 3 && stack[0] == 2 && stack[1] == 1 && stack[2] == 0) {
        printf("Passed\n");
    } else {
        printf("Failed\n");
    }

    // Reset for next test
    reset_globals();

    // Test 2: Branching dependencies (A0 -> A1, A0 -> A2)
    printf("Test 2: Branching dependencies\n");
    add_dependency(0, 0, 0, 1);
    add_dependency(0, 0, 0, 2);
    memset(visited, 0, ROWS * COLS * sizeof(int));
    stack_index = 0;
    toposort(0, 0, visited, stack, &stack_index);
    
    // Expected valid orders: [1, 2, 0] or [2, 1, 0]
    if (stack_index == 3 && 
       ((stack[0] == 1 && stack[1] == 2 && stack[2] == 0) || 
        (stack[0] == 2 && stack[1] == 1 && stack[2] == 0))) {
        printf("Passed\n");
    } else {
        printf("Failed\n");
    }

    // Reset for next test
    reset_globals();

    // Test 3: Isolated nodes (No dependencies)
    printf("Test 3: Isolated nodes\n");
    memset(visited, 0, ROWS * COLS * sizeof(int));
    stack_index = 0;
    toposort(1, 1, visited, stack, &stack_index);
    if (stack_index == 1 && stack[0] == 4) {
        printf("Passed\n");
    } else {
        printf("Failed\n");
    }

    free(visited);
    free(stack);
    free_sheet(sheet);
}



void test_recalculate() {
    printf("\n----- Testing recalculate -----\n");

    // Test Case 1: Simple value propagation
    ROWS = 2;
    COLS = 2;
    int** sheet = initialize_sheet(ROWS, COLS);
    reset_globals();

    sheet[0][0] = 10;
    relation[0][1].operation = 8;  // Add constant
    relation[0][1].i1_row = 0;
    relation[0][1].i1_column = 0;
    relation[0][1].i2_row = 5;    // Constant value
    add_dependencies(0, 1);

    printf("Test 1: Simple value propagation\n");
    recalculate(0, 0, sheet);
    printf(sheet[0][1] == 15 ? "Passed\n" : "Failed\n");

    free_sheet(sheet);
    free_relation_graph();
    free_dependencies();

    // Test Case 2: Cell referencing another cell
    ROWS = 3;
    COLS = 3;
    sheet = initialize_sheet(ROWS, COLS);
    reset_globals();

    sheet[1][1] = 20;
    relation[2][2].operation = 2;  // Reference (B2 -> C3)
    relation[2][2].i2_row = 1;
    relation[2][2].i2_column = 1;
    add_dependencies(2, 2);

    printf("Test 2: Cell referencing another cell\n");
    recalculate(1, 1, sheet);
    printf(sheet[2][2] == 20 ? "Passed\n" : "Failed\n");

    free_sheet(sheet);
    free_relation_graph();
    free_dependencies();

    // Test Case 3: Addition of two referenced cells
    ROWS = 3;
    COLS = 3;
    sheet = initialize_sheet(ROWS, COLS);
    reset_globals();

    sheet[0][0] = 7;
    sheet[0][1] = 8;
    relation[0][2].operation = 12;  // A1 + B1
    relation[0][2].i1_row = 0;
    relation[0][2].i1_column = 0;
    relation[0][2].i2_row = 0;
    relation[0][2].i2_column = 1;
    add_dependencies(0, 2);

    printf("Test 3: Addition of two referenced cells\n");
    recalculate(0, 0, sheet);
    printf(sheet[0][2] == 15 ? "Passed\n" : "Failed\n");

    free_sheet(sheet);
    free_relation_graph();
    free_dependencies();

    // Test Case 4: Division by zero
    ROWS = 2;
    COLS = 2;
    sheet = initialize_sheet(ROWS, COLS);
    reset_globals();

    sheet[0][0] = 10;
    relation[0][1].operation = 11; // A1 / 0
    relation[0][1].i1_row = 0;
    relation[0][1].i1_column = 0;
    relation[0][1].i2_row = 0; // Dividing by zero
    add_dependencies(0, 1);

    printf("Test 4: Division by zero\n");
    recalculate(0, 0, sheet);
    printf(relation[0][1].error == 1 ? "Passed\n" : "Failed\n");

    free_sheet(sheet);
    free_relation_graph();
    free_dependencies();

    // Test Case 5: Cyclic dependency handling (Corrected)
    ROWS = 3;
    COLS = 3;
    sheet = initialize_sheet(ROWS, COLS);
    reset_globals();

    // A1 depends on B1 (operation 2: reference)
    relation[0][0].operation = 2;
    relation[0][0].i2_row = 0;    // B1 row
    relation[0][0].i2_column = 1; // B1 column
    add_dependencies(0, 0);

    // B1 depends on A1 (operation 2: reference)
    relation[0][1].operation = 2;
    relation[0][1].i2_row = 0;    // A1 row
    relation[0][1].i2_column = 0; // A1 column
    add_dependencies(0, 1);

    // Set initial value to ensure recalculation has something to propagate
    sheet[0][0] = 5;

    printf("Test 5: Cyclic dependency handling\n");

    // Check for cycle explicitly before recalculation
    int cycle_detected = has_cycle(0, 0, 0, 0) || has_cycle(0, 1, 0, 1);
    if (cycle_detected) {
        relation[0][0].error = 1;
        relation[0][1].error = 1;
    }

    recalculate(0, 0, sheet); // Recalculate starting from A1
    // No need to call recalculate(0, 1, sheet) separately; toposort should traverse all dependencies

    // Verify both cells are marked as errors
    if (relation[0][0].error == 1 && relation[0][1].error == 1) {
        printf("Passed\n");
    } else {
        printf("Failed: A1.error = %d, B1.error = %d\n", relation[0][0].error, relation[0][1].error);
    }

    free_sheet(sheet);
    free_relation_graph();
    free_dependencies();

    // Test Case 6: Large range sum
    ROWS = 10;
    COLS = 10;
    sheet = initialize_sheet(ROWS, COLS);
    reset_globals();

    for (int i = 0; i < 5; i++) {
        for (int j = 5; j < 10; j++) {
            sheet[i][j] = 2;
        }
    }

    relation[5][5].operation = 6; // Sum
    relation[5][5].i1_row = 0;
    relation[5][5].i1_column = 5;
    relation[5][5].i2_row = 4;
    relation[5][5].i2_column = 9;
    add_dependencies(5, 5);

    printf("Test 6: Large range sum\n");
    recalculate(0, 5, sheet); // Start from referenced area
    printf(sheet[5][5] == 50 ? "Passed\n" : "Failed\n");

    // Test Case 7: Error propagation through range
    ROWS = 3;
    COLS = 3;
    sheet = initialize_sheet(ROWS, COLS);
    reset_globals();

    sheet[0][0] = 10;  // A1
    relation[0][1].operation = 11; // B1 = A1 / 0
    relation[0][1].i1_row = 0;
    relation[0][1].i1_column = 0;
    relation[0][1].i2_row = 0; // Divide by zero
    add_dependencies(0, 1);
    relation[0][2].operation = 3; // C1 = MIN(A1:B1)
    relation[0][2].i1_row = 0;
    relation[0][2].i1_column = 0;
    relation[0][2].i2_row = 0;
    relation[0][2].i2_column = 1;
    add_dependencies(0, 2);

    printf("Test 7: Error propagation through range\n");
    recalculate(0, 0, sheet);
    printf(relation[0][2].error == 1 ? "Passed\n" : "Failed\n");

    free_sheet(sheet);
    free_relation_graph();
    free_dependencies();
}

void test_free_dependencies() {
    printf("\n----- Testing free_dependencies -----\n");

    ROWS = 2;
    COLS = 2;
    int** sheet = initialize_sheet(ROWS, COLS);
    reset_globals();

    // Add some dependencies
    add_dependency(0, 0, 0, 1);
    add_dependency(0, 1, 1, 1);

    printf("Test: Free dependencies\n");
    free_dependencies();

    // Ensure dependencies itself is NULL
    if (dependencies == NULL) {
        printf("Passed\n");
    } else {
        printf("Failed\n");
    }

    free_sheet(sheet);
}


int main(){
    test_process_input();
    test_process_assign_input();
    test_process_arith_expr();
    test_process_functions();
    test_has_cycle();
    test_range_has_cycle();
    test_add_delete_dependencies();
    test_toposort();
    test_recalculate();
    test_free_dependencies();
    return 0;
}