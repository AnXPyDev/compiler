#include "compiler.c"

int main() {
    struct PrimitiveValue v;
    PrimitiveValue_createInt(&v, 12);

    struct ExpressionValue e;
    ExpressionValue_create(&e, (void*)&v);

    Value_fprint(Expression_evaluate(&e), stderr);
    Expression_destroy(&e);
}