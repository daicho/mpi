#include <stdio.h>
#include "mulproc.h"

int main(void) {
    struct NUMBER a, b, c, d;

    setInt(&a, 10);
    setInt(&b, 2);

    printf("a = ");
    dispNumber(&a);

    printf("b = ");
    dispNumber(&b);

    divide(&a, &b, &c, &d);

    printf("c = ");
    dispNumber(&c);

    printf("d = ");
    dispNumber(&d);

    return 0;
}
