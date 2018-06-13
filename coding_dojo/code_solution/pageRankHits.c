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

  /* Declaração de uma matriz de strings que armazena um vértice V em cada posição*/
  char **nome_vertices;
  nome_vertices = Aloca(7000, 400); /*suporta até 100 caracteres por vértice e 400 vértices*/

  /*Declaração de vetor auxiliar */
  char vertices_auxiliar[2][400];

  /* Leitura: Se A ganhou de B, há uma aresta de B para A*/
  int linha = 0;
  while(!feof(arq)) /*Enquanto não encontrar o fim do arquivo..*/
  {
    fgets(palavra,400, arq);  /*Pega a linha inteira e armazena na string auxiliar nome*/
    /*Para dividir a string utilizei a função strtok da biblioteca string.h utilizando a
    vírgula como primeiro delimitador e o NULL como segundo delimitador*/
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
    if(linha%1000==0){
      printf("Lendo linha: %d\n",linha);
    }
    for (k=0;k<2;k++)
    {
      char * found = (char *) bsearch(vertices_auxiliar[k], nome_vertices, j, sizeof(char *), myStrCmp);
      if (found==NULL)
      {
        sprintf(nome_vertices[j], "%s" ,vertices_auxiliar[k]);
        j++;
        qsort(nome_vertices, j, sizeof(char *), cmp);
      }
    }
    linha++;
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
    nome_vertices[i]= grafo->vertices[i].nome;//Preenche o vetor auxiliar
  }

  /* Declaração da Matriz de Adjacencia, contendo 0 ou 1 se houve arestas*/
  int **matrizadj;
  /* Com base no número de elementos, criaremos uma matriz de Adjacencia para as arestas */
  matrizadj= Aloca(grafo->tamanho,grafo->tamanho);
  /*Leremos o arquivo novamente para estabelecer as arestas */
  while(fgets(palavra,400, arq)!=NULL) /*Enquanto não encontrar o fim do arquivo..*/
  {
    /*Para dividir a string utilizei a função strtok da biblioteca string.h utilizando a
    vírgula como primeiro delimitador e o NULL como segundo delimitador*/
    aux = strtok (palavra,",");
    palavra1 = aux;
    while (aux != NULL)
    {
      palavra2 = aux;
      aux = strtok (NULL, ",");
    }
    /*retira a parte null da segunda palavra decorrente do strtok*/
    palavra2= strtok(palavra2,"\n");

    /* Encontraremos a posição da primeira palavra lida no vetor de vértices e relacionaremos
    essa posição com a posição da matriz i. Após ler a segunda palavra, faremos a relação entre a
    posição j da segunda palavra e a primeira palavra, atribuindo 1 para a aresta i,j lida*/

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

float* getVetorOutdegree(Grafo *grafo)
{
  /* Função que irá calcular o grau de saída de todos os vértices do grafo*/
  float *out_degree;
  out_degree = malloc(sizeof(float)*grafo->tamanho);
  int i,j;
  int saida=0;
  for(i=0;i<grafo->tamanho;i++)
  {
    for(j=0;j<grafo->tamanho;j++)
    {
      if(grafo->matrizadj[i][j] == 1)
      {
        saida++; //conta quantas ocorrencias saídas do vertices
      }
    }
    out_degree[i] = saida;
    saida=0;
  }

  /*  for(i=0;i<grafo->tamanho;i++)
  {
  printf("i: %d : %f\n",i,vetorSaida[i]);
}
*/
return out_degree;
}

float CalculaPageRankVertice(Grafo *grafo,float page_rank[],
  float out_degree[],int vertice,float dumping_factor)
  {
    /* Função auxiliar ao CalculaPageRank que irá calcular o Page Rank para cada vértice */
    int i;
    float pageRank=0;

    for(i=0; i<grafo->tamanho; i++)
    {
      if(grafo->matrizadj[i][vertice]==1)
      {
        pageRank += page_rank[i]/out_degree[i]; /*Soma o pagerank de cada vértice
        e divide pelo out degre */
      }
    }

    pageRank = pageRank*dumping_factor + (1-dumping_factor); /*Multiplica a soma encontrada pelo dumping factor*/
    return pageRank;
  }

  void CalculaPageRank(Grafo*grafo,float dumping_factor)
  {
    /* Função que irá calcular o Page Rank de todos os vértices com o auxílio
    da função auxiliar e irá normalizar os resultados até atingirem a condição
    de parada somaDifPR <=0.1 */
    int i,vertice;
    float *vetorPR;
    float *vetorPR_atual;
    vetorPR = malloc(sizeof(int)*grafo->tamanho);

    vetorPR_atual = malloc(sizeof(int)*grafo->tamanho);

    float *out_degree;
    out_degree = malloc(sizeof(float)*grafo->tamanho);

    out_degree = getVetorOutdegree(&(*grafo));


    for(i=0;i<grafo->tamanho;i++)
    {
      vetorPR[i] = 1-dumping_factor; //inicializa o vetor com 0
    }
    float somaDifPR =0;
    int count =0;
    do
    {

      for(vertice=0;vertice<grafo->tamanho;vertice++) //para cada vertice associado a linha analisada..
      {
        vetorPR_atual[vertice] = CalculaPageRankVertice(&(*grafo),vetorPR,out_degree,vertice,dumping_factor);

      }

      vetorPR_atual = normalizaVetor(vetorPR_atual,grafo->tamanho);

for(i=0;i<grafo->tamanho;i++)
    {
      printf("Posicao i %d possui page rank (nao normalizado) de %f\n",i,vetorPR_atual[i]);
    }

      somaDifPR =0;
      for(vertice=0;vertice<grafo->tamanho;vertice++)
      {
        somaDifPR += fabs(vetorPR_atual[vertice] - vetorPR[vertice]);
        vetorPR[vertice] = vetorPR_atual[vertice];
      }
	count++;
	printf("Iteração #%d soma da dif: %f",count,somaDifPR);
    }while(somaDifPR >=0.1);

    for(i=0;i<grafo->tamanho;i++)
    {
      grafo->vertices[i].score = vetorPR[i];
    }

    /*for(i=0;i<grafo->tamanho;i++)
    {
      printf("Posicao i %d possui page rank de %f\n",i,grafo->vertices[i].score);
    }*/
  }

  int main()
  {
    Grafo grafo;
    char arquivo[] = "../data/teste.txt";
    obtemVertices(&grafo,arquivo);
    criaMatrizAdjacencia(&grafo,arquivo);
    CalculaPageRank(&grafo,0.85);
    imprimeTopKPageRank(&grafo,20);
  }
