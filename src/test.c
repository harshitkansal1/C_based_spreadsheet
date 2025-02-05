#include<stdio.h>
#include<stdlib.h>

int main() {
    int status;

    status = system("make");
    if (status != 0) {
        fprintf(stderr, "Error: Make command failed.\n");
        return 1;
    }

    status = system("./sheet 10 10 < testinpout/input.txt > testinpout/output.txt");
    if (status != 0) {
        fprintf(stderr, "Error: Failed to run ./sheet.\n");
        return 1;
    }

    printf(" Program executed successfully. Output stored in output.txt\n");
    return 0;
}

