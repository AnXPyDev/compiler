struct IValue;

struct Value {
    const struct IValue *interface;
};

typedef void (*fn_Value_destroy)(void *this);
typedef struct Type *(*fn_Value_getType)(void *this);
typedef void (*fn_Value_fprint)(void *this, FILE *);

struct IValue {
    fn_Value_destroy destroy;
    fn_Value_getType getType;
    fn_Value_fprint fprint;
};

#define thiscast ((struct Value*)this)

void Value_destroy(void *this) {
    return thiscast->interface->destroy(this);
}

struct Type *Value_getType(void *this) {
    return thiscast->interface->getType(this);
}

void Value_fprint(void *this, FILE *outstream) {
    return thiscast->interface->fprint(this, outstream);
}

#undef thiscast