#include "arrayList.h"
#include <stdlib.h> /* malloc, free, realloc */

/* ========= definição interna =========
 * Invariante:
 *   - 0 <= count <= capacity
 *   - items aponta para um buffer válido de size capacity
 */
struct IntArrayList
{
  size_t count;
  size_t capacity;
  int *items;
};

/* ========= helpers privados ========= */

/* Centraliza a criação de ResultInt de erro */
static inline ResultInt resultInt_error(ArrayListError error)
{
  return (ResultInt){.error = error, .value = 0};
}

/* Centraliza a criação de ResultInt de sucesso */
static inline ResultInt resultInt_ok(int value)
{
  return (ResultInt){.error = ARRAYLIST_OK, .value = value};
}

/*
 * Aumenta a capacidade da lista.
 * Função privada: assume que a lista já existe.
 */
static ResultList intArrayList_increaseCapacity(IntArrayList *list)
{
  if (!list)
    return (ResultList){.error = ARRAYLIST_NULL, .list = NULL};

  size_t new_capacity = list->capacity * 2;
  int *new_items = realloc(list->items, new_capacity * sizeof *new_items);

  /*
   * realloc falhou: o buffer original continua válido.
   * NÃO modificamos o estado da lista.
   */
  if (!new_items)
    return (ResultList){.error = ARRAYLIST_OUT_OF_MEMORY, .list = NULL};

  list->items = new_items;
  list->capacity = new_capacity;

  return (ResultList){.error = ARRAYLIST_OK, .list = list};
}

/* ========= implementação pública ========= */

ResultList intArrayList_create(size_t initial_capacity)
{
  if (initial_capacity == 0)
    return (ResultList){.error = ARRAYLIST_INVALID_CAPACITY, .list = NULL};

  IntArrayList *list = malloc(sizeof *list);
  if (!list)
    return (ResultList){.error = ARRAYLIST_OUT_OF_MEMORY, .list = NULL};

  list->items = malloc(initial_capacity * sizeof *list->items);
  if (!list->items)
  {
    free(list);
    return (ResultList){.error = ARRAYLIST_OUT_OF_MEMORY, .list = NULL};
  }

  list->count = 0;
  list->capacity = initial_capacity;

  return (ResultList){.error = ARRAYLIST_OK, .list = list};
}

ArrayListError intArrayList_destroy(IntArrayList *list)
{
  if (!list)
    return ARRAYLIST_NULL;

  free(list->items);
  free(list);
  return ARRAYLIST_OK;
}

ResultInt intArrayList_count(const IntArrayList *list)
{
  if (!list)
    return resultInt_error(ARRAYLIST_NULL);

  return resultInt_ok((int)list->count);
}

ResultInt intArrayList_capacity(const IntArrayList *list)
{
  if (!list)
    return resultInt_error(ARRAYLIST_NULL);

  return resultInt_ok((int)list->capacity);
}

ResultList intArrayList_add(IntArrayList *list, int value)
{
  if (!list)
    return (ResultList){.error = ARRAYLIST_NULL, .list = NULL};

  if (list->count >= list->capacity)
  {
    ResultList grow = intArrayList_increaseCapacity(list);
    if (grow.error != ARRAYLIST_OK)
      return grow;
  }

  list->items[list->count++] = value;
  return (ResultList){.error = ARRAYLIST_OK, .list = list};
}

ResultInt intArrayList_get(const IntArrayList *list, size_t index)
{
  if (!list)
    return resultInt_error(ARRAYLIST_NULL);

  if (index >= list->count)
    return resultInt_error(ARRAYLIST_OUT_OF_BOUNDS);

  return resultInt_ok(list->items[index]);
}
