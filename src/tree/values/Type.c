typedef struct {
    Allocator allocator;
    Type type;
} TypeValue;

extern const struct IValue ITypeValue;

TypeValue TypeValue_new(Allocator allocator, Type type) {
    TypeValue this;
    this.allocator = allocator;
    this.type = Type_copy(type, this.allocator);
    return this;
}

TypeValue TypeValue_copy_(const TypeValue *this, Allocator allocator) {
    return TypeValue_new(allocator, this->type);
}

Value TypeValue_Value(TypeValue *this) {
    Value value = { &ITypeValue, this };
    return value;
}

#define this ((TypeValue*)vthis)

Type TypeValue_getType(const void *vthis) {
    return RT_basicTypes[TYPE_META_TYPE];
}

void TypeValue_destroy(void *vthis) {
    Type_destroy(this->type);
    Type_free(this->type, this->allocator);
}

void TypeValue_print(const void *vthis, OutStream stream) {
    OutStream_puts(stream, "{ ");
    Type_print(this->type, stream);
    OutStream_puts(stream, " }");
}

Value TypeValue_copy(const void *vthis, Allocator allocator) {
    TypeValue *copy = Allocator_malloc(allocator, sizeof(TypeValue));
    *copy = TypeValue_copy_(this, allocator);
    return TypeValue_Value(copy);
}

Value TypeValue_move(void *vthis, Allocator allocator) {
    return TypeValue_copy(vthis, allocator);
}

#undef this

int TypeValue_is(Value value) {
    return value.interface == &ITypeValue;
}

const struct IValue ITypeValue = {
    &TypeValue_destroy,
    &TypeValue_getType,
    &TypeValue_print,
    &TypeValue_copy,
    &TypeValue_move
};