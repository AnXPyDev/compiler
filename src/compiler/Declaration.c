typedef struct {
    Type type;
    Token token;
} Declaration;

Declaration Declaration_new(Type type, Token token) {
    Declaration this;
    this.type = type;
    this.token = token;
    return this;
}

Declaration Declaration_copy(const Declaration *decl, Allocator allocator) {
    Declaration this;
    this.type = decl->type;
    this.token = Token_copy(&decl->token, allocator);
    return this;
}

void Declaration_destroy(Declaration *this) {
    Token_destroy(&this->token);
}

