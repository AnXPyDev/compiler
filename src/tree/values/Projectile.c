typedef struct {
    Allocator allocator;
    Value value;
} ProjectileValue;

extern const struct IValue IProjectileValue;

ProjectileValue ProjectileValue_new(Allocator allocator, Value value) {
    ProjectileValue this;
    this.allocator = allocator;
    this.value = Value_copy(value, allocator);
    return this;
}

ProjectileValue ProjectileValue_copy_(const ProjectileValue *this, Allocator allocator) {
    return ProjectileValue_new(this->allocator, this->value);
}

Value ProjectileValue_Value(ProjectileValue *this) {
    Value value = { &IProjectileValue, this };
    return value;
}

#define this ((ProjectileValue*)vthis)

void ProjectileValue_destroy(void *vthis) {
    Value_destroy(this->value);
    Value_free(this->value, this->allocator);
}

void ProjectileValue_print(const void *vthis, OutStream stream) {
    OutStream_puts(stream, "Projectile{ ");
    Value_print(this->value, stream);
    OutStream_puts(stream, " }");
}

Type ProjectileValue_getType(const void *vthis) {
    return RT_basicTypes[TYPE_META_PROJECTILE];
}

Value ProjectileValue_copy(const void *vthis, Allocator allocator) {
    ProjectileValue *copy = Allocator_malloc(allocator, sizeof(ProjectileValue));
    *copy = ProjectileValue_copy_(this, allocator);
    return ProjectileValue_Value(copy);
}

Value ProjectileValue_move(void *vthis, Allocator allocator) {
    return ProjectileValue_copy(vthis, allocator);
}

#undef this

int ProjectileValue_is(Value value) {
    return value.interface == &IProjectileValue;
}

const struct IValue IProjectileValue = {
    &ProjectileValue_destroy,
    &ProjectileValue_getType,
    &ProjectileValue_print,
    &ProjectileValue_copy,
    &ProjectileValue_move
};
