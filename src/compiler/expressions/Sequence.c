typedef struct {
    Allocator allocator;
    Vector expressions;
} SequenceExpression;

extern const struct IExpression ISequenceExpression;

SequenceExpression SequenceExpression_new(Allocator allocator, const Vector *expressions) {
    SequenceExpression this;
    this.allocator = allocator;
    this.expressions = Vector_new(allocator, sizeof(Expression));
    Vector_init(&this.expressions, expressions->size);
    for (const Expression *e = Vector_cbegin(expressions); e < (const Expression*)Vector_cend(expressions); e++) {
        *(Expression*)Vector_push_unsafe(&this.expressions) = Expression_copy(*e, allocator);
    }
    return this;
}

SequenceExpression SequenceExpression_new_(Allocator allocator, const SequenceExpression *this) {
    return SequenceExpression_new(allocator, &this->expressions);
}

Expression SequenceExpression_Expression(SequenceExpression *this) {
    Expression expression = { &ISequenceExpression, this };
    return expression;
}

#define this ((SequenceExpression*)vthis)

void SequenceExpression_destroy(void *vthis) {
    for (Expression *e = Vector_begin(&this->expressions); e < (Expression*)Vector_end(&this->expressions); e++) {
        Expression_destroy(*e);
        Expression_free(*e, this->allocator);
    }
    Vector_destroy(&this->expressions);
}

void SequenceExpression_print(const void *vthis, OutStream stream) {
    OutStream_putc(stream, '(');
    
    if (this->expressions.size == 0) {
        goto skip;
    }

    Expression *e = Vector_begin(&this->expressions);
    Expression_print(*e, stream);

    for (; e < (Expression*)Vector_end(&this->expressions); e++) {
        OutStream_puts(stream, ", ");
        Expression_print(*e, stream);
    }
    
    
    skip:;

    OutStream_putc(stream, ')');
}

Value SequenceExpression_evaluate(const void *vthis, Context *context) {
    Allocator allocator = context->allocator;
    SequenceValue *value = Allocator_malloc(allocator, sizeof(SequenceValue));
    value->allocator = allocator;
    value->elements = Vector_new(allocator, sizeof(Value));
    Vector_init(&value->elements, this->expressions.size);

    value->type.allocator = allocator;
    value->type.elements = Vector_new(allocator, sizeof(Type));
    Vector_init(&value->type.elements, this->expressions.size);

    for (Expression *e = Vector_begin(&this->expressions); e < (Expression*)Vector_end(&this->expressions); e++) {
        Value res = Expression_evaluate(*e, context);
        *(Value*)Vector_push_unsafe(&value->elements) = res;
        *(Type*)Vector_push_unsafe(&value->type.elements) = Type_copy(Value_getType(res), allocator);
    }
    return SequenceValue_Value(value);
}

Expression SequenceExpression_copy(const void *vthis, Allocator allocator) {
    SequenceExpression *copy = Allocator_malloc(allocator, sizeof(SequenceExpression));
    *copy = SequenceExpression_new_(allocator, this);
    return SequenceExpression_Expression(copy);
}

#undef this

const struct IExpression ISequenceExpression = {
    &SequenceExpression_destroy,
    &SequenceExpression_evaluate,
    &SequenceExpression_print,
    &SequenceExpression_copy
};