struct IError;

typedef struct {
    const struct IError *interface;
    void *object;
} Error;