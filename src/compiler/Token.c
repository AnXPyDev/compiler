typedef struct Token {
    String value;
} Token;

Token Token_new(Allocator allocator, const String *value) {
    Token this;
    this.value = String_new(allocator);
    String_copy(&this.value, value);
    return this;
}

Token Token_new_cstring(Allocator allocator, const char *str) {
    Token this;
    this.value = String_new(allocator);
    String_cstring(&this.value, str);
    return this;
}

Token Token_copy(const Token *token, Allocator allocator) {
    return Token_new(allocator, &token->value);
}

void Token_destroy(Token *this) {
    String_destroy(&this->value);
}

void Token_print(Token *this, OutStream outstream) {
    OutStream_puts(outstream, "<<");
    String_print(&this->value, outstream);
    OutStream_puts(outstream, ">>");
}

DataBuffer Token_DataBuffer(const Token *this) {
    return String_DataBuffer(&this->value);
}