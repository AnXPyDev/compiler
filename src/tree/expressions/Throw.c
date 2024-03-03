typedef struct {
    Allocator allocator;
    Expression value;
} ThrowExpression;

extern const struct IExpression IThrowExpression;

ThrowExpression ThrowExpression_new(Allocator allocator, Expression value) {
    ThrowExpression this;
    this.allocator = allocator;
    this.value = Expression_copy(value, allocator);
    return this;
}

ThrowExpression ThrowExpression_copy_(const ThrowExpression *this, Allocator allocator) {
    return ThrowExpression_new(allocator, this->value);
}

Expression ThrowExpression_Expression(ThrowExpression *this) {
    Expression expression = { &IThrowExpression, this };
    return expression;
}

#define this ((ThrowExpression*)vthis)

void ThrowExpression_destroy(void *vthis) {
    Expression_destroy(this->value);
    Expression_free(this->value, this->allocator);
}

Value ThrowExpression_evaluate(const void *vthis, Context *context) {
    Value r = Expression_evaluate(this->value, context);
    if (ProjectileValue_is(r)) {
        return r;
    }

    return ProjectileValue_create_move(context->allocator, context->allocator, r);
}

void ThrowExpression_print(const void *vthis, OutStream stream) {
    OutStream_puts(stream, "Throw{ ");
    Expression_print(this->value, stream);
    OutStream_puts(stream, " }");
}

Expression ThrowExpression_copy(const void *vthis, Allocator allocator) {
    ThrowExpression *copy = Allocator_malloc(allocator, sizeof(ThrowExpression));
    *copy = ThrowExpression_copy_(this, allocator);
    return ThrowExpression_Expression(copy);
}

#undef this

const struct IExpression IThrowExpression = {
    &ThrowExpression_destroy,
    &ThrowExpression_evaluate,
    &ThrowExpression_print,
    &ThrowExpression_copy
};