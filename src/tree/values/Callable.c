typedef struct {
    Allocator allocator;
    CallableType type;
    Vector arguments;
    Expression expression;
} CallableValue;

extern const struct IValue ICallableValue;

CallableValue CallableValue_new(Allocator allocator, const CallableType *type, const Vector *args, Expression expression) {
    CallableValue this;
    this.allocator = allocator;
    this.type = CallableType_copy_(type, allocator);
    this.expression = Expression_copy(expression, allocator);
    this.arguments = Vector_new(allocator, sizeof(Token));
    Vector_init(&this.arguments, args->size);
    for (const Token *token = Vector_cbegin(args); token < (const Token*)Vector_cend(args); token++) {
        *(Token*)Vector_push(&this.arguments) = Token_copy(token, allocator);
    }
    return this;
}

Value CallableValue_Value(CallableValue *this) {
    Value value = { &ICallableValue, this };
    return value;
}

#define this ((CallableValue*)vthis)

Type CallableValue_getType(const void *vthis) {
    return CallableType_Type(&this->type);
}

void CallableValue_destroy(void *vthis) {
    CallableType_destroy(&this->type);
    for (Token *token = Vector_begin(&this->arguments); token < (Token*)Vector_end(&this->arguments); token++) {
        Token_destroy(token);
    }
    Vector_destroy(&this->arguments);
    Expression_destroy(this->expression);
    Expression_free(this->expression, this->allocator);
}

Value CallableValue_copy(const void *vthis, Allocator allocator) {
    CallableValue *copy = Allocator_malloc(allocator, sizeof(CallableValue));
    *copy = CallableValue_new(allocator, &this->type, &this->arguments, this->expression);
    return CallableValue_Value(copy);
}

Value CallableValue_move(void *vthis, Allocator allocator) {
    // TODO implement moving
    return CallableValue_copy(vthis, allocator);
}

void CallableValue_print(const void *vthis, OutStream stream) {
    OutStream_putc(stream, '(');
    CallableType_print(&this->type, stream);
    OutStream_puts(stream, " : (");

    if (this->arguments.size < 1) {
        goto skip;
    }

    Token *token = Vector_begin(&this->arguments);
    Token_print(token, stream);
    token++;
    for (; token < (Token*)Vector_end(&this->arguments); token++) {
        OutStream_puts(stream, ", ");
        Token_print(token, stream);
    }

    skip:;
    OutStream_puts(stream, ") => ");

    Expression_print(this->expression, stream);

    OutStream_putc(stream, ')');
}

#undef this

int CallableValue_is(Value value) {
    return value.interface == &ICallableValue;
}

const struct IValue ICallableValue = {
    &CallableValue_destroy,
    &CallableValue_getType,
    &CallableValue_print,
    &CallableValue_copy,
    &CallableValue_move
};