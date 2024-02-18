typedef struct {
    Allocator allocator;
    Size item_size;
    Size size;
    Size capacity;
    void *data;
} Vector;

void Vector_create(Vector *this, Allocator allocator, Size item_size) {
    this->allocator = allocator;
    this->item_size = item_size;
    this->size = 0;
    this->capacity = 0;
    this->data = NULL;
}

Vector Vector_new(Allocator allocator, Size item_size) {
    Vector this;
    Vector_create(&this, allocator, item_size);
    return this;
}

Vector Vector_const_proxy(void *data, Size item_size, Size size) {
    Vector this;
    this.item_size = item_size;
    this.data = data;
    this.size = size;
    return this;
}

void Vector_init(Vector *this, Size capacity) {
    this->capacity = capacity;
    this->size = 0;
    this->data = Allocator_malloc(this->allocator, this->item_size * this->capacity);
}

void Vector_nullify(Vector *this) {
    this->data = NULL;
    this->size = 0;
    this->capacity = 0;
}

void Vector_destroy(Vector *this) {
    Allocator_free(this->allocator, this->data);
    Vector_nullify(this);
}

void Vector_resize(Vector *this, Size capacity) {
    this->data = Allocator_realloc(this->allocator, this->data, this->item_size * capacity);
    this->capacity = capacity;
    if (this->size > this->capacity) {
        this->size = this->capacity;
    }
}

void Vector_expand(Vector *this) {
    if (LIBDEBUG) {
        fprintf(stderr, "Vector_expand %p %u -> ", this, this->capacity);
    }
    this->capacity = (this->capacity + 1) * 2;
    if (LIBDEBUG) {
        fprintf(stderr, "%u\n", this->capacity);
    }
    this->data = Allocator_realloc(this->allocator, this->data, this->item_size * this->capacity);
}

void Vector_expand_to(Vector *this, Size capacity) {
    if (capacity < this->capacity) {
        return Vector_resize(this, capacity);
    }
    Size newCapacity = this->capacity;
    while (newCapacity < capacity) {
        newCapacity = (newCapacity + 1) * 2;
    }
    this->capacity = newCapacity;
    this->data = Allocator_realloc(this->allocator, this->data, this->item_size * this->capacity);
}

void *Vector_push(Vector *this) {
    if (LIBDEBUG) {
        fprintf(stderr, "Vector_push %p %u %u\n", this, this->size, this->capacity);
    }

    if (this->size + 1 > this->capacity) {
        Vector_expand(this);
    }
    this->size++;
    return this->data + this->item_size * (this->size - 1);
}

void *Vector_push_unsafe(Vector *this) {
    this->size++;
    return this->data + this->item_size * (this->size - 1);
}

void Vector_pop(Vector *this) {
    if (this->size > 0) {
        this->size--;
    }
}

void *Vector_get(Vector *this, Size index) {
    if (index >= this->size) {
        return NULL;
    }

    return this->data + this->item_size * index;
}

const void *Vector_cget(const Vector *this, Size index) {
    if (index >= this->size) {
        return NULL;
    }

    return this->data + this->item_size * index;
}

void *Vector_begin(Vector *this) {
    return this->data;
}

void *Vector_end(Vector *this) {
    return this->data + this->item_size * this->size;
}

const void *Vector_cbegin(const Vector *this) {
    return this->data;
}

const void *Vector_cend(const Vector *this) {
    return this->data + this->item_size * this->size;
}

void Vector_join(Vector *this, const void *data, Size count) {
    if (this->capacity < this->size + count) {
        Vector_resize(this, this->size + count);
    }
    memcpy(Vector_end(this), data, this->item_size * count);
    this->size += count;
}

void Vector_append(Vector *this, const void *data, Size count) {
    if (this->capacity < this->size + count) {
        Vector_expand_to(this, this->size + count);
    }
    memcpy(Vector_end(this), data, this->item_size * count);
    this->size += count;
}

void Vector_remove(Vector *this, Size index) {
    if (this->size <= index) {
        return;
    }

    if (index == this->size - 1) {
        this->size--;
        return;
    }

    memmove(this->data + index * this->item_size, this->data + this->item_size * (index + 1), this->item_size * (this->size - index - 1));
    this->size--;
}

void Vector_clear(Vector *this) {
    this->size = 0;
}

void *Vector_move_buffer(Vector *this, void *buffer, Size capacity, Size size) {
    this->data = buffer;
    this->capacity = capacity;
    this->size = size;
}

void *Vector_copy_buffer(Vector *this, void *buffer, Size size) {
    this->data = Allocator_malloc(this->allocator, size * this->item_size);
    memcpy(this->data, buffer, size * this->item_size);
    this->capacity = size;
    this->size = size;
}

void *Vector_copy(Vector *this, Vector *other) {
    Vector_copy_buffer(this, other->data, other->size);
}