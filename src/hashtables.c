/**
 * @brief Studying hash table data structures. They're not thread-safe and not production-ready.
 */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

/**
 * @brief Internal calc hash value of a raw string using djb2 algorithm.
 *
 * @see https://theartincode.stanis.me/008-djb2/
 */
unsigned long hash_str_djb2(char *raw_string)
{
    unsigned long hash_value = 5381;

    int current_char;
    while ((current_char = *raw_string++))
    {
        hash_value = ((hash_value << 5) + hash_value) + current_char;
    }

    return hash_value;
};

/**
 * @brief Internal data structure, each bucket is a row in the hash table powering the HashStrSet.
 */
struct HashTableStrBucket
{
    unsigned long hash;
    char *value;
} typedef HashTableStrBucket;

/**
 * @brief Opaque. It implements the set abstract data structure, backed by a hash table of strings.
 */
struct HashStrSet
{
    size_t cardinality;
    size_t buckets_qtt;
    HashTableStrBucket buckets[];
} typedef HashStrSet;

/**
 * @brief Helper to iterate over the unique values of a set.
 */
struct HashStrSetValuesIterator
{
    char *current_value;
    size_t _current_index;
} typedef HashStrSetValuesIterator;

/**
 * @brief HashStrSet constructor.
 *
 * @return HashStrSet instance, empty.
 */
HashStrSet *hashstrset_init()
{
    HashStrSet *set = malloc(sizeof(HashStrSet));
    set->cardinality = 0;
    set->buckets_qtt = 101; // convenient and arbitrary prime number
    for (size_t i = 0; i < set->buckets_qtt; i++)
    {
        set->buckets[i].hash = 0;
        set->buckets[i].value = NULL;
    }
    return set;
}

/**
 * @brief HashStrSet destructor.
 *
 * @param set HashStrSet to be freed from memory.
 */
void hashstrset_free(HashStrSet *set)
{
    free(set);
}

/**
 * @brief Set cardinality.
 *
 * @param set HashStrSet instance.
 * @return size_t Number of elements in this set.
 */
size_t hashstrset_size(HashStrSet *set)
{
    return set->cardinality;
}

/**
 * @brief Adds the specified string to this set if it is not already present.
 *
 * @param set HashStrSet instance.
 * @param value String to be added to the set.
 */
void hashstrset_add(HashStrSet *set, char *value)
{
    bool has_to_scale = set->cardinality >= set->buckets_qtt / 2; // arbitrary load threshold
    if (has_to_scale)
    {
        size_t scaled_buckets_qtt = set->buckets_qtt * 2; // arbitrary scaling factor

        for (size_t i = set->buckets_qtt; i < scaled_buckets_qtt; i++)
        {
            set->buckets[i].hash = 0;
            set->buckets[i].value = NULL;
        }

        set->buckets_qtt = scaled_buckets_qtt;
    }

    unsigned long hashed = hash_str_djb2(value);
    unsigned long attempt_begin = hashed % set->buckets_qtt;
    for (size_t i = attempt_begin; i < set->buckets_qtt; i++)
    {
        bool is_empty_bucket = set->buckets[i].hash == 0;
        if (is_empty_bucket)
        {
            set->buckets[i].hash = hashed;
            set->buckets[i].value = value;
            set->cardinality++;
            return;
        }

        bool is_already_containing = set->buckets[i].hash == hashed && strcmp(set->buckets[i].value, value) == 0;
        if (is_already_containing)
        {
            return;
        }

        fprintf(stderr, "HashStrSet: Unexpected hash collision when adding '%s'.\n", value);
        exit(1);
    }
}

/**
 * @brief Removes the specified element from this set if it is present.
 *
 * @param set HashStrSet instance.
 * @param value String to be removed from the set.
 * @return true (1) if it worked, otherwise false (0).
 */
bool hashstrset_remove(HashStrSet *set, char *value)
{
    unsigned long hashed = hash_str_djb2(value);
    unsigned long attempt_begin = hashed % set->buckets_qtt;
    for (size_t i = attempt_begin; i < set->buckets_qtt; i++)
    {
        if (set->buckets[i].hash == hashed && strcmp(set->buckets[i].value, value) == 0)
        {
            set->buckets[i].hash = 0;
            set->buckets[i].value = NULL;
            set->cardinality--;
            return true;
        }
    }

    return false;
}

/**
 * @brief Makes a helper to iterate over the unique values of a set.
 *
 * Keep in mind that seeking all elements is O(n) operation, even if the set is empty,
 * considering n as the total amount of buckets in the internal hash table, so it's always
 * more than the amount of filled buckets.
 *
 * @return HashStrSetValuesIterator*
 */
HashStrSetValuesIterator *hashstrset_values_iterator()
{
    HashStrSetValuesIterator *iterator = malloc(sizeof(HashStrSetValuesIterator));
    iterator->current_value = NULL;
    iterator->_current_index = 0;
    return iterator;
}

/**
 * @brief HashStrSetValuesIterator destructor.
 *
 * @param iterator HashStrSetValuesIterator instance.
 */
void hashstrset_values_iterator_free(HashStrSetValuesIterator *iterator)
{
    free(iterator);
}

/**
 * @brief Seeks the next value in the set and assign it to `iterator->current_value`.
 *
 * Remember the warning at `hashstrset_values_iterator` documentation.
 *
 * @param set HashStrSet instance.
 * @param iterator HashStrSetValuesIterator instance.
 * @return true if it managed to seek a next value, otherwise false (it reached the end).
 */
bool hashstrset_values_iterator_seek(HashStrSet *set, HashStrSetValuesIterator *iterator)
{
    bool is_first_iteration = iterator->current_value == NULL;
    size_t begin_index = is_first_iteration ? 0 : iterator->_current_index + 1;
    for (size_t i = begin_index; i < set->buckets_qtt; i++)
    {
        bool has_found_filled = set->buckets[i].hash != 0;
        if (has_found_filled)
        {
            iterator->current_value = set->buckets[i].value;
            iterator->_current_index = i;
            return true;
        }
    }

    iterator->current_value = NULL;
    iterator->_current_index = 0;
    return false;
}

/**
 * @brief Experimental execution.
 */
int main()
{
    // create set
    HashStrSet *weekdays_set = hashstrset_init();

    // add some values to set
    hashstrset_add(weekdays_set, "Monday");
    hashstrset_add(weekdays_set, "Tuesday");
    hashstrset_add(weekdays_set, "Wednesday");
    hashstrset_add(weekdays_set, "Thursday");
    hashstrset_add(weekdays_set, "Friday");
    hashstrset_add(weekdays_set, "Saturday");

    // remove a value from set
    hashstrset_remove(weekdays_set, "Saturday");

    // try to add a duplicated value to set
    hashstrset_add(weekdays_set, "Monday");
    hashstrset_add(weekdays_set, "Monday");
    hashstrset_add(weekdays_set, "Monday");

    // print set values
    printf("There are %lu weeek days:\n", hashstrset_size(weekdays_set));

    HashStrSetValuesIterator *iterator = hashstrset_values_iterator();
    while (hashstrset_values_iterator_seek(weekdays_set, iterator))
    {
        printf("- %s\n", iterator->current_value);
    }
    hashstrset_values_iterator_free(iterator);

    // bye
    hashstrset_free(weekdays_set);

    return 0;
}
