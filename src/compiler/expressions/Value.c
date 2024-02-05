struct ExpressionValue {
    struct Expression header;
    struct Value *value;
};

#define thiscast ((struct ExpressionValue*)this)

void ExpressionValue_destroy(void *this) {
    Value_destroy(thiscast->value);
}

struct Value *ExpressionValue_evaluate(void *this) {
    return thiscast->value;
}

#undef thiscast

const struct IExpression IExpressionValue = {
    &ExpressionValue_destroy,
    &ExpressionValue_evaluate 
};

void ExpressionValue_create(struct ExpressionValue *this, struct Value *value) {
    this->header.interface = &IExpressionValue;
    this->value = value;
}