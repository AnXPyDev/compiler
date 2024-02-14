typedef struct {
    Token token;
} TokenExpression;

#define this ((TokenExpression*)vthis)

void TokenExpression_destroy(void *vthis) {
    Token_destroy(&this->token);
}

Value TokenExpression_evaluate(void *vthis, Context *context) {
    return Context_getValue(context, &this->token);
}

void TokenExpression_print(void *vthis, OutStream stream) {
    OutStream_puts(stream, "TokenExpression{ ");
    Token_print(&this->token, stream);
    OutStream_puts(stream, " }");
}

#undef this

const struct IExpression ITokenExpression = {
    &TokenExpression_destroy,
    &TokenExpression_evaluate,
    &TokenExpression_print
};

TokenExpression TokenExpression_new(Token token) {
    TokenExpression this;
    this.token = token;
    return this;
}

Expression TokenExpression_Expression(TokenExpression *this) {
    Expression expression = { &ITokenExpression, this };
    return expression;
}