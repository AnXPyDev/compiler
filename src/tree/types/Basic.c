typedef struct {
    EType_Basic type;
} BasicType;

extern const struct IType IBasicType;

Type BasicType_Type(BasicType *this) {
    Type type = { &IBasicType, this };
    return type;
}

#define this ((BasicType*)vthis)
#define other ((BasicType*)vother)

int BasicType_equal(const void *vthis, const void *vother) {
    if (this->type == other->type) {
        return 1;
    }
    return 0;
}

#undef other

void BasicType_print(const void *vthis, OutStream stream) {
    char buf[64];
    switch (this->type) {
        case TYPE_PRIMITIVE_I32:
            OutStream_puts(stream, "I32");
            break;
        case TYPE_META_NONE:
            OutStream_puts(stream, "None");
            break;
        case TYPE_META_ANY:
            OutStream_puts(stream, "Any");
            break;
        case TYPE_META_EXPRESSION:
            OutStream_puts(stream, "Expression");
            break;
        case TYPE_META_TYPE:
            OutStream_puts(stream, "Type");
            break;
        case TYPE_META_TOKEN:
            OutStream_puts(stream, "Token");
            break;
        default:
            sprintf(buf, "?%d?", (int)this->type);
            OutStream_puts(stream, buf);
    }
}

void BasicType_destroy(void *vthis) {

}

Type BasicType_copy(const void *vthis, Allocator allocator) {
    BasicType *copy = Allocator_malloc(allocator, sizeof(BasicType));
    *copy = *this;
    return BasicType_Type(copy);
}

#undef this

const struct IType IBasicType = {
    &BasicType_equal,
    &BasicType_print,
    &BasicType_destroy,
    &BasicType_copy
};

BasicType BasicType_new(EType_Basic type) {
    BasicType this;
    this.type = type;
    return this;
}

int Type_isAny(Type type) {
    if (type.interface == &IBasicType) {
        BasicType *this = type.object;
        return this->type == TYPE_META_ANY;
    }
    return 0;
}