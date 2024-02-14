#include <stdio.h>

typedef void (*fn_Test)(void *x);

void test(int *x) {

}


int main() {
    fn_Test f = (void*)321321;
    f(NULL);
}