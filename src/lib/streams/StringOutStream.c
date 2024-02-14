typedef struct {
    Vector buffer;
} StringOutStream;

#define this ((StringOutStream*)vthis)

int StringOutStream_putc(void *vthis, int c) {
    *(char*)Vector_push(&this->buffer) = (char)c;
    return c;
}

int StringOutStream_write(void *vthis, const void *data, Size size) {
    Vector_append(&this->buffer, data, size);
    return 0;
}

int StringOutStream_puts(void *vthis, const char *str) {
    Size length = strlen(str);
    return StringOutStream_write(vthis, str, length);
}

int StringOutStream_flush(void *vthis) {
    return 0;
}

int StringOutStream_close(void *vthis) {
    Vector_destroy(&this->buffer);
    return 0;
}

#undef this

const struct IOutStream IStringOutStream = {
    StringOutStream_putc,
    StringOutStream_puts,
    StringOutStream_write,
    StringOutStream_flush,
    StringOutStream_close
};

void StringOutStream_create(StringOutStream *this, Allocator allocator) {
    this->buffer = Vector_new(allocator, sizeof(char));
    Vector_init(&this->buffer, 32);
}

StringOutStream StringOutStream_new(Allocator allocator) {
    StringOutStream this;
    StringOutStream_create(&this, allocator);
    return this;
}

OutStream StringOutStream_OutStream(StringOutStream *this) {
    OutStream outstream = { &IStringOutStream, this };
    return outstream;
}

void StringOutStream_copy(StringOutStream *this, String *string) {
    String_copy_buffer(string, this->buffer.data, this->buffer.size);
}

void StringOutStream_move(StringOutStream *this, String *string) {
    String_move_buffer(string, this->buffer.data, this->buffer.capacity, this->buffer.size);
    Vector_nullify(&this->buffer);
    StringOutStream_close(this);
}