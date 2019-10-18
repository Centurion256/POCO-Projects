#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "../src/string.h"

int main(int argc, char *argv[])
{
    if (argc <= 1){
        printf("There is nothing to open and read.\n");
        return -1;
    }
    FILE * read_file;
    read_file = fopen(argv[1], "r");
    my_str_t word;
    my_str_create(&word, 0);
    int number_of_words = 0;
    int words_len = 0;
    int c;
    int letter;
    size_t i;
    do{
        c = my_str_read_file_delim_if(&word, read_file, isspace);
        number_of_words += (my_str_size(&word) != 0);
        for (i = 0; i < my_str_size(&word); i++){
            if(isalpha(my_str_getc(&word, i))){
                words_len += 1;
            }
        }
    }while (c != 1);
    
    printf("%.5f\n", (double)words_len/(double)number_of_words);
    return 0;
}