#include <ckit\stdio.h>
#include <ckit\string.h>

int main() {
    String* str = string_from(NULL, "Hello World!");
    string_pushback(str, '\n');
    String dup = {0};
    string_duplicate(&dup, str);
    string_popback(&dup);
    string_popback(&dup);
    string_pushback(&dup, '2');
    string_pushback(&dup, '!');
    string_pushback(&dup, '\n');
    print(str->data);
    print(dup.data);
    string_popback(str);
    string_copy(&dup, "Adios Mundos!", 0, 12);
    string_append(&dup, str->data);
    print(dup.data);
    string_destroy(str);
    string_destroy(&dup);
}