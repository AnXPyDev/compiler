typedef struct {
    Token token;
} TokenExpression;

extern const struct IExpression ITokenExpression;

TokenExpression TokenExpression_new(const Token *token, Allocator allocator) {
    TokenExpression this;
    this.token = Token_copy(token, allocator);
    return this;
}

Expression TokenExpression_Expression(TokenExpression *this) {
    Expression expression = { &ITokenExpression, this };
    return expression;
}

#define this ((TokenExpression*)vthis)

void TokenExpression_destroy(void *vthis) {
    Token_destroy(&this->token);
}

Value TokenExpression_evaluate(const void *vthis, Context *context) {
    return Value_copy(Context_getValue(context, &this->token), context->allocator);
}

void TokenExpression_print(const void *vthis, OutStream stream) {
    OutStream_puts(stream, "TokenExpression{ ");
    Token_print(&this->token, stream);
    OutStream_puts(stream, " }");
}

Expression TokenExpression_copy(const void *vthis, Allocator allocator) {
    TokenExpression *copy = Allocator_malloc(allocator, sizeof(TokenExpression));
    *copy = TokenExpression_new(&this->token, allocator);
    return TokenExpression_Expression(copy);
}

#undef this

const struct IExpression ITokenExpression = {
    &TokenExpression_destroy,
    &TokenExpression_evaluate,
    &TokenExpression_print,
    &TokenExpression_copy
};
