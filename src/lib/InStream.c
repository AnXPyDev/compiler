struct IInStream;

typedef struct {
    const struct IInStream *interface;
    void *object;
} InStream;

struct IInStream {
    int (*getc)(void *this);
    int (*read)(void *this, void *buf, Size size);
    int (*end)(void *this);
    int (*close)(void *this);
};

int InStream_getc(InStream this) {
    return this.interface->getc(this.object);
}

int InStream_read(InStream this, void *buf, Size size) {
    return this.interface->read(this.object, buf, size);
}

int InStream_end(InStream this) {
    return this.interface->end(this.object);
}

int InStream_close(InStream this) {
    return this.interface->close(this.object);
}