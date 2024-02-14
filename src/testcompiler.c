#include "compiler.c"

int main() {
    runtime_init();

    FileOutStream os_stderr_ = FileOutStream_new(stderr);
    OutStream os_stderr = FileOutStream_OutStream(&os_stderr_);

    Context ctx = Context_new(RT_ALLOC, NULL);
    PrimitiveValue pval = PrimitiveValue_newInt(321321);

    Value val = PrimitiveValue_Value(&pval);

    Token tkn = Token_new_cstring(RT_ALLOC, "testVar");
    Declaration decl = Declaration_new(Value_getType(val), Token_copy(&tkn, RT_ALLOC));

    Context_declare(&ctx, &decl);
    Context_setValue(&ctx, &tkn, val, NULL);

    Value_destroy(val);

    TokenExpression ex_get_ = TokenExpression_new(Token_copy(&tkn, RT_ALLOC));
    Expression ex_get = TokenExpression_Expression(&ex_get_);

    val = Expression_evaluate(ex_get, &ctx);

    Value_print(val, os_stderr);
    OutStream_puts(os_stderr, "\n");

    Expression_destroy(ex_get);
    Token_destroy(&tkn);
    Context_destroy(&ctx);
}