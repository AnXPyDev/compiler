PrimitiveType RT_primitiveTypes_[TYPE_PRIMITIVE__END];
Type RT_primitiveTypes[TYPE_PRIMITIVE__END];

MetaType RT_metaTypes_[TYPE_META__END];
Type RT_metaTypes[TYPE_META__END];

Allocator RT_standardAllocator;
Value RT_nullValue;

#define RT_ALLOC RT_standardAllocator
#define RT_NULL RT_nullValue

void runtime_init() {
    for (int i = 0; i < TYPE_PRIMITIVE__END; i++) {
        RT_primitiveTypes_[i] = PrimitiveType_new(i);
        RT_primitiveTypes[i] = PrimitiveType_Type(&RT_primitiveTypes_[i]);
    }

    for (int i = 0; i < TYPE_META__END; i++) {
        RT_metaTypes_[i] = MetaType_new(i);
        RT_metaTypes[i] = MetaType_Type(&RT_metaTypes_[i]);
    }

    RT_standardAllocator = StandardAllocator_create();
    RT_nullValue = Value_newNull();
}