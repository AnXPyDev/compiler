typedef enum {
    ESETEXPRESSION_DECLARE,
    ESETEXPRESSION_SET,
    ESETEXPRESSION_BOTH,
} SetExpression_Action;

typedef struct {
    Allocator allocator;
    SetExpression_Action action;
    Expression type;
    Expression token;
    Expression value;
} SetExpression;

extern const struct IExpression ISetExpression;

SetExpression SetExpression_new(Allocator allocator, SetExpression_Action action, Expression type, Expression token, Expression value) {
    SetExpression this;
    this.allocator = allocator;
    this.action = action;
    this.type = Expression_copy(type, allocator);
    this.token = Expression_copy(token, allocator);
    this.value = Expression_copy(value, allocator);
    return this;
}

SetExpression SetExpression_copy_(const SetExpression *this, Allocator allocator) {
    return SetExpression_new(allocator, this->action, this->type, this->token, this->value);
}

Expression SetExpression_Expression(SetExpression *this) {
    Expression expression = { &ISetExpression, this };
    return expression;
}

#define this ((SetExpression*)vthis)

void SetExpression_destroy(void *vthis) {
    Expression_destroy(this->type);
    Expression_free(this->type, this->allocator);
    Expression_destroy(this->token);
    Expression_free(this->token, this->allocator);
    Expression_destroy(this->value);
    Expression_free(this->value, this->allocator);
}

Value SetExpression_evaluate(const void *vthis, Context *context) {
    Context_Slot *slot = NULL;

    Value token = Expression_evaluate(this->token, context);
    if (!TokenValue_is(token)) {
        fprintf(stderr, "Value is not token");
        goto error;
    }

    TokenValue *tokenval = token.object;

    if (this->action == ESETEXPRESSION_SET) {
        goto set;
    }

    declare:;

    Value type = Expression_evaluate(this->type, context);
    if (!TypeValue_is(type)) {
        fprintf(stderr, "Value is not type");
        goto error;
    }

    TypeValue *typeval = type.object;
    slot = Context_declare(context, typeval->type, &tokenval->token);

    if (this->action != ESETEXPRESSION_BOTH) {
        return RT_NONE;
    }

    set:;

    Value value = Expression_evaluate(this->value, context);

    if (slot == NULL) {
        Context_setValue(context, &tokenval->token, value, NULL);
    } else {
        Context_setValueInSlot(context, slot, value, NULL);
    }

    return value;

    error:;
    return RT_NONE;
}

void SetExpression_print(const void *vthis, OutStream stream) {
    switch (this->action) {
        case ESETEXPRESSION_BOTH:
            OutStream_puts(stream, "DeclareSet");
            break;
        case ESETEXPRESSION_SET:
            OutStream_puts(stream, "Set");
            break;
        case ESETEXPRESSION_DECLARE:
            OutStream_puts(stream, "Declare");
            break;
    }

    OutStream_puts(stream, "{ ");
    Expression_print(this->type, stream);
    OutStream_puts(stream, " ");
    Expression_print(this->token, stream);
    OutStream_puts(stream, " = ");
    Expression_print(this->value, stream);
    OutStream_puts(stream, " }");
}

Expression SetExpression_copy(const void *vthis, Allocator allocator) {
    SetExpression *copy = Allocator_malloc(allocator, sizeof(SetExpression));
    *copy = SetExpression_copy_(this, allocator);
    return SetExpression_Expression(copy);
}

#undef this

const struct IExpression ISetExpression = {
    &SetExpression_destroy,
    &SetExpression_evaluate,
    &SetExpression_print,
    &SetExpression_copy
};