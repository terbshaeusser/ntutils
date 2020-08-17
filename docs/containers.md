# Containers

Containers allow to store and query values. All container types are as type safe as possible with C. As a result, every
container type declares a special macro that is used to declare the container matching to the passed value type. It also
defines all necessary functions to query, add and remove values from the container. All macros require as first
parameter a name that is used as prefix for the definitions. The container type itself is named `name_t`.


## Circular Buffer

Circular buffer types are defined with the `NT_CIRC_BUF(name, value_type, capacity)` macro. In case the capacity is not
known at compile time, the third parameter can be omitted. The capacity must then be passed to the `name_new` function.

Example:

```
#include "nt/circular.h"

NT_CIRC_BUF(static_int_buffer, int, 4)
NT_CIRC_BUF(dyn_int_buffer, int)

int main() {
  static_int_buffer_t static_buffer = static_int_buffer_new();
  static_int_buffer_push(&static_buffer, 1);
  static_int_buffer_free(&static_buffer);

  dyn_int_buffer_t dyn_buffer = dyn_int_buffer_new(4);
  dyn_int_buffer_push(&dyn_buffer, 1);
  dyn_int_buffer_free(&dyn_buffer);
  return 0;
}
```


### *_new

Include: [nt/circular.h](../include/nt/circular.h)

Parameters:

* capacity (`size_t`): The capacity of the new circular buffer instance. This parameter must only be passed if the
  capacity was omitted when defining the buffer type.

Return:

* (`name_t`) A new circular buffer instance with the passed capacity.

Creates a new circular buffer instance and returns it.


### *_free

Include: [nt/circular.h](../include/nt/circular.h)

Parameters:

* self (`name_t*`): The circular buffer instance to release.

Releases a previously initialized circular buffer.


### *_capacity

Include: [nt/circular.h](../include/nt/circular.h)

Parameters:

* self (`name_t const*`): The circular buffer instance.

Return:

* (`size_t`) The capacity of the given circular buffer instance.


### *_count

Include: [nt/circular.h](../include/nt/circular.h)

Parameters:

* self (`name_t const*`): The circular buffer instance.

Return:

* (`size_t`) The number of values currently pushed on the given circular buffer instance.


### *_push

Include: [nt/circular.h](../include/nt/circular.h)

Parameters:

* self (`name_t*`): The circular buffer instance.
* value (`value_type`): The value to push.

Pushes a new value to the end of the given circular buffer instance. The function asserts that the buffer has enough
space for another value.


### *_push_ref

Include: [nt/circular.h](../include/nt/circular.h)

Parameters:

* self (`name_t*`): The circular buffer instance.
* value (`value_type const*`): A pointer to the value to push.

Pushes a new value to the end of the given circular buffer instance. The function asserts that the buffer has enough 
space for another value.


### *_pop

Include: [nt/circular.h](../include/nt/circular.h)

Parameters:

* self (`name_t*`): The circular buffer instance.
* value (`value_type*`): A pointer to a place where the popped value should be stored or `NULL`.

Pops the next value from the beginning of a given circular buffer instance. An assert ensures that the buffer is not
empty.


### *_clear

Include: [nt/circular.h](../include/nt/circular.h)

Parameters:

* self (`name_t*`): The circular buffer instance.

Removes all values from the given circular buffer.


### *_get

Include: [nt/circular.h](../include/nt/circular.h)

Parameters:

* self (`name_t const*`): The circular buffer instance.
* index (`size_t`): The index of the value.

Return:

* (`value_type const*`) A pointer to the value with the given index.

Returns the value with the given index. The index is checked with an assert.


### *_iterate

Include: [nt/circular.h](../include/nt/circular.h)

Parameters:

* self (`name_t const*`): The circular buffer instance.

Return:

* (`name_iterator_t`) An iterator that can be used to iterate over values in the circular buffer. Modifications on the
  circular buffer invalidate any existing iterator.


### *_iterator_next

Include: [nt/circular.h](../include/nt/circular.h)

Parameters:

* self (`name_iterator_t*`): A circular buffer iterator instance.

Return:

* (`value_type const*`) A pointer to the next value or `NULL` if no more values exist.

Advances the iterator to the next value and returns it.


## List

List types are defined with the `NT_LIST(name, value_type)` macro.

**Hint:** The list implementation uses a continuous block of memory in the background.

Example:

```
#include "nt/list.h"

NT_LIST(int_list, int)

int main() {
  int_list_t list = int_list_new();
  int_list_add(&list, 2);
  int_list_insert(&list, 0, 1);
  int_list_free(&list);
  return 0;
}
```


### *_new

Include: [nt/list.h](../include/nt/list.h)

Return:

* (`name_t`) A new list instance with 0 values.

Creates a new list instance and returns it.


### *_free

Include: [nt/list.h](../include/nt/list.h)

Parameters:

* self (`name_t*`): The list instance to release.

Releases a previously initialized list.


### *_capacity

Include: [nt/list.h](../include/nt/list.h)

Parameters:

* self (`name_t const*`): The list instance.

Return:

* (`size_t`) The capacity of the given list instance.


### *_count

Include: [nt/list.h](../include/nt/list.h)

Parameters:

* self (`name_t const*`): The list instance.

Return:

* (`size_t`) The number of values currently stored in the list instance.


### *_reserve

Include: [nt/list.h](../include/nt/list.h)

Parameters:

* self (`name_t*`): The list instance.
* amount (`size_t`): The number of values the function should reserve space for.

Reserves space for the given number of values. If the space is already available, the list remains unchanged.


### *_shrink

Include: [nt/list.h](../include/nt/list.h)

Parameters:

* self (`name_t*`): The list instance.

Reduces the memory used by the list to the exact number of stored values.


### *_add

Include: [nt/list.h](../include/nt/list.h)

Parameters:

* self (`name_t*`): The list instance.
* value (`value_type`): The value to add.

Return:

* (`size_t`) The index inside the list where the value was inserted.

Adds a value to the end of the list.


### *_add_ref

Include: [nt/list.h](../include/nt/list.h)

Parameters:

* self (`name_t*`): The list instance.
* value (`value_type const*`): A pointer to the value to add.

Return:

* (`size_t`) The index inside the list where the value was inserted.

Adds a value to the end of the list.


### *_insert

Include: [nt/list.h](../include/nt/list.h)

Parameters:

* self (`name_t*`): The list instance.
* index (`size_t`): The index the new value should have.
* value (`value_type`): The value to add.

Inserts a value into the list at the given index. The index is checked with an assert.


### *_insert_ref

Include: [nt/list.h](../include/nt/list.h)

Parameters:

* self (`name_t*`): The list instance.
* index (`size_t`): The index the new value should have.
* value (`value_type const*`): A pointer to the value to add.

Inserts a value into the list at the given index. The index is checked with an assert.


### *_remove

Include: [nt/list.h](../include/nt/list.h)

Parameters:

* self (`name_t*`): The list instance.
* index (`size_t`): The index of the value to remove.

Removes the value with the passed index. The index is checked with an assert.


### *_clear

Include: [nt/list.h](../include/nt/list.h)

Parameters:

* self (`name_t*`): The list instance.

Removes all values from the given list.


### *_get

Include: [nt/list.h](../include/nt/list.h)

Parameters:

* self (`name_t const*`): The list instance.
* index (`size_t`): The index of the value.

Return:

* (`value_type const*`) A pointer to the value with the given index.

Returns the value with the given index. The index is checked with an assert.


### *_iterate

Include: [nt/list.h](../include/nt/list.h)

Parameters:

* self (`name_t const*`): The list instance.

Return:

* (`name_iterator_t`) An iterator that can be used to iterate over values in the list. Modifications on the list
  invalidate any existing iterator.


### *_iterator_next

Include: [nt/list.h](../include/nt/list.h)

Parameters:

* self (`name_iterator_t*`): A list iterator instance.

Return:

* (`value_type const*`) A pointer to the next value or `NULL` if no more values exist.

Advances the iterator to the next value and returns it.


## Set

The `NT_SET(name, key_type)` macro defines set containers using hashing internally.

Example:

```
#include "nt/hash.h"
#include "nt/set.h"

NT_SET(int_set, int)

static nt_comparison_result_t compare_ints(int const *a, int const *b) {
  if (*a == *b) {
    return NT_COMPARE_EQUAL;
  }

  if (*a < *b) {
    return NT_COMPARE_ASC;
  }

  return NT_COMPARE_DESC;
}

int main() {
  int_set_t set = int_set_new(nt_murmur3_hasher, compare_ints);
  int_set_add(&set, 2);
  int_set_free(&set);
  return 0;
}
```


### *_new

Include: [nt/set.h](../include/nt/set.h)

Parameters:

* hasher (`nt_hasher_t`): A function to get the hash code from a key. An implementation of the MurmurHash3 algorithm
  with the name `nt_murmur3_hasher` can be found in [nt/hash.h](../include/nt/hash.h).
* comparator (`nt_comparison_result_t (*)(key_type const*, key_type const*)`): A function to compare two keys.

Return:

* (`name_t`) A new set instance.

Creates a new set instance and returns it.


### *_free

Include: [nt/set.h](../include/nt/set.h)

Parameters:

* self (`name_t*`): The set instance to release.

Releases a previously initialized set.


### *_count

Include: [nt/set.h](../include/nt/set.h)

Parameters:

* self (`name_t const*`): An initialized set instance.

Return:

* (`size_t`): The number of keys in the set.


### *_reserve

Include: [nt/set.h](../include/nt/set.h)

Parameters:

* self (`name_t*`): An initialized set instance.
* amount (`size_t`): Number of keys to reserve space for.

Reserves space for the given number of keys. If there is already enough space available, the function does nothing.


### *_add

Include: [nt/set.h](../include/nt/set.h)

Parameters:

* self (`name_t*`): An initialized set instance.
* key (`key_type`): The key to add.

Return:

* (`bool`) `true` if the key did not already exist in the set.

Adds a given key to the set.


### *_add_ref

Include: [nt/set.h](../include/nt/set.h)

Parameters:

* self (`name_t*`): An initialized set instance.
* key (`key_type const*`): A pointer to the key to add.

Return:

* (`bool`) `true` if the key did not already exist in the set.

Adds a given key to the set.


### *_remove

Include: [nt/set.h](../include/nt/set.h)

Parameters:

* self (`name_t*`): An initialized set instance.
* key (`key_type`): A key that should be removed.

Return:

* (`bool`) `true` if the key existed in the set and was removed.

Removes a given key from the set.


### *_remove_ref

Include: [nt/set.h](../include/nt/set.h)

Parameters:

* self (`name_t*`): An initialized set instance.
* key (`key_type const*`): A pointer to the key that should be removed.

Return:

* (`bool`) `true` if the key existed in the set and was removed.

Removes a given key from the set.


### *_clear

Include: [nt/set.h](../include/nt/set.h)

Parameters:

* self (`name_t*`): An initialized set instance.

Removes all keys from the set.


### *_contains

Include: [nt/set.h](../include/nt/set.h)

Parameters:

* self (`name_t const*`): An initialized set instance.
* key (`key_type`): The key to check.

Return:

* (`bool`) `true` if the key exists in the set.

Checks whether a given key exists in the set.


### *_contains_ref

Include: [nt/set.h](../include/nt/set.h)

Parameters:

* self (`name_t const*`): An initialized set instance.
* key (`key_type const*`): A pointer to the key to check.

Return:

* (`bool`) `true` if the key exists in the set.

Checks whether a given key exists in the set.


### *_iterate

Include: [nt/set.h](../include/nt/set.h)

Parameters:

* self (`name_t const*`): An initialized set instance.
* key (`key_type const*`): A pointer to a key to find or `NULL` to iterate over all keys.

Return:

* (`name_iterator_t`) An iterator that can be used to iterate over keys in the set. Modifications on the set invalidate
  any existing iterator.


### *_iterator_next

Include: [nt/set.h](../include/nt/set.h)

Parameters:

* self (`name_iterator_t*`): A set iterator instance.

Return:

* (`key_type const*`) A pointer to the next key or `NULL` if no more keys exist.

Advances the iterator to the next key and returns it.


## Map

The `NT_MAP(name, key_type, value_type)` macro defines map containers using hashing internally.

Example:

```
#include "nt/map.h"

NT_MAP(int_str_map, int, char const*)

static nt_comparison_result_t compare_ints(int const *a, int const *b) {
  if (*a == *b) {
    return NT_COMPARE_EQUAL;
  }

  if (*a < *b) {
    return NT_COMPARE_ASC;
  }

  return NT_COMPARE_DESC;
}

int main() {
  int_str_map_t map = int_str_map_new(nt_murmur3_hasher, compare_ints);
  int_str_map_add(&map, 2, "2");
  int_str_map_free(&map);
  return 0;
}
```


### *_new

Include: [nt/map.h](../include/nt/map.h)

Parameters:

* hasher (`nt_hasher_t`): A function to get the hash code from a key. An implementation of the MurmurHash3 algorithm
  with the name `nt_murmur3_hasher` can be found in [nt/hash.h](../include/nt/hash.h).
* comparator (`nt_comparison_result_t (*)(key_type const*, key_type const*)`): A function to compare two keys.

Return:

* (`name_t`) A new map instance.

Creates a new map instance and returns it.


### *_free

Include: [nt/map.h](../include/nt/map.h)

Parameters:

* self (`name_t*`): The map instance to release.

Releases a previously initialized map.


### *_count

Include: [nt/map.h](../include/nt/map.h)

Parameters:

* self (`name_t const*`): An initialized map instance.

Return:

* (`size_t`): The number of keys in the map.


### *_reserve

Include: [nt/map.h](../include/nt/map.h)

Parameters:

* self (`name_t*`): An initialized map instance.
* amount (`size_t`): Number of key value pairs to reserve space for.

Reserves space for the given number of key value pairs. If there is already enough space available, the function does
nothing.


### *_add

Include: [nt/map.h](../include/nt/map.h)

Parameters:

* self (`name_t*`): An initialized map instance.
* key (`key_type`): The key to add.
* value (`value_type`): The value to add.

Return:

* (`bool`) `true` if the key did not already exist in the map. If the key already exists, the value will not be added.

Adds a given key and value to the map.


### *_add_ref

Include: [nt/map.h](../include/nt/map.h)

Parameters:

* self (`name_t*`): An initialized map instance.
* key (`key_type const*`): A pointer to the key to add.
* value (`value_type const*`): A pointer to the value to add.

Return:

* (`bool`) `true` if the key did not already exist in the map. If the key already exists, the value will not be added.

Adds a given key and value to the map.


### *_add_multi

Include: [nt/map.h](../include/nt/map.h)

Parameters:

* self (`name_t*`): An initialized map instance.
* key (`key_type`): The key to add.
* value (`value_type`): The value to add.

Adds a given key and value to the map. If the key already exists, a new key-value pair is inserted.
Use the iterator to query all values associated to a given key.


### *_add_multi_ref

Include: [nt/map.h](../include/nt/map.h)

Parameters:

* self (`name_t*`): An initialized map instance.
* key (`key_type const*`): A pointer to the key to add.
* value (`value_type const*`): A pointer to the value to add.

Adds a given key and value to the map. If the key already exists, a new key-value pair is inserted.
Use the iterator to query all values associated to a given key.


### *_put

Include: [nt/map.h](../include/nt/map.h)

Parameters:

* self (`name_t*`): An initialized map instance.
* key (`key_type`): The key to add.
* value (`value_type`): The value to add.

Adds a given key and value to the map. If the key already exists, one of the connected values will be overwritten.


### *_put_ref

Include: [nt/map.h](../include/nt/map.h)

Parameters:

* self (`name_t*`): An initialized map instance.
* key (`key_type const*`): A pointer to the key to add.
* value (`value_type const*`): A pointer to the value to add.

Adds a given key and value to the map. If the key already exists, one of the connected values will be overwritten.


### *_remove

Include: [nt/map.h](../include/nt/map.h)

Parameters:

* self (`name_t*`): An initialized map instance.
* key (`key_type`): A key that should be removed.

Return:

* (`bool`) `true` if the key existed in the map and was removed.

Removes a given key and one of the associated values from the map.


### *_remove_ref

Include: [nt/map.h](../include/nt/map.h)

Parameters:

* self (`name_t*`): An initialized map instance.
* key (`key_type const*`): A pointer to the key that should be removed.

Return:

* (`bool`) `true` if the key existed in the map and was removed.

Removes a given key and one of the associated values from the map.


### *_clear

Include: [nt/map.h](../include/nt/map.h)

Parameters:

* self (`name_t*`): An initialized map instance.

Removes all key value pairs from the map.


### *_contains

Include: [nt/map.h](../include/nt/map.h)

Parameters:

* self (`name_t const*`): An initialized map instance.
* key (`key_type`): The key to check.

Return:

* (`bool`) `true` if the key exists in the map.

Checks whether a given key exists in the map.


### *_contains_ref

Include: [nt/map.h](../include/nt/map.h)

Parameters:

* self (`name_t const*`): An initialized map instance.
* key (`key_type const*`): A pointer to the key to check.

Return:

* (`bool`) `true` if the key exists in the map.

Checks whether a given key exists in the map.


### *_get

Include: [nt/map.h](../include/nt/map.h)

Parameters:

* self (`name_t const*`): An initialized map instance.
* key (`key_type`): A key.

Return:

* (`value_type const*`) A pointer to one of the values associated with the passed key or `NULL`.

Searches for a key inside the map and returns one of the connected values.


### *_get_ref

Include: [nt/map.h](../include/nt/map.h)

Parameters:

* self (`name_t const*`): An initialized map instance.
* key (`key_type const*`): A pointer to a key.

Return:

* (`value_type const*`) A pointer to one of the values associated with the passed key or `NULL`.

Searches for a key inside the map and returns one of the connected values.


### *_iterate

Include: [nt/map.h](../include/nt/map.h)

Parameters:

* self (`name_t const*`): An initialized map instance.
* key (`key_type const*`): A pointer to a key or `NULL`.

Return:

* (`name_iterator_t`) An iterator that can be used to list keys and values.

Returns an iterator that can be used to iterate over the key value pairs inside the map. If `key` is set, only key value
pairs with this key will be returned by the iterator.

**Remark:** The key is needed through out the whole iteration. So make sure that the optional `key` value remains valid
as long as the iterator is used.


### *_iterator_next

Include: [nt/map.h](../include/nt/map.h)

Parameters:

* self (`name_iterator_t*`): A map iterator instance.

Return:

* (`name_pair_t`) A struct containing the next key value pair. If no more pairs were found, the struct's fields are set
  to `NULL`.

Advances the iterator to the next key value pair and returns it.


## Queue

Queue types are defined with the `NT_QUEUE(name, value_type)` macro.

Example:

```
#include "nt/queue.h"

NT_QUEUE(int_queue, int)

int main() {
  int_queue_t queue = int_queue_new();
  int_queue_add(&queue, 2);
  int i = int_queue_take(&queue);
  int_queue_free(&queue);
  return 0;
}
```


### *_new

Include: [nt/queue.h](../include/nt/queue.h)

Return:

* (`name_t`) A new queue instance with 0 values.

Creates a new queue instance and returns it.


### *_free

Include: [nt/queue.h](../include/nt/queue.h)

Parameters:

* self (`name_t*`): The queue instance to release.

Releases a previously initialized queue.


### *_capacity

Include: [nt/queue.h](../include/nt/queue.h)

Parameters:

* self (`name_t const*`): The queue instance.

Return:

* (`size_t`) The capacity of the given queue instance.


### *_count

Include: [nt/queue.h](../include/nt/queue.h)

Parameters:

* self (`name_t const*`): The queue instance.

Return:

* (`size_t`) The number of values currently stored in the queue instance.


### *_reserve

Include: [nt/queue.h](../include/nt/queue.h)

Parameters:

* self (`name_t*`): The queue instance.
* amount (`size_t`): The number of values the function should reserve space for.

Reserves space for the given number of values. If the space is already available, the queue remains unchanged.


### *_add

Include: [nt/queue.h](../include/nt/queue.h)

Parameters:

* self (`name_t*`): The queue instance.
* value (`value_type`): The value to add.

Adds a value to the end of the queue.


### *_add_ref

Include: [nt/queue.h](../include/nt/queue.h)

Parameters:

* self (`name_t*`): The queue instance.
* value (`value_type const*`): A pointer to the value to add.

Adds a value to the end of the queue.


### *_peek

Include: [nt/queue.h](../include/nt/queue.h)

Parameters:

* self (`name_t const*`): The queue instance.

Return:

* (`value_type`) The value at the beginning of the queue. If no values exist in the queue, a panic is raised.


### *_peek_ref

Include: [nt/queue.h](../include/nt/queue.h)

Parameters:

* self (`name_t const*`): The queue instance.

Return:

* (`value_type const*`) A pointer to the value at the beginning of the queue. If no values exist in the queue, a panic
  is raised.


### *_take

Include: [nt/queue.h](../include/nt/queue.h)

Parameters:

* self (`name_t const*`): The queue instance.

Return:

* (`value_type`) The value at the beginning of the queue.

Removes the first value in the queue and returns it. If no values exist in the queue, a panic is raised.


### *_take_ref

Include: [nt/queue.h](../include/nt/queue.h)

Parameters:

* self (`name_t const*`): The queue instance.
* dst (`value_type*`): Pointer to the memory where the value should be stored.

Removes the first value in the queue and stores it in the memory pointed by `dst`. If no values exist in the queue, a
panic is raised.


### *_clear

Include: [nt/queue.h](../include/nt/queue.h)

Parameters:

* self (`name_t*`): The queue instance.

Removes all values from the given queue.


### *_iterate

Include: [nt/queue.h](../include/nt/queue.h)

Parameters:

* self (`name_t const*`): The queue instance.

Return:

* (`name_iterator_t`) An iterator that can be used to iterate over values in the queue. Modifications on the queue
  invalidate any existing iterator.


### *_iterator_next

Include: [nt/queue.h](../include/nt/queue.h)

Parameters:

* self (`name_iterator_t*`): A queue iterator instance.

Return:

* (`value_type const*`) A pointer to the next value or `NULL` if no more values exist.

Advances the iterator to the next value and returns it.
