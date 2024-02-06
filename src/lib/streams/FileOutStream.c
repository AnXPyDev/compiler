typedef struct {
    OutStream header;
    FILE *fp;
} FileOutStream;


#define this ((FileOutStream*)vthis)

int FileOutStream_putc(void *vthis, int c) {
    return fputc(c, this->fp);
}

int FileOutStream_puts(void *vthis, const char *s) {
    return fputs(s, this->fp);
}

int FileOutStream_write(void *vthis, const void *data, Size size) {
    return fwrite(data, 1, size, this->fp);
}

int FileOutStream_flush(void *vthis) {
    return fflush(this->fp);
}

int FileOutStream_close(void *vthis) {
    return fclose(this->fp);
}

#undef this


const struct IOutStream IFileOutStream = {
    &FileOutStream_putc,
    &FileOutStream_puts,
    &FileOutStream_write,
    &FileOutStream_flush,
    FileOutStream_close
};

void FileOutStream_create(FileOutStream *this, FILE *fp) {
    this->header.interface = &IFileOutStream;
    this->fp = fp;
}