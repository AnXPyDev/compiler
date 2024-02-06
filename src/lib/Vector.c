typedef struct {
    Size item_size;
    Size size;
    Size capacity;
    void *data;
} Vector;

void Vector_create(Vector *this, Size item_size, Size capacity) {
    this->item_size = item_size;
    this->capacity = capacity;
    this->size = 0;
    this->data = malloc(this->item_size * this->capacity);
}

void Vector_destroy(Vector *this) {
    this->capacity = 0;
    this->size = 0;
    free(this->data);
    this->data = NULL;
}

void Vector_resize(Vector *this, Size capacity) {
    this->data = realloc(this->data, this->item_size * capacity);
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
    this->data = realloc(this->data, this->item_size * this->capacity);
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
    this->data = realloc(this->data, this->item_size * this->capacity);
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

void *Vector_begin(Vector *this) {
    return this->data;
}

void *Vector_end(Vector *this) {
    return this->data + this->item_size * this->size;
}

void Vector_join(Vector *this, void *data, Size count) {
    if (this->capacity < this->size + count) {
        Vector_resize(this, this->size + count);
    }
    memcpy(Vector_end(this), data, this->item_size * count);
}

void Vector_append(Vector *this, void *data, Size count) {
    if (this->capacity < this->size + count) {
        Vector_expand_to(this, this->size + count);
    }
    memcpy(Vector_end(this), data, this->item_size * count);
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