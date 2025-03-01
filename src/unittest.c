#include<stdio.h>  
#include<stdlib.h>
#include<string.h>
//include our program files and headers
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

//unit test to test process_input function in input_process.c
void test_process_input(){
    ROWS = 999;
    COLS = 18278;
    printf("-----Testing process_input function-----\n");
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

    //to avoid memory leaks while testing
    free(cell1);
    free(cell2);
    free(operation);
    free(cell3);
}

void test_process_assign_input() {
    printf("-----Testing process_assign_input function-----\n");

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
    printf("-----Testing process_arith_expr function-----\n");
    
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

    
    // Free allocated memory
    free_sheet(sheet);
}

void test_process_functions() {
    printf("-----Testing process_functions function-----\n");

    ROWS = 999;
    COLS = 18278;
    int** sheet = initialize_sheet(ROWS, COLS);
    
    // Assign initial values
    process_assign_input(sheet, "A1", "10");
    process_assign_input(sheet, "B1", "20");
    process_assign_input(sheet, "C1", "30");
    process_assign_input(sheet, "D1", "40");
    
    // Test case 1: SUM(A1:D1) -> E1
    printf("Test case 1: SUM(A1:D1) -> E1\n");
    if (process_functions(sheet, "E1", "A1", "SUM", "D1") == 1 && sheet[0][4] == 100) {
    } else {
        printf("Test failed\n");
    }
    
    // Test case 2: AVG(A1:D1) -> F1
    printf("Test case 2: AVG(A1:D1) -> F1\n");
    if (process_functions(sheet, "E2", "A1", "AVG", "D1") == 1 && sheet[1][4] == 25) {
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
    printf("Test case 4: MAX(A1:D1) -> H1\n");
    if (process_functions(sheet, "E4", "A1", "MAX", "D1") == 1 && sheet[3][4] == 40) {
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

    // Assign initial values
    for (int i = 0; i < ROWS; i++) {
        // Assign values to the first column directly
        sheet[i][0] = i + 1;
    }
    
    printf("-----Testing process_functions function for large ranges-----\n");

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

int main(){
    test_process_input();
    test_process_assign_input();
    test_process_arith_expr();
    test_process_functions();
    return 0;
}