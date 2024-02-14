typedef struct {
    EType_Primitive type;
    union {
        int32_t I32;
    } value;
} PrimitiveValue;

extern const struct IValue IPrimitiveValue;

Value PrimitiveValue_Value(PrimitiveValue *this) {
    Value value = { &IPrimitiveValue, this };
    return value;
}

#define this ((PrimitiveValue*)vthis)

void PrimitiveValue_destroy(void *vthis) {
}

Type PrimitiveValue_getType(const void *vthis) {
    return RT_primitiveTypes[this->type];
}

// TODO optimize
void PrimitiveValue_print(const void *vthis, OutStream stream) {
    char buf[64];
    switch (this->type) {
        case TYPE_PRIMITIVE_I32:
            sprintf(buf, "%d", this->value.I32);
            break;
        default:
            OutStream_puts(stream, "?");
            return;
    }
    OutStream_puts(stream, buf);
}

Value PrimitiveValue_copy(const void *vthis, Allocator allocator) {
    PrimitiveValue *copy = Allocator_malloc(allocator, sizeof(PrimitiveValue));
    *copy = *this;
    return PrimitiveValue_Value(copy);
}

Value PrimitiveValue_move(const void *vthis, Allocator allocator) {
    return PrimitiveValue_copy(vthis, allocator);
}

#undef this

const struct IValue IPrimitiveValue = {
    &PrimitiveValue_destroy,
    &PrimitiveValue_getType,
    &PrimitiveValue_print,
    &PrimitiveValue_copy
};

PrimitiveValue PrimitiveValue_newInt(int32_t value) {
    PrimitiveValue this;
    this.type = TYPE_PRIMITIVE_I32;
    this.value.I32 = value;
    return this;
}