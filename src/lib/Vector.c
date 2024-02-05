typedef struct {
    Size item_size;
    Size size;
    Size capacity;
    void *data;
} Vector;

void vector_create(Vector *this, Size item_size, Size capacity) {
    this->item_size = item_size;
    this->capacity = capacity;
    this->size = 0;
    this->data = malloc(this->item_size * this->capacity);
}

void vector_destroy(Vector *this) {
    this->capacity = 0;
    this->size = 0;
    free(this->data);
    this->data = NULL;
}

void vector_resize(Vector *this, Size capacity) {
    this->data = realloc(this->data, this->item_size * capacity);
    this->capacity = capacity;
    if (this->size > this->capacity) {
        this->size = this->capacity;
    }
}

void vector_expand(Vector *this) {
    if (LIBDEBUG) {
        fprintf(stderr, "vector_expand %p %u -> ", this, this->capacity);
    }
    this->capacity = (this->capacity + 1) * 2;
    if (LIBDEBUG) {
        fprintf(stderr, "%u\n", this->capacity);
    }
    this->data = realloc(this->data, this->item_size * this->capacity);
}

void *vector_push(Vector *this) {
    if (LIBDEBUG) {
        fprintf(stderr, "vector_push %p %u %u\n", this, this->size, this->capacity);
    }

    if (this->size + 1 > this->capacity) {
        vector_expand(this);
    }
    this->size++;
    return this->data + this->item_size * (this->size - 1);
}

void vector_pop(Vector *this) {
    if (this->size > 0) {
        this->size--;
    }
}

void *vector_get(Vector *this, Size index) {
    if (index >= this->size) {
        return NULL;
    }

    return this->data + this->item_size * index;
}