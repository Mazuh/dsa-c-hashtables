# Data Structures & Algorithms: [C] Hash Tables

Implementation of a hash table and related structures
like [set](<https://en.wikipedia.org/wiki/Set_(abstract_data_type)>),
inspired by a talk of [Luciano Ramalho](https://www.linkedin.com/in/lucianoramalho/)
during [PyBR2023](https://2023.pythonbrasil.org.br/).

## Setting up

If you have `cc` (clang LLVM compiler) command available, just run the Makefile:

```sh
make
```

It will run an example by playing around with a set of weekdays.

## Example of usage

The API for the set data structure is somewhat similar
to [Java 8 HashSet](https://docs.oracle.com/javase/8/docs/api/java/util/HashSet.html),
but of course in structured programming.

And running this:

```c
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
printf("There are %lu weekdays:\n", hashstrset_size(weekdays_set));

HashStrSetValuesIterator *iterator = hashstrset_values_iterator();
while (hashstrset_values_iterator_seek(weekdays_set, iterator))
{
    printf("- %s\n", iterator->current_value);
}
hashstrset_values_iterator_free(iterator);

// bye
hashstrset_free(weekdays_set);
```

Will output this:

```
There are 5 weekdays:
- Monday
- Friday
- Wednesday
- Tuesday
- Thursday
```

## License

It's under the [MIT License](https://github.com/Mazuh/dsa-c-hashtables/blob/main/LICENSE).
