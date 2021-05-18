#include "vector.h"

int main() {
    vector v;
    create_vector(&v, 10);

    int a, b, c, d;
    a = 0;
    b = 1;
    c = 2;
    d = 3;
    append(&v, &c);
    append(&v, &b);
    append(&v, &a);
    append(&v, &d);

    print_vector(&v);

    delete(&v, &b);

    print_vector(&v);

    free_vector(&v);
}