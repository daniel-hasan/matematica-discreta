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
  float hubs;
  float authority;
}Hits;

typedef struct
{
  Vertice* vertices;
  int tamanho;
  int ** matrizadj;
}Grafo;

typedef struct
{
  int * vetor;
  int tamanho;
}Lista;

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

void obtemVertices(Grafo* grafo,char arquivo[])
{
  /* Função que retorna um vetor de string que irá obter os vértices do arquivo passado por parâmetro*/
  /*variáveis  auxiliares*/
  Vertice *vertices;
  char palavra[400];
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
  nome_vertices = Aloca(7000, 400); /*suporta até
  100 caracteres por vértice e 1000 vértices*/

  /*Declaração de vetor auxiliar */
  char vertices_auxiliar[2][400];

  /* Leitura: Se A ganhou de B, há uma aresta de B para A*/

  while(!feof(arq)) /*Enquanto não encontrar o fim do arquivo..*/
  {
    fgets(palavra,400, arq);  /*Pega a linha inteira
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
  /* Função que retorna uma matriz com 0s e 1s. Caso o vértice A incida o vértice B, a aresta é 1 */
  char palavra[400];
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

  nome_vertices = Aloca(7000,400);
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
  while(fgets(palavra,400, arq)!=NULL) /*Enquanto não
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

Lista getVerticesIncidentes(Grafo*grafo,int vertice)
{
  /*Função que irá calcular quantos vértices estão incidindo no vértice passado.
  Para isso, devemos contar quantas vezes o vértice aparece na coluna da matriz e
  analisar quantas dessas aparições possuem valor igual a 1 na Matriz de Adjacencia.
  A função irá retornar um vetor com as posições i associadas a cada time. */
  Lista lstvertices;
  int*vertices_incidentes;
  int j,i=0;
  int num_incidentes =0;
  //contabiliza o numero de vertices adjacentes
  for(j=0; j<grafo->tamanho; j++)
  {
    if(grafo->matrizadj[j][vertice]==1) // Pega somente as saídas
    {
      num_incidentes++; // o tamanho do vetor será do tamanho dos vertices incidentes
    }
  }

  vertices_incidentes= (int*)malloc(sizeof(int)*num_incidentes);

  for(j=0;j<grafo->tamanho;j++)
  {
    if(grafo->matrizadj[j][vertice]==1)
    {
      vertices_incidentes[i] = j; //o vetor conterá somente a posicao dos incidentes
      i++;
    }
  }
  lstvertices.vetor = vertices_incidentes;
  lstvertices.tamanho = num_incidentes;
  return lstvertices;
}

float* getVetorOutdegree(Grafo *grafo)
{
  float vetorSaida[grafo->tamanho];


  /* Coloque aqui seu código */
  int lin,col, i;
  for(lin=0;lin<grafo->tamanho;lin++){
    vetorSaida[lin]=0;
    for(col=0;col<grafo->tamanho;col++){
      vetorSaida[lin] += grafo->matrizadj[lin][col];

    }
  }

  for(i=0;i<grafo->tamanho;i++)
  {
  printf("i: %d : %f\n",i,vetorSaida[i]);
}
return vetorSaida;

}
float CalculaPageRankVertice(Grafo *grafo,
                float page_rank[],int out_degree[],
              int vertice,float dumping_factor)
{

  float soma=0;
Lista lista1;
lista1=getVerticesIncidentes(grafo, vertice);

int i;
soma=1-dumping_factor;
for (i=0;i<lista1.tamanho;i++){
  soma+=dumping_factor*page_rank[lista1.vetor[i]]/out_degree[lista1.vetor[i]];
}
  /* Coloque seu código aqui */

  printf("Page Rank vertice %d , soma: %f\n",vertice,soma);
  return soma;
}

void CalculaPageRank(Grafo*grafo,float dumping_factor)
{

  /*Coloque seu código aqui*/
int i;
float vetorPR[grafo->tamanho];
float out_degree[grafo->tamanho];
 float page_rank[grafo->tamanho];
for(i=0;i<grafo->tamanho;i++)
  page_rank[i] = 1-dumping_factor;
  out_degree=getVetorOutdegree(grafo);
for(i=0;i<grafo->tamanho;i++)
{
  vetorPR[i]=CalculaPageRankVertice(grafo,
                  page_rank,out_degree,
                i,dumping_factor);
}

    for(int vertice=0;vertice<grafo->tamanho;vertice++)
  {
  printf("Vertice %d: PR: %f\n",vertice,vetorPR[vertice]);
}

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

int main()
{
  Grafo grafo;
  char arquivo[]="grafo.txt";
  obtemVertices(&grafo,arquivo);
  /*Obtem matriz de Adjacencia*/
  criaMatrizAdjacencia(&grafo,arquivo);
  getVetorOutdegree(&grafo);
  float page_rank[4]={0.15,0.15,0.15,0.15};
  int out_degree[4]={2,1,2,1};
  CalculaPageRankVertice(&grafo,
                  page_rank,out_degree,
                1,0.15);
}
