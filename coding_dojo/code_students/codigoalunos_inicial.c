#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct
{
  char *nome;
  float score;
}Vertice;

typedef struct
{
  Vertice* vertices;
  int tamanho;
  int ** matrizadj;
}Grafo;

int ** Aloca(int lin, int col){
  /*Aloca dinamicamente o vetor de vértices*/
  int i,j;
  int **m = (int**)malloc(lin * sizeof(int*));
  for (i = 0; i < lin; i++)
  {
    m[i] = (int*) malloc(col * sizeof(int));
    for (j = 0; j < col; j++)
    {
      m[i][j] = 0;
    }
  }
  return m;
}

float* normalizaVetor(float* vetor,int numero_vertices)
{
  /* Função que iŕa normalizar o vetor passado */
  float soma=0;
  int i;
  //somatorio do conteudo do vetor
  for(i=0;i<numero_vertices;i++)
  {
    soma += vetor[i];
  }
  //divide o conteudo de cada posicao pelo somatorio
  for(i=0;i<numero_vertices;i++)
  {
    vetor[i] = vetor[i]/soma;
  }
  return vetor;
}

static int cmp(const void *p1, const void *p2){
  /*Função comparadora para o Quick Sort*/
  /*Fonte: https://stackoverflow.com/questions/15824966/trouble-using-bsearch-with-an-array-of-strings*/
  return strcmp(* (char * const *) p1, * (char * const *) p2);
}

int myStrCmp(const void *s1, const void *s2) {
  /*Função comparadora para a pesquisa binária*/
  /*Fonte: https://stackoverflow.com/questions/15824966/trouble-using-bsearch-with-an-array-of-strings*/
  const char *key = s1;
  const char * const *arg = s2;
  //  printf("myStrCmp: s1(%p): %s, s2(%p): %s\n", s1, key, s2, *arg); imprime as ocorrencias dos repetidos
  return strcmp(key, *arg);
}

Vertice* ordenaBubbleSort(Vertice *vertice, int tamanho)
{
  int i,j;
  float aux;
  char *aux2;
  Vertice *verticeaux;
  verticeaux = malloc(sizeof(Vertice)*tamanho);
  for( i = 0; i <tamanho; i++ )
  {
    verticeaux[i] = vertice[i];
  }

  for( i = 0; i <tamanho; i++ )
  {
    for( j = i + 1; j < tamanho; j++ )
    {
      if ( verticeaux[i].score < verticeaux[j].score) //coloca o maior na primeira posicao
      {
        aux = verticeaux[i].score;
        verticeaux[i].score= verticeaux[j].score;;
        verticeaux[j].score = aux;

        aux2 = verticeaux[i].nome;
        verticeaux[i].nome = verticeaux[j].nome;
        verticeaux[j].nome = aux2;
      }
    }
  }
  return verticeaux;
}

void imprimeTopKPageRank(Grafo *grafo,int k)
{
  Vertice *aux;
  int i;
  aux = ordenaBubbleSort(grafo->vertices,grafo->tamanho);
  printf("Top %d:\n",k);
  for(i=0;i<k;i++)
  {
    printf("Posicao %d. Vertice %s - Score: %f\n",i+1,aux[i].nome,aux[i].score);
  }
}

void obtemVertices(Grafo* grafo,char arquivo[])
{
  /* Função que retorna um vetor de string
  que irá obter os vértices do arquivo passado
  por parâmetro*/
  Vertice *vertices;
  char palavra[100];
  char *palavra1;
  char *palavra2;
  char * aux;
  int i,k,j=0;
  FILE *arq;
  arq = fopen(arquivo, "r"); /*abre arquivo em modo leitura*/
  if(arq == NULL){
    printf("Erro, nao foi possivel abrir o arquivo\n");
    return;
  }

  /* Declaração de uma matriz de strings
  que armazena um vértice V em cada posição*/
  char **nome_vertices;
  nome_vertices = Aloca(1000, 100); /*suporta até
  100 caracteres por vértice e 1000 vértices*/

  /*Declaração de vetor auxiliar */
  char vertices_auxiliar[2][100];

  /* Leitura: Se A ganhou de B, há uma aresta de B para A*/

  while(!feof(arq)) /*Enquanto não encontrar o fim do arquivo..*/
  {
    fgets(palavra,100, arq);  /*Pega a linha inteira
    e armazena na string auxiliar nome*/

    /*Para dividir a string utilizei
    a função strtok da biblioteca string.h
    utilizando a vírgula como primeiro
    delimitador e o NULL como segundo delimitador*/
    aux = strtok (palavra,",");
    palavra1 = aux;
    while (aux != NULL)
    {
      palavra2 = aux;
      aux = strtok (NULL, ",");
    }
    /*Retira a parte null da segunda palavra decorrente do strtok*/
    palavra2= strtok(palavra2,"\n");
    /*Grava as palavras em um vetor auxiliar*/
    sprintf(vertices_auxiliar[0], "%s" ,palavra1);
    sprintf(vertices_auxiliar[1], "%s" ,palavra2);

    for (k=0;k<2;k++)
    {
      /*Procura a palavra no vetor de vértices nome_vertices*/
      char * found = (char *) bsearch(vertices_auxiliar[k], nome_vertices, j, sizeof(char *), myStrCmp);
      if (found==NULL) /*Se ela não existir, grava no vetor*/
      {
        sprintf(nome_vertices[j], "%s" ,vertices_auxiliar[k]);
        j++;
        qsort(nome_vertices, j, sizeof(char *), cmp);
      }
    }
  }
  vertices = malloc(sizeof(Vertice)*j);
  /*Fecha o arquivo após as operações*/
  for(i=0;i<j;i++)
  {
    vertices[i].nome = nome_vertices[i];
  }
  grafo->vertices = vertices;
  grafo->tamanho = j;

  for(i=0;i<grafo->tamanho;i++)
  {
    printf("Posicao %d. %s\n",i,grafo->vertices[i].nome); //imprime vetor
  }

  fclose(arq);
  free(nome_vertices);
}

void criaMatrizAdjacencia(Grafo *grafo,char arquivo[])
{
  /* Função que retorna uma matriz com 0s e 1s.
  Caso o vértice A incida o vértice B, a aresta é 1 */
  char palavra[100];
  char *palavra1;
  char *palavra2;
  char * aux;
  int linha,coluna,i;
  char **nome_vertices;
  FILE *arq;
  arq = fopen(arquivo, "r"); /*abre arquivo em modo leitura*/
  if(arq == NULL){
    printf("Erro, nao foi possivel abrir o arquivo\n");
    return;
  }

  nome_vertices = Aloca(1000,100);
  for(i=0;i<grafo->tamanho;i++)
  {
    nome_vertices[i]= grafo->vertices[i].nome;/*Preenche o
    vetor auxiliar*/
  }

  /* Declaração da Matriz de Adjacencia,
  contendo 0 ou 1 se houve arestas*/
  int **matrizadj;

  /* Com base no número de elementos, criaremos
  uma matriz de Adjacencia para as arestas */
  matrizadj= Aloca(grafo->tamanho,grafo->tamanho);

  /*Leremos o arquivo novamente para estabelecer as arestas */
  while(fgets(palavra,100, arq)!=NULL) /*Enquanto não
  encontrar o fim do arquivo..*/
  {
    /*Para dividir a string utilizei a função
    strtok da biblioteca string.h utilizando a
    vírgula como primeiro delimitador e o NULL
    como segundo delimitador*/

    aux = strtok (palavra,",");
    palavra1 = aux;
    while (aux != NULL)
    {
      palavra2 = aux;
      aux = strtok (NULL, ",");
    }

    /*retira a parte null da segunda palavra
    decorrente do strtok*/
    palavra2= strtok(palavra2,"\n");

    /* Encontraremos a posição da primeira
    palavra lida no vetor de vértices e
    relacionaremos essa posição com a
    posição da matriz i. Após ler a segunda
    palavra, faremos a relação entre a
    posição j da segunda palavra e a
    primeira palavra, atribuindo 1 para a
    aresta i,j lida*/

    /*Procura a palavra no vetor de vértices nome_vertices*/
    char * found = (char *) bsearch(palavra1, nome_vertices, grafo->tamanho, sizeof(char *), myStrCmp);
    char * found2 = (char *) bsearch(palavra2, nome_vertices, grafo->tamanho, sizeof(char *), myStrCmp);

    linha=((int)found-(int)nome_vertices)/sizeof(char*); //encontra a posição no vetor gerada pelo bsearch
    coluna=((int)found2-(int)nome_vertices)/sizeof(char*);

    matrizadj[linha][coluna] = 1;

  }
  grafo->matrizadj = matrizadj;
  fclose(arq);
}

int* getVetorOutdegree(Grafo *grafo)
{
  /* Gera, para cada vértice, o seu grau de saída
  (ou seja, o número de arestas adjacentes).
  Armazena todos no vetor out_degree.
  Em que out_degree[v] é o grau de saída do
  vértice v*/

  int *out_degree;
  out_degree = malloc(sizeof(int)*grafo->tamanho);




  /*    Seu código aqui         */






  /*  for(i=0;i<grafo->tamanho;i++)
  {
  printf("i: %d : %f\n",i,out_degree[i]);
}
*/
return out_degree;
}

float CalculaPageRankVertice(Grafo *grafo,float* page_rank,
  int* out_degree,int vertice,float dumping_factor)
  {
    /*  Calcula a partir do grafo o page rank
    do vértice passado como parametro.
    Também são parametros:
    page_rank[u]: o PageRank (anterior) de cada vértice u.
    out_degree[u]: o grau de saída do vértice u.
    dumping_factor: dumping factor a ser usado.
    Armazene o resultado final na variável
    pageRank.*/

    float pageRank=0;



    /*    Seu código aqui             */




    //printf("Page Rank vertice %d , soma: %f\n",vertice,pageRank);



    return pageRank;
  }

  void CalculaPageRank(Grafo*grafo,float dumping_factor)
  {
    /* Função que irá calcular o Page Rank de todos
    os vértices com o auxílio da função
    auxiliar e irá normalizar os
    resultados até atingirem a condição
    de parada somaDifPR <=0.1 */

    float somaDifPR =0;
    int iteracao=1;
    float *vetorPR;
    vetorPR = malloc(sizeof(int)*grafo->tamanho);

    float *vetorPR_atual;
    vetorPR_atual = malloc(sizeof(int)*grafo->tamanho);

    int *out_degree;
    out_degree = malloc(sizeof(int)*grafo->tamanho);

    /*        Seu código aqui         */





    do
    {





      /*        Seu código aqui         */






      /*for(i=0;i<grafo->tamanho;i++)
      { //Imprime o pagerank a cada interação
      printf("Iteração %d:
      Posicao i %d possui page rank de %f\n",iteracao,i,page_rank[i])
    }
    iteracao++;*/
  }while(somaDifPR >=0.1);


  for(i=0;i<grafo->tamanho;i++)
  {
    grafo->vertices[i].score = vetorPR[i]; /*transfere os dados
    do vetor para a stuct Grafo */
  }

  /*for(i=0;i<grafo->tamanho;i++)
  {
  printf("Posicao i %d possui page rank de %f\n",i,grafo->vertices[i].score);
}*/

free(vetorPR);
free(vetorPR_atual);
free(out_degree);
}

int main()
{
  Grafo grafo;
  char arquivo[] = "grafo.txt";
  obtemVertices(&grafo,arquivo);
  criaMatrizAdjacencia(&grafo,arquivo);

  /* Testa a getVetorOutdegree*/
  int *out_degree = malloc(sizeof(int)*grafo.tamanho);
  //out_degree = getVetorOutdegree(&grafo);

  /* Testa a CalculaPageRankVertice*/
  float *pagerank = malloc(sizeof(float)*grafo.tamanho);
  //CalculaPageRankVertice(&grafo,page_rank,out_degree,vertice,dumping_factor);

  CalculaPageRank(&grafo,0.85);
  //  imprimeTopKPageRank(&grafo,20);
}
