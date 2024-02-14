typedef struct {
    EType_Meta type;
} MetaType;

#define this ((MetaType*)vthis)
#define other ((MetaType*)vother)

int MetaType_equal(const void *vthis, const void *vother) {
    if (this->type == other->type) {
        return 1;
    }
    return 0;
}

#undef other

void MetaType_print(const void *vthis, OutStream stream) {
    OutStream_puts(stream, "Meta[");

    switch (this->type) {
        case TYPE_META_ANY:
            OutStream_puts(stream, "Any");
            break;
        case TYPE_META_EXPRESSION:
            OutStream_puts(stream, "Expression");
            break;
        default:
            OutStream_puts(stream, "?");

    }

    OutStream_puts(stream, "]");
}

void MetaType_destroy(void *vthis) {

}

#undef this

const struct IType IMetaType = {
    &MetaType_equal,
    &MetaType_print,
    &MetaType_destroy
};

MetaType MetaType_new(EType_Meta type) {
    MetaType this;
    this.type = type;
    return this;
}

Type MetaType_Type(MetaType *this) {
    Type type = { &IMetaType, this };
    return type;
}