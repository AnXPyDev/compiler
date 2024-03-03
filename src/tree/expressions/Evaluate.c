typedef struct {
    Allocator allocator;
    Expression expression;
} EvaluateExpression;

extern const struct IExpression IEvaluateExpression;

EvaluateExpression EvaluateExpression_new(Allocator allocator, Expression expression) {
    EvaluateExpression this;
    this.allocator = allocator;
    this.expression = Expression_copy(expression, allocator);
    return this;
}

Expression EvaluateExpression_Expression(EvaluateExpression *this) {
    Expression expression = { &IEvaluateExpression, this };
    return expression;
}

#define this ((EvaluateExpression*)vthis)

Value EvaluateExpression_evaluate(const void *vthis, Context *context) {
    Value r = Expression_evaluate(this->expression, context);
    if (ProjectileValue_is(r)) {
        return r;
    }

    if (!ExpressionValue_is(r)) {
        Value_delete(r, context->allocator);
        return runtime_errmsg(context->allocator, "EvaluateExpression: expression result is not an ExpressionValue");
    }

    ExpressionValue *x = r.object;
    Value res = Expression_evaluate(x->expression, context);

    Value_delete(r, context->allocator);

    return res;
}

void EvaluateExpression_destroy(void *vthis) {
    Expression_destroy(this->expression);
    Expression_free(this->expression, this->allocator);
}

void EvaluateExpression_print(const void *vthis, OutStream stream) {
    OutStream_puts(stream, "EvaluateExpression{ ");
    Expression_print(this->expression, stream);
    OutStream_puts(stream, " }");
}

Expression EvaluateExpression_copy(const void *vthis, Allocator allocator) {
    EvaluateExpression *copy = Allocator_malloc(allocator, sizeof(EvaluateExpression));
    *copy = EvaluateExpression_new(allocator, this->expression);
    return EvaluateExpression_Expression(copy);
}

#undef this

const struct IExpression IEvaluateExpression = {
    &EvaluateExpression_destroy,
    &EvaluateExpression_evaluate,
    &EvaluateExpression_print,
    &EvaluateExpression_copy
};