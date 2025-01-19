#include<stdio.h>
#include<stdlib.h>

#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))

int ROWS , COLS , origin_x , origin_y;

int** initialize_sheet(int rows, int cols){
    ROWS = rows;
    COLS = cols;
    int** sheet = (int**) malloc(rows * sizeof(int*));
    for(int i = 0; i < rows; i++){
        sheet[i] = (int*) malloc(cols * sizeof(int));
    }
    return sheet;
}

void print_sheet(int** sheet, int direction){
    if (direction == 's') {
        if (ROWS >= origin_y+11) origin_y+=1;
    }
    else if (direction == 'w'){
        origin_y = max(origin_y -1 , 0);
    }
    else if (direction == 'a'){
        origin_x = max(0 , origin_x-1);
    }
    else if (direction == 'd'){
        if (COLS >= origin_x+11) origin_x+=1;
    }
    printf("\t");
    for (int i = origin_x; i< min(origin_x+10 , COLS); i++){
        if (i < 26){
            printf("%c\t" , ('A' + i));
        }
        else if (i < 26 + 26*26) {
            char c1 = 'A'+((i-26)/(26));
            char c2 = 'A'+((i-26)%(26));
            printf("%c%c\t" , c1 , c2);
        }
        else {
            char c1 = 'A'+((i-26 - 26*26)/(26*26));
            char c2 = 'A'+((i-26 - 26*26)/(26));
            char c3 =  'A'+((i-26 - 26*26)%26);
            printf("%c%c%c\t" , c1 , c2 , c3);
        }
    }
    printf("\n");
    for(int i = origin_y; i < min(origin_y+10 , ROWS); i++){
        printf("%d\t" , i+1);
        for(int j = origin_x; j < min(origin_x+10 , COLS); j++){
            printf("%d\t", sheet[i][j]);
        }
        printf("\n");
    }
}

void process_control_input(int** sheet , char control){
    if (control == 'q') {
        exit(0);
    }
    else if (control == 's') print_sheet(sheet , 's');
    else if (control == 'w') print_sheet(sheet , 'w');
    else if (control == 'a') print_sheet(sheet , 'a');
    else if (control == 'd') print_sheet(sheet , 'd');
}


