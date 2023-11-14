#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * @brief Calculate hash value of a raw string using djb2 algorithm.
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
 * @brief Opaque data structure of a hash table to store string elements.
 */
struct HashStrSet
{
    size_t length;
    HashTableStrBucket buckets[];
} typedef HashStrSet;

/**
 * @brief HashStrSet constructor.
 */
HashStrSet *hashstrset_init()
{
    HashStrSet *set = malloc(sizeof(HashStrSet));
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
 * @param hashstrset HashStrSet to be released from memory.
 */
void hashstrset_free(HashStrSet *set)
{
    free(set);
}

/**
 * @brief Insert a string value into a HashStrSet.
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
        char is_empty_bucket = set->buckets[i].hash == 0;
        if (is_empty_bucket)
        {
            set->buckets[i].hash = hashed;
            set->buckets[i].value = value;
            return;
        }

        char is_hash_collision = set->buckets[i].hash == hashed;
        char is_same_value = strcmp(set->buckets[i].value, value) == 0;
        if (is_hash_collision && !is_same_value)
        {
            fprintf(stderr, "HashStrSet: Unexpected hash collision when adding '%s'.\n", value);
            exit(1); // awful, but somewhat expected to happen in a toy project like this
            return;
        }
    }
}

void hashstrset_debug(HashStrSet *set)
{
    printf("HashStrSet (having %lu buckets):\n", set->length);
    for (size_t i = 0; i < set->length; i++)
    {
        printf(set->buckets[i].value == NULL ? "." : "*");
    }

    printf("\n");
    for (size_t i = 0; i < set->length; i++)
    {
        if (set->buckets[i].value != NULL)
        {
            printf("%lu -> %s\n", set->buckets[i].hash, set->buckets[i].value);
        }
    }
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

    // TODO: remove a value from set

    // TODO: try to add a duplicated value to set

    // print set values
    hashstrset_debug(weekdays);

    // bye
    hashstrset_free(weekdays);

    return 0;
}
