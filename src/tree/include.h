#include "decl.h"
#include "types.h"

#include "Type.c"
#include "types/Basic.c"
#include "types/Sequence.c"
#include "types/Callable.c"

extern Type RT_basicTypes[];

#include "Token.c"
#include "Value.c"

extern Value RT_nullValue;

#include "Context.c"

#include "values/None.c"
#include "Expression.c"
#include "expressions/None.c"

#include "runtime.c"

#include "values/Primitive.c"
#include "values/Type.c"
#include "values/Token.c"
#include "values/Projectile.c"
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