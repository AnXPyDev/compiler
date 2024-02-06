typedef struct {
    Size length;
    Size capacity;
    char *data;
} String;

void String_zero(String *this) {
    this->data[this->length] = 0;
}

void String_normalize(String *this) {
    if (this->length > this->capacity) {
        this->length = this->capacity;
    }

    String_zero(this);
}


void String_blank(String *this, Size capacity) {
    this->data = malloc(capacity + 1);
    this->capacity = capacity;
    this->length = 0;
    String_zero(this);
}

void String_create(String *this, const char *str) {
    Size length = strlen(str);
    this->data = malloc(length + 1);
    this->length = length;
    this->capacity = length;
    memcpy(this->data, str, length);
    String_zero(this);
}

void String_destroy(String *this) {
    this->capacity = 0;
    this->length = 0;
    free(this->data);
    this->data = NULL;
}

char *String_begin(String *this) {
    return this->data;
}

char *String_end(String *this) {
    return this->data + this->length;
}

void String_resize(String *this, Size capacity) {
    this->data = realloc(this->data, capacity + 1);
    this->capacity = capacity;
    if (this->length > this->capacity) {
        this->length = this->capacity;
    }
    String_zero(this);
}

void String_append_buffer(String *this, const void *buf, Size length) {
    Size newlen = length + this->length;
    if (newlen > this->capacity) {
        String_resize(this, newlen);
    }
    memcpy(String_end(this), buf, length);
    this->length = newlen;
    String_zero(this);
}

void String_append(String *this, const char *str) {
    String_append_buffer(this, str, strlen(str));
}

void String_join(String *this, String *other) {
    String_append_buffer(this, other->data, other->length);
}

void String_copy(String *this, String *other) {
    this->data = malloc(other->length + 1);
    this->length = other->length;
    this->capacity = other->length;
    memcpy(this->data, other->data, other->length);
}