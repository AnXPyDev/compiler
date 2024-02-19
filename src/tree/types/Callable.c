typedef struct {
    Allocator allocator;
    Type ret;
    SequenceType args;
} CallableType;

extern const struct IType ICallableType;

CallableType CallableType_new(Allocator allocator, Type ret, const SequenceType *args) {
    CallableType this;
    this.allocator = allocator;
    this.ret = Type_copy(ret, allocator);
    this.args = SequenceType_copy_(args, allocator);
    return this;
}

CallableType CallableType_copy_(const CallableType *this, Allocator allocator) {
    CallableType copy = CallableType_new(allocator, this->ret, &this->args);
    return copy;
}

Type CallableType_Type(CallableType *this) {
    Type type = { &ICallableType, this };
    return type;
}

#define this ((CallableType*)vthis)

void CallableType_print(const void *vthis, OutStream stream) {
    OutStream_puts(stream, "{ ");
    SequenceType_print(&this->args, stream);
    OutStream_puts(stream, " -> ");
    Type_print(this->ret, stream);
    OutStream_puts(stream, " }");
}

#define other ((CallableType*)vother)
int CallableType_equal(const void *vthis, const void *vother) {

    if (!Type_equal(this->ret, other->ret)) {
        return 0;
    }

    if (!SequenceType_equal(&this->args, &other->args)) {
        return 0;
    }

    return 1;
}
#undef other

void CallableType_destroy(void *vthis) {
    SequenceType_destroy(&this->args);
    Type_destroy(this->ret);
    Type_free(this->ret, this->allocator);
}

Type CallableType_copy(const void *vthis, Allocator allocator) {
    CallableType *copy = Allocator_malloc(allocator, sizeof(CallableType));
    *copy = CallableType_copy_(this, allocator);
    return CallableType_Type(copy);
}


#undef this

const struct IType ICallableType = {
    &CallableType_equal,
    &CallableType_print,
    &CallableType_destroy,
    &CallableType_copy
};