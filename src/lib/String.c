typedef struct {
    char *data;
    Size length;
    Size capacity;
} String;

void string_normalize(String *this) {
    if (this->length > this->capacity) {
        this->length = this->capacity;
    }

    this->data[this->length] = 0;
}

void string_blank(String *this, Size capacity) {
    this->data = malloc(capacity + 1);
    this->capacity = capacity;
    this->length = 0;
    string_normalize(this);
}

void string_create(String *this, const char *str) {
    Size length = strlen(str);
    this->data = malloc(length);
    this->length = length;
    this->capacity = length;
    strcpy(this->data, str);
    string_normalize(this);
}

void string_destroy(String *this) {
    this->capacity = 0;
    this->length = 0;
    free(this->data);
    this->data = NULL;
}