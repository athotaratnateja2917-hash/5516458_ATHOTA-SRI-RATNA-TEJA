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

#define ALPHABET_SIZE 10

char* readline();
char* ltrim(char*);
char* rtrim(char*);

int parse_int(char*);

/*
 * Complete the 'noPrefix' function below.
 *
 * The function accepts STRING_ARRAY words as parameter.
 */
 
 typedef struct trinode{
    struct trinode* children[ALPHABET_SIZE];
    int is_end_of_word;
    int has_child;
 } trinode;
 
 trinode* createnode(){
    trinode* node = (trinode*)malloc(sizeof(trinode));
    node->is_end_of_word = 0;
    node->has_child = 0;
    for(int i = 0; i < ALPHABET_SIZE; i++){
        node->children[i] = NULL;
    }
    return node;
 }

void noPrefix(int words_count, char** words) {
    trinode* root = createnode();
    int bad_set_found = 0;
    for(int i = 0; i < words_count && !bad_set_found; i++){
        char* word = words[i];
        trinode* current = root;
        int len = strlen(word);
        int is_prefix = 0;
        
        for(int j = 0; j < len; j++){
            int k = word[j] - 'a';
            
            if(!current->children[k]){
                current->children[k] = createnode();
                current->has_child = 1;
            }
            current = current->children[k];
            
            if (current->is_end_of_word){
                is_prefix = 1;
                break;
            }
        }
        if (!is_prefix && current->has_child){
            is_prefix = 1;
        }
        
        if (is_prefix){
            printf("BAD SET\n%s\n", word);
            bad_set_found = 1;
        } else {
            current->is_end_of_word = 1;
        }
    }
    if(!bad_set_found){
        printf("GOOD SET\n");
    }

}

int main()
{
    int n = parse_int(ltrim(rtrim(readline())));

    char** words = malloc(n * sizeof(char*));

    for (int i = 0; i < n; i++) {
        char* words_item = readline();

        *(words + i) = words_item;
    }

    noPrefix(n, words);

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

int parse_int(char* str) {
    char* endptr;
    int value = strtol(str, &endptr, 10);

    if (endptr == str || *endptr != '\0') {
        exit(EXIT_FAILURE);
    }

    return value;
}
