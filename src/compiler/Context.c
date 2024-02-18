typedef struct Context {
    struct Context *parent;
    Allocator allocator;
    Map store;
} Context;

typedef struct {
    //Declaration declaration;
    int occupied;
    Type type;
    Value value;
} Context_Slot;

void Context_create(Context *this, Allocator allocator, Context *parent) {
    this->parent = parent;
    this->allocator = allocator;
    this->store = Map_new(allocator, sizeof(Context_Slot));
}

Context Context_new(Allocator allocator, Context *parent) {
    Context this;
    Context_create(&this, allocator, parent);
    return this;
}

Value Context_getValue(Context *this, const Token *token) {
    Context_Slot *slot = Map_get_db(&this->store, Token_DataBuffer(token));
    if (slot == NULL) {
        if (this->parent != NULL) {
            return Context_getValue(this->parent, token);
        }
        return RT_NONE;
    }
    if (slot->occupied == 0) {
        return RT_NONE;
    }
    return slot->value;
}

typedef Value (*fn_Value_mover)(Value, Allocator);

void Context_setValue(Context *this, const Token *token, Value value, fn_Value_mover mover) {
    // TODO: type checking

    Context_Slot *slot = Map_get_db(&this->store, Token_DataBuffer(token));
    if (slot == NULL) {
        if (this->parent != NULL) {
            return Context_setValue(this->parent, token, value, mover);
        }
        return;
    }

    if (mover == NULL) {
        mover = &Value_copy;
    }

    Value moved = mover(value, this->allocator);
    if (slot->occupied) {
        Value_destroy(slot->value);
        Value_free(slot->value, this->allocator);
    }
    slot->occupied = 1;
    slot->value = moved;
}

/*

void Context_declare(Context *this, const Declaration *declaration) {
    Context_Slot *slot = Map_ensure_db(&this->store, Token_DataBuffer(&declaration->token));
    slot->declaration = Declaration_copy(declaration, this->allocator);
    slot->occupied = 0;
}

*/

void Context_declare(Context *this, Type type, const Token *token) {
    Context_Slot *slot = Map_ensure_db(&this->store, Token_DataBuffer(token));
    slot->occupied = 0;
    slot->type = Type_copy(type, this->allocator);
}

void Context_Slot_destroy(Context *context, Context_Slot *this) {
    Value_destroy(this->value);
    Value_free(this->value, context->allocator);
    Type_destroy(this->type);
    Type_free(this->type, context->allocator);
}

void *Context_store_destructor(void *item, void *payload) {
    Context_Slot_destroy(payload, item);
}

void Context_destroy(Context *this) {
    Map_foreach(&this->store, &Context_store_destructor, this);
    Map_destroy(&this->store);
}

Context Context_inherit(Context *parent) {
    return Context_new(parent->allocator, parent);
}