struct IOutStream;

typedef struct {
    const struct IOutStream *interface;
    void *object;
} OutStream;

struct IOutStream {
    int (*putc)(void *this, int c);
    int (*puts)(void *this, const char *s);
    int (*write)(void *this, const void *data, Size size);
    int (*flush)(void *this);
    int (*close)(void *this);
};

int OutStream_putc(OutStream this, int c) {
    return this.interface->putc(this.object, c);
}

int OutStream_puts(OutStream this, const char *s) {
    return this.interface->puts(this.object, s);
}

int OutStream_write(OutStream this, const void *data, Size size) {
    return this.interface->write(this.object, data, size);
}

int OutStream_flush(OutStream this) {
    return this.interface->flush(this.object);
}

int OutStream_close(OutStream this) {
    return this.interface->close(this.object);
}