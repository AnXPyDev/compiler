typedef struct {
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
    &StringInStream_getc,
    &StringInStream_read,
    &StringInStream_end,
    &StringInStream_close
};

void StringInStream_create(StringInStream *this, DataBuffer buf) {
    this->begin = buf.data;
    this->end = buf.data + buf.size;
    this->ptr = buf.data;
}

StringInStream StringInStream_new(DataBuffer buf) {
    StringInStream this;
    StringInStream_create(&this, buf);
    return this;
}

StringInStream StringInStream_newString(const String *string) {
    return StringInStream_new(String_DataBuffer(string));
}

InStream StringInStream_InStream(StringInStream *this) {
    InStream instream = { &IStringInStream, this };
    return instream;
}