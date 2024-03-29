#define MAP_BUCKETS 16

#define ENTRY_ITEM(entry) ((void*)entry->blob)
#define ENTRY_KEY(entry, item_size) (entry->blob + item_size)

typedef unsigned int Map_Hash;

struct Map_Entry {
    struct Map_Entry *next;
    Map_Hash hash;
    Size key_size;
    // char[Map.item_size]; char[key_size]
    char blob[0];
};

typedef struct {
    Allocator allocator;
    Size item_size;
    struct Map_Entry *buckets[MAP_BUCKETS];
} Map;

Map Map_new(Allocator allocator, Size item_size) {
    Map this;
    this.allocator = allocator;
    this.item_size = item_size;
    for (Size i = 0; i < MAP_BUCKETS; i++) {
        this.buckets[i] = NULL;
    }
    return this;
}

struct Map_Entry *Map_createEntry(Map *this, Map_Hash hash, const char *key, Size key_size) {
    struct Map_Entry *entry = Allocator_calloc(this->allocator, sizeof(struct Map_Entry) + key_size + this->item_size);
    entry->key_size = key_size;
    entry->hash = hash;
    memcpy(ENTRY_KEY(entry, this->item_size), key, key_size);
    return entry;
}

void Map_destroyEntry(Map *this, struct Map_Entry *entry) {
    Allocator_free(this->allocator, entry);
}

Map_Hash Map_hash(const char *data, Size size) {
    Map_Hash hash = 5381;
	while ( size != 0 ) {
        hash = ((hash << 5) + hash) + *data;
        data++;
        size--;
	}
    return hash;
}

#define MAP_BUCKET(hash) (hash % MAP_BUCKETS)

struct Map_Entry *Map_pushKey(Map *this, Map_Hash hash, const char *key, Size key_size) {
    struct Map_Entry *entry = Map_createEntry(this, hash, key, key_size);
    struct Map_Entry *next_entry = this->buckets[MAP_BUCKET(hash)];
    entry->next = next_entry;
    this->buckets[MAP_BUCKET(hash)] = entry;
    return entry;
}

void Map_removeEntry(Map *this, struct Map_Entry **entryp) {
    struct Map_Entry *next = (*entryp)->next;
    Map_destroyEntry(this, *entryp);
    *entryp = next;
}

struct Map_Entry *Map_findEntry(Map *this, Map_Hash hash, const char *key, Size key_size) {
    struct Map_Entry *entry = this->buckets[MAP_BUCKET(hash)];
    while (1) {
        if (entry == NULL) { return NULL; }
        if (entry->hash == hash && entry->key_size == key_size && memcmp(ENTRY_KEY(entry, this->item_size), key, key_size) == 0) {
            return entry;
        }
        entry = entry->next;
    }
}

struct Map_Entry **Map_findEntryP(Map *this, Map_Hash hash, const char *key, Size key_size) {
    struct Map_Entry **entryp = this->buckets + MAP_BUCKET(hash);
    while (1) {
        struct Map_Entry *entry = *entryp;
        if (entry == NULL) { return NULL; }
        if (entry->hash == hash && entry->key_size == key_size && memcmp(ENTRY_KEY(entry, this->item_size), key, key_size) == 0) {
            return entryp;
        }
        entryp = &entry->next;
    }
}

void Map_destroyBucket(Map *this, struct Map_Entry *entry) {
    struct Map_Entry *next;
    while (1) {
        if (entry == NULL) { return; }
        next = entry->next;
        Map_destroyEntry(this, entry);
        entry = next;
    }
}

void *Map_add(Map *this, const char *key, Size key_size) {
    Map_Hash hash = Map_hash(key, key_size);
    struct Map_Entry *entry = Map_pushKey(this, hash, key, key_size);
    return ENTRY_ITEM(entry);
}

void *Map_add_db(Map *this, DataBuffer key) {
    return Map_add(this, key.data, key.size);
}

void *Map_ensure(Map *this, const char *key, Size key_size) {
    Map_Hash hash = Map_hash(key, key_size);
    struct Map_Entry *entry = Map_findEntry(this, hash, key, key_size);
    if (entry == NULL) {
        entry = Map_pushKey(this, hash, key, key_size);
    }
    return ENTRY_ITEM(entry);
}

void *Map_ensure_db(Map *this, DataBuffer key) {
    return Map_ensure(this, key.data, key.size);
}

void *Map_get(Map *this, const char *key, Size key_size) {
    Map_Hash hash = Map_hash(key, key_size);
    struct Map_Entry *entry = Map_findEntry(this, hash, key, key_size);
    if (entry == NULL) {
        return NULL;
    }
    return ENTRY_ITEM(entry);
}

void *Map_get_db(Map *this, DataBuffer key) {
    return Map_get(this, key.data, key.size);
}

void Map_delete(Map *this, const char *key, Size key_size) {
    Map_Hash hash = Map_hash(key, key_size);
    struct Map_Entry **entryp = Map_findEntryP(this, hash, key, key_size);
    if (entryp == NULL) {
        return;
    }
    Map_removeEntry(this, entryp);
}

void Map_delete_db(Map *this, DataBuffer key) {
    return Map_delete(this, key.data, key.size);
}

void Map_destroy(Map *this) {
    for (Size i = 0; i < MAP_BUCKETS; i++) {
        Map_destroyBucket(this, this->buckets[i]);
        this->buckets[i] = NULL;
    }
}

void *Map_foreach(Map *this, void *(*loop)(void *item, void *payload), void *payload) {
    for (Size i = 0; i < MAP_BUCKETS; i++) {
        struct Map_Entry *entry = this->buckets[i];
        while (entry) {
            void *r = loop(ENTRY_ITEM(entry), payload);
            if (r) {
                return r;
            }
        }
    }
    return NULL;
}

#undef ENTRY_ITEM
#undef ENTRY_KEY
#undef MAP_BUCKET