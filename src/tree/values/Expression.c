typedef struct {
    Allocator allocator;
    Expression expression;
} ExpressionValue;

extern const struct IValue IExpressionValue;

ExpressionValue ExpressionValue_new(Allocator allocator, Expression expression) {
    ExpressionValue this;
    this.allocator = allocator;
    this.expression = Expression_copy(expression, allocator);
    return this;
}

ExpressionValue ExpressionValue_copy_(const ExpressionValue *this, Allocator allocator) {
    return ExpressionValue_new(allocator, this->expression);
}

Value ExpressionValue_Value(ExpressionValue *this) {
    Value value = { &IExpressionValue, this };
    return value;
}

#define this ((ExpressionValue*)vthis)

void ExpressionValue_destroy(void *vthis) {
    Expression_destroy(this->expression);
    Expression_free(this->expression, this->allocator);
}

void ExpressionValue_print(const void *vthis, OutStream stream) {
    OutStream_puts(stream, "<< ");
    Expression_print(this->expression, stream);
    OutStream_puts(stream, " >>");
}

Type ExpressionValue_getType(const void *vthis) {
    return RT_basicTypes[TYPE_META_EXPRESSION];
}

Value ExpressionValue_copy(const void *vthis, Allocator allocator) {
    ExpressionValue *copy = Allocator_malloc(allocator, sizeof(ExpressionValue));
    *copy = ExpressionValue_copy_(this, allocator);
    return ExpressionValue_Value(copy);
}

Value ExpressionValue_move(void *vthis, Allocator allocator) {
    return ExpressionValue_copy(vthis, allocator);
}

#undef this

int ExpressionValue_is(Value value) {
    return value.interface == &IExpressionValue;
}

const struct IValue IExpressionValue = {
    &ExpressionValue_destroy,
    &ExpressionValue_getType,
    &ExpressionValue_print,
    &ExpressionValue_copy,
    &ExpressionValue_move
};