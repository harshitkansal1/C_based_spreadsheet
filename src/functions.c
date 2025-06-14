#include<stdio.h>
#include<stdlib.h>
#include"sheet.h"
#include"recalculations.h"
#include<unistd.h>
#include<time.h>
#include<math.h>
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
    sheet[coords1[0]][coords1[1]] = minn;

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

    sheet[coords1[0]][coords1[1]] = maxx;
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

    sheet[coords1[0]][coords1[1]] = sum;
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
    sheet[coords1[0]][coords1[1]] = sum/count;
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
    double sum_sq = 0.0;
    for(int i = coords2[0]; i <= coords3[0]; i++){
        for(int j = coords2[1]; j <= coords3[1]; j++){
            sum_sq += (sheet[i][j] - avg)*(sheet[i][j] - avg);
        }
    }
    sum_sq = sqrt(sum_sq/count);
    sheet[coords1[0]][coords1[1]] = (int)round(sum_sq);
    return 1;
}

int sleep_value(int **sheet, char *cell, int value){
    int val = value;
    clock_t start, end;
    start = clock();
    end = clock();
    while(((double) (end - start)) / CLOCKS_PER_SEC < val){
        end = clock();
    }
    int coords1[2];
    cell_to_coords(cell , coords1);
    sheet[coords1[0]][coords1[1]] = val;
    return 1;
}