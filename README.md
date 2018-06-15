# Calculo do Page Rank
# Funções importantes prontas:
# 1) float* normalizaVetor(float* vetor,int numero_vertices)
Irá normalizar o `vetor` passado como parâmetro
e retornará o mesmo normalizado.

# 2) Vertice* ordenaBubbleSort(Vertice *vertice, int tamanho)

Nesta função, o vetor de vértices passado como parâmetro é ordenado pelo método de ordenação BubbleSort. Também é passado como parametro o tamanho do vetor `vertice`.

# 3) imprimeTopKPageRank(Grafo *grafo,int k)*

Chama a função `ordenaBubbleSort` para ordenar os PageRanks do
maior para o menor e, logo após, imprime os top `k` vértices de acordo com o PageRank

# 4) void obtemVertices(Grafo* grafo,char arquivo[])

Lê o arquivo .txt passado como parâmetro e grava os
vértices lidos (sem repetições) no Grafo
# 5)void criaMatrizAdjacencia(Grafo *grafo,char arquivo[])*

Lê o arquivo .txt novamente e cria a matriz de adjacência
a partir das posições de cada vértice do vetor de Vértice `vertices[]` 
que está no grafo passado como paramtero.
