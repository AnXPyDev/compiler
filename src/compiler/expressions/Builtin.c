typedef Value (*fn_Builtin)(Context *context, void *payload);

typedef struct {
    const char *name;
    fn_Builtin fn;
    void *payload;
} BuiltinExpression;

extern const struct IExpression IBuiltinExpression;

BuiltinExpression BuiltinExpression_new(const char *name, fn_Builtin fn, void *payload) {
    BuiltinExpression this;
    this.name = name;
    this.fn = fn;
    this.payload = payload;
    return this;
}

Expression BuiltinExpression_Expression(BuiltinExpression *this) {
    Expression expression = { &IBuiltinExpression, this };
    return expression;
}

#define this ((BuiltinExpression*)vthis)

void BuiltinExpression_destroy(void *vthis) {

}

Value BuiltinExpression_evaluate(const void *vthis, Context *context) {
    return this->fn(context, this->payload);
}

void BuiltinExpression_print(const void *vthis, OutStream stream) {
    OutStream_puts(stream, "BuiltinExpression{ ");
    OutStream_puts(stream, this->name);
    OutStream_puts(stream, " }");
}

Expression BuiltinExpression_copy(const void *vthis, Allocator allocator) {
    BuiltinExpression *copy = Allocator_malloc(allocator, sizeof(BuiltinExpression));
    *copy = BuiltinExpression_new(this->name, this->fn, this->payload);
    return BuiltinExpression_Expression(copy);
}

#undef this

const struct IExpression IBuiltinExpression = {
    &BuiltinExpression_destroy,
    &BuiltinExpression_evaluate,
    &BuiltinExpression_print,
    &BuiltinExpression_copy
};