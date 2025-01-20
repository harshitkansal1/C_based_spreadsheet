int cell_to_coords(char*  , int*);
int** initialize_sheet(int, int);
void print_sheet(int** , int);
void process_control_input(int** , char);
int process_assign_input(int** sheet, char* , char*);
extern int ROWS , COLS;