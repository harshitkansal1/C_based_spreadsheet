#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#define MAX_TEST_OUTPUT_SIZE 4096

// Helper function to read output file - incomplete, not using yet, will see later if needed
char* read_output(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (!file) return NULL;
    
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char *buffer = malloc(length + 1);
    if (buffer) fread(buffer, 1, length, file);
    fclose(file);
    
    buffer[length] = '\0';
    return buffer;
}

int lines = 0;

//Test basic stuff - sheet creation, invalid commands, exit(q)
int test_basics(FILE *fp) {
    // Write input commands to input.txt
    FILE *inp = fopen("./testinpout/input.txt", "w");
    fprintf(inp, "This checks random input by user\n"); // Invalid command
    fprintf(inp, "q\n");                                // Exit command
    fclose(inp);

    // Run sheet with input redirected from input.txt
    // freopen("output.txt", "w", stdout);  // Redirect stdout for ./sheet
    system("./sheet 5 5 < ./testinpout/input.txt");

    // Write expected output template
    fprintf(fp, "\tA\tB\tC\tD\tE\t\n");
    for(int i = 1; i <= 5; i++) {
        fprintf(fp, "%d\t0\t0\t0\t0\t0\t\n", i);
    }
    fprintf(fp, "[0.0] (ok) > ");
    fprintf(fp, "\tA\tB\tC\tD\tE\t\n");
    for(int i = 1; i <= 5; i++) {
        fprintf(fp, "%d\t0\t0\t0\t0\t0\t\n", i);
    }
    fprintf(fp, "[0.0] (invalid command) > ");
    lines += 13;
    return lines;
}

//Test control inputs w a s d in corner cases, test creation of large sheet
int test_control_inputs_and_large_sheet(FILE *fp){
    FILE *inp = fopen("./testinpout/input.txt", "w");
    fprintf(inp, "w\n"); // Scroll up when already at upper end
    fprintf(inp, "a\n"); // Scroll left when already at left end
    fprintf(inp, "s\n"); // Scroll down normal
    fprintf(inp, "d\n"); // Scroll right normal
    fprintf(inp, "w\n"); // Scroll up normal
    fprintf(inp, "a\n"); // Scroll left normal
    fprintf(inp, "q\n"); // Exit
    fclose(inp);

    // freopen("output.txt", "w", stdout);
    system("./sheet 999 18278 < ./testinpout/input.txt"); // Large sheet
    // fclose(stdout);

    // FILE *fp = fopen("./testinpout/expected.txt", "w");
    fprintf(fp, "\tA\tB\tC\tD\tE\tF\tG\tH\tI\tJ\t\n");
    for(int i = 1; i <= 10; i++) {
        fprintf(fp, "%d\t0\t0\t0\t0\t0\t0\t0\t0\t0\t0\t\n", i);
    }
    fprintf(fp, "[0.0] (ok) > ");
    fprintf(fp, "\tA\tB\tC\tD\tE\tF\tG\tH\tI\tJ\t\n");
    for(int i = 1; i <= 10; i++) {
        fprintf(fp, "%d\t0\t0\t0\t0\t0\t0\t0\t0\t0\t0\t\n", i);
    }
    fprintf(fp, "[0.0] (ok) > ");
    fprintf(fp, "\tA\tB\tC\tD\tE\tF\tG\tH\tI\tJ\t\n");
    for(int i = 1; i <= 10; i++) {
        fprintf(fp, "%d\t0\t0\t0\t0\t0\t0\t0\t0\t0\t0\t\n", i);
    }
    fprintf(fp, "[0.0] (ok) > ");
    fprintf(fp, "\tA\tB\tC\tD\tE\tF\tG\tH\tI\tJ\t\n");
    for(int i = 11; i <= 20; i++) {
        fprintf(fp, "%d\t0\t0\t0\t0\t0\t0\t0\t0\t0\t0\t\n", i);
    }
    fprintf(fp, "[0.0] (ok) > ");
    fprintf(fp, "\tK\tL\tM\tN\tO\tP\tQ\tR\tS\tT\t\n");
    for(int i = 11; i <= 20; i++) {
        fprintf(fp, "%d\t0\t0\t0\t0\t0\t0\t0\t0\t0\t0\t\n", i);
    }
    fprintf(fp, "[0.0] (ok) > ");
    fprintf(fp, "\tK\tL\tM\tN\tO\tP\tQ\tR\tS\tT\t\n");
    for(int i = 1; i <= 10; i++) {
        fprintf(fp, "%d\t0\t0\t0\t0\t0\t0\t0\t0\t0\t0\t\n", i);
    }
    fprintf(fp, "[0.0] (ok) > ");
    fprintf(fp, "\tA\tB\tC\tD\tE\tF\tG\tH\tI\tJ\t\n");
    for(int i = 1; i <= 10; i++) {
        fprintf(fp, "%d\t0\t0\t0\t0\t0\t0\t0\t0\t0\t0\t\n", i);
    }
    fprintf(fp, "[0.0] (ok) > ");

    lines += 77;
    return lines;
}

//Test control inputs w a s d in small sheet where movement of sheet isnt possible
int test_control_inputs_on_small_sheet(FILE *fp){
    FILE *inp = fopen("./testinpout/input.txt", "w");
    fprintf(inp, "w\n"); // Scroll up when no space to scroll
    fprintf(inp, "a\n"); // Scroll left when no space to scroll
    fprintf(inp, "s\n"); // Scroll down when no space to scroll
    fprintf(inp, "d\n"); // Scroll right when no space to scroll
    fprintf(inp, "q\n"); // Exit
    fclose(inp);

    // freopen("output.txt", "w", stdout);
    system("./sheet 5 5 < ./testinpout/input.txt"); // Small sheet
    // fclose(stdout);

    // FILE *fp = fopen("./testinpout/expected.txt", "w");
    fprintf(fp, "\tA\tB\tC\tD\tE\t\n");
    for(int i = 1; i <= 5; i++) {
        fprintf(fp, "%d\t0\t0\t0\t0\t0\t\n", i);
    }
    fprintf(fp, "[0.0] (ok) > ");
    fprintf(fp, "\tA\tB\tC\tD\tE\n");
    for(int i = 1; i <= 5; i++) {
        fprintf(fp, "%d\t0\t0\t0\t0\t0\t\n", i);
    }
    fprintf(fp, "[0.0] (ok) > ");
    fprintf(fp, "\tA\tB\tC\tD\tE\t\n");
    for(int i = 1; i <= 5; i++) {
        fprintf(fp, "%d\t0\t0\t0\t0\t0\t\n", i);
    }
    fprintf(fp, "[0.0] (ok) > ");
    fprintf(fp, "\tA\tB\tC\tD\tE\t\n");
    for(int i = 1; i <= 5; i++) {
        fprintf(fp, "%d\t0\t0\t0\t0\t0\t\n", i);
    }
    fprintf(fp, "[0.0] (ok) > ");
    fprintf(fp, "\tA\tB\tC\tD\tE\t\n");
    for(int i = 1; i <= 5; i++) {
        fprintf(fp, "%d\t0\t0\t0\t0\t0\t\n", i);
    }
    fprintf(fp, "[0.0] (ok) > ");

    lines += 30;
    return lines;
}

//Test normal value assignment to the cells, checking if large value can be assigned or not
int test_value_assignment(FILE *fp) {
    // Write input commands to input.txt
    FILE *inp = fopen("./testinpout/input.txt", "w");
    fprintf(inp, "A1=10\n");          // Assign constant to A1
    fprintf(inp, "B1=A1\n");          // Assign cell reference to B1
    fprintf(inp, "C1=99999\n");       // Assign large constant
    fprintf(inp, "D1=A1+B1\n");       // Assign arithmetic expression
    fprintf(inp, "E1=0\n");           // Assign zero
    fprintf(inp, "q\n");              // Exit
    fclose(inp);

    // Run sheet with input redirected from input.txt
    system("./sheet 5 5 < ./testinpout/input.txt");

    // Write expected output template
    // Initial grid
    fprintf(fp, "\tA\tB\tC\tD\tE\t\n");
    for (int i = 1; i <= 5; i++) {
        fprintf(fp, "%d\t0\t0\t0\t0\t0\t\n", i);
    }
    fprintf(fp, "[0.0] (ok) > ");

    // After A1=10
    fprintf(fp, "\tA\tB\tC\tD\tE\t\n");
    fprintf(fp, "1\t10\t0\t0\t0\t0\t\n");
    for (int i = 2; i <= 5; i++) {
        fprintf(fp, "%d\t0\t0\t0\t0\t0\t\n", i);
    }
    fprintf(fp, "[0.0] (ok) > ");

    // After B1=A1
    fprintf(fp, "\tA\tB\tC\tD\tE\t\n");
    fprintf(fp, "1\t10\t10\t0\t0\t0\t\n");
    for (int i = 2; i <= 5; i++) {
        fprintf(fp, "%d\t0\t0\t0\t0\t0\t\n", i);
    }
    fprintf(fp, "[0.0] (ok) > ");

    // After C1=99999
    fprintf(fp, "\tA\tB\tC\tD\tE\t\n");
    fprintf(fp, "1\t10\t10\t99999\t0\t0\t\n");
    for (int i = 2; i <= 5; i++) {
        fprintf(fp, "%d\t0\t0\t0\t0\t0\t\n", i);
    }
    fprintf(fp, "[0.0] (ok) > ");

    // After D1=A1+B1
    fprintf(fp, "\tA\tB\tC\tD\tE\t\n");
    fprintf(fp, "1\t10\t10\t99999\t20\t0\t\n");
    for (int i = 2; i <= 5; i++) {
        fprintf(fp, "%d\t0\t0\t0\t0\t0\t\n", i);
    }
    fprintf(fp, "[0.0] (ok) > ");

    // After E1=0
    fprintf(fp, "\tA\tB\tC\tD\tE\t\n");
    fprintf(fp, "1\t10\t10\t99999\t20\t0\t\n");
    for (int i = 2; i <= 5; i++) {
        fprintf(fp, "%d\t0\t0\t0\t0\t0\t\n", i);
    }
    fprintf(fp, "[0.0] (ok) > ");

    int lines = 36;  // Number of lines written to expected.txt
    return lines;
}

//Test arithmetic operations on the cells, division by zero, multiplication by zero, etc.
int test_arithmetic_operations(FILE *fp) {
    // Write input commands to input.txt
    FILE *inp = fopen("./testinpout/input.txt", "w");
    fprintf(inp, "A1=10\n");          // Assign constant to A1
    fprintf(inp, "B1=5\n");           // Assign constant to B1
    fprintf(inp, "C1=A1+B1\n");       // Addition
    fprintf(inp, "D1=A1-B1\n");       // Subtraction
    fprintf(inp, "E1=A1*B1\n");       // Multiplication
    fprintf(inp, "F1=A1/B1\n");       // Division (valid)
    fprintf(inp, "G1=A1/0\n");        // Division by zero (invalid)
    fprintf(inp, "H1=0*B1\n");        // Multiplication by zero
    fprintf(inp, "q\n");              // Exit
    fclose(inp);

    // Run sheet with input redirected from input.txt
    system("./sheet 8 8 < ./testinpout/input.txt");

    // Write expected output template
    // Initial grid
    fprintf(fp, "\tA\tB\tC\tD\tE\tF\tG\tH\t\n");
    for (int i = 1; i <= 8; i++) {
        fprintf(fp, "%d\t0\t0\t0\t0\t0\t0\t0\t0\t\n", i);
    }
    fprintf(fp, "[0.0] (ok) > ");

    // After A1=10
    fprintf(fp, "\tA\tB\tC\tD\tE\tF\tG\tH\t\n");
    fprintf(fp, "1\t10\t0\t0\t0\t0\t0\t0\t0\t\n");
    for (int i = 2; i <= 8; i++) {
        fprintf(fp, "%d\t0\t0\t0\t0\t0\t0\t0\t0\t\n", i);
    }
    fprintf(fp, "[0.0] (ok) > ");

    // After B1=5
    fprintf(fp, "\tA\tB\tC\tD\tE\tF\tG\tH\t\n");
    fprintf(fp, "1\t10\t5\t0\t0\t0\t0\t0\t0\t\n");
    for (int i = 2; i <= 8; i++) {
        fprintf(fp, "%d\t0\t0\t0\t0\t0\t0\t0\t0\t\n", i);
    }
    fprintf(fp, "[0.0] (ok) > ");

    // After C1=A1+B1
    fprintf(fp, "\tA\tB\tC\tD\tE\tF\tG\tH\t\n");
    fprintf(fp, "1\t10\t5\t15\t0\t0\t0\t0\t0\t\n");
    for (int i = 2; i <= 8; i++) {
        fprintf(fp, "%d\t0\t0\t0\t0\t0\t0\t0\t0\t\n", i);
    }
    fprintf(fp, "[0.0] (ok) > ");

    // After D1=A1-B1
    fprintf(fp, "\tA\tB\tC\tD\tE\tF\tG\tH\t\n");
    fprintf(fp, "1\t10\t5\t15\t5\t0\t0\t0\t0\t\n");
    for (int i = 2; i <= 8; i++) {
        fprintf(fp, "%d\t0\t0\t0\t0\t0\t0\t0\t0\t\n", i);
    }
    fprintf(fp, "[0.0] (ok) > ");

    // After E1=A1*B1
    fprintf(fp, "\tA\tB\tC\tD\tE\tF\tG\tH\t\n");
    fprintf(fp, "1\t10\t5\t15\t5\t50\t0\t0\t0\n");
    for (int i = 2; i <= 8; i++) {
        fprintf(fp, "%d\t0\t0\t0\t0\t0\t0\t0\t0\t\n", i);
    }
    fprintf(fp, "[0.0] (ok) > ");

    // After F1=A1/B1
    fprintf(fp, "\tA\tB\tC\tD\tE\tF\tG\tH\t\n");
    fprintf(fp, "1\t10\t5\t15\t5\t50\t2\t0\t0\t\n");
    for (int i = 2; i <= 8; i++) {
        fprintf(fp, "%d\t0\t0\t0\t0\t0\t0\t0\t0\t\n", i);
    }
    fprintf(fp, "[0.0] (ok) > ");

    // After G1=A1/0 (division by zero)
    fprintf(fp, "\tA\tB\tC\tD\tE\tF\tG\tH\t\n");
    fprintf(fp, "1\t10\t5\t15\t5\t50\t2\tERR\t0\t\n");
    for (int i = 2; i <= 8; i++) {
        fprintf(fp, "%d\t0\t0\t0\t0\t0\t0\t0\t0\t\n", i);
    }
    fprintf(fp, "[0.0] (division by zero) > ");

    // After H1=0*B1
    fprintf(fp, "\tA\tB\tC\tD\tE\tF\tG\tH\t\n");
    fprintf(fp, "1\t10\t5\t15\t5\t50\t2\tERR\t0\t\n");
    for (int i = 2; i <= 8; i++) {
        fprintf(fp, "%d\t0\t0\t0\t0\t0\t0\t0\t0\t\n", i);
    }
    fprintf(fp, "[0.0] (ok) > ");

    int lines = 81;  // Number of lines written to expected.txt
    return lines;
}

//Test range operations like sum, avg, min, max, stdev, etc. along with invalid ranges and ranges exceeding sheet size
int test_range_operations(FILE *fp) {
    // Write input commands to input.txt
    FILE *inp = fopen("./testinpout/input.txt", "w");
    fprintf(inp, "A1=10\n");          // Assign constant to A1
    fprintf(inp, "A2=20\n");          // Assign constant to A2
    fprintf(inp, "A3=30\n");          // Assign constant to A3
    fprintf(inp, "B1=SUM(A1:A3)\n");  // Sum of A1:A3
    fprintf(inp, "B2=AVG(A1:A3)\n");  // Average of A1:A3
    fprintf(inp, "B3=MIN(A1:A3)\n");  // Minimum of A1:A3
    fprintf(inp, "B4=MAX(A1:A3)\n");  // Maximum of A1:A3
    fprintf(inp, "B5=STDEV(A1:A3)\n");// Standard deviation of A1:A3
    fprintf(inp, "C1=SUM(A1:A1)\n");  // Single-cell range
    fprintf(inp, "C2=SUM(A3:A1)\n");  // Invalid range (A3:A1)
    fprintf(inp, "C3=SUM(A1:Z100)\n");// Range exceeding sheet size
    fprintf(inp, "q\n");              // Exit
    fclose(inp);

    // Run sheet with input redirected from input.txt
    system("./sheet 5 3 < ./testinpout/input.txt");

    // Write expected output template
    // Initial grid
    fprintf(fp, "\tA\tB\tC\t\n");
    for (int i = 1; i <= 5; i++) {
        fprintf(fp, "%d\t0\t0\t0\t\n", i);
    }
    fprintf(fp, "[0.0] (ok) > ");

    // After A1=10
    fprintf(fp, "\tA\tB\tC\t\n");
    fprintf(fp, "1\t10\t0\t0\t\n");
    for (int i = 2; i <= 5; i++) {
        fprintf(fp, "%d\t0\t0\t0\t\n", i);
    }
    fprintf(fp, "[0.0] (ok) > ");

    // After A2=20
    fprintf(fp, "\tA\tB\tC\t\n");
    fprintf(fp, "1\t10\t0\t0\t\n");
    fprintf(fp, "2\t20\t0\t0\t\n");
    for (int i = 3; i <= 5; i++) {
        fprintf(fp, "%d\t0\t0\t0\t\n", i);
    }
    fprintf(fp, "[0.0] (ok) > ");

    // After A3=30
    fprintf(fp, "\tA\tB\tC\t\n");
    fprintf(fp, "1\t10\t0\t0\t\n");
    fprintf(fp, "2\t20\t0\t0\t\n");
    fprintf(fp, "3\t30\t0\t0\t\n");
    for (int i = 4; i <= 5; i++) {
        fprintf(fp, "%d\t0\t0\t0\t\n", i);
    }
    fprintf(fp, "[0.0] (ok) > ");

    // After B1=SUM(A1:A3)
    fprintf(fp, "\tA\tB\tC\t\n");
    fprintf(fp, "1\t10\t60\t0\t\n");
    fprintf(fp, "2\t20\t0\t0\t\n");
    fprintf(fp, "3\t30\t0\t0\t\n");
    for (int i = 4; i <= 5; i++) {
        fprintf(fp, "%d\t0\t0\t0\t\n", i);
    }
    fprintf(fp, "[0.0] (ok) > ");

    // After B2=AVG(A1:A3)
    fprintf(fp, "\tA\tB\tC\t\n");
    fprintf(fp, "1\t10\t60\t0\t\n");
    fprintf(fp, "2\t20\t20\t0\t\n");
    fprintf(fp, "3\t30\t0\t0\t\n");
    for (int i = 4; i <= 5; i++) {
        fprintf(fp, "%d\t0\t0\t0\t\n", i);
    }
    fprintf(fp, "[0.0] (ok) > ");

    // After B3=MIN(A1:A3)
    fprintf(fp, "\tA\tB\tC\t\n");
    fprintf(fp, "1\t10\t60\t0\t\n");
    fprintf(fp, "2\t20\t20\t0\t\n");
    fprintf(fp, "3\t30\t10\t0\t\n");
    for (int i = 4; i <= 5; i++) {
        fprintf(fp, "%d\t0\t0\t0\t\n", i);
    }
    fprintf(fp, "[0.0] (ok) > ");

    // After B4=MAX(A1:A3)
    fprintf(fp, "\tA\tB\tC\t\n");
    fprintf(fp, "1\t10\t60\t0\t\n");
    fprintf(fp, "2\t20\t20\t0\t\n");
    fprintf(fp, "3\t30\t10\t0\t\n");
    fprintf(fp, "4\t0\t30\t0\t\n");
    for (int i = 5; i <= 5; i++) {
        fprintf(fp, "%d\t0\t0\t0\t\n", i);
    }
    fprintf(fp, "[0.0] (ok) > ");

    // After B5=STDEV(A1:A3)
    fprintf(fp, "\tA\tB\tC\t\n");
    fprintf(fp, "1\t10\t60\t0\t\n");
    fprintf(fp, "2\t20\t20\t0\t\n");
    fprintf(fp, "3\t30\t10\t0\t\n");
    fprintf(fp, "4\t0\t30\t0\t\n");
    fprintf(fp, "5\t0\t66\t0\t\n");
    fprintf(fp, "[0.0] (ok) > ");

    // After C1=SUM(A1:A1)
    fprintf(fp, "\tA\tB\tC\t\n");
    fprintf(fp, "1\t10\t60\t10\t\n");
    fprintf(fp, "2\t20\t20\t0\t\n");
    fprintf(fp, "3\t30\t10\t0\t\n");
    fprintf(fp, "4\t0\t30\t0\t\n");
    fprintf(fp, "5\t0\t66\t0\t\n");
    fprintf(fp, "[0.0] (ok) > ");

    // After C2=SUM(A3:A1) (invalid range)
    fprintf(fp, "\tA\tB\tC\t\n");
    fprintf(fp, "1\t10\t60\t10\t\n");
    fprintf(fp, "2\t20\t20\tERR\t\n");
    fprintf(fp, "3\t30\t10\t0\t\n");
    fprintf(fp, "4\t0\t30\t0\t\n");
    fprintf(fp, "5\t0\t66\t0\t\n");
    fprintf(fp, "[0.0] (invalid range) > ");

    // After C3=SUM(A1:Z100) (range exceeding sheet size)
    fprintf(fp, "\tA\tB\tC\t\n");
    fprintf(fp, "1\t10\t60\t10\t\n");
    fprintf(fp, "2\t20\t20\tERR\t\n");
    fprintf(fp, "3\t30\t10\tERR\t\n");
    fprintf(fp, "4\t0\t30\t0\t\n");
    fprintf(fp, "5\t0\t66\t0\t\n");
    fprintf(fp, "[0.0] (invalid command) > ");

    int lines = 81;  // Number of lines written to expected.txt
    return lines;
}

// Test error handling explicitly - cycles, division by zero, invalid ranges
void test_errors() {
    
}

// Test dependency chain - multi level dependencies, self dependencies, etc.
void test_dependencies() {
    
}

// Test output suppression - enable_output, disable_output
void test_output_control() {
    
}


int main() {
    // Build program
    if (system("make") != 0) {
        fprintf(stderr, "Make failed\n");
        return 1;
    }

    // Prepare files
    FILE *fp = fopen("./expected.txt", "w");
    freopen("output.txt", "w", stdout);  // Redirect all stdout to output.txt

    // Run tests
    test_basics(fp);
    test_control_inputs_and_large_sheet(fp);
    test_control_inputs_on_small_sheet(fp);
    test_value_assignment(fp);
    test_arithmetic_operations(fp);
    test_range_operations(fp);


    // Cleanup
    fclose(fp);
    freopen("/dev/tty", "w", stdout);  // Restore terminal output
    printf("Tests completed. Compare output.txt with expected.txt\n");
    
    return 0;
}
