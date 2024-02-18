struct IExpression;

typedef struct {
    const struct IExpression *interface;
    void *object;
} Expression;

struct IExpression {
    void   (*destroy)(void *this);
    Value  (*evaluate)(const void *this, Context*);
    void   (*print)(const void *this, OutStream stream);
    Expression (*copy)(const void *this, Allocator allocator);
};

void Expression_destroy(Expression this) {
    return this.interface->destroy(this.object);
};

Value Expression_evaluate(Expression this, Context *context) {
    return this.interface->evaluate(this.object, context);
};

void Expression_print(Expression this, OutStream stream) {
    return this.interface->print(this.object, stream);
}

Expression Expression_copy(Expression this, Allocator allocator) {
    return this.interface->copy(this.object, allocator);
}

void Expression_free(Expression this, Allocator allocator) {
    return Allocator_free(allocator, this.object);
}