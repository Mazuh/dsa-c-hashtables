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
 * @brief Experimental execution.
 */
int main()
{
    // TODO: hash of some given values
    char *str_0 = "";
    char *str_1 = "Hello, world!";
    char *str_2 = "Mazuh";

    printf("Hash of '%s': %lu\n", str_0, hash_str_djb2(str_0));
    printf("Hash of '%s': %lu\n", str_1, hash_str_djb2(str_1));
    printf("Hash of '%s': %lu\n", str_1, hash_str_djb2(str_1));
    printf("Hash of '%s': %lu\n", str_2, hash_str_djb2(str_2));

    // TODO: create set

    // TODO: add some values to set

    // TODO: remove a value from set

    // TODO: try to add a duplicated value to set

    // TODO: print set values

    printf("Hello, World!\n");
    return 0;
}
