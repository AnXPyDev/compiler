typedef struct {
    InStream header;
    const char *begin;
    const char *end;
    const char *ptr;
} StringInStream;

#define this ((StringInStream*)vthis)

int StringInStream_getc(void *vthis) {
    int c = *this->ptr;
    this->ptr++;
    return c;
}

int StringInStream_read(void *vthis, void *buf, Size size) {
    Size copied = MIN2(size, this->end - this->ptr);
    memcpy(buf, this->ptr, copied);
    this->ptr += copied;
    return copied;
}

int StringInStream_end(void *vthis) {
    return this->ptr >= this->end;
}

int StringInStream_close(void *vthis) {}

#undef this

const struct IInStream IStringInStream = {

};

void StringInStream_create(StringInStream *this, const char *begin, const char *end) {
    this->header.interface = &IStringInStream;
}

