#include <stdio.h>

struct X {
    int x;
    char blob[0]; 
};

int main() {
    struct X x;
    fprintf(stderr, "Sizeof X %u\n", sizeof(struct X));
    fprintf(stderr, "%p %p %p %p\n", &x, &x.x, &x.blob, (void*)&x + 4);
}