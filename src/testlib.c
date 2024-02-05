#include "compiler.c"

void fprint_int(FILE *outstream, int *i) {
    fprintf(outstream, "%d", *i);
}

void fprint_vector(FILE *outstream, Vector *vector, void (*fprint_item)(FILE *outstream, void*)) {
    fprintf(outstream, "Vector[ ");

    if (vector->size == 0) {
        goto quit;
    }

    fprint_item(outstream, vector_get(vector, 0));

    for (Size i = 1; i < vector->size; i++) {
        fprintf(outstream, ", ");
        fprint_item(outstream, vector_get(vector, i));
    }

    quit:;
    fprintf(outstream, " ]");
}

int main() {
    fprintf(stderr, "Testing Vector\n");

    Vector v;
    vector_create(&v, sizeof(int), 10);

    for (Size i = 0; i < 50; i++) {
        *(int*)vector_push(&v) = i * 2;
    }

    for (Size i = 0; i < 20; i++) {
        vector_pop(&v);
    }

    fprint_vector(stderr, &v, (void*)&fprint_int);
    fprintf(stderr, "\n");

    vector_destroy(&v);
}