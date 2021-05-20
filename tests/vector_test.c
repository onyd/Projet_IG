#include "vector.h"

int main() {
    vector *v = create_vector(10);

    int a, b, c, d, e, f;
    a = 0;
    b = 1;
    c = 2;
    d = 3;
    e = 4;
    f = 5;
    append_vector(v, &a);
    append_vector(v, &b);
    append_vector(v, &c);
    append_vector(v, &d);
    append_vector(v, &e);
    append_vector(v, &f);

    print_vector(v);

    remove_vector(v, &a);
    remove_vector(v, &c);
    remove_vector(v, &e);

    print_vector(v);

    remove_vector(v, &f);

    print_vector(v);

    free_vector(v);
}