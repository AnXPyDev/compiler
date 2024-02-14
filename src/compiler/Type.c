struct IType;

typedef struct {
    const struct IType *interface;
    void *object;
} Type;

struct IType {
    int  (*equal  )(const void *this, const void *other);
    void (*print  )(const void *this, OutStream stream);
    void (*destroy)(      void *this);
};

int Type_equal(const Type this, const Type other) {
    if (this.object == other.object) {
        return 1;
    }

    if (this.interface != other.interface) {
        return 0;
    }

    return this.interface->equal(this.object, other.object);
}

void Type_print(const Type this, OutStream stream) {
    return this.interface->print(this.object, stream);
}

void Type_destroy(Type this) {
    return this.interface->destroy(this.object);
}

