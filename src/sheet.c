#include<stdio.h>
#include<stdlib.h>
#include"functions.h"
#include<string.h>
#include"sheet.h"
#include"recalculations.h"
#define not !

#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))

int ROWS , COLS , origin_x , origin_y , enable;

int cell_to_coords(char *cell , int* coords){
    int i = 0;
    int j = 0;
    int col = 0;
    int row = 0;
    if (cell[0] == '\0') return 0;
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
    if (cell[i] == '\0') return 0;
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
void coords_to_cell(int* coords , char* cell){
    int col = coords[1]+1;
    int row = coords[0]+1;
    int i = 0;
    while (col > 0){
        cell[i] = 'A' + (col-1)%26;
        col = (col-1)/26;
        i++;
    }
    int i2 = i;
    int j = 0;
    while (i2 > j){
        char temp = cell[j];
        cell[j] = cell[i2-1];
        cell[i2-1] = temp;
        i2--;
        j++;
    }
    int i3 = i;
    while (row > 0){
        cell[i] = (row%10) + '0';
        row = row/10;
        i++;
    }
    int i4 = i-1;
    while (i4 > i3){
        char temp = cell[i4];
        cell[i4] = cell[i3];
        cell[i3] = temp;
        i4--;
        i3++;
    }
    
    cell[i] = '\0';
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
        if (ROWS >= origin_y+11) origin_y = min(origin_y+10 ,ROWS-10 );
    }
    else if (direction == 'w'){
        origin_y = max(origin_y -10 , 0);
    }
    else if (direction == 'a'){
        origin_x = max(0 , origin_x-10);
    }
    else if (direction == 'd'){
        if (COLS >= origin_x+11) origin_x = min(origin_x+10 , COLS-10);
    }
    printf("    ");
    for (int i = origin_x; i< min(origin_x+10 , COLS); i++){
        if (i < 26){
            printf("%11c " , ('A' + i));
        }
        else if (i < 26 + 26*26) {
            char c1 = 'A'+((i-26)/(26));
            char c2 = 'A'+((i-26)%(26));
            char temp[2] = {c1 , c2};
            printf("%11s " , temp);
        }
        else {
            char c1 = 'A'+((i-26 - 26*26)/(26*26));
            char c2 = 'A'+(((i-26 - 26*26)/(26))%26);
            char c3 =  'A'+((i-26 - 26*26)%26);
            char temp[3] = {c1 , c2 , c3};
            printf("%11s " , temp);
        }
    }
    printf("\n");
    for(int i = origin_y; i < min(origin_y+10 , ROWS); i++){
        printf("%3d " , i+1);
        for(int j = origin_x; j < min(origin_x+10 , COLS); j++){
            if (relation[i][j].error ==1 ) printf("%11s " , "ERR");
            else
            printf("%11d ", sheet[i][j]);
        }
        printf("\n");
    }
}

void process_control_input(int** sheet , char* control){                                                                                                                           
    if (control[0] == 'q') {
        exit(0);
    }
    else if (strcmp(control , "enable_output") == 0) {enable = 1; print_sheet(sheet , 0);}
    else if (strcmp(control , "disable_output") == 0) {enable = 0; }
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

int process_assign_input(int** sheet, char* cell , char* value){
    int coords1[2];
    int coords2[2];
    cell_to_coords(cell , coords1);
    if (cell_to_coords(value , coords2) == 0) {
        delete_dependencies(coords1[0],coords1[1]);
        sheet[coords1[0]][coords1[1]] = atoi(value);
        relation[coords1[0]][coords1[1]].operation = 1;
        relation[coords1[0]][coords1[1]].i1_row = -1;
        relation[coords1[0]][coords1[1]].i1_column = -1;
        relation[coords1[0]][coords1[1]].i2_row = -1;
        relation[coords1[0]][coords1[1]].i2_column = -1;
        relation[coords1[0]][coords1[1]].error = 0;
        recalculate(coords1[0],coords1[1],sheet);
        return 1;
        // printf("value: %d\n" , sheet[coords1[0]][coords1[1]]);
    }
    else{
        if(not has_cycle(coords1[0], coords1[1], coords2[0], coords2[1]) )
        {
        delete_dependencies(coords1[0],coords1[1] );
        sheet[coords1[0]][coords1[1]] = sheet[coords2[0]][coords2[1]];
        relation[coords1[0]][coords1[1]].operation = 2;
        relation[coords1[0]][coords1[1]].i1_row = coords2[0];
        relation[coords1[0]][coords1[1]].i1_column = coords2[1];
        relation[coords1[0]][coords1[1]].i2_row = coords2[0];
        relation[coords1[0]][coords1[1]].i2_column = coords2[1];
        add_dependencies(coords1[0],coords1[1]);
        recalculate(coords1[0],coords1[1],sheet);
        return 1;
        }
        else
        {
            return 0;
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
        if (flag == 1){
            delete_dependencies(coords1[0],coords1[1] );
            if (operation[0] == '+') {sheet[coords1[0]][coords1[1]] = v1 + v2;  relation[coords1[0]][coords1[1]].error = 0;}
        else if (operation[0] == '-') {sheet[coords1[0]][coords1[1]] = v1 - v2;  relation[coords1[0]][coords1[1]].error = 0;}
        else if (operation[0] == '*') {sheet[coords1[0]][coords1[1]] = v1 * v2;  relation[coords1[0]][coords1[1]].error = 0;}
            else if (operation[0] == '/') {
                if (v2 == 0) { relation[coords1[0]][coords1[1]].error = 1;
                }
                else 
                {sheet[coords1[0]][coords1[1]] = v1 / v2;  relation[coords1[0]][coords1[1]].error = 0;}
            }
            relation[coords1[0]][coords1[1]].operation = 1;
            relation[coords1[0]][coords1[1]].i1_row = -1;
            relation[coords1[0]][coords1[1]].i1_column = -1;
            relation[coords1[0]][coords1[1]].i2_row = -1;
            relation[coords1[0]][coords1[1]].i2_column = -1;
            recalculate(coords1[0],coords1[1],sheet);
            return 1;
        }
        flag = 2;
    }
    else v2 = sheet[coords3[0]][coords3[1]];
    if(flag==0)
    {
        if(not (has_cycle(coords1[0], coords1[1], coords2[0], coords2[1]) || has_cycle(coords1[0], coords1[1], coords3[0], coords3[1])))
        {
            delete_dependencies(coords1[0],coords1[1]);
            if (operation[0] == '+') {sheet[coords1[0]][coords1[1]] = v1 + v2; relation[coords1[0]][coords1[1]].operation = 12;}
        else if (operation[0] == '-') {sheet[coords1[0]][coords1[1]] = v1 - v2; relation[coords1[0]][coords1[1]].operation = 13;}
        else if (operation[0] == '*') {sheet[coords1[0]][coords1[1]] = v1 * v2; relation[coords1[0]][coords1[1]].operation = 14;}
        else if (operation[0] == '/') {
            if (v2 == 0) {relation[coords1[0]][coords1[1]].error = 1;
            }
            else
            sheet[coords1[0]][coords1[1]] = v1 / v2;
            relation[coords1[0]][coords1[1]].operation = 15;
        }
            relation[coords1[0]][coords1[1]].i1_row = coords2[0];
            relation[coords1[0]][coords1[1]].i1_column = coords2[1];
            relation[coords1[0]][coords1[1]].i2_row = coords3[0];
            relation[coords1[0]][coords1[1]].i2_column = coords3[1];
            add_dependencies(coords1[0],coords1[1]);
            recalculate(coords1[0],coords1[1] , sheet);
            return 1;
        }
        else
        {
           return 0;
        }
    }

    if(flag==2)
    {
        if(not (has_cycle(coords1[0], coords1[1], coords2[0], coords2[1])))
        {
            delete_dependencies(coords1[0],coords1[1]);
            if (operation[0] == '+') {sheet[coords1[0]][coords1[1]] = v1 + v2; relation[coords1[0]][coords1[1]].operation = 8;}
        else if (operation[0] == '-') {sheet[coords1[0]][coords1[1]] = v1 - v2; relation[coords1[0]][coords1[1]].operation = 9;}
        else if (operation[0] == '*') {sheet[coords1[0]][coords1[1]] = v1 * v2; relation[coords1[0]][coords1[1]].operation = 10;}
        else if (operation[0] == '/') {
            if (v2 == 0) {relation[coords1[0]][coords1[1]].error = 1;
            }
            else
            sheet[coords1[0]][coords1[1]] = v1 / v2;
            relation[coords1[0]][coords1[1]].operation = 11;
        }
            relation[coords1[0]][coords1[1]].i1_row = coords2[0];
            relation[coords1[0]][coords1[1]].i1_column = coords2[1];
            relation[coords1[0]][coords1[1]].i2_row = v2;
            relation[coords1[0]][coords1[1]].i2_column = v2;
            add_dependencies(coords1[0],coords1[1]);
            recalculate(coords1[0],coords1[1] , sheet);
            return 1;
        }
        else
        {
            return 0;
        }
    }

    if(flag==1)
    {
        if(not (has_cycle(coords1[0], coords1[1], coords3[0], coords3[1])))
        {

            delete_dependencies(coords1[0],coords1[1]);
            if (operation[0] == '+') {sheet[coords1[0]][coords1[1]] = v1 + v2; relation[coords1[0]][coords1[1]].operation = 16;}
        else if (operation[0] == '-') {sheet[coords1[0]][coords1[1]] = v1 - v2; relation[coords1[0]][coords1[1]].operation = 17;}
        else if (operation[0] == '*') {sheet[coords1[0]][coords1[1]] = v1 * v2; relation[coords1[0]][coords1[1]].operation = 18;}
        else if (operation[0] == '/') {
            if (v2 == 0) {relation[coords1[0]][coords1[1]].error = 1;
            }
            else
            sheet[coords1[0]][coords1[1]] = v1 / v2;
            relation[coords1[0]][coords1[1]].operation = 19;
        }
            relation[coords1[0]][coords1[1]].i1_row = v1;
            relation[coords1[0]][coords1[1]].i1_column = v1;
            relation[coords1[0]][coords1[1]].i2_row = coords3[0];
            relation[coords1[0]][coords1[1]].i2_column = coords3[1];
            add_dependencies(coords1[0],coords1[1]);
            recalculate(coords1[0],coords1[1] , sheet);
            return 1;
        }
        else{
            return 0;
        }
    }
    return 1;
}

int process_functions(int **sheet, char *cell, char *start, char *operation, char *end)
{
    if (strcmp(operation, "SLEEP") == 0)
    {
        int coords1[2];
        int coords2[2];
        cell_to_coords(cell, coords1);
        if (cell_to_coords(start, coords2) == 0)
        {
            int value = atoi(start);
            delete_dependencies(coords1[0], coords1[1]);
            sleep_value(sheet, cell, value);
            relation[coords1[0]][coords1[1]].operation = 1;
            relation[coords1[0]][coords1[1]].i1_row = -1;
            relation[coords1[0]][coords1[1]].i1_column = -1;
            relation[coords1[0]][coords1[1]].i2_row = -1;
            relation[coords1[0]][coords1[1]].i2_column = -1;
            recalculate(coords1[0], coords1[1], sheet);
            return 1;
        }
        else
        {
            if (not has_cycle(coords1[0], coords1[1], coords2[0], coords2[1]))
            {
                delete_dependencies(coords1[0], coords1[1]);
                relation[coords1[0]][coords1[1]].operation = 20;
                relation[coords1[0]][coords1[1]].i1_row = coords2[0];
                relation[coords1[0]][coords1[1]].i1_column = coords2[1];
                relation[coords1[0]][coords1[1]].i2_row = coords2[0];
                relation[coords1[0]][coords1[1]].i2_column = coords2[1];
                add_dependencies(coords1[0], coords1[1]);
                recalculate(coords1[0], coords1[1], sheet);
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }
    else
    {
        int coords1[2];
        int coords2[2];
        int coords3[2];
        cell_to_coords(cell, coords1);
        cell_to_coords(start, coords2);
        cell_to_coords(end, coords3);
        if (coords2[0] > coords3[0] || coords2[1] > coords3[1])
            return 0;
        int copy_1 = relation[coords1[0]][coords1[1]].operation;
        int copy_2 = relation[coords1[0]][coords1[1]].i1_row;
        int copy_3 = relation[coords1[0]][coords1[1]].i1_column;
        int copy_4 = relation[coords1[0]][coords1[1]].i2_row;
        int copy_5 = relation[coords1[0]][coords1[1]].i2_column;
        delete_dependencies(coords1[0], coords1[1]);
        relation[coords1[0]][coords1[1]].i1_row = coords2[0];
        relation[coords1[0]][coords1[1]].i1_column = coords2[1];
        relation[coords1[0]][coords1[1]].i2_row = coords3[0];
        relation[coords1[0]][coords1[1]].i2_column = coords3[1];
        if (range_has_cycle(coords1[0], coords1[1]))
        {
            relation[coords1[0]][coords1[1]].operation = copy_1;
            relation[coords1[0]][coords1[1]].i1_row = copy_2;
            relation[coords1[0]][coords1[1]].i1_column = copy_3;
            relation[coords1[0]][coords1[1]].i2_row = copy_4;
            relation[coords1[0]][coords1[1]].i2_column = copy_5;
            add_dependencies(coords1[0], coords1[1]);
            return 0;
        }
        else
        {
            if (strcmp(operation, "MIN") == 0)
            {
                // printf("coords1: %d\n" , relation[coords1[0]][coords1[1]].operation);
                relation[coords1[0]][coords1[1]].operation = 3;
                min_range(sheet, cell, start, end);
            }
            else if (strcmp(operation, "MAX") == 0)
            {
                relation[coords1[0]][coords1[1]].operation = 4;
                max_range(sheet, cell, start, end);
            }
            else if (strcmp(operation, "SUM") == 0)
            {
                relation[coords1[0]][coords1[1]].operation = 6;
                sum_range(sheet, cell, start, end);
            }
            else if (strcmp(operation, "AVG") == 0)
            {
                relation[coords1[0]][coords1[1]].operation = 5;
                avg_range(sheet, cell, start, end);
            }
            else if (strcmp(operation, "STDEV") == 0)
            {
                relation[coords1[0]][coords1[1]].operation = 7;
                std_dev_range(sheet, cell, start, end);
            }
            else
                return 2;
            add_dependencies(coords1[0], coords1[1]);
            recalculate(coords1[0], coords1[1], sheet);
            return 1;
        }
    }
}

void free_sheet(int **sheet) {
    if (!sheet) return;
    for (int i = 0; i < ROWS; i++) free(sheet[i]);
    free(sheet);
}