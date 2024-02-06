typedef struct {
    OutStream header;
    Vector buffer;
} StringOutStream;

#define this ((StringOutStream*)vthis)

#undef this

void StringOutStream_create(void *vthis) {
    
}