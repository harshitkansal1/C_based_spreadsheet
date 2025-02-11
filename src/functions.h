#ifndef FUNCTIONS_H
#define FUNCTIONS_H

int min_range(int **, char *, char *, char *);
int max_range(int **, char *, char *, char *);
int sum_range(int **, char *, char *, char *);
int avg_range(int **, char *, char *, char *);
int std_dev_range(int **, char *, char *, char *);
void create_relation_graph(void);
int cell_to_coords(char *cell , int* coords);
int sleep_value(int**, char*, int);

#endif