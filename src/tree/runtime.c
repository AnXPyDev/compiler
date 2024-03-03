void runtime_init() {
    for (int i = 0; i < TYPE_BASIC__END; i++) {
        RT_basicTypes_[i] = BasicType_new(i);
        RT_basicTypes[i] = BasicType_Type(&RT_basicTypes_[i]);
    }

    RT_standardAllocator = StandardAllocator_create();
    RT_nullValue = NoneValue_create();
    RT_nullType = RT_basicTypes[TYPE_META_NONE];

    RT_nullExpression = NoneExpression_create();

    RT_STDERR_ = FileOutStream_new(stderr);
    RT_STDERR = FileOutStream_OutStream(&RT_STDERR_);
    RT_STDOUT_ = FileOutStream_new(stdout);
    RT_STDOUT = FileOutStream_OutStream(&RT_STDOUT_);
    RT_STDIN_ = FileInStream_new(stdin);
    RT_STDIN = FileInStream_InStream(&RT_STDIN_);
}

void runtime_shutdown() {
    Expression_destroy(RT_nullExpression);
}

Value runtime_projectile(Allocator allocator, Value value) {
    ProjectileValue *projectile = Allocator_malloc(allocator, sizeof(ProjectileValue)); 
    *projectile = ProjectileValue_new(allocator, value);
    return ProjectileValue_Value(projectile);
}

Value runtime_errno(Allocator allocator, int errcode) {
    PrimitiveValue errnum = PrimitiveValue_newInt(errcode);
    Value err = runtime_projectile(allocator, PrimitiveValue_Value(&errnum));
    PrimitiveValue_destroy(&errnum);
    return err;
}

Value runtime_errsmsg(Allocator allocator, const String *message) {
    MessageValue msg = MessageValue_new(allocator, message);
    Value err = runtime_projectile(allocator, MessageValue_Value(&msg));
    MessageValue_destroy(&msg);
    return err;
}

Value runtime_errmsg(Allocator allocator, const char *message) {
    String s = String_const_proxy(message);
    return runtime_errsmsg(allocator, &s);
}
