#include<stdio.h>
#include<stdlib.h>
#include<functions.h>
#include<string.h>

#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))

int ROWS , COLS , origin_x , origin_y;

int cell_to_coords(char *cell , int* coords){
    int i = 0;
    int j = 0;
    int col = 0;
    int row = 0;
    while ((cell[i] >= 'A' && cell[i] <= 'Z') && cell[i] != '\0'){
        i++;
    }
    if (i == 0 || i > 3) return 0;
    while (j<i){
        col*=26;
        col+=(cell[j]-'A')+1;
        j++;
    }
    if (col > COLS) return 0;
    if (cell[i] == '0') return 0;
    while ((cell[i] >= '0' && cell[i] <= '9') && cell[i] != '\0'){
        row*=10;
        row+=(cell[i]-'0');
        i++;
    }
    if (cell[i] != '\0') return 0;
    if (row > ROWS) return 0;
    coords[0] = row-1;
    coords[1] = col-1;
    return 1;
}

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

int process_assign_input(int** sheet, char* cell , char* value){
    int coords1[2];
    int coords2[2];
    cell_to_coords(cell , coords1);
    if (cell_to_coords(value , coords2) == 0) {
        sheet[coords1[0]][coords1[1]] = atoi(value);
        printf("value: %d\n" , sheet[coords1[0]][coords1[1]]);
    }
    else{
        sheet[coords1[0]][coords1[1]] = sheet[coords2[0]][coords2[1]];
    }
}

int process_arith_expr(int **sheet, char *cell, char *val1 , char *operation , char *val2){
    int coords1[2];
    int coords2[2];
    int coords3[2];
    cell_to_coords(cell , coords1);
    int v1;
    int v2;

    if(cell_to_coords(val1 , coords2) == 0) v1 = atoi(val1);
    else v1 = sheet[coords2[0]][coords2[1]];

    if(cell_to_coords(val2 , coords3) == 0) v2 = atoi(val2);
    else v2 = sheet[coords3[0]][coords3[1]];
    
    if (operation[0] == '+') sheet[coords1[0]][coords1[1]] = v1 + v2;
    else if (operation[0] == '-') sheet[coords1[0]][coords1[1]] = v1 - v2;
    else if (operation[0] == '*') sheet[coords1[0]][coords1[1]] = v1 * v2;
    else if (operation[0] == '/') sheet[coords1[0]][coords1[1]] = v1 / v2;
    printf("value: %d\n" , sheet[coords1[0]][coords1[1]]);

}

int process_functions(int **sheet, char *cell, char *start, char *operation, char *end){
    if(strcmp(operation, "MIN") == 0){
        min_range(sheet, cell, start, end);
    }
    else if(strcmp(operation, "MAX") == 0){
        max_range(sheet, cell, start, end);
    }
    else if(strcmp(operation, "SUM") == 0){
        sum_range(sheet, cell, start, end);
    }
    else if(strcmp(operation, "AVG") == 0){
        avg_range(sheet, cell, start, end);
    }
    else if(strcmp(operation, "STDEV") == 0){
        std_dev_range(sheet, cell, start, end);
    }
}
