#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./string.h"

//Creates a new structure(datatype)
/*
typedef struct
{
    size_t capacity_m; // Розмір блока
    size_t size_m;	   // Фактичний розмір стрічки
    char*  data;	   // Вказівник на блок пам'яті
} my_str_t;
*/
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

size_t my_str_cstr_len(const char* cstr){
    size_t res = 0;
    while(cstr[res] != '\0')
    {
        res +=1;
    }
    return res;

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
    if (index > my_str_size(str))
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
    if (index >= my_str_size(str))
        //we can't change null terminator
        return -1;
    else
    {
        str->data[index] = c;
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
    str->data[str->size_m] = '\0';
    return str->data;
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
    if(!str){
      return -1;
    }
    if(my_str_resize(str, my_str_size(str)+1, 0)){
    return -2;
    }
    str->data[str->size_m-1] = c;
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
    //! Copy to empty?
    if(!from){
      return -1;
    }
    int new_size;
    if(reserve){new_size = from->size_m;}
    else{new_size = from->capacity_m;}
    if(!to){
      if(my_str_create(to, new_size)){
        return -2;
      }
    }
    if(my_str_reserve(to, new_size)){
      return -2;}
    if(my_str_resize(to, my_str_size(from), '\0')){
      return -2;}
    memcpy(to->data, from->data, sizeof(char)*from->size_m);
    return 0;
}

//! Очищає стрічку -- робить її порожньою. Складність має бути О(1).
//! Уточнення (чомусь ця ф-ція викликала багато непорозумінь):
//! стрічка продовжує існувати, буфер той самий, того ж розміру, що був,
//! лише містить 0 символів -- єдине, що вона робить, це size_m = 0.
void my_str_clear(my_str_t *str)
{
    str->size_m = 0;
}

//! Вставити символ у стрічку в заданій позиції, змістивши решту символів праворуч.
//! За потреби -- збільшує буфер.
//! У випадку помилки повертає різні від'ємні числа, якщо все ОК -- 0.
int my_str_insert_c(my_str_t *str, char c, size_t pos)
{
    if(!str){
        return -1;
    }
    if(pos<0 || pos>my_str_size(str)){
        return -1;
    }
    if(my_str_resize(str, my_str_size(str)+1, '\0')){
        return -2;
    }
    
    memcpy(str->data+sizeof(char)*(pos+1), str->data+(sizeof(char)*pos), sizeof(char)*(my_str_size(str)-(pos+1)));
    memcpy(str->data+sizeof(char)*pos, &c, sizeof(char)*1);
    str->size_m+=1;
    return 0;
}

//! Вставити стрічку в заданій позиції, змістивши решту символів праворуч.
//! За потреби -- збільшує буфер.
//! У випадку помилки повертає різні від'ємні числа, якщо все ОК -- 0.
int my_str_insert(my_str_t *str, const my_str_t *from, size_t pos)
{
    if(!str||!from){
        return -1;
    }
    if(pos<0 || pos>my_str_size(str)){
        return -1;
    }
    if(my_str_resize(str, my_str_size(str) + my_str_size(from),'d')){
        return -2;
    }
    size_t csize = my_str_size(from);
    // printf("%zu\n", csize);
    if (csize >= 1)
    {
        memmove(str->data+sizeof(char)*(my_str_size(str)-csize), str->data+sizeof(char)*pos, sizeof(char)*(csize));
        memcpy(str->data+sizeof(char)*pos, from->data, sizeof(char)*csize);
    }
    return 0;
}

//! Вставити C-стрічку в заданій позиції, змістивши решту символів праворуч.
//! За потреби -- збільшує буфер.
//! У випадку помилки повертає різні від'ємні числа, якщо все ОК -- 0.
int my_str_insert_cstr(my_str_t *str, const char *from, size_t pos)
{
    if(!str||!from){
        return -1;
    }
    if(pos<0 || pos>my_str_size(str)){
        return -1;
    }

    size_t csize = my_str_cstr_len(from);
    
    if(my_str_resize(str, my_str_size(str)+csize, 0)){
        return -2;
    }
    if (csize >= 1)
    {
        memcpy(str->data+sizeof(char)*(pos+csize), str->data+(sizeof(char)*pos), sizeof(char)*(my_str_size(str)-(pos+csize)));
        memcpy(str->data+sizeof(char)*pos, from, sizeof(char)*csize);
    }
    return 0;
}


//! Додати стрічку в кінець.
//! За потреби -- збільшує буфер.
//! У випадку помилки повертає різні від'ємні числа, якщо все ОК -- 0.
int my_str_append(my_str_t *str, const my_str_t *from)
{
    if(!str){
        return -1;
    }
    if(!from){
        return -1;
    }
    if(my_str_resize(str, my_str_size(str)+my_str_size(from), '\0')){
        return -2;
    }
    memcpy(str->data + sizeof(char)*(my_str_size(str)-my_str_size(from)),
                 from->data, sizeof(char)*my_str_size(from));
    return 0;
}

//! Додати С-стрічку в кінець.
//! За потреби -- збільшує буфер.
//! У випадку помилки повертає різні від'ємні числа, якщо все ОК -- 0.
int my_str_append_cstr(my_str_t *str, const char *from)
{
    if(!str){
        return -1;
    }
    if(!from){
        return -1;
    }
    size_t csize = my_str_cstr_len(from);
    if(my_str_resize(str, my_str_size(str)+csize, 0)){
        return -2;
    }
    memcpy(str->data+my_str_size(str), from, sizeof(char)*csize);
    
    return 0;
}

//! Скопіювати підстрічку, із beg включно, по end не включно ([beg, end)).
//! Якщо end за межами початкової стрічки -- це не помилка, копіювати всі
//! символи до кінця. beg має бути в її межах -- якщо beg>size_m, це помилка.
//! За потреби -- збільшує буфер.
//! У випадку помилки повертає різні від'ємні числа, якщо все ОК -- 0.
int my_str_substr(const my_str_t *from, my_str_t *to, size_t beg, size_t end)
{
    if(!from || !to){
        return -1;
    }
    if(beg>=my_str_size(from)||beg<0){
        return -1;
    }
    if(end >= my_str_size(from)){
        end = my_str_size(from)-1;
    }
    my_str_clear(to);
    if(my_str_resize(to, end-beg, 0)){
        return -2;
    }
    memcpy(to->data, from->data+(sizeof(char)*beg), sizeof(char) *(end-beg));
    return 0;
}

//! C-string варіант my_str_substr().
//! Вважати, що в цільовій С-стрічці достатньо місц.
int my_str_substr_cstr(const my_str_t *from, char *to, size_t beg, size_t end)
{
    if(!from || !to){
        return -1;
    }
    if(beg>=my_str_size(from)||beg<0){
        return -1;
    }
    if(end >= my_str_size(from)){
        end = my_str_size(from)-1;
    }
    memset(to+sizeof(char) *(end-beg+1), '\0', sizeof(char)*1);
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
        return -2;
    }

    if(str->data){
        memcpy(new_buffer, str->data, sizeof(char)*str->size_m);
        free(str->data);
    }
    memset(new_buffer+buf_size, '\0', sizeof(char)*1);
    str->data = new_buffer;
    str->capacity_m = buf_size;
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
    memcpy(new_capacity, str->data, sizeof(char)*str->size_m);
    memset(new_capacity+str->size_m, '\0', sizeof(char)*1);
    free(str->data);
    str->data = new_capacity;
    str->capacity_m = str->size_m;
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
        memset(str->data + (new_size), 0, sizeof(char)*(str->size_m - new_size));
        memset(str->data + (str->size_m - new_size), '\0', sizeof(char)*1);
	    str->size_m = new_size;
        return 0;
    }
    else if (new_size < str->capacity_m)
    {
        size_t diff = new_size-str->size_m;
        memset(str->data + str->size_m, sym, sizeof(char)*diff);
        str->size_m = new_size;
        return 0;
    }
    else
    {
        size_t new_capacity = (str->capacity_m != 0) ? (str->capacity_m*2) : 2;
        my_str_reserve(str, new_capacity);
        str->capacity_m = new_capacity;
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
    int char1, char2;
    int i = 0;
    
    do{
        char1 = my_str_getc(str1, i);
        char2 = my_str_getc(str2, i);
        if (char1>char2){
            return 1;
        }
        if (char1<char2){
            return -1;
        }
        i+=1;
    }while((i <= my_str_size(str1)));
    return 0;
    
}

//! Порівняти стрічку із С-стрічкою, повернути 0, якщо рівні (за вмістом!)
//! -1 (або інше від'ємне значення), якщо перша менша,
//! 1 (або інше додатне значення) -- якщо друга.
//! Поведінка має бути такою ж, як в strcmp.
int my_str_cmp_cstr(const my_str_t *str1, const char *cstr2)
{
    int char1, char2;
    int i = 0;
    do{
        char1 = my_str_getc(str1, i);
        char2 = cstr2[i];
        if (char1>char2){
            return 1;
        }
        if (char1<char2){
            return -1;
        }
        i+=1;
    }while((i <= my_str_size(str1)));
    return 0;
}

//! Знайти перший символ в стрічці, повернути його номер
//! або (size_t)(-1), якщо не знайдено. from -- місце, з якого починати шукати.
//! Якщо більше за розмір -- вважати, що не знайдено.
size_t my_str_find_c(const my_str_t *str, char tofind, size_t from)
{
    if(!str)
    {
        return (size_t)-1;
    }
    if(from>my_str_size(str))
    {
        return (size_t)-1;
    }
    size_t i = from;
    if(from < 0){
        i = 0;
    }
    for(i; i<str->size_m; i++)
    {
        if(my_str_getc(str, i)==tofind)
        {
            return i;
        }
    }
    return (size_t)-1;
}

//! Знайти символ в стрічці, для якого передана
//! функція повернула true, повернути його номер
//! або (size_t)(-1), якщо не знайдено:
size_t my_str_find_if(const my_str_t *str, int (*predicat)(int))
{
  if(!str)
  {
    return (size_t)-1;
  }
  size_t i;
  for(i=0; i<str->size_m; i++)
  {
    //Function call syntax: (*name_of_the_function)(...)
    if((*predicat)(my_str_getc(str, i)))
    {
      return i;
    }
  }
    return (size_t)-1;
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
    return my_str_read_file_delim(str,file,EOF);
}

//! Аналог my_str_read_file, із stdin.
int my_str_read(my_str_t *str)
{
    char text[sizeof(size_t)];
    fgets(text,sizeof(size_t),stdin);
    return my_str_append_cstr(str, text);
}

//! Записати стрічку в файл:
//! У випадку помилки повертає різні від'ємні числа, якщо все ОК -- 0.
int my_str_write_file(const my_str_t *str, FILE *file)
{
    char *data;
    int size=str->size_m;
    data = str->data;
    for(int i=0; i<size;++i,++data)
        fputc(*data, file );
    return 0;
}

//! Записати стрічку на консоль:
//! У випадку помилки повертає різні від'ємні числа, якщо все ОК -- 0.
int my_str_write(const my_str_t *str)
{
    char *ptr = str->data;
    size_t length = 0;
    while (*ptr != '\0' && length < str->size_m)
    {
        //printf("%c",*ptr);
        ptr++;
        length++;
    }
    return 0;
}

//! На відміну від my_str_read_file(), яка читає до кінця файлу,
//! читає по вказаний delimiter, за потреби
//! збільшує стрічку.
//! У випадку помилки повертає різні від'ємні числа, якщо все ОК -- 0.
int my_str_read_file_delim(my_str_t *str, FILE *file, int delimiter)
{
    int c;
    do{
        c = fgetc(file);
        my_str_pushback(str, (char)c);
    }
    while (c != delimiter && c != EOF);
    if (c == EOF){
        return 1;
    }
    return 0;
}

//! Читає файл доки передана функція не поверне true, за потреби
//! збільшує стрічку.
//! У випадку помилки повертає різні від'ємні числа, якщо все ОК -- 0.
int my_str_read_file_delim_if(my_str_t *str, FILE *file, int (*predicat)(int))
{
    my_str_clear(str);
    int c;
    c = fgetc(file);
    while(!( (*predicat)(c) || c==EOF)){
        my_str_pushback(str, (char)c);
        c = fgetc(file);
    }
    if(c==EOF){
        return 1;
    }
    return 0;
}


/* Return codes reference:
     0 - Success.
     1 - End of file.
     -2 - Couldn't allocate enough memory/not enough memory.
    -1 - Invalid arguments.
 */

void my_str_free(my_str_t *str)
{
    free(str->data);
}
//Constructor for my_str_t
int my_str_create(my_str_t *str, size_t buf_size)
{
    str->size_m = 0;
    str->capacity_m = buf_size;
    str->data = (char *)malloc(sizeof(char) * (buf_size + 1));
    if (!str->data)
    {
        my_str_free(str);
        return -2;
    }
    memset(str->data, '\0', 1*sizeof(char));
    return 0;

}

int my_str_from_cstr(my_str_t* str, const char* cstr, size_t buf_size){
    size_t cstr_len = my_str_cstr_len(cstr);
    if (buf_size == 0) {
        buf_size = cstr_len;
    }
    if (buf_size < cstr_len){
        return -1;
    }
    if (my_str_reserve(str, buf_size) != 0) {
        return -2;
    }
    if (my_str_resize(str, cstr_len, '\0') != 0) {
        return -2;
    }
    memmove(str->data, cstr, cstr_len*sizeof(char));
    return 0;
}

/*
int main(int argc, char *argv[])
{
    printf("I'm at the beginning!\n");
    char *word = argv[1];
    puts(argv[1]);
    size_t buf_size = (size_t)atoi(argv[2]);
    printf("%u\n", buf_size);
    my_str_t string;
    my_str_create(&string, buf_size);
    size_t leng = (size_t)my_str_cstr_len(word);
    printf("%u\n", leng);
    my_str_resize(&string, leng, 'R');
    my_str_write(&string, stdout);
}
*/
