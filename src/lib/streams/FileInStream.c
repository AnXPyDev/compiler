typedef struct {
    InStream header;
    FILE *fp;
} FileInStream;

#define this ((FileInStream*)vthis)

int FileInStream_getc(void *vthis) {
    return fgetc(this->fp);
}

int FileInStream_read(void *vthis, void *buf, Size size) {
    return fread(buf, 1, size, this->fp);
}

int FileInStream_end(void *vthis) {
    return feof(this->fp);
}

int FileInStream_close(void *vthis) {
    return fclose(this->fp);
}

#undef this

const struct IInStream IFileInStream = {
    &FileInStream_getc,
    &FileInStream_read,
    &FileInStream_end,
    &FileInStream_close
};

void FileInStream_create(FileInStream *this, FILE *fp) {
    this->header.interface = &IFileInStream;
    this->fp = fp;
}