// Gabriel Martins Vicente
/* Sua tarefa será construir um heap (binário, max-heap), e criar métodos para
criar a árvore e a partir de um array, inserir, excluir, e buscar valores na
árvore. Seu objetivo é explicar o funcionamento dos  métodos  que  você  irá
criar.  Para  isso,  deve  buscar  em  sites  acadêmicos,  exemplos  de
implementação de árvores heap.
Para  testar,  você deve usar  um  array  com,  no  mínimo, 500  itens,  gerados
randomicamente  e criar métodos para testar se a estrutura criada obedece a
regra de criação que você determinou. */

/* Sites de Referência: https://joaoarthurbm.github.io/eda/posts/heap/
                        https://www.cos.ufrj.br/~rfarias/cos121/aula_09.html
                        https://www.ime.usp.br/~pf/analise_de_algoritmos/aulas/heap.html
                        https://wagnergaspar.com/como-implementar-uma-tabela-hash-com-string-na-pratica/
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define TAM 50
#define TAM_S 52

/* 2 Propriedades para a Estrutura de dado HEAP MÁXIMO:

1) O valor de um nó é maior ou igual ao valor de seus filhos;

2) O Heap é uma árvore binária completa ou quase-completa
da esquerda para a direita.

*/

struct conteudo { // Estrutura de dado HEAP MÁXIMO

  int indice;            //Valor do indice na árvore HEAP e também do retorno da função Hash
  char senha[TAM_S];         //Guarda conteúdo, no caso, a própria senha

};

int funcaoHash(char chave[]){ /* Função hash modular sobre os caracteres da string */
  
  int tam_string = strlen(chave);
  unsigned int hash = 0;

  for (int i = 0; i < tam_string; i++){
    hash += chave[i] * (i+1);
  }

  return hash % TAM;

}

//----------------------------------------------------------------------------------------------

struct heap { // Estrutura de dado HEAP MÁXIMO

  struct conteudo *vetor; // Ponteiro onde vai apontar para o endereco de memória de uma struct conteudo
  int tamanho; // Tamanho do vetor (capacidade de memória)
  int tail;    // Indica o indice no *p do último valor armazenado caso tenha
};

int pai(int index) { // Função retorna o indice da posição do nó pai

  return (index - 1) / 2;
  // Obs: Como a função retorna um valor int, não é preciso preocupar com
  // valores decimais (após a vírgula)
}

int esquerda(int index) { // Função retorna o indice da posição do nó filho esquerdo

  return 2 * index + 1;
  // Obs: Como a função retorna um valor int, não é preciso preocupar com
  // valores decimais (após a vírgula)
}

int direita(int index) { // Função retorna o indice da posição do nó filho direito

  return 2 * (index + 1);
  // Obs: Como a função retorna um valor int, não é preciso preocupar com
  // valores decimais (após a vírgula)
}

bool estaVazio(struct heap *heap) { // Verifica se meu heap esta vazio

  if (heap->tail == -1) {
    return true;
  }

  else {
    return false;
  }
}

bool estaValidoIndice(struct heap *heap,int indice) { // Verificando se o indice é válido

  if (indice >= 0 && indice <= heap->tamanho) {
    // Verificando se o indice esta no limite estipulado pelo heap: 0 <= indice
    // <= tamanho (capacidade)

    return true;
  }

  else {

    return false;
  }
}

bool isFolha(struct heap *heap, int indice) { // Verificando se é folha

  if (indice > pai(heap->tail) && indice <= heap->tail) {
    /* Para saber se é uma folha, necessariamente deve ser maior que o último nó
       pai e estar dentro do limite de capacidade :

       último indice nó pai <= indice <= tamanho (capacidade)

         */

    return true;
  }

  else {

    return false;
  }
}

int max_indice(struct heap *heap, int pai, int direita,int esquerda) { // Verificar entre os indices qual tem o valor maior

  /* Obs: Por conta da Propriedade 2) da estrutura Heap Máximo e também por
    conta de uma verificação no início na
    função heapfy, não existe necessidade de verificar o valor nó a esquerda */

  if (heap->vetor[pai].indice > heap->vetor[esquerda].indice) {
    // Se o valor: nó_pai > que o nó_esquerda

    if (estaValidoIndice(heap, direita) == true) {
      // Se o valor a direita for válido

      if (heap->vetor[pai].indice < heap->vetor[direita].indice) {
        // Se o valor: nó_pai < nó_direita, retorna indice_direita

        return direita;
      }
    }

    return pai;
    // Se nó_pai > nó_esquerda e nó_pai > nó_direita, retorna indice_pai

  } else {

    if (estaValidoIndice(heap, direita) == true) {
      // Se o valor a direita for válido

      if (heap->vetor[esquerda].indice < heap->vetor[direita].indice) {
        // Se o valor nó_esquerda < nó_direita

        return direita;
        // Retorna indice_direita
      }
    }

    return esquerda;
    // Se nó_esquerda > nó_direita e nó_esquerda > nó_pai
    // retorna nó_esquerda
  }
}

void heapfy(struct heap *heap, int indice) {
  /* Função tem o objetivo de comparar o nó_pai, nó_direita e nó_esquerda e
    fazer o MAIOR valor entre os 3 assumir
    o nó_pai, isto é tornando a estrutura um HEAP MÁXIMO*/

  if (isFolha(heap, indice) || !estaValidoIndice(heap, indice)) {
    return;
    // Se a posicção do indice for uma folha a aplicação do heapfy é inútil,
    // porque ja atende as Propriedades do heap Se o valor for inválido não faz
    // sentido aplicar o heapfy
  }

  int indece_max = max_indice(heap, indice, esquerda(indice), direita(indice));
  // Variável indece_max recebe o indice do valor maior entre os 3

  if (indece_max != indice) {
    // indece_max != do nó_pai, ou seja, necessário ter que fazer a troca dos
    // valores

    struct conteudo aux = heap->vetor[indice]; // Variável auxiliar para fazer a troca
    heap->vetor[indice] = heap->vetor[indece_max];
    heap->vetor[indece_max] = aux;

    heapfy(heap, indece_max);
    /* A recursividade da função verifica se a troca dos 2 nós influência na
       necessidade de ter mais trocas subsequentes,
        portanto, no valor do antigo nó_pai é verificado se nessa nova posição o
       valor é maior que seus filhos */
  }
}

bool busca(struct heap *heap,char senha []) { // Verificar se o valor esta na estrutura Heap Máximo

  int indice = funcaoHash(senha);
  
  for (int i = 0; i <= heap->tail; i++) { // Interação por todo o heap

    if (heap->vetor[i].indice == indice) {

      printf("\nValor se encontra no heap com o indice = %d\n", i);
      return true;
      // Se encontrar o valor return true
    }
  }

  return false;
  // Se não encontrar o valor return false
}

struct heap *insercao(struct heap *heap,char senha[]) {

  /* Observações importantes com relação a Inserção de um Heap Máximo:

       A adição de um novo elemento sempre é feita na próxima
      posição livre do array, isto é, tail + 1. Essa estratégia
      garante que o Heap sempre será completo ou quase completo
      da esquerda para a direita.

  */

  int indice = funcaoHash(senha);

  if(busca(heap,senha) == false){
  
    if (heap->tail >= (heap->tamanho)) {
      printf("\nO Heap esta cheio\n");
      return heap;
      // Verificando se o heap esta cheio
    }
  
    heap->tail += 1;
    struct conteudo novo;
    novo.indice = indice;
    strcat(novo.senha,senha);
    heap->vetor[heap->tail] = novo;
  
    int i = heap->tail;
    while (i > 0 && heap->vetor[pai(i)].indice < heap->vetor[i].indice) {
  
      /* While funciona como um pseudo heapfy, basicamente vai fazer a troca do
        novo valor, se o mesmo for maior que o seu nó_pai */
      struct conteudo aux = heap->vetor[i];
      heap->vetor[i] = heap->vetor[pai(i)];
      heap->vetor[pai(i)] = aux;
      i = pai(i);
    }
    return heap;
  }
  return heap;  
}

struct heap *remover(struct heap *heap) {

  /* Observações importantes com relação a Remoção de um Heap Máximo:

      A remoção em um heap é sempre feita na raiz.

      E para manter a propriedade de ser completo ou quase-completo da esquerda
     para a direita, trocamos o valor da raiz com a última folha e removemos
     essa última folha. */

  if (estaVazio(heap) == true) {
    printf("\nO heap está vazio\n");
    return heap;
    // Verificando se o heap esta vazio
  }

  else {

    heap->vetor[0] = heap->vetor[heap->tail]; // Raiz recebe o valor da última folha
    heap->vetor[heap->tail].indice = 0;
    memset(heap->vetor[heap->tail].senha, 0, TAM_S);
    heap->tail -= 1;

    heapfy(heap, 0);
    /* A verificação dos valores começa pela raiz para manter as Propriedades da
      estrutura HEAP MÁXIMO, por conta da recursividade da função heapfy é
      garantido que a estrutura vai se tornar um Heap máximo pelas aplicações
      sucessivas do algoritmo heapify */
  }

  printf("\n\n!!Exclusão feita com sucesso!!\n\n");
  
  return heap;
}

bool validacao(struct heap *heap) { // Verifica se a Estrutura condiz com as
                                    // Propriedades do Heap-Máximo

  for (int i = 0; i <= heap->tail; i++) {

    if (heap->vetor[i].indice == 0) { // Verifica a Propriedade 2)
      return false;
      /* Se dentro dos intervalos de valores dentro do heap existir um valor
         nulo (no caso o valor 0), então a estrutura não esta sendo completada
         da esquerda para a direita, não condiz com a Propriedade 2) */
    }
  }

  for (int i = heap->tail; i >= 0; i--) { // Verifica a Propriedade 1)

    /* Observação: A interação dos valores começa pelos últimos valores, no caso
       as folhas, é interessante observar que não vai ser necessário fazer a
       interação da raiz, pois pelos filhos ja é feito a verificação */

    if (heap->vetor[pai(i)].indice < heap->vetor[i].indice) {
      return false;
      // Se nó_filho > nó_pai não condiz com a propriedade 1)
    }
  }

  return true;
  // Se as 2 Propriedades estiverem ok, a estrutura esta coerente
}

//---------------------------------------------------------------------------------------------

void imprimirTabela(struct heap *heap){
  
  for(int i = 0; i<TAM; i++){
    
    if((int)strlen(heap->vetor[i].senha)>0){
      printf("Heap[%d] = %s",i,heap->vetor[i].senha);
    }
    else{
       printf("Heap[%d] = %s\n",i,heap->vetor[i].senha);
    }

  }

}

void le_senha(char senha[]){

  fgets(senha,TAM_S,stdin);
  if(strlen(senha)<50){
    printf("\nPressione ENTER");
    while ((getchar()) != '\n');
  }
  setbuf(stdin,NULL);
  
  while(strlen(senha)>50 || strlen(senha)<10){
    
    printf("\n!!A senha deve ter no mínimo 10 caracteres e no máximo 50 caracteres!!\n\n");
    
   if(strlen(senha)>50){     
      printf("\nPressione ENTER");
      while ((getchar()) != '\n');
      setbuf(stdin,NULL);
      
      if(strlen(senha)<50){
        printf("\nPressione ENTER");
      }
    }  

    printf("\n\tQual senha desseja inserir? ");
    fgets(senha,TAM_S,stdin);
    
    if(strlen(senha)<50){
     printf("\nPressione ENTER");
    }
    while ((getchar()) != '\n');
    
  }

}


int main(){

  int opcao, retorno, tabela[TAM];
  char senha[TAM_S];
  struct heap *heap;
  heap = (struct heap *)malloc(sizeof(struct heap)); // Cria na memória uma estrutura heap
  heap->vetor = (struct conteudo *)calloc(TAM, sizeof(struct conteudo));
  heap->tamanho = TAM;  // Inseri o valor da capacidade
  heap->tail = -1; /* Inicia com valor -1 pelo mesmo motivo da estrutura de pilha. Quando
                       for inserido o 1° valor, tail obtem o valor 0, identificando que o
                       último e somente valor esta na posição 0 do vetor */


  do{
      printf("\n\t0 - Sair\n\t1 - Inserir\n\t2 - exclusão\n\t3 - Busca\n\t4 - Imprimir\n\n>");
      scanf("%d", &opcao);
      getchar();
      switch(opcao){
      case 1:
            printf("\tQual senha desseja inserir? ");
            le_senha(senha);
            heap = insercao(heap, senha);
          break;
      case 2:
          remover(heap);
          break;
      case 3:
          printf("\tDigite a senha para fazer a busca: ");
          le_senha(senha);
          busca(heap,senha);
          break;
      case 4:
          imprimirTabela(heap);
          break;
      default:
          printf("Opcao invalida!\n");
      }
    
  }while(opcao != 0);

  return 0;
}