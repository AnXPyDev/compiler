struct IValue;

typedef struct {
    const struct IValue *interface;
    void *object;
} Value;

struct IValue {
    void  (*destroy)(void *this);
    const Type  (*getType)(const void *this);
    void  (*print  )(const void *this, OutStream stream);
    Value (*copy   )(const void *this, Allocator allocator);
    Value (*move   )(void *this, Allocator allocator);
};

void Value_destroy(Value this) {
    return this.interface->destroy(this.object);
}

const Type Value_getType(const Value this) {
    return this.interface->getType(this.object);
}

void Value_print(const Value this, OutStream stream) {
    return this.interface->print(this.object, stream);
}

Value Value_copy(const Value this, Allocator allocator) {
    return this.interface->copy(this.object, allocator);
}

Value Value_move(const Value this, Allocator allocator) {
    return this.interface->move(this.object, allocator);
}

void Value_free(Value this, Allocator allocator) {
    return Allocator_free(allocator, this.object);
}

void Value_delete(Value this, Allocator allocator) {
    Value_destroy(this);
    Value_free(this, allocator);
}