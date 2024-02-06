struct IInStream;

typedef struct {
    const struct IInStream *interface;
} InStream;


typedef int (*fn_InStream_getc)(void *this);
typedef int (*fn_InStream_read)(void *this, void *buf, Size size);
typedef int (*fn_InStream_end)(void *this);
typedef int (*fn_InStream_close)(void *this);

struct IInStream {
    fn_InStream_getc getc;
    fn_InStream_read read;
    fn_InStream_end end;
    fn_InStream_close close;
};

#define this ((InStream*)vthis)

int InStream_getc(void *vthis) {
    return this->interface->getc(vthis);
}

int InStream_read(void *vthis, void *buf, Size size) {
    return this->interface->read(vthis, buf, size);
}

int InStream_end(void *vthis) {
    return this->interface->end(vthis);
}

int InStream_close(void *vthis) {
    return this->interface->close(vthis);
}

#undef this