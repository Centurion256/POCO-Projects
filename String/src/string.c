#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Creates a new structure(datatype)
typedef struct
{
    size_t capacity_m; // Розмір блока
    size_t size_m;	   // Фактичний розмір стрічки
    char*  data;	   // Вказівник на блок пам'яті
} my_str_t;

//!============================================================================
//! Інформація про стрічку
//!============================================================================

//! Повертає розмір стрічки.
//! Для нульового вказівника -- 0.
size_t my_str_size(const my_str_t *str)
{
    if (!str)
        return 0;
    else
        return str->size_m;
}

//! Повертає розмір буфера.
//! Для нульового вказівника -- 0.
size_t my_str_capacity(const my_str_t *str)
{
    if (!str)
        return 0;
    else
        return str->capacity_m;
}

//! Повертає булеве значення, чи стрічка порожня:
int my_str_empty(const my_str_t * str)
{
//!    return str->size_m == 0;
    return my_str_size(str) == 0;
}

//!===========================================================================
//! Доступ до символів стрічки
//!===========================================================================

//! Повертає символ у вказаній позиції, або -1, якщо вихід за межі стрічки,
//! включаючи переданий нульовий вказівник.
//! Тому, власне, int а не char
int my_str_getc(const my_str_t *str, size_t index)
{
    if (my_str_empty(str) == 1)
        return -1;

    if (index > str->size_m)
        return -1;
    else
    {
        return str->data[index];
    }
}

//! Записує символ у вказану позиції (заміняючи той, що там був),
//! Повертає 0, якщо позиція в межах стрічки,
//! Поветає -1, не змінюючи її вмісту, якщо ні.
int my_str_putc(my_str_t *str, size_t index, char c)
{
    if (my_str_empty(str))
        return -1;
    if (index > str->size_m)
        return -1;
    else
    {
        *(str->data+index) = c;
        return 0;
    }
}

//! Повернути вказівник на С-стрічку, еквівалентну str.
//! Вважатимемо, що змінювати цю С-стрічку заборонено.
//! Вважатимемо, що як тільки my_str_t змінено, цей вказівник
//! може стати некоректним.
//! Якщо в буфері було зарезервовано на байт більше за макс. розмір, можна
//! просто додати нульовий символ в кінці та повернути вказівник data.
const char *my_str_get_cstr(my_str_t *str)
{
    return 0;
}

//!===========================================================================
//! Модифікації стрічки, що змінюють її розмір і можуть викликати реалокацію.
//!===========================================================================
//! Якщо буфер недостатній -- ці функції збільшують його,
//! викликом my_str_reserve().
//! Розумним є буфер кожного разу збільшувати в 1.8-2 рази.
//! ==========================================================================

//! Додає символ в кінець.
//! Повертає 0, якщо успішно,
//! -1 -- якщо передано нульовий вказівник,
//! -2 -- помилка виділення додаткової пам'яті.
int my_str_pushback(my_str_t *str, char c){
    return 0;
}

//! Викидає символ з кінця.
//! Повертає його, якщо успішно,
//! -1 -- якщо передано нульовий вказівник,
//! -2 -- якщо стрічка порожня.
int my_str_popback(my_str_t *str)
{
    if(!str){
        return -1;
    }
    if(my_str_empty(str)){
        return -2;
    }
    char c = my_str_getc(str, str->size_m-1);
    str->size_m -=1;
    return c;
}

//! Копіює стрічку. Якщо reserve == true,
//! то із тим же розміром буферу, що й вихідна,
//! інакше -- із буфером мінімального достатнього розміру.
//! (Старий вміст стрічки перед тим звільняє, за потреби).
//! Повертає 0, якщо успішно, різні від'ємні числа для діагностики
//! проблеми некоректних аргументів.
int my_str_copy(const my_str_t *from, my_str_t *to, int reserve)
{
    return 0;
}

//! Очищає стрічку -- робить її порожньою. Складність має бути О(1).
//! Уточнення (чомусь ця ф-ція викликала багато непорозумінь):
//! стрічка продовжує існувати, буфер той самий, того ж розміру, що був,
//! лише містить 0 символів -- єдине, що вона робить, це size_m = 0.
void my_str_clear(my_str_t *str)
{
    str->size_m = 0;
    //return 0;
}

//! Вставити символ у стрічку в заданій позиції, змістивши решту символів праворуч.
//! За потреби -- збільшує буфер.
//! У випадку помилки повертає різні від'ємні числа, якщо все ОК -- 0.
int my_str_insert_c(my_str_t *str, char c, size_t pos)
{
    return 0;
}

//! Вставити стрічку в заданій позиції, змістивши решту символів праворуч.
//! За потреби -- збільшує буфер.
//! У випадку помилки повертає різні від'ємні числа, якщо все ОК -- 0.
int my_str_insert(my_str_t *str, const my_str_t *from, size_t pos)
{
    return 0;
}

//! Вставити C-стрічку в заданій позиції, змістивши решту символів праворуч.
//! За потреби -- збільшує буфер.
//! У випадку помилки повертає різні від'ємні числа, якщо все ОК -- 0.
int my_str_insert_cstr(my_str_t *str, const char *from, size_t pos)
{
    return 0;
}

//! Додати стрічку в кінець.
//! За потреби -- збільшує буфер.
//! У випадку помилки повертає різні від'ємні числа, якщо все ОК -- 0.
int my_str_append(my_str_t *str, const my_str_t *from)
{
    return 0;
}

//! Додати С-стрічку в кінець.
//! За потреби -- збільшує буфер.
//! У випадку помилки повертає різні від'ємні числа, якщо все ОК -- 0.
int my_str_append_cstr(my_str_t *str, const char *from)
{
    return 0;
}

//! Скопіювати підстрічку, із beg включно, по end не включно ([beg, end)).
//! Якщо end за межами початкової стрічки -- це не помилка, копіювати всі
//! символи до кінця. beg має бути в її межах -- якщо beg>size_m, це помилка.
//! За потреби -- збільшує буфер.
//! У випадку помилки повертає різні від'ємні числа, якщо все ОК -- 0.
int my_str_substr(const my_str_t *from, my_str_t *to, size_t beg, size_t end)
{
    return 0;
}

//! C-string варіант my_str_substr().
//! Вважати, що в цільовій С-стрічці достатньо місц.
int my_str_substr_cstr(const my_str_t *from, char *to, size_t beg, size_t end)
{
    return 0;
}

//!===========================================================================
//! Маніпуляції розміром стрічки
//!===========================================================================

//! Збільшує буфер стрічки, із збереженням вмісту,
//! якщо новий розмір більший за попередній,
//! не робить нічого, якщо менший або рівний.
//! (Як показує практика, це -- корисний підхід).
//! Для збільшення виділяє новий буфер, копіює вміст
//! стрічки (size_m символів -- немає сенсу копіювати
//! решту буфера) із старого буфера та звільняє його.
//! У випадку помилки повертає різні від'ємні числа, якщо все ОК -- 0.
int my_str_reserve(my_str_t *str, size_t buf_size)
{
    if (buf_size <= str->capacity_m)
    {
        return 0;
    }
    char* new_buffer = (char*)malloc(sizeof(char) * (buf_size+1));
    if (!new_buffer)
    {
        free(new_buffer);
        return 2;
    }
    memcpy(new_buffer, str->data, str->size_m);
    memset(new_buffer+buf_size, '\0', 1);
    free(str->data);
    str->data = new_buffer;
    str->size_m = buf_size;
    return 0;
}

//! Робить буфер розміром, рівний необхідному:
//! так, щоб capacity_m == size_t. Єдиний "офіційний"
//! спосіб зменшити фактичний розмір буфера.
//! У випадку помилки повертає різні від'ємні числа, якщо все ОК -- 0.
int my_str_shrink_to_fit(my_str_t *str)
{
    char* new_capacity = (char*)malloc(sizeof(char) * (str->size_m+1));
    if (!new_capacity)
    {
        free(new_capacity);
        return 2;
    }
    memcpy(new_capacity, str->data, str->size_m);
    memset(new_capacity+str->size_m+1, '\0', 1);
    free(str->data);
    str->data = new_capacity;
    return 0;
}

//! Якщо new_size менший за поточний розмір -- просто
//! відкидає зайві символи (зменшуючи size_m). Якщо
//! більший -- збільшує фактичний розмір стрічки,
//! встановлюючи нові символи рівними sym.
//! За потреби, збільшує буфер.
//! Сподіваюся, різниця між розміром буфера та фактичним
//! розміром стрічки зрозуміла?
//! У випадку помилки повертає різні від'ємні числа, якщо все ОК -- 0.
int my_str_resize(my_str_t *str, size_t new_size, char sym)
{
    if (new_size < str->size_m)
    {
        memset(str->data + (new_size), 0, (str->size_m - new_size));
        memset(str->data + (str->size_m - new_size), '\0', 1);
	    str->size_m = new_size;
        return 0;
    }
    else if (new_size < str->capacity_m)
    {
        size_t diff = new_size-str->size_m;
        memset(str->data + str->size_m, sym, diff);
        str->size_m = new_size;
        return 0;
    }
    else
    {
        str->capacity_m = str->capacity_m*2;
        my_str_reserve(str, str->capacity_m);
        //Warning! untested recursion. - RECURSION TESTED.
        return my_str_resize(str, new_size, sym);
    }

}

//!===========================================================================
//! Функції пошуку та порівняння
//!===========================================================================

//! Знайти першу підстрічку в стрічці, повернути номер її
//! початку або (size_t)(-1), якщо не знайдено. from -- місце, з якого починати шукати.
//! Якщо більше за розмір -- вважати, що не знайдено.
size_t my_str_find(const my_str_t *str, const my_str_t *tofind, size_t from)
{
    if (from > str->size_m)
    {
        return (size_t)-1;
    }
    size_t j = 0;
    size_t i;
    for (i = 0; i < str->size_m; i++)
    {
        if (j == tofind->size_m)
        {
            //unsure whether we should add 1 to compensate for the 0-index.
            return i - tofind->size_m + 1;
        }
        else if (str->data[i] == tofind->data[from+j])
        {
            i++;
            j++;
        }
        else 
        {
            j = 0;
        }
    }   
    return (size_t)0;
}

//! Порівняти стрічки, повернути 0, якщо рівні (за вмістом!)
//! -1 (або інше від'ємне значення), якщо перша менша,
//! 1 (або інше додатне значення) -- якщо друга.
//! Поведінка має бути такою ж, як в strcmp.
int my_str_cmp(const my_str_t *str1, const my_str_t *str2)
{
    if (str1->size_m < str2->size_m)
        return -1;
    else if (str1->size_m > str2->size_m)
        return 1;
    else
    {
        char *pt1, *pt2;
        pt1 = str1->data;
        pt2 = str2->data;
        size_t i;
        for (i = 0; i < str1->size_m; i++)
            if (*pt1 < *pt2)
                return -1;
            else if (*pt1 > *pt2)
                return 1;
            else
            {
                pt1++;
                pt2++;
            }
        return 0;
    }
}

//! Порівняти стрічку із С-стрічкою, повернути 0, якщо рівні (за вмістом!)
//! -1 (або інше від'ємне значення), якщо перша менша,
//! 1 (або інше додатне значення) -- якщо друга.
//! Поведінка має бути такою ж, як в strcmp.
int my_str_cmp_cstr(const my_str_t *str1, const char *cstr2)
{
    return 0;
}

//! Знайти перший символ в стрічці, повернути його номер
//! або (size_t)(-1), якщо не знайдено. from -- місце, з якого починати шукати.
//! Якщо більше за розмір -- вважати, що не знайдено.
size_t my_str_find_c(const my_str_t *str, char tofind, size_t from)
{
    return 0;
}

//! Знайти символ в стрічці, для якого передана
//! функція повернула true, повернути його номер
//! або (size_t)(-1), якщо не знайдено:
size_t my_str_find_if(const my_str_t *str, int (*predicat)(int))
{
    return 0;
}

//!===========================================================================
//! Ввід-вивід
//!===========================================================================

//! Прочитати стрічку із файлу. Читає цілий файл.
//! Не давайте читанню вийти за межі буфера! За потреби --
//! збільшуйте буфер.
//! Рекомендую скористатися fgets().
//! У випадку помилки повертає різні від'ємні числа, якщо все ОК -- 0.
int my_str_read_file(my_str_t *str, FILE *file)
{
    return 0;
}

//! Аналог my_str_read_file, із stdin.
int my_str_read(my_str_t *str)
{
    return 0;
}

//! Записати стрічку в файл:
//! У випадку помилки повертає різні від'ємні числа, якщо все ОК -- 0.
int my_str_write_file(const my_str_t *str, FILE *file)
{
    return 0;
}

//! Записати стрічку на консоль:
//! У випадку помилки повертає різні від'ємні числа, якщо все ОК -- 0.
int my_str_write(const my_str_t *str, FILE *file)
{
    char *ptr = str->data;
    size_t length = 0;
    memset(ptr+str->size_m, 'X', 1);
    while (*ptr != '\0' && length < str->size_m)
    {
        printf("%c",*ptr);
        ptr++;
        length++;
    }
    return 0;
}

//! На відміну від my_str_read_file(), яка читає до кінця файлу,
//! читає по вказаний delimiter, за потреби
//! збільшує стрічку.
//! У випадку помилки повертає різні від'ємні числа, якщо все ОК -- 0.
int my_str_read_file_delim(my_str_t *str, FILE *file, char delimiter)
{
    return 0;
}

/* Return codes reference:
     0 - the program finished successfully.
     1 - buffer size_m too small.
     2 - couldn't allocate enough memory/not enough memory.
    -1 - a diffrerent error occured.
 */

void my_str_free(my_str_t *str)
{
    free(str);
}
//Constructor for my_str_t
int my_str_create(my_str_t *str, size_t buf_size)
{
    //set the capacity_m and size_m
    str->capacity_m = buf_size;
    str->size_m = 0;
    //attempt to allocate enough memory for data
    str->data = (char *)malloc(sizeof(char) * (buf_size + 1));
    //if there isn't enough memory, malloc() returns 0
    if (!str->data)
    {
        my_str_free(str);
        return 2;
    }
    return 0;
}


int main(int argc, char *argv[])
{
    printf("I'm at the beginning!\n");
    char *word = argv[1];
    puts(argv[1]);
    size_t buf_size = (size_t)atoi(argv[2]);
    printf("%u\n", buf_size);
    my_str_t string;
    my_str_create(&string, buf_size);
    size_t leng = (size_t)strlen(word);
    printf("%u\n", leng);
    my_str_resize(&string, leng, 'R');
    my_str_write(&string, stdout);
}