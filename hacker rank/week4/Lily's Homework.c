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
 * Complete the 'lilysHomework' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts INTEGER_ARRAY arr as parameter.
 */
 
 typedef struct{
    int value;
    int index;
 } item;
 
 int asc_cmp(const void * a, const void *b){
    return((item*)a)->value - ((item*)b)->value;
 }
 
 int desc_cmp(const void *a, const void *b){
    return ((item*)b)->value - ((item*)a)->value;
 }
 
 static int count_swaps(int n, int *arr, int (*cmp)(const void*, const void*)){
    item *items = malloc(n * sizeof(item));
    if (!items) return -1;
    
    for (int i = 0; i < n; i++){
        items[i].value = arr[i];
        items[i].index = i;
    }
    
    qsort(items, n, sizeof(item), cmp);
    
    int *new_pos = malloc(n * sizeof(int));
    int *visited = calloc(n, sizeof(int));
    if(!new_pos || !visited){
        free(items);
        free(new_pos);
        free(visited);
        return -1;
    }
    for (int i = 0; i < n; i++){
        new_pos[items[i].index] = i;
    }
    
    int swaps = 0;
    for(int i = 0; i < n; i++){
        if(visited[i] || new_pos[i] == i) continue;
        
        int cycle_len = 0, j = i;
        while (!visited[j]){
            visited[j] = 1;
            j = new_pos[j];
            cycle_len++;
        }
        swaps += (cycle_len - 1);
    }
    free(items);
    free(new_pos);
    free(visited);
    return swaps;
 }
 

int lilysHomework(int arr_count, int* arr) {
    int asc_swaps = count_swaps(arr_count, arr, asc_cmp);
    int desc_swaps = count_swaps(arr_count, arr, desc_cmp);
    return (asc_swaps < desc_swaps) ? asc_swaps : desc_swaps;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    int n = parse_int(ltrim(rtrim(readline())));

    char** arr_temp = split_string(rtrim(readline()));

    int* arr = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        int arr_item = parse_int(*(arr_temp + i));

        *(arr + i) = arr_item;
    }

    int result = lilysHomework(n, arr);

    fprintf(fptr, "%d\n", result);

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
