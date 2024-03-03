typedef enum {
    ECALLEXPRESSION_SUB, ECALLEXPRESSION_GLOBAL
} CallExpression_ContextBehavior;

typedef struct {
    Allocator allocator;
    CallExpression_ContextBehavior behavior;
    Expression callable;
    Expression arguments;
} CallExpression;

extern const struct IExpression ICallExpression;

CallExpression CallExpression_new(Allocator allocator, CallExpression_ContextBehavior behavior, Expression callable, Expression arguments) {
    CallExpression this;
    this.allocator = allocator;
    this.behavior = behavior;
    this.callable = Expression_copy(callable, allocator);
    this.arguments = Expression_copy(arguments, allocator);
    return this;
}

CallExpression CallExpression_new_(Allocator allocator, const CallExpression *src) {
    return CallExpression_new(allocator, src->behavior, src->callable, src->arguments);
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

Value CallExpression_evaluate(const void *vthis, Context *context) {
    Value callable = Expression_evaluate(this->callable, context);
    if (ProjectileValue_is(callable)) {
        return callable;
    }

    if (!CallableValue_is(callable)) {
        Value err = runtime_errmsg(context->allocator, "CallExpression: callable eval returned non callable value");
        Value_delete(callable, context->allocator);
        return err;
    }

    CallableValue *func = callable.object;

    Value arguments = Expression_evaluate(this->arguments, context);
    if (ProjectileValue_is(arguments)) {
        Value_delete(callable, context->allocator);
        return arguments;
    }

    if (!SequenceValue_is(arguments)) {
        Value err = runtime_errmsg(context->allocator, "CallExpression: arguments eval returned non sequence value");
        Value_delete(callable, context->allocator);
        Value_delete(arguments, context->allocator);
        return err;
    }

    SequenceValue *args = arguments.object;
    if (!SequenceType_equal(&func->type.args, &args->type)) {
        Value err = runtime_errmsg(context->allocator, "CallExpression: arguments sequence type does not match");
        Value_delete(callable, context->allocator);
        Value_delete(arguments, context->allocator);
        return err;
    }

    Context inner_ctx;
    switch (this->behavior) {
        case ECALLEXPRESSION_SUB:
            inner_ctx = Context_new(context->allocator, context, context->global);
            break;
        case ECALLEXPRESSION_GLOBAL:
            inner_ctx = Context_new(context->allocator, NULL, context->global);
            break;
    }

    Vector *types = &args->type.elements;
    Vector *tokens = &func->arguments;
    Vector *values = &args->elements;

    const Size len = types->size;

    for (Size i = 0; i < len; i++) {
        Type type = *(Type*)Vector_get(types, i);
        Token *token = (Token*)Vector_get(tokens, i);
        Value value = *(Value*)Vector_get(values, i);

        Context_declare(&inner_ctx, type, token);

        // TODO: Optimize to get context slot when declaring
        Context_setValue(&inner_ctx, token, value, &Value_copy);
    }

    Value ret = Expression_evaluate(func->expression, &inner_ctx);
    Context_destroy(&inner_ctx);

    Value_delete(callable, context->allocator);
    Value_delete(arguments, context->allocator);

    return ret;
}

void CallExpression_print(const void *vthis, OutStream stream) {
    OutStream_puts(stream, "CallExpression{ ");
    switch (this->behavior) {
        case ECALLEXPRESSION_SUB:
            OutStream_puts(stream, "SUB; ");
            break;
        case ECALLEXPRESSION_GLOBAL:
            OutStream_puts(stream, "GLOBAL; ");
            break;
    }
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