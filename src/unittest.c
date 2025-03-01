#include<stdio.h>  
#include<stdlib.h>
#include<string.h>
//include our program files and headers
#include"functions.h"
#include"recalculations.h"
#include"sheet.h"
#include"input_process.h"


//unit test to test process_input function in input_process.c
int test_process_input(){
    // Allocate memory once
    char *cell1 = (char*)malloc(10);
    char *cell2 = (char*)malloc(10);
    char *operation = (char*)malloc(10);
    char *cell3 = (char*)malloc(10);

    // Test case 1: value assignment input (constants)
    char *line = "A1=10";
    int result = process_input(line, cell1, cell2, operation, cell3);
    if (result == 2){
        if (!(strcmp(cell1, "A1") == 0 && strcmp(cell2, "10") == 0 && strcmp(operation, "=") == 0 && strcmp(cell3, "") == 0)) return 0;
    }
    else return 0;

    // Test case 2: arithmetic operation input (constants)
    line = "A1=10+20";
    result = process_input(line, cell1, cell2, operation, cell3);
    if (result == 3){
        if (!(strcmp(cell1, "A1") == 0 && strcmp(cell2, "10") == 0 && strcmp(operation, "=") == 0 && strcmp(cell3, "20") == 0)) return 0;
    }
    else return 0;

    // Test case 3: invalid input
    line = "A1=10+20+30+";
    result = process_input(line, cell1, cell2, operation, cell3);
    if (!(result == 0)) return 0;

    // Test case 4: control command input
    line = "enable_output";
    result = process_input(line, cell1, cell2, operation, cell3);
    if (result == 1){
        if (!(strcmp(cell1, "") == 0 && strcmp(cell2, "") == 0 && strcmp(operation, "enable_output") == 0 && strcmp(cell3, "") == 0)) return 0;
    }
    else return 0;

    // Test case 5: control command input
    line = "w";
    result = process_input(line, cell1, cell2, operation, cell3);
    if (result == 1){
        if (!(strcmp(cell1, "") == 0 && strcmp(cell2, "") == 0 && strcmp(operation, "w") == 0 && strcmp(cell3, "") == 0)) return 0;
        else return 0;
    }
    else return 0;

    // Test case 6: function assignment
    line = "A1=MIN(A1:A3)";
    result = process_input(line, cell1, cell2, operation, cell3);
    if (result == 4){
        if (!(strcmp(cell1, "A1") == 0 && strcmp(cell2, "MIN") == 0 && strcmp(operation, "(") == 0 && strcmp(cell3, "A1:A3") == 0)) return 0;
        else return 0;
    }
    else return 0;

    // Test case 7: function assignment
    line = "A1=SUM(A1:A3)";
    result = process_input(line, cell1, cell2, operation, cell3);
    if (result == 4){
        if (!(strcmp(cell1, "A1") == 0 && strcmp(cell2, "SUM") == 0 && strcmp(operation, "(") == 0 && strcmp(cell3, "A1:A3") == 0)) return 0;
        else return 0;
    }
    else return 0;

    // Test case 8: invalid input
    line = "A1=";
    result = process_input(line, cell1, cell2, operation, cell3);
    if (!(result == 0)) return 0;

    // Test case 9: invalid input
    line = "A1=10+";
    result = process_input(line, cell1, cell2, operation, cell3);
    if (!(result == 0)) return 0;

    // Test case 10: invalid input
    line = "A1=10+20+30";
    result = process_input(line, cell1, cell2, operation, cell3);
    if (!(result == 0)) return 0;   

    // Test case 11: invalid input
    line = "A1=SUM(A1:A3";
    result = process_input(line, cell1, cell2, operation, cell3);
    if (!(result == 0)) return 0;

    // Test case 12: invalid input
    line = "A1=MAX";
    result = process_input(line, cell1, cell2, operation, cell3);
    if (!(result == 0)) return 0;

    //to avoid memory leaks while testing
    free(cell1);
    free(cell2);
    free(operation);
    free(cell3);

    return 1;
}

int main(){
    //unit test for process_input function
    if (test_process_input()){
        printf("Test passed\n");
    }
    else{
        printf("Test failed\n");
    }
    return 0;
}