struct IExpression;

struct Expression {
    const struct IExpression *interface;
};

typedef void (*fn_Expression_destroy)(void *this);
typedef struct Value *(*fn_Expression_evaluate)(void *this);

struct IExpression {
    fn_Expression_destroy destroy;
    fn_Expression_evaluate evaluate;
};

#define thiscast ((struct Expression*)this)

void Expression_destroy(void *this) {
    return thiscast->interface->destroy(this);
};

struct Value *Expression_evaluate(void *this) {
    return thiscast->interface->evaluate(this);
};

#undef thiscast