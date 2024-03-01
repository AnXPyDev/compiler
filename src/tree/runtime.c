BasicType RT_basicTypes_[TYPE_BASIC__END];
Type RT_basicTypes[TYPE_BASIC__END];

Allocator RT_standardAllocator;
Value RT_nullValue;
Type RT_nullType;
Expression RT_nullExpression;

#define RT_ALLOC RT_standardAllocator
#define RT_NONE RT_nullValue
#define RT_NONETYPE RT_nullType
#define RT_NONEX RT_nullExpression

FileOutStream RT_STDERR_;
FileOutStream RT_STDOUT_;
FileInStream RT_STDIN_;

OutStream RT_STDERR;
OutStream RT_STDOUT;
InStream RT_STDIN;

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