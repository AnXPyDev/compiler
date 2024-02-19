#include "decl.h"
#include "types.h"

#include "Type.c"
#include "types/Basic.c"
#include "types/Sequence.c"
#include "types/Callable.c"

extern Type RT_basicTypes[];

#include "Value.c"
#include "values/None.c"

#include "runtime.c"

#include "Token.c"

#include "Context.c"

#include "Expression.c"

#include "values/Primitive.c"
#include "values/Callable.c"
#include "values/Sequence.c"

#include "expressions/Value.c"
#include "expressions/Token.c"
#include "expressions/Sequence.c"
#include "expressions/Builtin.c"
#include "expressions/Call.c"