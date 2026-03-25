#include <siex-dsl/core/hashmap.h>
#include <string.h>
#include <stddef.h>


int hashmap_init(hashmap* hm, arena* memory, size_t capacity) {
  if (!hm | !memory | capacity == 0)
    return 1;

  hm->memory = memory;
  hm->capacity = capacity;
  hm->size = 0;
  hm->entries = arena_calloc_aligned(hm->memory, capacity * sizeof(hashmap_entry), _Alignof(hashmap_entry));

  if (!hm->entries)
    return 1;

  return 0;
}

/*
===============================================================================
FNV-1a HASH EXPLANATION (for future me)
===============================================================================

This function computes a hash from a null-terminated string:

    uint32_t hashmap_hash_str(const char* str) {
        uint32_t hash = 2166136261u;

        while (*str) {
            hash ^= (unsigned char)(*str++);
            hash *= 16777619u;
        }

        return hash;
    }

-------------------------------------------------------------------------------
1. HOW THE `while (*str)` LOOP WORKS
-------------------------------------------------------------------------------

- `str` is a pointer to a null-terminated string:

    "abc" in memory:
        [ 'a' ][ 'b' ][ 'c' ][ '\0' ]
           ^
           str starts here

- The loop condition:

    while (*str)

  means:

    while (*str != '\0')

- Each iteration:
  
    hash ^= (unsigned char)(*str++);
  
  IMPORTANT:
  
    `*str++` means:
        1. read the current character (*str)
        2. then increment the pointer (str++)

    It is equivalent to:
        *(str++)

    NOT:
        (*str)++   // this would modify the string (WRONG)

- Step-by-step execution:

    Iteration 1:
        *str = 'a'
        use 'a'
        str now points to 'b'

    Iteration 2:
        *str = 'b'
        use 'b'
        str now points to 'c'

    Iteration 3:
        *str = 'c'
        use 'c'
        str now points to '\0'

    Iteration 4:
        *str = '\0' → loop stops

- This is a fast, zero-overhead way to iterate a string:
    - no strlen
    - no indexing
    - linear memory traversal


-------------------------------------------------------------------------------
2. HOW A SINGLE CHARACTER AFFECTS THE HASH (BIT-LEVEL IDEA)
-------------------------------------------------------------------------------

Initial hash (FNV offset basis):

    hash = 2166136261 = 0x811C9DC5

Example character:

    'a' = 0x61 = 01100001

Step 1: XOR

    hash ^= 'a'

    Only the lowest byte is directly affected:

        before: ... 11000101
        'a'   :      01100001
        ---------------------
        after :      10100100

    XOR mixes the new byte into the current state.

Step 2: Multiply

    hash *= 16777619 (0x01000193)

    This is NOT random.

    This multiplication:
        - spreads changes across all bits
        - mixes high and low bits
        - avoids local patterns

    Conceptually:

        hash * PRIME ≈
            (hash << 24) + (hash << 8) + (other mixes)

    Result:
        A small change (1 byte) affects the ENTIRE 32-bit hash.

-------------------------------------------------------------------------------
3. AVALANCHE EFFECT (WHY THIS WORKS)
-------------------------------------------------------------------------------

Example:

    "a" → 0xE40C292C
    "b" → 0xE70C2DE5

Even though:

    'a' = 01100001
    'b' = 01100010   (only 1 bit difference)

The resulting hashes differ in MANY bits.

This is called:

    "diffusion" or partial "avalanche effect"

-------------------------------------------------------------------------------
4. WHY `unsigned char` IS USED
-------------------------------------------------------------------------------

    (unsigned char)(*str)

This avoids issues where:

    char may be signed (-128 to 127)

Example:

    char c = 0xFF → could be -1

That would break the hash if not casted.

-------------------------------------------------------------------------------
5. IMPORTANT NOTES
-------------------------------------------------------------------------------

- The constants are NOT arbitrary:

    2166136261 → offset basis
    16777619   → FNV prime

  They were chosen experimentally for good distribution.

- Do NOT replace them with random numbers.

- This hash is:
    - fast
    - simple
    - good for hash tables

  But:
    - not cryptographic

-------------------------------------------------------------------------------
6. SUMMARY
-------------------------------------------------------------------------------

This loop:

    while (*str) {
        hash ^= (unsigned char)(*str++);
        hash *= PRIME;
    }

Transforms:

    a sequence of bytes → a well-distributed 32-bit integer

With:
    - good diffusion
    - low cost
    - predictable behavior

===============================================================================
*/
uint32_t hashmap_hash_str(const char* str) {
  // experimental value
  uint32_t hash = 2166136261u;

  while (*str) {
    // XOR operation
    hash ^= (unsigned char)(*str++);
    // last bit << affects more
    hash *= 16777619u;
  }

  return hash;
}

int hashmap_put(hashmap* hm, const char* key, void* value) {
  if (!hm || !key) return 1;

  uint32_t hash = hashmap_hash_str(key);
  size_t idx = hash & (hm->capacity - 1);

  hashmap_entry new_entry;
  new_entry.key = key;
  new_entry.value = value;
  new_entry.hash = hash;
  new_entry.distance = 0;

  for (;;) {
    hashmap_entry* entry = &hm->entries[idx];

    // SLOT VACÍO → insert directo
    if (entry->key == NULL) {
      *entry = new_entry;
      hm->size++;
      return 0;
    }

    // MISMA KEY → overwrite
    if (entry->hash == hash && strcmp(entry->key, key) == 0) {
      entry->value = value;
      return 0;
    }

    // ROBIN HOOD: comparar distancias
    if (entry->distance < new_entry.distance) {
      // swap completo
      hashmap_entry temp = *entry;
      *entry = new_entry;
      new_entry = temp;
    }

    // avanzar
    idx = (idx + 1) & (hm->capacity - 1);
    new_entry.distance++;
  }
}

void* hashmap_get(hashmap* hm, const char* key) {
  if (!hm || !key) return NULL;

  uint32_t hash = hashmap_hash_str(key);
  size_t idx = hash & (hm->capacity - 1);

  uint32_t distance = 0;

  for (;;) {
    hashmap_entry* entry = &hm->entries[idx];

    // slot vacío → no existe
    if (entry->key == NULL) {
      return NULL;
    }

    // match
    if (entry->hash == hash && strcmp(entry->key, key) == 0) {
      return entry->value;
    }

    // EARLY EXIT (clave de Robin Hood)
    if (entry->distance < distance) {
      return NULL;
    }

    idx = (idx + 1) & (hm->capacity - 1);
    distance++;
  }
}

int hashmap_exists(hashmap* hm, const char* key) {
  return hashmap_get(hm, key) != NULL;
}


int hashmap_put_hashed(hashmap* hm, const char* key, uint32_t hash, void* value) {
  if (!hm || !key) return 1;

  size_t idx = hash & (hm->capacity - 1);

  hashmap_entry new_entry;
  new_entry.key = key;
  new_entry.value = value;
  new_entry.hash = hash;
  new_entry.distance = 0;

  for (;;) {
    hashmap_entry* entry = &hm->entries[idx];

    if (entry->key == NULL) {
      *entry = new_entry;
      hm->size++;
      return 0;
    }

    if (entry->hash == hash && strcmp(entry->key, key) == 0) {
      entry->value = value;
      return 0;
    }

    if (entry->distance < new_entry.distance) {
      hashmap_entry temp = *entry;
      *entry = new_entry;
      new_entry = temp;
    }

    idx = (idx + 1) & (hm->capacity - 1);
    new_entry.distance++;
  }
}

void* hashmap_get_hashed(hashmap* hm, const char* key, uint32_t hash) {
  if (!hm || !key) return NULL;

  size_t idx = hash & (hm->capacity - 1);
  uint32_t distance = 0;

  for (;;) {
    hashmap_entry* entry = &hm->entries[idx];

    if (entry->key == NULL)
      return NULL;

    if (entry->hash == hash && strcmp(entry->key, key) == 0)
      return entry->value;

    if (entry->distance < distance)
      return NULL;

    idx = (idx + 1) & (hm->capacity - 1);
    distance++;
  }
}

