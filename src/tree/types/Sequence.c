typedef struct {
    Allocator allocator;
    Vector elements;
} SequenceType;

extern const struct IType ISequenceType;

SequenceType SequenceType_new(Allocator allocator, const Vector *elements) {
    SequenceType this;
    this.allocator = allocator;
    this.elements = Vector_new(allocator, sizeof(Type));
    Vector_init(&this.elements, elements->size);
    for (const Type *type = Vector_cbegin(elements); type < (const Type*)Vector_cend(elements); type++) {
        *(Type*)Vector_push(&this.elements) = Type_copy(*type, allocator);
    }
    return this;
}

SequenceType SequenceType_copy_(const SequenceType *this, Allocator allocator) {
    return SequenceType_new(allocator, &this->elements);
}

Type SequenceType_Type(SequenceType *this) {
    Type type = { &ISequenceType, this };
    return type;
}

#define this ((SequenceType*)vthis)

void SequenceType_print(const void *vthis, OutStream stream) {
    OutStream_putc(stream, '(');
    if (this->elements.size == 0) {
        goto skip;
    }

    Type *type = Vector_begin(&this->elements);
    Type_print(*type, stream);
    type++;
    for (; type < (Type*)Vector_end(&this->elements); type++) {
        OutStream_puts(stream, ", ");
        Type_print(*type, stream);
    }

    skip:;
    OutStream_putc(stream, ')');
}

void SequenceType_destroy(void *vthis) {
    for (Type *type = Vector_begin(&this->elements); type < (Type*)Vector_end(&this->elements); type++) {
        Type_destroy(*type);
        Type_free(*type, this->allocator);
    }

    Vector_destroy(&this->elements);
}

Type SequenceType_copy(const void *vthis, Allocator allocator) {
    SequenceType *copy = Allocator_malloc(allocator, sizeof(SequenceType));
    *copy = SequenceType_copy_(this, allocator);
    return SequenceType_Type(copy);
}

#define other ((SequenceType*)vother)
int SequenceType_equal(const void *vthis, const void *vother) {
    if (this->elements.size != other->elements.size) {
        return 0;
    }

    for (Size i = 0; i < this->elements.size; i++) {
        Type *t1 = Vector_get(&this->elements, i);
        Type *t2 = Vector_get(&other->elements, i);
        if (!Type_equal(*t1, *t2)) {
            return 0;
        }
    }

    return 1;
}
#undef other

#undef this

const struct IType ISequenceType = {
    &SequenceType_equal,
    &SequenceType_print,
    &SequenceType_destroy,
    &SequenceType_copy
};
