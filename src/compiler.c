#include "config.h"

int DEBUG = 0;
int LIBDEBUG = 0;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef size_t Size;

#include "lib/Util.c"
#include "lib/String.c"
#include "lib/IO.c"
#include "lib/Vector.c"
#include "lib/Map.c"

#include "compiler/Type.c"

#include "compiler/Value.c"
#include "compiler/values/Primitive.c"
#include "compiler/values/Sequence.c"

#include "compiler/Expression.c"
#include "compiler/expressions/Value.c"

#include "compiler/Token.c"
#include "compiler/Tree.c"