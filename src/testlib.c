#include "compiler.c"

void fprint_int(FILE *outstream, int *i) {
    fprintf(outstream, "%d", *i);
}

void fprint_vector(FILE *outstream, Vector *vector, void (*fprint_item)(FILE *outstream, void*)) {
    fprintf(outstream, "Vector[ ");

    if (vector->size == 0) {
        goto quit;
    }

    fprint_item(outstream, Vector_get(vector, 0));

    for (Size i = 1; i < vector->size; i++) {
        fprintf(outstream, ", ");
        fprint_item(outstream, Vector_get(vector, i));
    }

    quit:;
    fprintf(outstream, " ]");
}

void test_vector() {
    fprintf(stderr, "Testing Vector\n");

    Vector v;
    Vector_create(&v, sizeof(int), 10);

    for (Size i = 0; i < 50; i++) {
        *(int*)Vector_push(&v) = i * 2;
    }

    for (Size i = 0; i < 20; i++) {
        Vector_pop(&v);
    }

    fprint_vector(stderr, &v, (void*)&fprint_int);
    fprintf(stderr, "\n");

    Vector_destroy(&v);
}

#define keyc 10

void test_map() {
    fprintf(stderr, "Testing map\n");

    Map m;
    Map_create(&m, sizeof(int));

    const char *keys[keyc] = {
        "key1", "key2", "key3", "key4", "key5", "key6", "key7", "key8", "key9", "key10"
    };

    for (Size i = 0; i < keyc; i++) {
        const char *key = keys[i];
        Size key_size = strlen(key);
        *(int*)Map_ensure(&m, key, key_size) = i * 2;
    }

    for (Size i = 0; i < keyc; i++) {
        const char *key = keys[i];
        Size key_size = strlen(key);
        int *val = (int*)Map_get(&m, key, key_size);
        if (val == NULL) {
            fprintf(stderr, "[%s] = NULL\n", key);
            continue;
        }
        fprintf(stderr, "[%s] = %d\n", key, *val);
    }

    for (Size i = 0; i < keyc; i += 2) {
        const char *key = keys[i];
        Size key_size = strlen(key);
        Map_delete(&m, key, key_size);
    }

    for (Size i = 0; i < keyc; i++) {
        const char *key = keys[i];
        Size key_size = strlen(key);
        int *val = (int*)Map_get(&m, key, key_size);
        if (val == NULL) {
            fprintf(stderr, "[%s] = NULL\n", key);
            continue;
        }
        fprintf(stderr, "[%s] = %d\n", key, *val);
    }


    Map_destroy(&m);
}

#undef keyc

int main() {
    test_vector();
    test_map();
    return 0;
}