#include <stdio.h>
#include <sheet.h>
#include <string.h>
// zero for invalid input
// one for control command
// two for value assignment
// three for arithmetic expression
// four for function assignment
int process_input(char *line  , char *cell1 , char *cell2 , char *operation , char* cell3 ){
    printf("Processing input\n");
    int i = 0;
    int j = 0;
    int col = 0;
    int row = 0;
    int coords[2];
    int temp = 0;
    while (line[i] != '\0'){
        if (line[i] == ' ') return 0;
        i++;
    }   
    if (line[2] == '\0'){
        if (line[0] == 'q' || line[0] == 's' || line[0] == 'w' || line[0] == 'a' || line[0] == 'd') {operation[0] = line[0]; operation[1] = '\0'; return 1;}
        else return 0;
    }
    i = 0;
    while (line[i] != '=' && line[i] != '\0'){
        cell1[i] = line[i];
        i++;
    }
    if (line[i] != '=') return 0;
    cell1[i] = '\0';
    if (!cell_to_coords(cell1 , coords)) return 0;
    i++;
    while (line[i] != '\0' && line[i] != '+' && line[i] != '-' && line[i] != '*' && line[i] != '/' && line[i] != '(' && line[i]!='\n'){
        cell2[j] = line[i];
        j++;
        i++;
    }
    if (cell2[0] == '0') return 0; // number starts with zero
    cell2[j] = '\0';
    if (!(line[i] == '(')) {
        for (int k = 0; k < j; k++){
        if (cell2[k] < '0' || cell2[k] > '9') temp = 1;
    }   
    if (temp == 0){
        if (j>10) return 0; //overflow
        else return 2;
    }
    if (!cell_to_coords(cell2 , coords)) return 0; // invalid cell
    }
    if (line[i] == '\0' || line[i] == '\n') return 2;
    if (!(line[i] == '(')){
        operation[0] = line[i];
        operation[1] = '\0';
        i++;
        j = 0;
        while (line[i] != '\0'){
            cell3[j] = line[i];
            j++;
            i++;
        }
        cell3[j] = '\0';
        if (cell3[0] == '0') return 0; // number starts with zero
        for (int k = 0; k < j; k++){
            if (cell3[k] < '0' || cell3[k] > '9') temp = 1;
        }   
        if (temp == 0){
            if (j>10) return 0; //overflow
            else return 3;
        }
        if (!cell_to_coords(cell3 , coords)) return 0; // invalid cell
        return 3;
    }
    if (strcmp(cell2 , "SLEEP") == 0){
        strcpy(operation , "SLEEP");
        i++;
        j = 0;
        while (line[i] != '\n' && line[i] != ')' && line[i] != '\0'){
            cell2[j] = line[i];
            j++;
            i++;
        }
        if (line[i] == '\0' || line[i] == '\n') return 0;
        cell2[j] = '\0';
        i++;
        if (line[i] != '\0' && line[i]!= '\n') return 0;
        if (cell2[0] == '0') return 0; // number starts with zero
        for (int k = 0; k < j; k++){
        if (cell2[k] < '0' || cell2[k] > '9') temp = 1;
        }   
        if (temp == 0){
            if (j>10) return 0; //overflow
        }   
    if (!cell_to_coords(cell2 , coords) && temp) return 0; // invalid cell
    return 4;
    }
    if (strcmp(cell2 , "SUM") == 0 || strcmp(cell2 , "AVG") == 0 || strcmp(cell2 , "MAX") == 0 || strcmp(cell2 , "MIN") == 0 || strcmp(cell2 , "STDEV") == 0 ) {
        strcpy(operation , cell2);
        i++;
        j = 0;
        while (line[i] != '\0' && line[i] != ':'){
            cell2[j] = line[i];
            j++;
            i++;
        }
        if (line[i] != ':') return 0;
        cell2[j] = '\0';
        i++;
    if (!cell_to_coords(cell2 , coords)) return 0; // invalid cell
        j = 0;
        while (line[i] != '\0' && line[i] != ')'){
            cell3[j] = line[i];
            j++;
            i++;
        }
        if (line[i] != ')') return 0;
        i++;
        if (line[i] != '\0' && line[i]!= '\n') return 0;
        cell3[j] = '\0';
        if (!cell_to_coords(cell3 , coords)) return 0; // invalid cell
        return 4;}
    return 0;
}