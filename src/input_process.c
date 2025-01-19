#include <stdio.h>


// zero for invalid input
// one for control command
// two for value assignment
// three for arithmetic expression
// four for function assignment
int process_input(char *line  , char *cell1 , char *cell2 , char *operation , char* cell3 ){
    printf("Processing input\n");
    int i = 0;
    while (line[i] != '\0'){
        if (line[i] == ' ') return 0;
        i++;
    }   
    if (line[2] == '\0'){
        if (line[0] == 'q' || line[0] == 's' || line[0] == 'w' || line[0] == 'a' || line[0] == 'd') {operation[0] = line[0]; operation[1] = '\0'; return 1;}
        else return 0;
    }
    
}