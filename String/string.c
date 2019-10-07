#include <stdio.h>
#include <stdlib.h>


//Creates a new structure(datatype)
typedef struct 
{
    //The current capacity of the string(how many places are filled)
    size_t capacity_m;
    //The maximum capacity of a string
    size_t size_m;
    //A character array containing the elemnts of a string.
    char* data;
} my_str_t;

//BEGIN function declaration
int my_str_create(my_str_t* str, size_t buf_size);
void my_str_free(my_str_t* str);
//END function declaration

/* Return codes reference:
     0 - the program finished successfully.
     1 - buffer size too small.
     2 - couldn't allocate enough memory/not enough memory.
    -1 - a diffrerent error occured.
 */

//Constructor for my_str_t
int my_str_create(my_str_t* str, size_t buf_size)
{
    //If the string already exists, there is no point in calling the constuctor.
    if (str != NULL)
    {
        //free the memory
        my_str_free(str);
        return -1;
    }
    //set the capacity and size
    str->capacity_m = 0;
    str->size_m = buf_size;
    //attempt to allocate enough memory for data
    str->data = (char*)malloc(sizeof(char)*(buf_size + 1));
    //if there isn't enough memory, malloc() returns 0
    if (!str->data)
    {
        my_str_free(str);
        return 2;
    }
    return 0;
}

void my_str_free(my_str_t* str)
{
    free(str);
}

int main(int argc, char* argv[])
{
    char* word = argv[1];
    my_str_t* string;
    my_str_create(string, (size_t)argv[2]);
}