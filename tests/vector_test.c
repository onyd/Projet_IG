#include "vector.h"

int main() {
    vector *v = create_vector(10);

    int a, b, c, d;
    a = 0;
    b = 1;
    c = 2;
    d = 3;
    append_vector(v, &c);
    append_vector(v, &b);
    append_vector(v, &a);
    append_vector(v, &d);

    print_vector(v);

    remove_vector(v, &b);

    print_vector(v);

    free_vector(v);
}