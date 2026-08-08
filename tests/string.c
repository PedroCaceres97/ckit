#include <ckit\stdio.h>
#include <ckit\string.h>

int main() {
    String* str = string_from(NULL, "Hello World!");
    string_pushback(str, '\n');
    print(str->data);
    string_popback(str);
    print(str->data);
    string_destroy(str);
}