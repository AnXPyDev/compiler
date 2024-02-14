typedef struct {
    Allocator allocator;
    Size length;
    Size capacity;
    char *data;
} String;

void String_create(String *this, Allocator allocator) {
    this->allocator = allocator;
    this->length = 0;
    this->capacity = 0;
    this->data = NULL;
}

String String_new(Allocator allocator) {
    String this;
    String_create(&this, allocator);
    return this;
}

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
    this->data = Allocator_malloc(this->allocator, capacity + 1);
    this->capacity = capacity;
    this->length = 0;
    String_zero(this);
}

void String_copy_buffer(String *this, const void *buffer, Size length) {
    this->data = Allocator_malloc(this->allocator, length + 1);
    this->length = length;
    this->capacity = length;
    memcpy(this->data, buffer, length);
    String_zero(this);
}

void String_cstring(String *this, const char *str) {
    Size length = strlen(str);
    String_copy_buffer(this, str, length);
}

void String_move_buffer(String *this, void *buffer, Size bufsize, Size length) {
    this->data = buffer;
    this->length = length;
    this->capacity = bufsize - 1;
    if (this->capacity <= this->length) {
        this->data = Allocator_realloc(this->allocator, this->data, this->length + 1);
    }
    String_zero(this);
}

void String_copy(String *this, const String *other) {
    String_copy_buffer(this, other->data, other->length);
}

void String_destroy(String *this) {
    this->capacity = 0;
    this->length = 0;
    Allocator_free(this->allocator, this->data);
    this->data = NULL;
}

char *String_begin(String *this) {
    return this->data;
}

char *String_end(String *this) {
    return this->data + this->length;
}

void String_resize(String *this, Size capacity) {
    this->data = Allocator_realloc(this->allocator, this->data, capacity + 1);
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

void String_print(String *this, OutStream outstream) {
    OutStream_puts(outstream, this->data);
}

DataBuffer String_DataBuffer(const String *this) {
    DataBuffer buf;
    buf.data = this->data;
    buf.size = this->length;
    return buf;
}

DataBuffer cstring_DataBuffer(const char *str) {
    DataBuffer buf;
    buf.data = str;
    buf.size = strlen(str);
    return buf;
}