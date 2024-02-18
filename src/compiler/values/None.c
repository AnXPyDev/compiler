extern const struct IValue INoneValue;

Value NoneValue_create() {
    Value value = { &INoneValue, NULL };
    return value;
}

Type NoneValue_getType(const void *vthis) {
    return RT_basicTypes[TYPE_META_NONE];
}

void NoneValue_destroy(void *vthis) {
}

Value NoneValue_copy(const void *vthis, Allocator allocator) {
    return NoneValue_create();
}

Value NoneValue_move(const void *vthis, Allocator allocator) {
    return NoneValue_create();
}

void NoneValue_print(const void *vthis, OutStream stream) {
    OutStream_puts(stream, "None");
}

const struct IValue INoneValue = {
    &NoneValue_destroy,
    &NoneValue_getType,
    &NoneValue_print,
    &NoneValue_copy,
    &NoneValue_move
};