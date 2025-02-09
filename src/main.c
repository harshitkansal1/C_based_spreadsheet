#include<stdio.h>
#include<stdlib.h>
#include"sheet.h"
#include<time.h>
#include<string.h>
#include"input_process.h"

#define MAX_SIZE 100
void print_status(double *time , char *msg){
    printf("[%.8f] (%s) > " , *time , msg);
}

int main(int argc , char** argv){
    if (argc != 3) {
        printf("Invalid number of arguments\n");
        return 1;
    }
    clock_t start_time , end_time;
    double timed = 0.0;
    char input[MAX_SIZE];
    int input_type = 0;
    char cell1[MAX_SIZE] , cell2[MAX_SIZE] , cell3[MAX_SIZE];
    char operation[MAX_SIZE];
    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);
    if (rows > 999 || rows < 1 ) {printf("Invalid number of rows"); return 1;}
    if (cols > 18278 || cols < 1) {printf("Invalid number of columns"); return 1;}
    start_time = clock();
    int **sheet = initialize_sheet(rows, cols);
    print_sheet(sheet , 0);
    end_time = clock();
    timed = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
    print_status( &timed , "ok");
    while (1){
        fgets(input , MAX_SIZE, stdin);
        start_time = clock();
        input_type  = process_input(input , cell1 , cell2 , operation , cell3);   
        if (input_type== 0){
            if (enable) {print_sheet(sheet , 0); }
            end_time = clock();
            timed = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
            print_status(&timed , "invalid command");
            continue;
        }
        else if (input_type == 1){
            process_control_input(sheet , operation);
            end_time = clock();
            timed = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
            print_status(&timed , "ok");
            continue;
        } 
        else if (input_type == 2){
            process_assign_input(sheet , cell1 , cell2);
            if (enable) print_sheet(sheet , 0);
            end_time = clock();
            timed = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
            print_status(&timed , "ok");
            continue;
        }
        else if (input_type == 3){
            int flag = process_arith_expr(sheet , cell1 , cell2 , operation , cell3);
            if (enable) print_sheet(sheet , 0);
            end_time = clock();
            timed = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
            print_status(&timed , (flag == 0) ? "division by zero" : "ok");
            continue;
        }
        else if (input_type == 4){
            int flag = process_functions(sheet , cell1 , cell2 , operation , cell3);
            if (enable) print_sheet(sheet , 0);
            end_time = clock();
            timed = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
            if(flag == 0) print_status(&timed , "invalid range");
            else if(flag==1) print_status(&timed , "ok");
            else print_status(&timed , "invalid function");
            continue;
        }
        printf("%d" , input_type);
    }
    return 0;
}