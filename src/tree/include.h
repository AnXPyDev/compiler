#include "decl.h"
#include "types.h"

#include "Type.c"
#include "types/Basic.c"
#include "types/Sequence.c"
#include "types/Callable.c"

#include "Token.c"
#include "Value.c"

BasicType RT_basicTypes_[TYPE_BASIC__END];
Type RT_basicTypes[TYPE_BASIC__END];

Allocator RT_standardAllocator;
Value RT_nullValue;
Type RT_nullType;

#define RT_ALLOC RT_standardAllocator
#define RT_NONE RT_nullValue
#define RT_NONETYPE RT_nullType
#define RT_NONEX RT_nullExpression

FileOutStream RT_STDERR_;
FileOutStream RT_STDOUT_;
FileInStream RT_STDIN_;

OutStream RT_STDERR;
OutStream RT_STDOUT;
InStream RT_STDIN;

void runtime_init();
void runtime_shutdown();
Value runtime_projectile(Allocator allocator, Value value);
Value runtime_errno(Allocator allocator, int code);
Value runtime_errmsg(Allocator allocator, const char *message);
Value runtime_errsmsg(Allocator allocator, const String *message);

#include "Context.c"
#include "Expression.c"

Expression RT_nullExpression;

#include "values/None.c"
#include "expressions/None.c"

#include "values/Primitive.c"
#include "values/Type.c"
#include "values/Token.c"
#include "values/Projectile.c"
#include "values/Message.c"
#include "values/Expression.c"
#include "values/Sequence.c"
#include "values/Callable.c"

#include "expressions/Value.c"
#include "expressions/Token.c"
#include "expressions/Sequence.c"
#include "expressions/Builtin.c"
#include "expressions/Call.c"
#include "expressions/Get.c"
#include "expressions/Set.c"
#include "expressions/Throw.c"
#include "expressions/Catch.c"
#include "expressions/Evaluate.c"

#include "runtime.c"