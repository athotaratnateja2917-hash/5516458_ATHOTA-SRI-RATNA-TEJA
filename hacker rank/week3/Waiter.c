#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* readline();
char* ltrim(char*);
char* rtrim(char*);
char** split_string(char*);

int parse_int(char*);

/*
 * Complete the 'waiter' function below.
 *
 * The function is expected to return an INTEGER_ARRAY.
 * The function accepts following parameters:
 *  1. INTEGER_ARRAY number
 *  2. INTEGER q
 */

/*
 * To return the integer array from the function, you should:
 *     - Store the size of the array to be returned in the result_count variable
 *     - Allocate the array statically or dynamically
 *
 * For example,
 * int* return_integer_array_using_static_allocation(int* result_count) {
 *     *result_count = 5;
 *
 *     static int a[5] = {1, 2, 3, 4, 5};
 *
 *     return a;
 * }
 *
 * int* return_integer_array_using_dynamic_allocation(int* result_count) {
 *     *result_count = 5;
 *
 *     int *a = malloc(5 * sizeof(int));
 *
 *     for (int i = 0; i < 5; i++) {
 *         *(a + i) = i + 1;
 *     }
 *
 *     return a;
 * }
 *
 */
 
 static int* first_primes(int count){
    if(count <= 0) return NULL;
    
    const int LIMIT = 10001;
    bool* is_prime = calloc(LIMIT, sizeof(bool));
    if (!is_prime) return NULL;
    
    for (int i = 2; i < LIMIT; i++) is_prime[i] = true;
    
    for(int p = 2; p *p < LIMIT; p++){
        if(is_prime[p]){
            for(int m = p * p; m < LIMIT; m+= p){
                is_prime[m] = false;
            }
        }
    }
    
    int* primes = malloc(count * sizeof(int));
    if (!primes){ free(is_prime); return NULL; }
    
    int k = 0;
    for (int i = 2; i < LIMIT && k < count; i++){
        if(is_prime[i]) primes[k++] = i;
    }
    
    free(is_prime);
    return primes;
 }
 
int* waiter(int number_count, int* number, int q, int* result_count) {
    *result_count = number_count;
    int* result = malloc(number_count * sizeof(int));
    if(!result) return NULL;
    
    int* primes = first_primes(q);
    if(!primes){ free(result); return NULL; }
    
    int top = number_count;
    int* current = malloc(number_count * sizeof(int));
    int* next = malloc(number_count * sizeof(int));
    
    if(!current || !next){
        free(primes);
        free(result);
        free(current);
        free(next);
        return NULL;
    }
    
    for (int i = 0; i < number_count; i++){
        current[i] = number[i];
    }
    int res_index = 0;
    
    for(int round = 0; round < q; round++){
        int prime = primes[round];
        int next_top = 0;
        int b_stack[number_count];
        int b_top = 0;
        
        for (int i = top - 1; i >= 0; i--){
            if(current[i] % prime == 0){
                b_stack[b_top++] = current[i];
            } else {
                next[next_top++] = current[i];
            }
        }
        
        for(int i = b_top - 1; i >= 0; i--){
            result[res_index++] = b_stack[i];
        }
        
        int* temp = current;
        current = next;
        next = temp;
        top = next_top;
        
        if (top == 0) break;
    }
    
    for(int i = top - 1; i >= 0; i--){
        result[res_index++] = current[i];
    }
    
    free(primes);
    free(current);
    free(next);
    
    return result;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char** first_multiple_input = split_string(rtrim(readline()));

    int n = parse_int(*(first_multiple_input + 0));

    int q = parse_int(*(first_multiple_input + 1));

    char** number_temp = split_string(rtrim(readline()));

    int* number = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        int number_item = parse_int(*(number_temp + i));

        *(number + i) = number_item;
    }

    int result_count;
    int* result = waiter(n, number, q, &result_count);

    for (int i = 0; i < result_count; i++) {
        fprintf(fptr, "%d", *(result + i));

        if (i != result_count - 1) {
            fprintf(fptr, "\n");
        }
    }

    fprintf(fptr, "\n");

    fclose(fptr);

    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;

    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) {
            break;
        }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }

        alloc_length <<= 1;

        data = realloc(data, alloc_length);

        if (!data) {
            data = '\0';

            break;
        }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';

        data = realloc(data, data_length);

        if (!data) {
            data = '\0';
        }
    } else {
        data = realloc(data, data_length + 1);

        if (!data) {
            data = '\0';
        } else {
            data[data_length] = '\0';
        }
    }

    return data;
}

char* ltrim(char* str) {
    if (!str) {
        return '\0';
    }

    if (!*str) {
        return str;
    }

    while (*str != '\0' && isspace(*str)) {
        str++;
    }

    return str;
}

char* rtrim(char* str) {
    if (!str) {
        return '\0';
    }

    if (!*str) {
        return str;
    }

    char* end = str + strlen(str) - 1;

    while (end >= str && isspace(*end)) {
        end--;
    }

    *(end + 1) = '\0';

    return str;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");

    int spaces = 0;

    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);

        if (!splits) {
            return splits;
        }

        splits[spaces - 1] = token;

        token = strtok(NULL, " ");
    }

    return splits;
}

int parse_int(char* str) {
    char* endptr;
    int value = strtol(str, &endptr, 10);

    if (endptr == str || *endptr != '\0') {
        exit(EXIT_FAILURE);
    }

    return value;
}
