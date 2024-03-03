#include "lib/include.h"
#include "tree/include.h"

Value builtin_test(Context *context, void *payload) {
    return RT_NONE;
}


CallableValue make_function(const Token *argname, BuiltinExpression *expression) {
    CallableValue val;
    val.expression = BuiltinExpression_Expression(expression);
    val.arguments = Vector_const_proxy((void*)argname, sizeof(Token), 1);
    val.type.ret = RT_basicTypes[TYPE_META_ANY];
    val.type.args.elements = Vector_const_proxy((void*)&RT_basicTypes[TYPE_PRIMITIVE_I32], sizeof(Type), 1);
    return val;
}

Value func_test(Context *context, void *payload) {
    const Token *argname = payload;
    Token_print(argname, RT_STDERR);
    OutStream_puts(RT_STDERR, " ");
    Value val = Context_getValue(context, argname);
    Value_print(val, RT_STDERR);
    OutStream_puts(RT_STDERR, "\n");
    return RT_NONE;
}

int main() {
    runtime_init();

    Context ctx = Context_new(RT_ALLOC, NULL, NULL);

    Token funcname = Token_const_proxy("func");
    Token argname = Token_const_proxy("arg1");
    BuiltinExpression func = BuiltinExpression_new("test", &func_test, &argname);

    CallableValue callable = make_function(&argname, &func);
    Context_declare(&ctx, CallableValue_getType(&callable), &funcname);
    Context_setValue(&ctx, &funcname, CallableValue_Value(&callable), NULL);

    Value val = Context_getValue(&ctx, &funcname);
    Value_print(val, RT_STDERR);
    OutStream_puts(RT_STDERR, "\n");


    TokenExpression ex_getCallable;
    ex_getCallable.token = funcname;

    PrimitiveValue numval = PrimitiveValue_newInt(1234);
    ValueExpression ex_arg;
    ex_arg.value = PrimitiveValue_Value(&numval);
    Expression ex_a1 = ValueExpression_Expression(&ex_arg);

    SequenceExpression ex_getArgs;
    ex_getArgs.expressions = Vector_const_proxy(&ex_a1, sizeof(Expression), 1);

    CallExpression call;
    call.behavior = ECALLEXPRESSION_GLOBAL;
    call.callable = TokenExpression_Expression(&ex_getCallable);
    call.arguments = SequenceExpression_Expression(&ex_getArgs);

    CallExpression_evaluate(&call, &ctx);

    Context_destroy(&ctx);

    runtime_shutdown();
}