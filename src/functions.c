#include<stdio.h>
#include<stdlib.h>
#include"sheet.h"
#include"recalculations.h"
#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))


int min_range(int **sheet, char *cell, char *start, char *end){
    int coords1[2];
    int coords2[2];
    int coords3[2];
    cell_to_coords(cell , coords1);
    cell_to_coords(start , coords2);
    cell_to_coords(end , coords3);

    if(coords2[0] > coords3[0] || coords2[1] > coords3[1]) return 0;

    int minn = sheet[coords2[0]][coords2[1]];
    for(int i = coords2[0]; i <= coords3[0]; i++){
        for(int j = coords2[1]; j <= coords3[1]; j++){
            if (sheet[i][j] < minn) minn = sheet[i][j];
        }
    }
    int copy_1 = relation[coords1[0]][coords1[1]].operation;
    int copy_2 = relation[coords1[0]][coords1[1]].i1_row;
    int copy_3 = relation[coords1[0]][coords1[1]].i1_column;
    int copy_4 = relation[coords1[0]][coords1[1]].i2_row;
    int copy_5 = relation[coords1[0]][coords1[1]].i2_column;


    relation[coords1[0]][coords1[1]].operation = 3;
    relation[coords1[0]][coords1[1]].i1_row = coords2[0];
    relation[coords1[0]][coords1[1]].i1_column = coords2[1];
    relation[coords1[0]][coords1[1]].i2_row = coords3[0];
    relation[coords1[0]][coords1[1]].i2_column = coords3[1];


    if(range_has_cycle(coords1[0],coords1[1]))
    {
        printf("cycle");
        relation[coords1[0]][coords1[1]].operation = copy_1;
        relation[coords1[0]][coords1[1]].i1_row = copy_2;
        relation[coords1[0]][coords1[1]].i1_column = copy_3;
        relation[coords1[0]][coords1[1]].i2_row = copy_4;
        relation[coords1[0]][coords1[1]].i2_column = copy_5;

    }
    else
    {
        sheet[coords1[0]][coords1[1]] = minn;

    }

    return 1;
}

int max_range(int **sheet, char *cell, char *start, char *end){
    int coords1[2];
    int coords2[2];
    int coords3[2];
    cell_to_coords(cell , coords1);
    cell_to_coords(start , coords2);
    cell_to_coords(end , coords3);

    if(coords2[0] > coords3[0] || coords2[1] > coords3[1]) return 0;

    int maxx = sheet[coords2[0]][coords2[1]];
    for(int i = coords2[0]; i <= coords3[0]; i++){
        for(int j = coords2[1]; j <= coords3[1]; j++){
            if (sheet[i][j] > maxx) maxx = sheet[i][j];
        }
    }

    int copy_1 = relation[coords1[0]][coords1[1]].operation;
    int copy_2 = relation[coords1[0]][coords1[1]].i1_row;
    int copy_3 = relation[coords1[0]][coords1[1]].i1_column;
    int copy_4 = relation[coords1[0]][coords1[1]].i2_row;
    int copy_5 = relation[coords1[0]][coords1[1]].i2_column;

    relation[coords1[0]][coords1[1]].operation = 4;
    relation[coords1[0]][coords1[1]].i1_row = coords2[0];
    relation[coords1[0]][coords1[1]].i1_column = coords2[1];
    relation[coords1[0]][coords1[1]].i2_row = coords3[0];
    relation[coords1[0]][coords1[1]].i2_column = coords3[1];

    if(range_has_cycle(coords1[0],coords1[1]))
    {
        printf("cycle");
        relation[coords1[0]][coords1[1]].operation = copy_1;
        relation[coords1[0]][coords1[1]].i1_row = copy_2;
        relation[coords1[0]][coords1[1]].i1_column = copy_3;
        relation[coords1[0]][coords1[1]].i2_row = copy_4;
        relation[coords1[0]][coords1[1]].i2_column = copy_5;

    }
    else
    {
        sheet[coords1[0]][coords1[1]] = maxx;

    }


    return 1;
}

int sum_range(int **sheet, char *cell, char *start, char *end){
    int coords1[2];
    int coords2[2];
    int coords3[2];
    cell_to_coords(cell , coords1);
    cell_to_coords(start , coords2);
    cell_to_coords(end , coords3);

    if(coords2[0] > coords3[0] || coords2[1] > coords3[1]) return 0;
    int sum = 0;
    for(int i = coords2[0]; i <= coords3[0]; i++){
        for(int j = coords2[1]; j <= coords3[1]; j++){
            sum += sheet[i][j];
        }
    }

    int copy_1 = relation[coords1[0]][coords1[1]].operation;
    int copy_2 = relation[coords1[0]][coords1[1]].i1_row;
    int copy_3 = relation[coords1[0]][coords1[1]].i1_column;
    int copy_4 = relation[coords1[0]][coords1[1]].i2_row;
    int copy_5 = relation[coords1[0]][coords1[1]].i2_column;

    relation[coords1[0]][coords1[1]].operation = 6;
    relation[coords1[0]][coords1[1]].i1_row = coords2[0];
    relation[coords1[0]][coords1[1]].i1_column = coords2[1];
    relation[coords1[0]][coords1[1]].i2_row = coords3[0];
    relation[coords1[0]][coords1[1]].i2_column = coords3[1];

        if(range_has_cycle(coords1[0],coords1[1]))
    {
        printf("cycle");
        relation[coords1[0]][coords1[1]].operation = copy_1;
        relation[coords1[0]][coords1[1]].i1_row = copy_2;
        relation[coords1[0]][coords1[1]].i1_column = copy_3;
        relation[coords1[0]][coords1[1]].i2_row = copy_4;
        relation[coords1[0]][coords1[1]].i2_column = copy_5;

    }
    else
    {
        sheet[coords1[0]][coords1[1]] = sum;


        return 1;
    }

    return 1;
}

int avg_range(int **sheet, char *cell, char *start, char *end){
    int coords1[2];
    int coords2[2];
    int coords3[2];
    cell_to_coords(cell , coords1);
    cell_to_coords(start , coords2);
    cell_to_coords(end , coords3);
    
    if(coords2[0] > coords3[0] || coords2[1] > coords3[1]) return 0;

    int sum = 0;
    int count = 0;
    for(int i = coords2[0]; i <= coords3[0]; i++){
        for(int j = coords2[1]; j <= coords3[1]; j++){
            sum += sheet[i][j];
            count++;
        }
    }

    int copy_1 = relation[coords1[0]][coords1[1]].operation;
    int copy_2 = relation[coords1[0]][coords1[1]].i1_row;
    int copy_3 = relation[coords1[0]][coords1[1]].i1_column;
    int copy_4 = relation[coords1[0]][coords1[1]].i2_row;
    int copy_5 = relation[coords1[0]][coords1[1]].i2_column;

    relation[coords1[0]][coords1[1]].operation = 5;
    relation[coords1[0]][coords1[1]].i1_row = coords2[0];
    relation[coords1[0]][coords1[1]].i1_column = coords2[1];
    relation[coords1[0]][coords1[1]].i2_row = coords3[0];
    relation[coords1[0]][coords1[1]].i2_column = coords3[1];

    if(range_has_cycle(coords1[0],coords1[1]))
    {
        printf("cycle");
        relation[coords1[0]][coords1[1]].operation = copy_1;
        relation[coords1[0]][coords1[1]].i1_row = copy_2;
        relation[coords1[0]][coords1[1]].i1_column = copy_3;
        relation[coords1[0]][coords1[1]].i2_row = copy_4;
        relation[coords1[0]][coords1[1]].i2_column = copy_5;

    }
    else
    {
    sheet[coords1[0]][coords1[1]] = sum/count;

        find_and_modify_impactors(coords1[0],coords1[1]);

    }

    return 1;
}

int std_dev_range(int **sheet, char *cell, char *start, char *end){
    int coords1[2];
    int coords2[2];
    int coords3[2];
    cell_to_coords(cell , coords1);
    cell_to_coords(start , coords2);
    cell_to_coords(end , coords3);

    if(coords2[0] > coords3[0] || coords2[1] > coords3[1]) return 0;

    int sum = 0;
    int count = 0;
    for(int i = coords2[0]; i <= coords3[0]; i++){
        for(int j = coords2[1]; j <= coords3[1]; j++){
            sum += sheet[i][j];
            count++;
        }
    }
    int avg = sum/count;
    int sum_sq = 0;
    for(int i = coords2[0]; i <= coords3[0]; i++){
        for(int j = coords2[1]; j <= coords3[1]; j++){
            sum_sq += (sheet[i][j] - avg)*(sheet[i][j] - avg);
        }
    }

    int copy_1 = relation[coords1[0]][coords1[1]].operation;
    int copy_2 = relation[coords1[0]][coords1[1]].i1_row;
    int copy_3 = relation[coords1[0]][coords1[1]].i1_column;
    int copy_4 = relation[coords1[0]][coords1[1]].i2_row;
    int copy_5 = relation[coords1[0]][coords1[1]].i2_column;

    relation[coords1[0]][coords1[1]].operation = 7;
    relation[coords1[0]][coords1[1]].i1_row = coords2[0];
    relation[coords1[0]][coords1[1]].i1_column = coords2[1];
    relation[coords1[0]][coords1[1]].i2_row = coords3[0];
    relation[coords1[0]][coords1[1]].i2_column = coords3[1];

    if(range_has_cycle(coords1[0],coords1[1]))
    {
        printf("cycle");
        relation[coords1[0]][coords1[1]].operation = copy_1;
        relation[coords1[0]][coords1[1]].i1_row = copy_2;
        relation[coords1[0]][coords1[1]].i1_column = copy_3;
        relation[coords1[0]][coords1[1]].i2_row = copy_4;
        relation[coords1[0]][coords1[1]].i2_column = copy_5;

    }
    else
    {
        sheet[coords1[0]][coords1[1]] = sum_sq/count;

        find_and_modify_impactors(coords1[0],coords1[1]);

    }

    return 1;
}
