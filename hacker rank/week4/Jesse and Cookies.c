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
 * Complete the 'cookies' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts following parameters:
 *  1. INTEGER k
 *  2. INTEGER_ARRAY A
 */


typedef struct{
    long long *data;
    int size;
    int capacity;
} minheap;

minheap* initheap(int capacity){
    minheap* heap = malloc(sizeof(minheap));
    heap->data = malloc(sizeof(long long) * capacity);
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

static inline void swap(long long *a, long long *b){
    long long temp = *a;
    *a = *b;
    *b = temp;
}

void heapifydown(minheap* heap, int i){
    int smallest = i;
    int left = i * 2+ 1;
    int right = i * 2 + 2;
    
    if (left < heap->size && heap->data[left] < heap->data[smallest])
        smallest = left;
    if (right < heap->size && heap->data[right] < heap->data[smallest])
        smallest = right;
    if (smallest != i){
        swap(&heap->data[i], &heap->data[smallest]);
        heapifydown(heap, smallest);
    }
}

void heapifyup (minheap* heap, int i){
    while (i > 0) {
        int parent = (i - 1)/2;
        if(heap->data[parent] <= heap->data[i]) break;
        swap(&heap->data[parent], &heap->data[i]);
        i = parent;
    }
}

void buildheap(minheap* heap, int *arr, int count){
    heap->size = count;
    for(int i = 0; i < count; i++){
        heap->data[i] = arr[i];
    }
    for(int i = count / 2 - 1; i >=0; i--){
        heapifydown(heap, i);
    }
}

long long popmin(minheap* heap){
    if(heap->size == 0) return -1;
    long long root = heap->data[0];
    heap->data[0] = heap->data[--heap->size];
    heapifydown(heap, 0);
    return root;
}

void pushheap(minheap* heap, long long value){
    if (heap->size >= heap->capacity) return;
    heap->data[heap->size] = value;
    heapifyup(heap, heap->size++);
}

int cookies(int k, int A_count, int* A) {
    minheap* heap = initheap(A_count);
    buildheap(heap, A, A_count);
    
    int ops = 0;
    while (heap->size >= 2 && heap->data[0] < k) {
        long long first = popmin(heap);
        long long second = popmin(heap);
        pushheap(heap, first + 2 * second);
        ops++;
    }
    
    int result = (heap->data[0] >= k) ? ops : -1;
    free(heap->data);
    free(heap);
    return result;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char** first_multiple_input = split_string(rtrim(readline()));

    int n = parse_int(*(first_multiple_input + 0));

    int k = parse_int(*(first_multiple_input + 1));

    char** A_temp = split_string(rtrim(readline()));

    int* A = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        int A_item = parse_int(*(A_temp + i));

        *(A + i) = A_item;
    }

    int result = cookies(k, n, A);

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
