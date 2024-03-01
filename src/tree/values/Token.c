typedef struct {
    Token token;
} TokenValue;

extern const struct IValue ITokenValue;

TokenValue TokenValue_new(Allocator allocator, const Token *token) {
    TokenValue this;
    this.token = Token_copy(token, allocator);
    return this;
}

TokenValue TokenValue_copy_(const TokenValue *this, Allocator allocator) {
    return TokenValue_new(allocator, &this->token);
}

Value TokenValue_Value(TokenValue *this) {
    Value value = { &ITokenValue, this };
    return value;
}

#define this ((TokenValue*)vthis)

void TokenValue_destroy(void *vthis) {
    Token_destroy(&this->token);
}

Type TokenValue_getType(const void *vthis) {
    return RT_basicTypes[TYPE_META_TOKEN];
}

void TokenValue_print(const void *vthis, OutStream stream) {
    OutStream_puts(stream, "`");
    Token_print(&this->token, stream);
    OutStream_puts(stream, "`");
}

Value TokenValue_copy(const void *vthis, Allocator allocator) {
    TokenValue *copy = Allocator_malloc(allocator, sizeof(TokenValue));
    *copy = TokenValue_copy_(this, allocator);
    return TokenValue_Value(copy);
}

Value TokenValue_move(void *vthis, Allocator allocator) {
    return TokenValue_copy(vthis, allocator);
}

#undef this

int TokenValue_is(Value value) {
    return value.interface == &ITokenValue;
}

const struct IValue ITokenValue = {
    &TokenValue_destroy,
    &TokenValue_getType,
    &TokenValue_print,
    &TokenValue_copy,
    &TokenValue_move
};