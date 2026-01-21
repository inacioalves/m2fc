#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <stddef.h> /* size_t */

/*
 * Tipos de erro explícitos.
 * Não usamos errno nem códigos mágicos.
 */
typedef enum
{
  ARRAYLIST_OK = 0,
  ARRAYLIST_NULL,
  ARRAYLIST_OUT_OF_MEMORY,
  ARRAYLIST_OUT_OF_BOUNDS,
  ARRAYLIST_INVALID_CAPACITY
} ArrayListError;

/*
 * Forward declaration.
 * O usuário não conhece a estrutura interna.
 */
typedef struct IntArrayList IntArrayList;

/*
 * Result para operações que retornam um inteiro.
 * value só é válido quando error == ARRAYLIST_OK.
 */
typedef struct
{
  ArrayListError error;
  int value;
} ResultInt;

/*
 * Result para operações que retornam uma lista.
 * list só é válida quando error == ARRAYLIST_OK.
 */
typedef struct
{
  ArrayListError error;
  IntArrayList *list;
} ResultList;

/* ========= API pública ========= */

/*
 * Cria uma nova lista com capacidade inicial > 0.
 * A lista começa vazia (count == 0).
 */
ResultList intArrayList_create(size_t initial_capacity);

/*
 * Libera todos os recursos da lista.
 * É seguro chamar apenas uma vez.
 */
ArrayListError intArrayList_destroy(IntArrayList *list);

/*
 * Retorna a quantidade de elementos armazenados.
 */
ResultInt intArrayList_count(const IntArrayList *list);

/*
 * Retorna a capacidade atual (tamanho do buffer).
 */
ResultInt intArrayList_capacity(const IntArrayList *list);

/*
 * Adiciona um elemento ao final da lista.
 * Aumenta a capacidade automaticamente se necessário.
 */
ResultList intArrayList_add(IntArrayList *list, int value);

/*
 * Retorna o elemento no índice informado.
 * Índices válidos: [0, count).
 */
ResultInt intArrayList_get(const IntArrayList *list, size_t index);

#endif /* ARRAYLIST_H */
