typedef struct {
    Allocator allocator;
    Type type;
    Token token;
} Declaration;

Declaration Declaration_new(Type type, const Token *token, Allocator allocator) {
    Declaration this;
    this.allocator = allocator;
    this.type = Type_copy(type, allocator);
    this.token = Token_copy(token, allocator);
    return this;
}

Declaration Declaration_copy(const Declaration *decl, Allocator allocator) {
    Declaration this;
    this.allocator = allocator;
    this.type = Type_copy(decl->type, allocator);
    this.token = Token_copy(&decl->token, allocator);
    return this;
}

void Declaration_destroy(Declaration *this) {
    Type_destroy(this->type);
    Type_free(this->type, this->allocator);
    Token_destroy(&this->token);
}

void Declaration_print(const Declaration *this, OutStream stream) {
    OutStream_putc(stream, '[');
    Type_print(this->type, stream);
    OutStream_putc(stream, ' ');
    Token_print(&this->token, stream);
    OutStream_putc(stream, ']');
}