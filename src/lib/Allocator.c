struct IAllocator;

typedef struct {
    const struct IAllocator *interface;
    void *object;
} Allocator;

struct IAllocator {
    void *(*malloc )(void *this, Size size);
    void *(*calloc )(void *this, Size size);
    void *(*realloc )(void *this, void *mem, Size size);
    void  (*free   )(void *this, void *mem);
    void  (*destroy)(void *this);
};

void *Allocator_malloc(Allocator this, Size size) {
    return this.interface->malloc(this.object, size);
}

void *Allocator_calloc(Allocator this, Size size) {
    return this.interface->calloc(this.object, size);
}

void *Allocator_realloc(Allocator this, void *mem, Size size) {
    return this.interface->realloc(this.object, mem, size);
}

void Allocator_free(Allocator this, void *mem) {
    if (mem == NULL) {
        return;
    }
    return this.interface->free(this.object, mem);
}

void Allocator_destroy(Allocator this) {
    return this.interface->destroy(this.object);
}