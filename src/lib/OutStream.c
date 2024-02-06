struct IOutStream;

typedef struct {
    const struct IOutStream *interface;
} OutStream;

typedef int (*fn_OutStream_putc)(void *this, int c);
typedef int (*fn_OutStream_puts)(void *this, const char *s);
typedef int (*fn_OutStream_write)(void *this, const void *data, Size size);
typedef int (*fn_OutStream_flush)(void *this);
typedef int (*fn_OutStream_close)(void *this);

struct IOutStream {
    fn_OutStream_putc putc;
    fn_OutStream_puts puts;
    fn_OutStream_write write;
    fn_OutStream_flush flush;
    fn_OutStream_close close;
};

#define this ((OutStream*)vthis)

int OutStream_putc(void *vthis, int c) {
    return this->interface->putc(this, c);
}

int OutStream_puts(void *vthis, const char *s) {
    return this->interface->puts(this, s);
}

int OutStream_write(void *vthis, const void *data, Size size) {
    return this->interface->write(this, data, size);
}

int OutStream_flush(void *vthis) {
    return this->interface->flush(this);
}

int OutStream_close(void *vthis) {
    return this->interface->close(this);
}

#undef this