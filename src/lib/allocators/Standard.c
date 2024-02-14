void *StandardAllocator_malloc(void *this, Size size) {
    return malloc(size);
}

void *StandardAllocator_calloc(void *this, Size size) {
    return calloc(size, 1);
}

void *StandardAllocator_realloc(void *this, void *mem, Size size) {
    return realloc(mem, size);
}

void StandardAllocator_free(void *this, void *mem) {
    return free(mem);
}

void StandardAllocator_destroy(void *this) {

}

const struct IAllocator IStandardAllocator = {
    &StandardAllocator_malloc,
    &StandardAllocator_calloc,
    &StandardAllocator_realloc,
    &StandardAllocator_free,
    &StandardAllocator_destroy
};

Allocator StandardAllocator_create() {
    Allocator allocator = { &IStandardAllocator, NULL };
    return allocator;
}