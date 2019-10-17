#include <stdio.h>
#include <stdlib.h>
#include "string.h"

int main(int argc, char *argv[])
{
    if (argc <= 1){
        printf("The nothing to open and read.");
        return -1;
    }
    FILE * read_file;
    read_file = fopen(argv[1], "r");
    my_str_t word;
    int number_of_words = 0;
    int words_len = 0;
    int c;
    int letter;
    do{
        c = my_str_read_file_delim(word, read_file, " ");
        number_of_words += 1;
        for (size_t i = 1; i < my_str_size(word) + 1; i++){
            letter = my_str_getc(i - 1);
            if isalpha(letter){
                words_len += 1;
            }
        }
    }
    while (c != 1);
    printf("%.5f\n", (double)words_len/(double)number_of_words);
    }
    return 0;
}