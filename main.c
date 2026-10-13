/*
 * main.c
 * perform tokenization, and handle methods
 * Nick Erdmann
 * 9/29/2026
 * version 1.0
 * make, ./main
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "vector_calc.h"

#define INPUT_SIZE 256
#define NAME_SIZE 20

// display the available commands
void print_help(void)
{
    printf("\nvector calculator commands\n");
    printf("--------------------------\n");

    printf("\nvector assignment\n");
    printf("  a = 1 2 3       create a vector\n");
    printf("  a               display vector a\n");

    printf("\nvector operations\n");
    printf("  a + b           add two vectors\n");
    printf("  a - b           subtract two vectors\n");
    printf("  a * 2           multiply a vector by a scalar\n");
    printf("  2 * a           multiply a vector by a scalar\n");
    printf("  a . b           calculate the dot product\n");
    printf("  a x b           calculate the cross product\n");

    printf("\nnamed results\n");
    printf("  c = a + b       store the sum in c\n");
    printf("  c = a - b       store the difference in c\n");
    printf("  c = a * 2       store the scaled vector in c\n");
    printf("  c = a x b       store the cross product in c\n");

    printf("\nother commands\n");
    printf("  list            display all stored vectors\n");
    printf("  clear           remove all stored vectors\n");
    printf("  -h              display this help message\n");
    printf("  quit            exit the program\n\n");
}

// remove whitespace from the beginning and end of a string
char *trim(char *str)
{
    char *end;

    while (isspace((unsigned char)*str))
    {
        str++;
    }

    if (*str == '\0')
    {
        return str;
    }

    end = str + strlen(str) - 1;

    while (end > str && isspace((unsigned char)*end))
    {
        end--;
    }

    *(end + 1) = '\0';

    return str;
}

// check whether a string is a valid vector name
int valid_name(const char *name)
{
    size_t length = strlen(name);

    if (length == 0 || length >= NAME_SIZE)
    {
        return 0;
    }

    if (!isalpha((unsigned char)name[0]) && name[0] != '_')
    {
        return 0;
    }

    for (size_t i = 1; i < length; i++)
    {
        if (!isalnum((unsigned char)name[i]) && name[i] != '_')
        {
            return 0;
        }
    }

    return 1;
}

// convert a string to a double
int parse_number(const char *str, double *value)
{
    char *end;
    double number;

    errno = 0;
    number = strtod(str, &end);

    if (str == end || *end != '\0' || errno == ERANGE)
    {
        return 0;
    }

    *value = number;

    return 1;
}

// determine whether a string is a valid scalar
int is_number(const char *str)
{
    double value;

    return parse_number(str, &value);
}

// parse a numeric vector assignment
int parse_vector_assignment(char *name, char *values)
{
    char *tokens[4];
    char *token;
    int count = 0;
    Vector v;

    if (!valid_name(name))
    {
        printf("Error: Invalid vector name\n");
        return 1;
    }

    token = strtok(values, " ,\t\r\n");

    while (token != NULL && count < 4)
    {
        tokens[count++] = token;
        token = strtok(NULL, " ,\t\r\n");
    }

    if (count < 2 || count > 3)
    {
        printf("Error: A vector requires two or three components\n");
        return 1;
    }

    if (token != NULL)
    {
        printf("Error: Too many vector components\n");
        return 1;
    }

    strcpy(v.name, name);

    if (!parse_number(tokens[0], &v.x) ||
        !parse_number(tokens[1], &v.y))
    {
        printf("Error: Invalid vector component\n");
        return 1;
    }

    v.z = 0.0;

    if (count == 3 && !parse_number(tokens[2], &v.z))
    {
        printf("Error: Invalid vector component\n");
        return 1;
    }

    if (add_vector(v) != 0)
    {
        printf("Error: Could not store vector\n");
        return 1;
    }

    print_vector(v);

    return 1;
}

// parse a vector operation
int parse_operation(char *expression, const char *result_name)
{
    char *tokens[3];
    char *token;
    int count = 0;
    double scalar;
    double result;
    Vector v;

    token = strtok(expression, " \t\r\n");

    while (token != NULL && count < 3)
    {
        tokens[count++] = token;
        token = strtok(NULL, " \t\r\n");
    }

    if (count != 3 || token != NULL)
    {
        return 0;
    }

    // handle vector addition
    if (strcmp(tokens[1], "+") == 0)
    {
        if (find_vector(tokens[0], &v) != 0)
        {
            printf("Error: Vector '%s' not found\n", tokens[0]);
            return 1;
        }

        Vector other;

        if (find_vector(tokens[2], &other) != 0)
        {
            printf("Error: Vector '%s' not found\n", tokens[2]);
            return 1;
        }

        add_vectors(tokens[0], tokens[2], result_name);
        return 1;
    }

    // handle vector subtraction
    if (strcmp(tokens[1], "-") == 0)
    {
        Vector other;

        if (find_vector(tokens[0], &v) != 0)
        {
            printf("Error: Vector '%s' not found\n", tokens[0]);
            return 1;
        }

        if (find_vector(tokens[2], &other) != 0)
        {
            printf("Error: Vector '%s' not found\n", tokens[2]);
            return 1;
        }

        sub_vectors(tokens[0], tokens[2], result_name);
        return 1;
    }

    // handle scalar multiplication
    if (strcmp(tokens[1], "*") == 0)
    {
        if (is_number(tokens[0]))
        {
            if (!parse_number(tokens[0], &scalar))
            {
                return 1;
            }

            if (find_vector(tokens[2], &v))
            {
                printf("Error: Vector '%s' not found\n", tokens[2]);
                return 1;
            }

            mult_vectors(tokens[2], scalar, result_name);
            return 1;
        }

        if (is_number(tokens[2]))
        {
            if (!parse_number(tokens[2], &scalar))
            {
                return 1;
            }

            if (find_vector(tokens[0], &v) != 0)
            {
                printf("Error: Vector '%s' not found\n", tokens[0]);
                return 1;
            }

            mult_vectors(tokens[0], scalar, result_name);
            return 1;
        }

        printf("Error: Multiplication requires a vector and a scalar\n");
        return 1;
    }

    // handle dot product
    if (strcmp(tokens[1], ".") == 0)
    {
        if (find_vector(tokens[0], &v) != 0)
        {
            printf("Error: Vector '%s' not found\n", tokens[0]);
            return 1;
        }

        Vector other;

        if (find_vector(tokens[2], &other) != 0)
        {
            printf("Error: Vector '%s' not found\n", tokens[2]);
            return 1;
        }

        result = dot_vector(tokens[0], tokens[2]);
        printf("%g\n", result);

        return 1;
    }

    // handle cross product
    if (strcmp(tokens[1], "x") == 0)
    {
        Vector other;

        if (find_vector(tokens[0], &v) != 0)
        {
            printf("Error: Vector '%s' not found\n", tokens[0]);
            return 1;
        }

        if (find_vector(tokens[2], &other) != 0)
        {
            printf("Error: Vector '%s' not found\n", tokens[2]);
            return 1;
        }

        cross_vectors(tokens[0], tokens[2], result_name);
        return 1;
    }

    printf("Error: Unknown operator '%s'\n", tokens[1]);

    return 1;
}

// process a line containing an assignment
int process_assignment(char *input)
{
    char *equals;
    char name[NAME_SIZE];
    char expression[INPUT_SIZE];
    char *left;
    char *right;
    char *end;

    equals = strchr(input, '=');

    if (equals == NULL)
    {
        return 0;
    }

    *equals = '\0';

    left = trim(input);
    right = trim(equals + 1);

    if (!valid_name(left))
    {
        printf("Error: Invalid vector name\n");
        return 1;
    }

    strcpy(name, left);

    if (*right == '\0')
    {
        printf("Error: Missing assignment value\n");
        return 1;
    }

    // check whether the right side is a vector expression
    strncpy(expression, right, sizeof(expression) - 1);
    expression[sizeof(expression) - 1] = '\0';

    char expression_copy[INPUT_SIZE];
    strcpy(expression_copy, expression);

    char *tokens[4];
    int count = 0;
    char *token = strtok(expression_copy, " \t\r\n");

    while (token != NULL && count < 4)
    {
        tokens[count++] = token;
        token = strtok(NULL, " \t\r\n");
    }

    if (count == 3 &&
        (strcmp(tokens[1], "+") == 0 ||
         strcmp(tokens[1], "-") == 0 ||
         strcmp(tokens[1], "*") == 0 ||
         strcmp(tokens[1], "x") == 0 ||
         strcmp(tokens[1], ".") == 0))
    {
        return parse_operation(expression, name);
    }

    // otherwise treat the right side as numeric vector components
    end = right + strlen(right);

    while (end > right && isspace((unsigned char)*(end - 1)))
    {
        end--;
    }

    *end = '\0';

    return parse_vector_assignment(name, right);
}

// display a vector by name
int display_named_vector(const char *name)
{
    Vector v;

    if (find_vector(name, &v) != 0)
    {
        printf("Error: Vector '%s' not found\n", name);
        return 1;
    }

    print_vector(v);

    return 1;
}

//LAB7
int load_file(char *file_name)
{
    clear_array();








}





// process a single input line
int process_input(char *input)
{
    char expression[INPUT_SIZE];
    char *command;
    char *rest;

    input = trim(input);

    if (*input == '\0')
    {
        return 1;
    }

    if (strcmp(input, "quit") == 0 ||
        strcmp(input, "exit") == 0)
    {
        return 0;
    }

    if (strcmp(input, "-h") == 0 ||
        strcmp(input, "help") == 0)
    {
        print_help();
        return 1;
    }

    if (strcmp(input, "list") == 0 ||
        strcmp(input, "ls") == 0)
    {
        list_array();
        return 1;
    }

    if (strcmp(input, "clear") == 0)
    {
        clear_array();
        printf("Vectors cleared\n");
        return 1;
    }

    // //LAB 7
    // //check for load command and if exist load given file
    // char potential_load[5];
    // strncpy(potential_load, input, 4);
    // potential_load[4] = '\0';

    // if (strcmp(potential_load, "load") == 0) {

    //     char file_name[sizeof(input)-5];

    //     strncpy(file_name, input + 5, sizeof(file_name) - 1);
    //     file_name[sizeof(file_name) - 1] = '\0';

    //     load_file(file_name);

    //     return 1;
    // }

    // //LAB 7
    // //check for save command and if exist save given file
    // char potential_save[5];
    // strncpy(potential_save, input, 4);
    // potential_save[4] = '\0';

    // if (strcmp(potential_save, "save") == 0) {

    //     char file_name[sizeof(input)-5];

    //     strncpy(file_name, input + 5, sizeof(file_name) - 1);
    //     file_name[sizeof(file_name) - 1] = '\0';

    //     load_file(file_name);

    //     return 1;
    // }



    // handle assignments before standalone operations
    if (strchr(input, '=') != NULL)
    {
        process_assignment(input);
        return 1;
    }

    // make a copy before tokenizing the input
    strncpy(expression, input, sizeof(expression) - 1);
    expression[sizeof(expression) - 1] = '\0';

    command = strtok(expression, " \t\r\n");
    rest = strtok(NULL, " \t\r\n");

    if (command == NULL)
    {
        return 1;
    }

    // display a single named vector
    if (rest == NULL)
    {
        return display_named_vector(command);
    }

    // process an operation and store the default result in ans
    strncpy(expression, input, sizeof(expression) - 1);
    expression[sizeof(expression) - 1] = '\0';

    return parse_operation(expression, "ans") ? 1 : 1;
}

// main program
int main(int argc, char *argv[])
{
    char input[INPUT_SIZE];

    // handle help requested at program startup
    if (argc > 1 && strcmp(argv[1], "-h") == 0)
    {
        print_help();
        return 0;
    }

    printf("vector calculator\n");
    printf("enter -h for help or quit to exit\n");

    while (1)
    {
        printf("minimat> ");
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        //all returns of 0 will stop program
        if (!process_input(input))
        {
            break;
        }
    }

    return 0;
}