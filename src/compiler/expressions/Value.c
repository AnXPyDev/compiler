typedef struct {
    Value value;
} ValueExpression;

#define this ((ValueExpression*)vthis)

void ValueExpression_destroy(void *vthis) {
    Value_destroy(this->value);
}

Value ValueExpression_evaluate(void *vthis, Context *context) {
    return this->value;
}

void ValueExpression_print(void *vthis, OutStream stream) {
    OutStream_puts(stream, "ValueExpression{ ");
    Value_print(this->value, stream);
    OutStream_puts(stream, " }");
}

#undef this

const struct IExpression IValueExpression = {
    &ValueExpression_destroy,
    &ValueExpression_evaluate,
    &ValueExpression_print
};

ValueExpression ValueExpression_new(Value value) {
    ValueExpression this;
    this.value = value;
    return this;
}

Expression ValueExpression_Expression(ValueExpression *this) {
    Expression expression = { &IValueExpression, this };
    return expression;
}