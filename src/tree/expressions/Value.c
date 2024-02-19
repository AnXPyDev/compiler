typedef struct {
    Allocator allocator;
    Value value;
} ValueExpression;

extern const struct IExpression IValueExpression;

ValueExpression ValueExpression_new(Allocator allocator, Value value) {
    ValueExpression this;
    this.allocator = allocator;
    this.value = Value_copy(value, allocator);
    return this;
}

Expression ValueExpression_Expression(ValueExpression *this) {
    Expression expression = { &IValueExpression, this };
    return expression;
}

#define this ((ValueExpression*)vthis)

void ValueExpression_destroy(void *vthis) {
    Value_destroy(this->value);
    Value_free(this->value, this->allocator);
}

Value ValueExpression_evaluate(const void *vthis, Context *context) {
    return Value_copy(this->value, context->allocator);
}

void ValueExpression_print(const void *vthis, OutStream stream) {
    OutStream_puts(stream, "ValueExpression{ ");
    Value_print(this->value, stream);
    OutStream_puts(stream, " }");
}

Expression ValueExpression_copy(const void *vthis, Allocator allocator) {
    ValueExpression *copy = Allocator_malloc(allocator, sizeof(ValueExpression));
    *copy = ValueExpression_new(allocator, this->value);
    return ValueExpression_Expression(copy);
}

#undef this

const struct IExpression IValueExpression = {
    &ValueExpression_destroy,
    &ValueExpression_evaluate,
    &ValueExpression_print,
    &ValueExpression_copy
};
