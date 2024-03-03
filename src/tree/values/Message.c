typedef struct {
    String message;
} MessageValue;

extern const struct IValue IMessageValue;

MessageValue MessageValue_new(Allocator allocator, const String *message) {
    MessageValue this;
    this.message = String_new(allocator);
    String_copy(&this.message, message);
    return this;
}

MessageValue MessageValue_copy_(const MessageValue *this, Allocator allocator) {
    return MessageValue_new(allocator, &this->message);
}

Value MessageValue_Value(MessageValue *this) {
    Value value = { &IMessageValue, this };
    return value;
}

#define this ((MessageValue*)vthis)

void MessageValue_destroy(void *vthis) {
    String_destroy(&this->message);
}

Type MessageValue_getType(const void *vthis) {
    return RT_basicTypes[TYPE_META_MESSAGE];
}

void MessageValue_print(const void *vthis, OutStream stream) {
    OutStream_puts(stream, "'");
    String_print(&this->message, stream);
    OutStream_puts(stream, "'");
}

Value MessageValue_copy(const void *vthis, Allocator allocator) {
    MessageValue *copy = Allocator_malloc(allocator, sizeof(MessageValue));
    *copy = MessageValue_copy_(this, allocator);
    return MessageValue_Value(copy);
}

Value MessageValue_move(void *vthis, Allocator allocator) {
    return MessageValue_copy(vthis, allocator);
}

#undef this

int MessageValue_is(Value value) {
    return value.interface == &IMessageValue;
}

const struct IValue IMessageValue = {
    &MessageValue_destroy,
    &MessageValue_getType,
    &MessageValue_print,
    &MessageValue_copy,
    &MessageValue_move
};

