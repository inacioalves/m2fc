# ArrayList – Do gerenciamento manual à abstração estrutural em C

## A evolução da manipulação de arrays em C

A manipulação de arrays dinâmicos em C tradicionalmente exige que o programador gerencie explicitamente três componentes em cada chamada de função: o **ponteiro para o array**, o **tamanho atual** e a **capacidade máxima**.

### O problema da abordagem manual

Imagine uma função simples para adicionar um elemento. Na abordagem manual, o programador é responsável por toda a logística:

```c
// Abordagem Manual (Propensa a Erros)
int* array_add(int *array, size_t *size, size_t *capacity, int value);
```

Essa abordagem, embora funcional, é **frágil** e **repetitiva**. O programador precisa se lembrar de:

1.  Verificar se o `size` atingiu o `capacity`.
2.  Chamar `realloc` para dobrar a capacidade.
3.  Atualizar o ponteiro do array (se `realloc` o moveu).
4.  Atualizar o valor do `size` e do `capacity`.
5.  Tratar falhas de alocação em cada etapa.

A complexidade de gerenciar esses detalhes em cada função leva a erros fáceis de esquecer, como *memory leaks* ou corrupção de ponteiros.

### A solução estrutural: Encapsulamento

A solução é o **encapsulamento**. Em vez de passar os três componentes separadamente, agrupamos todos os metadados em uma única estrutura de controle.

O `IntArrayList` é essa estrutura, que atua como um objeto opaco para o programador:

```c
// arrayList.c (Estrutura Interna)
struct IntArrayList
{
  size_t count;    // O tamanho atual (o que o usuário vê)
  size_t capacity; // O tamanho alocado (o que o sistema vê)
  int *items;      // O ponteiro para os dados
};
```

Ao passar apenas o ponteiro para `IntArrayList` (`IntArrayList *list`) para as funções, o programador se liberta da gestão de metadados, que é movida para dentro da biblioteca.

### Inspiração no design de coleções modernas (C#)

Essa abordagem de encapsulamento e abstração é a base de frameworks de coleções em linguagens de alto nível, como o `List<T>` do C#. A ideia central é: **o programador deve se preocupar com o que fazer (adicionar um item), e não com como fazer (gerenciar `realloc` e ponteiros)**.

| Característica | C (Abordagem Estruturada) | C# (`List<T>`) |
| :--- | :--- | :--- |
| **Objeto Principal** | `IntArrayList *` | Instância de `List<int>` |
| **Adicionar** | `intArrayList_add(list, value)` | `list.Add(value)` |
| **Metadados** | `count`, `capacity` (Ocultos) | `Count`, `Capacity` (Propriedades) |

### Detalhamento das funções e exemplos

A interface do `IntArrayList` é projetada para ser intuitiva e segura.

#### Acesso seguro com `Option`

A segurança no acesso é garantida pelo uso do tipo `OptionInt`, uma técnica inspirada em linguagens funcionais como Haskell e F#, com seus respectivos tipos *Maybe* e *Option*.

**O Contexto Funcional:** Em linguagens funcionais, um tipo *Option* pode conter uma informação (*Just/Some*) ou nada (*Nothing/None*). Isso obriga o programador a verificar se o retorno contém um valor antes de usá-lo.

A `struct` que define o `OptionInt` é a seguinte:

```c
typedef struct
{
  bool is_some;
  int value;
} OptionInt;
```

Ao retornar um `OptionInt` em vez de um ponteiro ou um valor que pode ser inválido, a função `intArrayList_get` força o programador a verificar se o acesso foi bem-sucedido (`is_some`) antes de usar o valor, evitando o acesso a índices fora dos limites do array.

```c
// Exemplo de Uso: Acesso Seguro
OptionInt o1 = intArrayList_get(list, 1);

if (o1.is_some) {
  printf("Valor encontrado: %d\n", o1.value);
} else {
  printf("Índice inválido: nenhum valor.\n");
}
```

#### Funções funcionais (Map, Filter, Fold)

A inclusão de funções de ordem superior eleva o nível de abstração, permitindo operações complexas sem *loops* explícitos.

| Função | Ideia por Trás da Abordagem |
| :--- | :--- |
| `intArrayList_map` | **Transformação:** Cria uma **nova** lista aplicando uma função a cada elemento. |
| `intArrayList_filter` | **Seleção:** Cria uma **nova** lista contendo apenas os elementos que satisfazem uma condição. |
| `intArrayList_fold` | **Redução:** Reduz a lista a um único valor (soma, produto, etc.). |

## Conclusão e próximos passos

O `IntArrayList` estruturado resolve o problema da complexidade e da segurança do código, mas ainda está sujeito ao gargalo de performance do gerenciamento de memória tradicional.

A função `intArrayList_add` e, principalmente, a função interna de crescimento (`grow`), dependem de chamadas caras a `realloc`, que geram **Heap Churn** e fragmentação de memória.

**Como podemos manter a segurança e a abstração do `IntArrayList` enquanto eliminamos o custo de performance de cada `malloc` e `realloc`?**

No próximo artigo, exploraremos a **Arena Allocation**, uma técnica que transforma a alocação de memória de uma operação de alto custo em um simples incremento de ponteiro **O(1)**, e demonstraremos como ela pode ser aplicada para otimizar o `IntArrayList`.

