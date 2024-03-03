typedef struct {
    Allocator allocator;
    Expression expression;
} CatchExpression;

extern const struct IExpression ICatchExpression;

CatchExpression CatchExpression_new(Allocator allocator, Expression expression) {
    CatchExpression this;
    this.allocator = allocator;
    this.expression = Expression_copy(expression, allocator);
    return this;
}

CatchExpression CatchExpression_copy_(const CatchExpression *this, Allocator allocator) {
    return CatchExpression_new(allocator, this->expression);
}

Expression CatchExpression_Expression(CatchExpression *this) {
    Expression expression = { &ICatchExpression, this };
    return expression;
}

#define this ((CatchExpression*)vthis)

void CatchExpression_destroy(void *vthis) {
    Expression_destroy(this->expression);
    Expression_free(this->expression, this->allocator);
}

Value CatchExpression_evaluate(const void *vthis, Context *context) {
    Value r = Expression_evaluate(this->expression, context);
    if (!ProjectileValue_is(r)) {
        return r;
    }

    ProjectileValue *proj = r.object;
    Value val = proj->value;
    Value_free(r, context->allocator);
    return val;
}

void CatchExpression_print(const void *vthis, OutStream stream) {
    OutStream_puts(stream, "Catch{ ");
    Expression_print(this->expression, stream);
    OutStream_puts(stream, " }");
}

Expression CatchExpression_copy(const void *vthis, Allocator allocator) {
    CatchExpression *copy = Allocator_malloc(allocator, sizeof(CatchExpression));
    *copy = CatchExpression_copy_(this, allocator);
    return CatchExpression_Expression(copy);
}

#undef this

const struct IExpression ICatchExpression = {
    &CatchExpression_destroy,
    &CatchExpression_evaluate,
    &CatchExpression_print,
    &CatchExpression_copy
};