struct IExpression;

typedef struct {
    const struct IExpression *interface;
    void *object;
} Expression;

struct IExpression {
    void   (*destroy)(void *this);
    Value  (*evaluate)(void *this, Context*);
    void   (*print)(void *this, OutStream stream);
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