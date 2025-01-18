#include<stdio.h>
#include<stdlib.h>
#include<sheet.h>
#include<time.h>
#include<string.h>
#define MAX_SIZE 100;
void print_status(double *time , char *msg){
    printf("[%.1f] (%s) > " , *time , msg);
}

int main(int argc , char** argv){
    if (argc != 3) {
        printf("Invalid number of arguments\n");
        return 1;
    }
    time_t start , end;
    double timed = 0.0;
    char input[100] , input_test[100];
    start = time(NULL);
    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);
    if (rows > 999 || rows < 1 ) {printf("Invalid number of rows"); return 1;}
    if (cols > 18278 || cols < 1) {printf("Invalid number of columns"); return 1;}
    int **sheet = initialize_sheet(rows, cols);
    print_sheet(sheet , 0);
    end = time(NULL);
    timed = difftime(end , start);
    print_status( &timed , "ok");
    while (1){
        start = time(NULL);
        if (scanf("%s %s" , input , input_test) == 2){
            end = time(NULL);
            timed = difftime(end , start);
            print_status(&timed , "invalid command");
            continue;
        }
        if (input[1] == '\0'){
            if (input[0] == 'q') break;
            else if (input[0] == 's') print_sheet(sheet , 's');
            else if (input[0] == 'w') print_sheet(sheet , 'w');
            else if (input[0] == 'a') print_sheet(sheet , 'a');
            else if (input[0] == 'd') print_sheet(sheet , 'd');
            else {print_sheet(sheet , 0); end = time(NULL); timed = difftime(end , start); print_status(&timed , "invalid command"); continue;}
            end = time(NULL);
            timed = difftime(end , start);
            print_status(&timed , "ok");
            continue;
        }

    }
    return 0;
}