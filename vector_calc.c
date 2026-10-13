/*
 * vector_calc.c
 * Hold vector navigation and operation functions
 * Nick Erdmann
 * 9/29/2026
 * version 1.1
 * make, ./main
 */

#include <stdio.h>
#include <string.h>
#include "vector_calc.h"


// vector storage

static Vector vectors[MAX_VECTORS + 1];
static int vector_count = 0;
static int ans_exists = 0;


/* find vector : int find_vector()
 *
 * searches the array for a vector with the given name
 * copies the matching vector into result
 * returns 0 if found, -1 otherwise
 */

int find_vector(const char *name, Vector *result)
{
     // search for ans in its reserved slot
    if (strcmp(name, "ans") == 0)
    {
        if (!ans_exists)
        {
            return -1;
        }

        *result = vectors[MAX_VECTORS];
        return 0;
    }

    // search the ordinary vector slots
    for (int i = 0; i < vector_count; i++)
    {
        if (strcmp(vectors[i].name, name) == 0)
        {
            *result = vectors[i];
            return 0;
        }
    }

    return -1;
}


/* add vector to array : int add_vector()
 *
 * replaces an existing vector if its name matches
 * otherwise, adds the vector to the next available position
 * returns 0 on success and -1 if storage is full
 */

int add_vector(Vector v)
{
    // store ans in its reserved slot
    if (strcmp(v.name, "ans") == 0)
    {
        vectors[MAX_VECTORS] = v;
        ans_exists = 1;
        return 0;
    }

    // update an existing vector if its name matches
    for (int i = 0; i < vector_count; i++)
    {
        if (strcmp(vectors[i].name, v.name) == 0)
        {
            vectors[i] = v;
            return 0;
        }
    }

    // reject new vectors when all ordinary slots are occupied
    if (vector_count >= MAX_VECTORS)
    {
        printf("Error: maximum of %d vectors reached\n", MAX_VECTORS);
        return -1;
    }

    // add the new vector
    vectors[vector_count] = v;
    vector_count++;

    return 0;
}


/* print vector : void print_vector()
 *
 * prints a vector's name and components
 */

void print_vector(Vector v)
{
    printf("%s = %g %g %g\n", v.name, v.x, v.y, v.z);
}


/* find answer vector : Vector *find_answer_vector()
 *
 * finds the vector named "ans".
 * if it does not exist, creates it if storage has room
 * returns a pointer to the stored answer vector
 * returns NULL if storage is full
 */

Vector *find_answer_vector(void)
{

    // search for an existing answer vector
    for (int i = 0; i < vector_count; i++)
    {
        if (strcmp(vectors[i].name, "ans") == 0)
        {
            return &vectors[i];
        }
    }

    // check whether storage has room
    if (vector_count >= MAX_VECTORS)
    {
        return NULL;
    }

    // create a new answer vector for use
    strcpy(vectors[vector_count].name, "ans");

    vectors[vector_count].x = 0.0;
    vectors[vector_count].y = 0.0;
    vectors[vector_count].z = 0.0;

    vector_count++;

    return &vectors[vector_count - 1];
}

//---------------------------------------------------
//OPERATIONS
//------------------------------------------------------


/* add method : void add_vectors()
 *
 * finds vectors a and b
 * adds their components
 * stores and prints the result under the specified name
 */

void add_vectors(const char *a, const char *b, const char *result)
{
    Vector alpha;
    Vector beta;
    Vector temp;

    // find the two source vectors
    if (find_vector(a, &alpha) != 0 ||
        find_vector(b, &beta) != 0)
    {
        printf("Error: Vector not found\n");
        return;
    }

    // calculate the result
    temp.x = alpha.x + beta.x;
    temp.y = alpha.y + beta.y;
    temp.z = alpha.z + beta.z;

    // give the result its name
    if (strlen(result) >= sizeof(temp.name))
    {
        printf("Error: Vector name is too long\n");
        return;
    }

    strcpy(temp.name, result);

    // save the result
    if (add_vector(temp) != 0)
    {
        return;
    }

    // print the result
    print_vector(temp);
}


/* subtract method : void sub_vectors()
 *
 * finds vectors a and b
 * subtracts their components
 * stores and prints the result under the specified name
 */

void sub_vectors(const char *a, const char *b, const char *result)
{
    Vector alpha;
    Vector beta;
    Vector temp;

    // find the two source vectors
    if (find_vector(a, &alpha) != 0 ||
        find_vector(b, &beta) != 0)
    {
        printf("Error: Vector not found.\n");
        return;
    }

    // calculate the result
    temp.x = alpha.x - beta.x;
    temp.y = alpha.y - beta.y;
    temp.z = alpha.z - beta.z;

    // give the result its name
    if (strlen(result) >= sizeof(temp.name))
    {
        printf("Error: Vector name is too long.\n");
        return;
    }

    strcpy(temp.name, result);

    // save the result
    if (add_vector(temp) != 0)
    {
        return;
    }

    // print the result
    print_vector(temp);
}


/* multiply method : void mult_vectors()
 *
 * multiplies a vector by a scalar
 * stores and prints the result under the specified name
 */

void mult_vectors(const char *name, double scalar, const char *result)
{
    Vector alpha;
    Vector temp;

    // find the source vector
    if (find_vector(name, &alpha) != 0)
    {
        printf("Error: Vector not found.\n");
        return;
    }

    // calculate the result
    temp.x = alpha.x * scalar;
    temp.y = alpha.y * scalar;
    temp.z = alpha.z * scalar;

    // give the result its name
    if (strlen(result) >= sizeof(temp.name))
    {
        printf("Error: Vector name is too long.\n");
        return;
    }

    strcpy(temp.name, result);

    // save the result
    if (add_vector(temp) != 0)
    {
        return;
    }

    // print the result
    print_vector(temp);
}


/* dot product : double dot_vector()
 *
 * calculates the dot product of two vectors
 * returns the resulting scalar
 *
 * optional extra credit
 */

double dot_vector(const char *a, const char *b)
{
    Vector alpha;
    Vector beta;

    // find the source vectors
    if (find_vector(a, &alpha) != 0 ||
        find_vector(b, &beta) != 0)
    {
        printf("Error: Vector not found.\n");
        return 0.0;
    }

    // calculate the dot product
    return alpha.x * beta.x
         + alpha.y * beta.y
         + alpha.z * beta.z;
}


/* cross product : void cross_vectors()
 *
 * calculates the cross product of two vectors
 * stores and prints the result under the specified name
 *
 * optional extra credit
 */

void cross_vectors(const char *a, const char *b, const char *result)
{
    Vector alpha;
    Vector beta;
    Vector temp;

    // find the source vectors
    if (find_vector(a, &alpha) != 0 ||
        find_vector(b, &beta) != 0)
    {
        printf("Error: Vector not found.\n");
        return;
    }

    // calculate the cross product
    temp.x = alpha.y * beta.z - alpha.z * beta.y;
    temp.y = alpha.z * beta.x - alpha.x * beta.z;
    temp.z = alpha.x * beta.y - alpha.y * beta.x;

    // give the result its name
    if (strlen(result) >= sizeof(temp.name))
    {
        printf("Error: Vector name is too long.\n");
        return;
    }

    strcpy(temp.name, result);

    // save the result
    if (add_vector(temp) != 0)
    {
        return;
    }

    // print the result
    print_vector(temp);
}

//---------------------------------------
//vector manipulation
//--------------------------------------



/* clear : void clear_array()
 *
 * removes all stored vectors
 */

void clear_array(void)
{
    vector_count = 0;
    ans_exists = 0;

    printf("Vector storage cleared.\n");
}


/* list : void list_array()
 *
 * prints all stored vectors except "ans"
 */

void list_array(void)
{

    for (int i = 0; i < vector_count; i++)
    {
        print_vector(vectors[i]);
    }
}