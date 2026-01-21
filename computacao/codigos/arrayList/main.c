#include <stdio.h>
#include "arrayList.h"

int main(void)
{
    ResultList result = intArrayList_create(4);
    if (result.error != ARRAYLIST_OK) {
        printf("Error creating array list: %d\n", result.error);
        return 1;
    }

    IntArrayList *list = result.list;

    ResultInt countResult = intArrayList_count(list);
    if (countResult.error != ARRAYLIST_OK) {
        printf("Error getting count: %d\n", countResult.error);
        intArrayList_destroy(list);
        return 1;
    }

    printf("Initial count: %d\n", countResult.value);

    for(int i = 0; i < 20; i++) {
      int value = 2 * i;
      ResultList addResult = intArrayList_add(list, value);
      if (addResult.error != ARRAYLIST_OK)
      {
        printf("Error adding value %d: %d\n", value, addResult.error);
        intArrayList_destroy(list);
        return 1;
      }
    }
    size_t count = intArrayList_count(list).value;
    printf("Count after adds: %ld\n", count);
    
    for(int i = 0; i < count; i++) {
      ResultInt getResult = intArrayList_get(list, i);
      if (getResult.error != ARRAYLIST_OK) {
        printf("Error getting value at index %d: %d\n", i, getResult.error);
        intArrayList_destroy(list);
        return 1;
      }
      printf("Value at index %d: %d\n", i, getResult.value);
    }

    ArrayListError destroyError = intArrayList_destroy(list);
    if (destroyError != ARRAYLIST_OK) {
        printf("Error destroying array list: %d\n", destroyError);
        return 1;
    }

    return 0;
}
