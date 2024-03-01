typedef struct {
    Allocator allocator;
    SequenceType type;
    Vector elements;
} SequenceValue;

extern const struct IValue ISequenceValue;

SequenceValue SequenceValue_new(Allocator allocator, const SequenceType *type, const Vector *values) {
    SequenceValue this;
    this.allocator = allocator;
    this.type = SequenceType_copy_(type, allocator);
    this.elements = Vector_new(allocator, sizeof(Value));
    Vector_init(&this.elements, values->size);
    for (const Value *value = Vector_cbegin(values); value < (const Value*)Vector_cend(values); value++) {
        *(Value*)Vector_push(&this.elements) = Value_copy(*value, allocator);
    }
    return this;
}

SequenceValue SequenceValue_copy_(const SequenceValue *this, Allocator allocator) {
    return SequenceValue_new(allocator, &this->type, &this->elements);
}

Value SequenceValue_Value(SequenceValue *this) {
    Value value = { &ISequenceValue, this };
    return value;
}

#define this ((SequenceValue*)vthis)

Type SequenceValue_getType(const void *vthis) {
    return SequenceType_Type(&this->type);
}

void SequenceValue_destroy(void *vthis) {
    SequenceType_destroy(&this->type);
    for (Value *value = Vector_begin(&this->elements); value < (Value*)Vector_end(&this->elements); value++) {
        Value_destroy(*value);
        Value_free(*value, this->allocator);
    }
    Vector_destroy(&this->elements);
}

Value SequenceValue_copy(const void *vthis, Allocator allocator) {
    SequenceValue *copy = Allocator_malloc(allocator, sizeof(SequenceValue));
    *copy = SequenceValue_copy_(this, allocator);
    return SequenceValue_Value(copy);
}

Value SequenceValue_move(void *vthis, Allocator allocator) {
    return SequenceValue_copy(vthis, allocator);
}

void SequenceValue_print(const void *vthis, OutStream stream) {
    OutStream_putc(stream, '(');

    if (this->elements.size == 0) {
        goto skip;
    }

    Value *value = Vector_begin(&this->elements);
    Value_print(*value, stream);
    value++;
    for (; value < (Value*)Vector_end(&this->elements); value++) {
        OutStream_puts(stream, ", ");
        Value_print(*value, stream);
    }

    skip:;

    OutStream_putc(stream, ')');
}

#undef this

int SequenceValue_is(Value value) {
    return value.interface == &ISequenceValue;
}

const struct IValue ISequenceValue = {
    &SequenceValue_destroy,
    &SequenceValue_getType,
    &SequenceValue_print,
    &SequenceValue_copy,
    &SequenceValue_move
};