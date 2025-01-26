#include<stdio.h>
#include<stdlib.h>
#include"functions.h"
#include<string.h>
#include"sheet.h"
#define not !

#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))

int ROWS , COLS , origin_x , origin_y , enable;

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
    enable = 1;
    int** sheet = (int**) malloc(rows * sizeof(int*));
    for(int i = 0; i < rows; i++){
        sheet[i] = (int*) malloc(cols * sizeof(int));
    }

    create_relation_graph();

    return sheet;

}

void print_sheet(int** sheet, int direction){
    if (direction == 's') {
        if (ROWS >= origin_y+2) origin_y+=1;
    }
    else if (direction == 'w'){
        origin_y = max(origin_y -1 , 0);
    }
    else if (direction == 'a'){
        origin_x = max(0 , origin_x-1);
    }
    else if (direction == 'd'){
        if (COLS >= origin_x+2) origin_x+=1;
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

void process_control_input(int** sheet , char* control){                                                                                                                           
    if (control[0] == 'q') {
        exit(0);
    }
    else if (strcmp(control , "enable_output") == 0) enable = 1;
    else if (strcmp(control , "disable_output") == 0) {enable = 0; printf("output disabled\n");}
    else if (control[0] == 's' && enable) print_sheet(sheet , 's');
    else if (control[0] == 'w' && enable) print_sheet(sheet , 'w');
    else if (control[0] == 'a' && enable) print_sheet(sheet , 'a');
    else if (control[0] == 'd' && enable) print_sheet(sheet , 'd');
    else {
        int coords[2];
        cell_to_coords(control , coords);
        origin_x = coords[1];
        origin_y = coords[0];
        if (enable) print_sheet(sheet , 0);
    }
}

void process_assign_input(int** sheet, char* cell , char* value){
    int coords1[2];
    int coords2[2];
    cell_to_coords(cell , coords1);
    if (cell_to_coords(value , coords2) == 0) {
        find_and_modify_impactors(coords1[0],coords1[1]);
        sheet[coords1[0]][coords1[1]] = atoi(value);
        relation[coords1[0]][coords1[1]].operation = 1;
        relation[coords1[0]][coords1[1]].i1_row = atoi(value);
        relation[coords1[0]][coords1[1]].i1_column = atoi(value);
        relation[coords1[0]][coords1[1]].i2_row = atoi(value);
        relation[coords1[0]][coords1[1]].i2_column = atoi(value);

        printf("value: %d\n" , sheet[coords1[0]][coords1[1]]);
        
    }
    else{
        if(not has_cycle(coords1[0], coords1[1], coords2[0], coords2[1]) )
        {
        find_and_modify_impactors(coords1[0],coords1[1]);
        add_dependency(coords2[0],coords2[1],coords1[0],coords1[1]);
        sheet[coords1[0]][coords1[1]] = sheet[coords2[0]][coords2[1]];
        relation[coords1[0]][coords1[1]].operation = 2;
        relation[coords1[0]][coords1[1]].i1_row = coords2[0];
        relation[coords1[0]][coords1[1]].i1_column = coords2[1];
        relation[coords1[0]][coords1[1]].i2_row = coords2[0];
        relation[coords1[0]][coords1[1]].i2_column = coords2[1];
        }
        else
        {
            printf("cycle");
        }
    }
}

int process_arith_expr(int **sheet, char *cell, char *val1 , char *operation , char *val2){
    int coords1[2];
    int coords2[2];
    int coords3[2];
    cell_to_coords(cell , coords1);
    int v1;
    int v2;
    int flag = 0;

    if(cell_to_coords(val1 , coords2) == 0) 
    {
        v1 = atoi(val1);
        flag = 1;
    }
    else v1 = sheet[coords2[0]][coords2[1]];

    if(cell_to_coords(val2 , coords3) == 0) 
    {
        v2 = atoi(val2);
        if(flag == 1 && operation[0] == '+')
        {
            find_and_modify_impactors(coords1[0],coords1[1]);

            sheet[coords1[0]][coords1[1]] = v1 + v2;
            relation[coords1[0]][coords1[1]].operation = 1;
            relation[coords1[0]][coords1[1]].i1_row = -1;
            relation[coords1[0]][coords1[1]].i1_column = -1;
            relation[coords1[0]][coords1[1]].i2_row = -1;
            relation[coords1[0]][coords1[1]].i2_column = -1;
            return 1;
        }
        if(flag == 1 && operation[0] == '-')
        {
        find_and_modify_impactors(coords1[0],coords1[1]);
            sheet[coords1[0]][coords1[1]] = v1 - v2;
            relation[coords1[0]][coords1[1]].operation = 1;
            relation[coords1[0]][coords1[1]].i1_row = -1;
            relation[coords1[0]][coords1[1]].i1_column = -1;
            relation[coords1[0]][coords1[1]].i2_row = -1;
            relation[coords1[0]][coords1[1]].i2_column = -1;
            return 1;
        }
        if(flag == 1 && operation[0] == '*')
        {
            find_and_modify_impactors(coords1[0],coords1[1]);
            sheet[coords1[0]][coords1[1]] = v1 * v2;
            relation[coords1[0]][coords1[1]].operation = 1;
            relation[coords1[0]][coords1[1]].i1_row = -1;
            relation[coords1[0]][coords1[1]].i1_column = -1;
            relation[coords1[0]][coords1[1]].i2_row = -1;
            relation[coords1[0]][coords1[1]].i2_column = -1;
            return 1;
        }
        if(flag == 1 && operation[0] == '/')
        {

            if(v2 == 0) {
            return 0;
            }
            find_and_modify_impactors(coords1[0],coords1[1]);
            sheet[coords1[0]][coords1[1]] = v1 / v2;
            relation[coords1[0]][coords1[1]].operation = 1;
            relation[coords1[0]][coords1[1]].i1_row = -1;
            relation[coords1[0]][coords1[1]].i1_column = -1;
            relation[coords1[0]][coords1[1]].i2_row = -1;
            relation[coords1[0]][coords1[1]].i2_column = -1;
            return 1;
        }
        flag = 2;
    }
    else v2 = sheet[coords3[0]][coords3[1]];
    
    if (operation[0] == '+') 
    {
        if(flag==0)
        {
            if(not (has_cycle(coords1[0], coords1[1], coords2[0], coords2[1]) || has_cycle(coords1[0], coords1[1], coords3[0], coords3[1])))
            {
                find_and_modify_impactors(coords1[0],coords1[1]);
                add_dependency(coords2[0],coords2[1],coords1[0],coords1[1]);
                add_dependency(coords3[0],coords3[1],coords1[0],coords1[1]);

                sheet[coords1[0]][coords1[1]] = v1 + v2;
                relation[coords1[0]][coords1[1]].operation = 12;
                relation[coords1[0]][coords1[1]].i1_row = coords2[0];
                relation[coords1[0]][coords1[1]].i1_column = coords2[1];
                relation[coords1[0]][coords1[1]].i2_row = coords3[0];
                relation[coords1[0]][coords1[1]].i2_column = coords3[1];
            }
            else
            {
                printf("cycle");
            }
        }

        if(flag==2)
        {
            if(not (has_cycle(coords1[0], coords1[1], coords2[0], coords2[1])))
            {
                find_and_modify_impactors(coords1[0],coords1[1]);
                add_dependency(coords2[0],coords2[1],coords1[0],coords1[1]);

                
                sheet[coords1[0]][coords1[1]] = v1 + v2;
                relation[coords1[0]][coords1[1]].operation = 8;
                relation[coords1[0]][coords1[1]].i1_row = coords2[0];
                relation[coords1[0]][coords1[1]].i1_column = coords2[1];
                relation[coords1[0]][coords1[1]].i2_row = v2;
                relation[coords1[0]][coords1[1]].i2_column = v2;
            }
            else
            {
                printf("cycle");
            }
        }

        if(flag==1)
        {
            if(not (has_cycle(coords1[0], coords1[1], coords3[0], coords3[1])))
            {

             find_and_modify_impactors(coords1[0],coords1[1]);
                add_dependency(coords3[0],coords3[1],coords1[0],coords1[1]);   
                sheet[coords1[0]][coords1[1]] = v1 + v2;
                relation[coords1[0]][coords1[1]].operation = 16;
                relation[coords1[0]][coords1[1]].i1_row = v1;
                relation[coords1[0]][coords1[1]].i1_column = v1;
                relation[coords1[0]][coords1[1]].i2_row = coords3[0];
                relation[coords1[0]][coords1[1]].i2_column = coords3[1];
            }
            else{
                printf("cycle");
            }
        }
    }
    if (operation[0] == '-') 
    {
        if(flag==0)
        {
            if(not (has_cycle(coords1[0], coords1[1], coords2[0], coords2[1]) || has_cycle(coords1[0], coords1[1], coords3[0], coords3[1])))
            {
                find_and_modify_impactors(coords1[0],coords1[1]);
                add_dependency(coords2[0],coords2[1],coords1[0],coords1[1]);
                add_dependency(coords3[0],coords3[1],coords1[0],coords1[1]);
                sheet[coords1[0]][coords1[1]] = v1 - v2;
                relation[coords1[0]][coords1[1]].operation = 13;
                relation[coords1[0]][coords1[1]].i1_row = coords2[0];
                relation[coords1[0]][coords1[1]].i1_column = coords2[1];
                relation[coords1[0]][coords1[1]].i2_row = coords3[0];
                relation[coords1[0]][coords1[1]].i2_column = coords3[1];
            }
            else{
                printf("cycle");
            }
        }
        if(flag==2)
        {
            if(not (has_cycle(coords1[0], coords1[1], coords2[0], coords2[1])))
            {
                find_and_modify_impactors(coords1[0],coords1[1]);
                add_dependency(coords2[0],coords2[1],coords1[0],coords1[1]);
                sheet[coords1[0]][coords1[1]] = v1 - v2;
                relation[coords1[0]][coords1[1]].operation = 9;
                relation[coords1[0]][coords1[1]].i1_row = coords2[0];
                relation[coords1[0]][coords1[1]].i1_column = coords2[1];
                relation[coords1[0]][coords1[1]].i2_row = v2;
                relation[coords1[0]][coords1[1]].i2_column = v2;
            }
            else
            {
                printf("cycle");
            }
        }
        if(flag==1)
        {
            if(not (has_cycle(coords1[0], coords1[1], coords3[0], coords3[1])))
            {

                find_and_modify_impactors(coords1[0],coords1[1]);
                add_dependency(coords3[0],coords3[1],coords1[0],coords1[1]);  
                sheet[coords1[0]][coords1[1]] = v1 - v2;
                relation[coords1[0]][coords1[1]].operation = 17;
                relation[coords1[0]][coords1[1]].i1_row = v1;
                relation[coords1[0]][coords1[1]].i1_column = v1;
                relation[coords1[0]][coords1[1]].i2_row = coords3[0];
                relation[coords1[0]][coords1[1]].i2_column = coords3[1];
            }
            else{
                printf("cycle");
            }
        }
    }
    if (operation[0] == '*') 
    {
        if(flag==0)
        {
            if(not (has_cycle(coords1[0], coords1[1], coords2[0], coords2[1]) || has_cycle(coords1[0], coords1[1], coords3[0], coords3[1])))
            {
                find_and_modify_impactors(coords1[0],coords1[1]);
                add_dependency(coords2[0],coords2[1],coords1[0],coords1[1]);
                add_dependency(coords3[0],coords3[1],coords1[0],coords1[1]);
                sheet[coords1[0]][coords1[1]] = v1 * v2;
                relation[coords1[0]][coords1[1]].operation = 14;
                relation[coords1[0]][coords1[1]].i1_row = coords2[0];
                relation[coords1[0]][coords1[1]].i1_column = coords2[1];
                relation[coords1[0]][coords1[1]].i2_row = coords3[0];
                relation[coords1[0]][coords1[1]].i2_column = coords3[1];
            }
            else
            {
                printf("cycle");
            }
        }
        if(flag==2)
        {
            if(not (has_cycle(coords1[0], coords1[1], coords2[0], coords2[1])))
            {
                                find_and_modify_impactors(coords1[0],coords1[1]);
                add_dependency(coords2[0],coords2[1],coords1[0],coords1[1]);
                sheet[coords1[0]][coords1[1]] = v1 * v2;
                relation[coords1[0]][coords1[1]].operation = 10;
                relation[coords1[0]][coords1[1]].i1_row = coords2[0];
                relation[coords1[0]][coords1[1]].i1_column = coords2[1];
                relation[coords1[0]][coords1[1]].i2_row = v2;
                relation[coords1[0]][coords1[1]].i2_column = v2;
            }
            else
            {
                printf("cycle");
            }
        }
        if(flag==1)
        {
            if(not (has_cycle(coords1[0], coords1[1], coords3[0], coords3[1])))
            {
                find_and_modify_impactors(coords1[0],coords1[1]);
                add_dependency(coords3[0],coords3[1],coords1[0],coords1[1]);  
                sheet[coords1[0]][coords1[1]] = v1 * v2;
                relation[coords1[0]][coords1[1]].operation = 18;
                relation[coords1[0]][coords1[1]].i1_row = v1;
                relation[coords1[0]][coords1[1]].i1_column = v1;
                relation[coords1[0]][coords1[1]].i2_row = coords3[0];
                relation[coords1[0]][coords1[1]].i2_column = coords3[1];
            }
            else
            {
                printf("cycle");
            }
        }
    }
    else if (operation[0] == '/'){
        if(v2 == 0) {
            return 0;
        }
        if(flag==0)
        {
            if(not (has_cycle(coords1[0], coords1[1], coords2[0], coords2[1]) || has_cycle(coords1[0], coords1[1], coords3[0], coords3[1])))
            {
                find_and_modify_impactors(coords1[0],coords1[1]);
                add_dependency(coords2[0],coords2[1],coords1[0],coords1[1]);
                add_dependency(coords3[0],coords3[1],coords1[0],coords1[1]);
                sheet[coords1[0]][coords1[1]] = v1 / v2;
                relation[coords1[0]][coords1[1]].operation = 15;
                relation[coords1[0]][coords1[1]].i1_row = coords2[0];
                relation[coords1[0]][coords1[1]].i1_column = coords2[1];
                relation[coords1[0]][coords1[1]].i2_row = coords3[0];
                relation[coords1[0]][coords1[1]].i2_column = coords3[1];
            }
            else
            {
                printf("cycle");
            }
        }
        if(flag==2)
        {
            if(not (has_cycle(coords1[0], coords1[1], coords2[0], coords2[1])))
            {
                                find_and_modify_impactors(coords1[0],coords1[1]);
                add_dependency(coords2[0],coords2[1],coords1[0],coords1[1]);
                sheet[coords1[0]][coords1[1]] = v1 / v2;
                relation[coords1[0]][coords1[1]].operation = 11;
                relation[coords1[0]][coords1[1]].i1_row = coords2[0];
                relation[coords1[0]][coords1[1]].i1_column = coords2[1];
                relation[coords1[0]][coords1[1]].i2_row = v2;
                relation[coords1[0]][coords1[1]].i2_column = v2;
            }
            else
            {
                printf("cycle");
            }
        }
        if(flag==1)
        {
            if(not (has_cycle(coords1[0], coords1[1], coords3[0], coords3[1])))
            {
                find_and_modify_impactors(coords1[0],coords1[1]);
                add_dependency(coords3[0],coords3[1],coords1[0],coords1[1]);  
                sheet[coords1[0]][coords1[1]] = v1 / v2;
                relation[coords1[0]][coords1[1]].operation = 19;
                relation[coords1[0]][coords1[1]].i1_row = v1;
                relation[coords1[0]][coords1[1]].i1_column = v1;
                relation[coords1[0]][coords1[1]].i2_row = coords3[0];
                relation[coords1[0]][coords1[1]].i2_column = coords3[1];
            }
            else
            {
                printf("cycle");
            }
        }
    }
    return 1;

}

int process_functions(int **sheet, char *cell, char *start, char *operation, char *end){
    if(strcmp(operation, "MIN") == 0){
        return min_range(sheet, cell, start, end);
    }
    else if(strcmp(operation, "MAX") == 0){
        return max_range(sheet, cell, start, end);
    }
    else if(strcmp(operation, "SUM") == 0){
        return sum_range(sheet, cell, start, end);
    }
    else if(strcmp(operation, "AVG") == 0){
        return avg_range(sheet, cell, start, end);
    }
    else if(strcmp(operation, "STDEV") == 0){
        return std_dev_range(sheet, cell, start, end);
    }
    else return 2;
}
