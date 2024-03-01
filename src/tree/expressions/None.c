extern const struct IExpression INoneExpression;

Expression NoneExpression_create() {
    Expression expression = { &INoneExpression, NULL };
}

void NoneExpression_destroy(void *vthis) {
}

Expression NoneExpression_copy(const void *vthis, Allocator allocator) {
    return NoneExpression_create();
}

void NoneExpression_print(const void *vthis, OutStream stream) {
    OutStream_puts(stream, "NoneExpression");
}

Value NoneExpression_evaluate(const void *vthis, Context *context) {
    return RT_nullValue;
}

const struct IExpression INoneExpression = {
    &NoneExpression_destroy,
    &NoneExpression_evaluate,
    &NoneExpression_print,
    &NoneExpression_copy
};