# Árvore Rubro-Negra (Red-Black Tree) — Seminário 04

Estrutura de Dados II · IFTM Campus Patrocínio · Prof. Júnio Moreira

**Grupo:** [Integrante 1], [Integrante 2], [Integrante 3]
**Videoaula:** [link do YouTube]

Implementação própria e completa da Árvore Rubro-Negra em C++17, com **nó sentinela NIL**,
inserção com *insert-fixup*, remoção com *transplante* e *delete-fixup*, validação das cinco
propriedades e a visão equivalente como Árvore 2-3-4. Também traz as resoluções dos 3 exercícios
da lista.

## Estrutura

```
rubro-negra-ed2/
├── include/
│   ├── comum.hpp            # cabeçalho padrão do grupo (includes, typedefs, macros)
│   ├── desenho.hpp          # desenha a árvore "em pé" com / e \
│   └── RedBlackTree.hpp     # interface da classe RedBlackTree
├── src/
│   ├── RedBlackTree.cpp     # rotações, insert-fixup, transplante, delete-fixup...
│   ├── demo_rb.cpp          # demonstração passo a passo (usada no vídeo)
│   └── exercicios/
│       ├── ex1_validar_rubro_negra.cpp
│       ├── ex2_insercao_fixup.cpp
│       └── ex3_remocao_fixup.cpp
├── tests/testes.cpp         # 27 verificações automáticas
├── exemplos/exN/            # entrada.txt e saida.txt de cada exercício
└── CMakeLists.txt
```

## As cinco propriedades

1. Todo nó é **rubro** ou **negro**.
2. A **raiz** é negra.
3. Toda folha (**NIL**) é negra.
4. Um nó rubro **não tem filho rubro**.
5. Todo caminho de um nó até as folhas NIL abaixo dele tem o **mesmo número de nós negros**
   (altura-negra).

Daí sai que `altura ≤ 2·log2(n+1)`: buscar, inserir e remover custam **O(log n)**.

## Por que o sentinela NIL?

Em vez de `nullptr`, todas as folhas e o pai da raiz apontam para um único nó `NIL`, sempre negro.
Assim o código pode ler `x->cor` e `x->pai` mesmo quando `x` é uma folha, sem testar ponteiro nulo.
Isso deixa o *delete-fixup* bem mais simples.

## Operações

| Operação | Ideia | Rotações |
|---|---|---|
| `inserir` | insere como BST, pinta de **rubro** e chama o `inserirFixup` | no máximo **2** |
| `inserirFixup` | caso 1: tio rubro, só recolore e sobe · caso 2: tio negro e triângulo, uma rotação vira o caso 3 · caso 3: tio negro e linha, recolore e rotaciona no avô | — |
| `remover` | usa o `transplante` e, se saiu um nó **negro**, chama o `removerFixup` | no máximo **3** |
| `removerFixup` | caso 1: irmão rubro · caso 2: irmão negro com filhos negros · caso 3: sobrinho de perto rubro · caso 4: sobrinho de longe rubro | — |
| `valida` | confere as 5 propriedades, a ordem de BST e os ponteiros `pai` | — |
| `niveis234` | mostra a árvore como Árvore 2-3-4 (negro + filhos rubros = 1 nó-B) | — |

## Como rodar

**CLion:** abra a pasta como projeto CMake. Escolha `demo_rb`, `testes` ou um exercício no seletor
de configurações e clique em Run. Para os exercícios, em *Edit Configurations → Redirect input from*,
aponte para `exemplos/exN/entrada.txt`.

**Terminal:**

```bash
cmake -S . -B build && cmake --build build
./build/demo_rb
./build/testes                       # 27/27 verificacoes passaram
./build/ex1_validar_rubro_negra < exemplos/ex1/entrada.txt
```

## Exercícios

| # | Arquivo | Baseado em | Tema |
|---|---|---|---|
| 1 | `ex1_validar_rubro_negra.cpp` | PAT 1135 — *Is It A Red-Black Tree* | as 5 propriedades |
| 2 | `ex2_insercao_fixup.cpp` | HackerRank *Self Balancing Tree* + LeetCode 102 | insert-fixup |
| 3 | `ex3_remocao_fixup.cpp` | LeetCode 450 + beecrowd 1201 | transplante e delete-fixup |

Referência principal: CORMEN et al. *Algoritmos: Teoria e Prática*, cap. 13.
