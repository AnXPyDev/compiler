enum PrimitiveTypes {
    PRIMITIVE_I32
};

struct PrimitiveValue {
    struct Value header;
    enum PrimitiveTypes type;
    union {
        int32_t I32;
    } value;
};

#define thiscast ((struct PrimitiveValue*)this)

struct Type *PrimitiveValue_getType(void *this) {
    return NULL;
}

void PrimitiveValue_fprint(void *this, FILE *outstream) {
    switch (thiscast->type) {
        case PRIMITIVE_I32:
            fprintf(outstream, "%d", thiscast->value.I32);
        default:
            break;
    }
}

#undef thiscast

const struct IValue IPrimitiveValue = {
   &emptyDestructor,
   &PrimitiveValue_getType,
   &PrimitiveValue_fprint
};


void PrimitiveValue_setup(struct PrimitiveValue *this) {
    this->header.interface = &IPrimitiveValue;
}

void PrimitiveValue_createInt(struct PrimitiveValue *this, int32_t value) {
    PrimitiveValue_setup(this);
    this->type = PRIMITIVE_I32;
    this->value.I32 = value;
}