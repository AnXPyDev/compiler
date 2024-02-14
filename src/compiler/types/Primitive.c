typedef struct {
    EType_Primitive type;
} PrimitiveType;

#define this ((PrimitiveType*)vthis)
#define other ((PrimitiveType*)vother)

int PrimitiveType_equal(const void *vthis, const void *vother) {
    if (this->type == other->type) {
        return 1;
    }
    return 0;
}

#undef other

void PrimitiveType_print(const void *vthis, OutStream stream) {
    OutStream_puts(stream, "Primitive[");

    switch (this->type) {
        case TYPE_PRIMITIVE_I32:
            OutStream_puts(stream, "i32");
            break;
        default:
            OutStream_puts(stream, "?");

    }

    OutStream_puts(stream, "]");
}

void PrimitiveType_destroy(void *vthis) {

}

#undef this

const struct IType IPrimitiveType = {
    &PrimitiveType_equal,
    &PrimitiveType_print,
    &PrimitiveType_destroy
};

PrimitiveType PrimitiveType_new(EType_Primitive type) {
    PrimitiveType this;
    this.type = type;
    return this;
}

Type PrimitiveType_Type(PrimitiveType *this) {
    Type type = { &IPrimitiveType, this };
    return type;
}