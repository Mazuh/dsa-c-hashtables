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
    size_t length;
    HashTableStrBucket buckets[];
} typedef HashStrSet;

/**
 * @brief HashStrSet constructor.
 *
 * @return HashStrSet instance, empty.
 */
HashStrSet *hashstrset_init()
{
    HashStrSet *set = malloc(sizeof(HashStrSet));
    set->cardinality = 0;
    set->length = 101; // convenient prime number
    for (size_t i = 0; i < set->length; i++)
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
 * @brief Adds the specified string to this set if it is not already present.
 *
 * @param set HashStrSet instance.
 * @param value String to be added to the set.
 */
void hashstrset_add(HashStrSet *set, char *value)
{
    unsigned long hashed = hash_str_djb2(value);
    unsigned long initial_index_attempt = hashed % set->length;

    for (size_t i = initial_index_attempt; i < set->length; i++)
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
    unsigned long initial_index_attempt = hashed % set->length;

    for (size_t i = initial_index_attempt; i < set->length; i++)
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
 * @brief Print useful info about the given HashStrSet.
 *
 * @param set HashStrSet instance.
 */
void hashstrset_debug(HashStrSet *set)
{
    printf("--------- HashStrSet -----------\n");
    printf("It has %lu buckets:\n", set->length);
    for (size_t i = 0; i < set->length; i++)
    {
        printf(set->buckets[i].value == NULL ? "." : "*");
    }

    printf("\n");

    for (size_t i = 0; i < set->length; i++)
    {
        if (set->buckets[i].hash != 0)
        {
            printf("%lu -> %s\n", set->buckets[i].hash, set->buckets[i].value);
        }
    }

    printf("It has %lu values.\n", hashstrset_size(set));

    printf("-------------------------\n");
}

/**
 * @brief Experimental execution.
 */
int main()
{
    // create set
    HashStrSet *weekdays = hashstrset_init();

    // add some values to set
    hashstrset_add(weekdays, "Monday");
    hashstrset_add(weekdays, "Tuesday");
    hashstrset_add(weekdays, "Wednesday");
    hashstrset_add(weekdays, "Thursday");
    hashstrset_add(weekdays, "Friday");
    hashstrset_add(weekdays, "Saturday");

    // remove a value from set
    hashstrset_remove(weekdays, "Saturday");

    // try to add a duplicated value to set
    hashstrset_add(weekdays, "Monday");
    hashstrset_add(weekdays, "Monday");
    hashstrset_add(weekdays, "Monday");

    // print set values
    hashstrset_debug(weekdays);

    // bye
    hashstrset_free(weekdays);

    return 0;
}
