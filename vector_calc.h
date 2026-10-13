#ifndef VECTOR_CALC_H
#define VECTOR_CALC_H

#define MAX_VECTORS 10

// structure for a 3d vector 
typedef struct
{
    char name[20];
    double x;
    double y;
    double z;
} Vector;

//vector ops
void add_vectors(const char *a, const char *b, const char *result);
void sub_vectors(const char *a, const char *b, const char *result);
void mult_vectors(const char *name, double scalar, const char *result);

//extra vector ops
double dot_vector(const char *a, const char *b);
void cross_vectors(const char *a, const char *b, const char *result);

//vector manipulation
int add_vector(Vector v);
int find_vector(const char *name, Vector *result);
void clear_array(void);
void list_array(void);

//find answer vector
Vector *find_answer_vector(void);

//display vector
void print_vector(Vector v);

#endif
