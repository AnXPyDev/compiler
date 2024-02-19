typedef struct {
    Allocator allocator;
    Expression callable;
    Expression arguments;
} CallExpression;

extern const struct IExpression ICallExpression;

CallExpression CallExpression_new(Allocator allocator, Expression callable, Expression arguments) {
    CallExpression this;
    this.allocator = allocator;
    this.callable = Expression_copy(callable, allocator);
    this.arguments = Expression_copy(arguments, allocator);
    return this;
}

CallExpression CallExpression_new_(Allocator allocator, const CallExpression *src) {
    return CallExpression_new(allocator, src->callable, src->arguments);
}

Expression CallExpression_Expression(CallExpression *this) {
    Expression expression = { &ICallExpression, this };
    return expression;
}

#define this ((CallExpression*)vthis)

void CallExpression_destroy(void *vthis) {
    Expression_destroy(this->callable);
    Expression_free(this->callable, this->allocator);
    Expression_destroy(this->arguments);
    Expression_free(this->arguments, this->allocator);
}

int Value_isCallable(Value value) {
    if (value.interface == &ICallableValue) {
        return 1;
    }
    return 0;
}

int Value_isSequence(Value value) {
    if (value.interface == &ISequenceValue) {
        return 1;
    }
    return 0;
}

Value CallExpression_evaluate(const void *vthis, Context *context) {
    Value callable = Expression_evaluate(this->callable, context);
    if (!Value_isCallable(callable)) {
        fprintf(stderr, "Value is not callable\n");
        goto error;
    }

    CallableValue *func = callable.object;

    Value arguments = Expression_evaluate(this->arguments, context);
    if (!Value_isSequence(arguments)) {
        fprintf(stderr, "Arguments is not a sequence\n");
        goto error;
    }

    SequenceValue *args = arguments.object;
    if (!SequenceType_equal(&func->type.args, &args->type)) {
        fprintf(stderr, "Sequence types do not match\n");
    }

    Context inner_ctx = Context_inherit(context);

    Vector *types = &args->type.elements;
    Vector *tokens = &func->arguments;
    Vector *values = &args->elements;

    const Size len = types->size;

    for (Size i = 0; i < len; i++) {
        Type type = *(Type*)Vector_get(types, i);
        Token *token = (Token*)Vector_get(tokens, i);
        Value value = *(Value*)Vector_get(values, i);

        Context_declare(&inner_ctx, type, token);
        Context_setValue(&inner_ctx, token, value, &Value_copy);
    }

    Value ret = Expression_evaluate(func->expression, &inner_ctx);

    Value_destroy(callable);
    Value_free(callable, context->allocator);

    Value_destroy(arguments);
    Value_free(arguments, context->allocator);

    return ret;

    error:;

    // Implement throwing
    return RT_NONE;
}

void CallExpression_print(const void *vthis, OutStream stream) {
    OutStream_puts(stream, "CallExpression{ ");
    Expression_print(this->callable, stream);
    OutStream_puts(stream, "; ");
    Expression_print(this->arguments, stream);
    OutStream_puts(stream, " }");
}

Expression CallExpression_copy(const void *vthis, Allocator allocator) {
    CallExpression *copy = Allocator_malloc(allocator, sizeof(CallExpression));
    *copy = CallExpression_new_(allocator, this);
    return CallExpression_Expression(copy);
}

#undef this

const struct IExpression ICallExpression = {
    &CallExpression_destroy,
    &CallExpression_evaluate,
    &CallExpression_print,
    &CallExpression_copy
};