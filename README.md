# StudyGraphTheory

## Desenvolvido por:
> DANIEL FAGUNDES - 100%

### Objetivo do trabalho:
Desenvolver um Tipo Abstrato de Dados - TAD ou uma Classe que represente grafos simples, orientados e não
orientados, ponderados e não ponderados (nos vértices e arestas) e implemente o conjunto de funcionalidades
apresentadas a seguir,

### Funcionalidades:
> Todas 100% implementadas

• O programa deve apresentar em tela a saída para as seguintes funcionalidades definidas nas saídas
conforme os parâmetros:

a) Parâmetro: um ID de um vértice de um grafo direcionado; (0,5 pontos)
```
Saída: o fecho transitivo direto deste vértice.
```
b) Parâmetro: um ID de um vértice de um grafo direcionado; (0,5 pontos)
```
Saída: o fecho transitivo indireto deste vértice.
```
c) Parâmetro: o ID de vértices do grafo (1,0 pontos)
```
Saída: o coeficiente de agrupamento local do vértice
```
d) Parâmetro: um grafo; (3 pontos)
```
Saída: o coeficiente de agrupamento médio do grafo;
```
e) Parâmetro: dois IDs de vértices do grafo; (1 ponto)
```
Saída: o caminho mínimo entre estes dois vértices usando algoritmo de Djkstra;
```
f) Parâmetro: dois IDs de vértices do grafo; (1 ponto)
```
Saída: o caminho mínimo entre estes dois vértices usando algoritmo de Floyd;
```
g) Parâmetro: um subconjunto X de vértices de um grafo; (1 ponto)
```
Saída: uma Árvore Geradora Mínima sobre o subgrafo vértice-induzido por X usando o algoritmo de Prim;
```
h) Parâmetro: um subconjunto X de vértices do grafo; (1 ponto)
```
Saída: uma Árvore Geradora Mínima sobre o subgrafo vértice-induzido por X usando o algoritmo de
Kruskal;
```
i) Parâmetro: um ID de vértice; (1 ponto)
```
Saída: a árvore dada pela ordem de caminhamento em profundidade a partir de nó dado parâmetro,
destacando as arestas de retorno;
```

> Os itens g, h e i deverão ter saídas dadas na linguagem “dot” para geração de grafos para no software
Graphviz

## BUILD 1

PRE REQUISITOS:
> CMAKE

### PARA EXECUTAR

Crie uma pasta chamada Cmake e dentro da pasta execute:
> cmake -DCMAKE_BUILD_TYPE=Release <path_CMakeLists.txt>
```
cmake -DCMAKE_BUILD_TYPE=Release ../.
```
```
make && ./execGrupo5 <arquivo_entrada> <arquivo_saida> <Opc_Direc> <Opc_Peso_Aresta> <Opc_Peso_Nos>
```

### BUILD 2

Comando para compilar:

```
g++ -O3 *.cpp -o execGrupoX -g
```

```
./execGrupo5 <arquivo_entrada> <arquivo_saida> <Opc_Direc> <Opc_Peso_Aresta> <Opc_Peso_Nos>
```
Exemplo:
```
./execGrupo5 entrada.txt saida.dot 0 1 0
```
