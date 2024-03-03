typedef struct {
    Allocator allocator;
    Expression token;
} GetExpression;

extern const struct IExpression IGetExpression;


GetExpression GetExpression_new(Allocator allocator, Expression token) {
    GetExpression this;
    this.allocator = allocator;
    this.token = Expression_copy(token, allocator);
    return this;
}

GetExpression GetExpression_copy_(const GetExpression *this, Allocator allocator) {
    return GetExpression_new(allocator, this->token);
}

Expression GetExpression_Expression(GetExpression *this) {
    Expression expression = { &IGetExpression, this };
    return expression;
}

#define this ((GetExpression*)vthis)

void GetExpression_destroy(void *vthis) {
    Expression_destroy(this->token);
    Expression_free(this->token, this->allocator);
}

Value GetExpression_evaluate(const void *vthis, Context *context) {
    Value token = Expression_evaluate(this->token, context);
    if (!TokenValue_is(token)) {
        Value_destroy(token);
        Value_free(token, context->allocator);
        return runtime_errmsg(context->allocator, "GetExpression: token eval result is not a TokenValue");
    }

    TokenValue *tokenval = token.object;

    Value ret = Value_copy(Context_getValue(context, &tokenval->token), context->allocator);
    Value_destroy(token);
    Value_free(token, context->allocator);

    return ret;
}

void GetExpression_print(const void *vthis, OutStream stream) {
    OutStream_puts(stream, "Get{ ");
    Expression_print(this->token, stream);
    OutStream_puts(stream, " }");
}

Expression GetExpression_copy(const void *vthis, Allocator allocator) {
    GetExpression *copy = Allocator_malloc(allocator, sizeof(GetExpression));
    *copy = GetExpression_copy_(this, allocator);
    return GetExpression_Expression(copy);
}

#undef this

const struct IExpression IGetExpression = {
    &GetExpression_destroy,
    &GetExpression_evaluate,
    &GetExpression_print,
    &GetExpression_copy
};