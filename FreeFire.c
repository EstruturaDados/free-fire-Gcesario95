//=============================================================================
// Desafio Free-Fire Aventureiro - Sistema de Inventario - Mochila de Loot
// Versao Avancada: Vetor (lista sequencial) + Lista Encadeada
//
// Comparacao de estruturas de dados:
//   - Vetor      : acesso O(1) por indice, busca binaria apos ordenacao
//   - Lista enc. : insercao/remocao O(1) no inicio, sem necessidade de realocar
//
// Operacoes implementadas nas duas estruturas:
//   Inserir | Remover | Listar | Busca Sequencial
//   + Ordenacao (Bubble Sort) e Busca Binaria exclusivas do vetor
//=============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// =============================================================================
// --- DEFINICOES E CONSTANTES ---
// =============================================================================

#define MAX_ITENS  10    // Capacidade maxima do vetor
#define MAX_NOME   30    // Tamanho maximo do nome do item
#define MAX_TIPO   20    // Tamanho maximo do tipo do item

// =============================================================================
// --- STRUCT ITEM ---
// Representa cada objeto coletado pelo jogador.
//   nome     : identificador do item  (ex: "AK-47", "Kit Medico")
//   tipo     : categoria             (ex: "arma", "municao", "cura")
//   quantidade: unidades na mochila
// =============================================================================

typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int  quantidade;
} Item;

// =============================================================================
// --- STRUCT NO ---
// Nó da lista encadeada.
//   dados    : o Item armazenado neste no
//   proximo  : ponteiro para o proximo no da lista (NULL se for o ultimo)
// =============================================================================

typedef struct No {
    Item        dados;
    struct No  *proximo;
} No;

// =============================================================================
// --- ESTRUTURA: VETOR ---
// Vetor estatico de Items com contador de ocupacao.
// Vantagem  : acesso direto por indice (O(1)), compativel com busca binaria.
// Desvantagem: tamanho fixo, remocao exige deslocamento dos elementos.
// =============================================================================

Item mochila[MAX_ITENS]; // vetor estatico de itens
int  totalItens = 0;     // quantidade de itens atualmente no vetor

// =============================================================================
// --- ESTRUTURA: LISTA ENCADEADA ---
// Cabeca da lista (ponteiro para o primeiro no).
// Vantagem  : tamanho dinamico, insercao/remocao no inicio em O(1).
// Desvantagem: sem acesso direto; busca sempre sequencial O(n).
// =============================================================================

No *listaHead = NULL;    // ponteiro para o primeiro no da lista encadeada

// =============================================================================
// --- CONTADORES GLOBAIS DE COMPARACOES ---
// Permitem medir empiricamente quantas comparacoes cada busca realiza.
// Zerar antes de cada busca garante resultados independentes.
// =============================================================================

int comparacoesSeqVetor  = 0; // comparacoes na busca sequencial do vetor
int comparacoesBinVetor  = 0; // comparacoes na busca binaria do vetor
int comparacoesSeqLista  = 0; // comparacoes na busca sequencial da lista

// =============================================================================
// --- PROTOTIPOS ---
// =============================================================================

// -- Vetor --
void inserirItemVetor();
void removerItemVetor();
void listarItensVetor();
void buscarSequencialVetor();
void ordenarVetor();
void buscarBinariaVetor();

// -- Lista Encadeada --
void inserirItemLista();
void removerItemLista();
void listarItensLista();
void buscarSequencialLista();
void liberarLista();

// -- Auxiliares --
void lerString(char *destino, int tamanho);
void menuVetor();
void menuLista();
void menuPrincipal();

// =============================================================================
// --- FUNCAO PRINCIPAL ---
// =============================================================================

int main() {
    menuPrincipal();
    liberarLista(); // libera memoria da lista antes de encerrar
    return 0;
}

// =============================================================================
// --- MENU PRINCIPAL ---
// Ponto de entrada: permite ao jogador escolher qual estrutura usar
// ou sair do programa.
// =============================================================================

void menuPrincipal() {
    int opcao;
    do {
        printf("\n╔══════════════════════════════════════╗\n");
        printf("║    FREE FIRE - SISTEMA DE MOCHILA    ║\n");
        printf("╠══════════════════════════════════════╣\n");
        printf("║  1. Mochila com VETOR                ║\n");
        printf("║  2. Mochila com LISTA ENCADEADA      ║\n");
        printf("║  0. Sair                             ║\n");
        printf("╚══════════════════════════════════════╝\n");
        printf("Escolha a estrutura: ");
        scanf("%d", &opcao);
        while (getchar() != '\n');

        switch (opcao) {
            case 1: menuVetor(); break;
            case 2: menuLista(); break;
            case 0: printf("\n[ Encerrando o jogo... Ate a proxima! ]\n\n"); break;
            default: printf("\n[!] Opcao invalida!\n");
        }
    } while (opcao != 0);
}

// =============================================================================
// --- MENU DO VETOR ---
// Sub-menu dedicado as operacoes sobre o vetor estatico.
// Inclui opcoes exclusivas: ordenar e busca binaria.
// =============================================================================

void menuVetor() {
    int opcao;
    do {
        printf("\n┌──────────────────────────────────────┐\n");
        printf("│        MOCHILA - VETOR               │\n");
        printf("├──────────────────────────────────────┤\n");
        printf("│  1. Cadastrar item                   │\n");
        printf("│  2. Remover item                     │\n");
        printf("│  3. Listar itens                     │\n");
        printf("│  4. Busca sequencial                 │\n");
        printf("│  5. Ordenar por nome (Bubble Sort)   │\n");
        printf("│  6. Busca binaria (requer ordenacao) │\n");
        printf("│  0. Voltar ao menu principal         │\n");
        printf("└──────────────────────────────────────┘\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        while (getchar() != '\n');

        switch (opcao) {
            case 1: inserirItemVetor();       break;
            case 2: removerItemVetor();       break;
            case 3: listarItensVetor();       break;
            case 4: buscarSequencialVetor();  break;
            case 5: ordenarVetor();           break;
            case 6: buscarBinariaVetor();     break;
            case 0: break;
            default: printf("\n[!] Opcao invalida!\n");
        }
    } while (opcao != 0);
}

// =============================================================================
// --- MENU DA LISTA ENCADEADA ---
// Sub-menu para as operacoes sobre a lista encadeada dinamica.
// =============================================================================

void menuLista() {
    int opcao;
    do {
        printf("\n┌──────────────────────────────────────┐\n");
        printf("│      MOCHILA - LISTA ENCADEADA       │\n");
        printf("├──────────────────────────────────────┤\n");
        printf("│  1. Cadastrar item                   │\n");
        printf("│  2. Remover item                     │\n");
        printf("│  3. Listar itens                     │\n");
        printf("│  4. Busca sequencial                 │\n");
        printf("│  0. Voltar ao menu principal         │\n");
        printf("└──────────────────────────────────────┘\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        while (getchar() != '\n');

        switch (opcao) {
            case 1: inserirItemLista();      break;
            case 2: removerItemLista();      break;
            case 3: listarItensLista();      break;
            case 4: buscarSequencialLista(); break;
            case 0: break;
            default: printf("\n[!] Opcao invalida!\n");
        }
    } while (opcao != 0);
}

// =============================================================================
// ==================== OPERACOES DO VETOR =====================================
// =============================================================================

// -----------------------------------------------------------------------------
// INSERIR ITEM NO VETOR
// Verifica capacidade e adiciona o novo item na proxima posicao livre.
// Complexidade: O(1) - insere diretamente na posicao totalItens.
// -----------------------------------------------------------------------------

void inserirItemVetor() {
    if (totalItens >= MAX_ITENS) {
        printf("\n[!] Mochila (vetor) cheia! Remova um item antes de adicionar.\n");
        return;
    }

    Item novoItem;
    printf("\n--- Cadastrar Item no Vetor ---\n");

    printf("Nome do item         : ");
    lerString(novoItem.nome, MAX_NOME);

    printf("Tipo (arma/municao/cura/ferramenta): ");
    lerString(novoItem.tipo, MAX_TIPO);

    printf("Quantidade           : ");
    scanf("%d", &novoItem.quantidade);
    while (getchar() != '\n');

    if (novoItem.quantidade <= 0) {
        printf("\n[!] Quantidade invalida! Item nao cadastrado.\n");
        return;
    }

    mochila[totalItens] = novoItem;
    totalItens++;

    printf("\n[+] Item \"%s\" inserido no vetor com sucesso!\n", novoItem.nome);
    listarItensVetor();
}

// -----------------------------------------------------------------------------
// REMOVER ITEM DO VETOR
// Busca o item pelo nome e, ao encontra-lo, desloca todos os elementos
// seguintes uma posicao para a esquerda para fechar o espaco.
// Complexidade: O(n) - busca sequencial + deslocamento.
// -----------------------------------------------------------------------------

void removerItemVetor() {
    if (totalItens == 0) {
        printf("\n[!] Vetor vazio! Nada para remover.\n");
        return;
    }

    char nomeBusca[MAX_NOME];
    printf("\n--- Remover Item do Vetor ---\n");
    printf("Nome do item: ");
    lerString(nomeBusca, MAX_NOME);

    int pos = -1;

    // Busca sequencial para localizar o item
    for (int i = 0; i < totalItens; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("\n[!] Item \"%s\" nao encontrado no vetor.\n", nomeBusca);
        return;
    }

    // Desloca elementos para fechar o espaco deixado pelo item removido
    for (int i = pos; i < totalItens - 1; i++) {
        mochila[i] = mochila[i + 1];
    }

    totalItens--;
    printf("\n[-] Item \"%s\" removido do vetor com sucesso!\n", nomeBusca);
    listarItensVetor();
}

// -----------------------------------------------------------------------------
// LISTAR ITENS DO VETOR
// Percorre o vetor do indice 0 ate totalItens-1 e exibe os dados de cada item.
// -----------------------------------------------------------------------------

void listarItensVetor() {
    printf("\n======== MOCHILA - VETOR (%d/%d) ========\n", totalItens, MAX_ITENS);

    if (totalItens == 0) {
        printf("  [ Vetor vazio ]\n");
        printf("=========================================\n");
        return;
    }

    for (int i = 0; i < totalItens; i++) {
        printf("  [%d] %-20s | %-12s | Qtd: %d\n",
               i + 1,
               mochila[i].nome,
               mochila[i].tipo,
               mochila[i].quantidade);
    }
    printf("=========================================\n");
}

// -----------------------------------------------------------------------------
// BUSCA SEQUENCIAL NO VETOR
// Percorre o vetor do inicio ao fim comparando cada nome com o alvo.
// Contabiliza cada comparacao no contador global.
// Complexidade: O(n) no pior caso.
// -----------------------------------------------------------------------------

void buscarSequencialVetor() {
    if (totalItens == 0) {
        printf("\n[!] Vetor vazio! Nada para buscar.\n");
        return;
    }

    char nomeBusca[MAX_NOME];
    printf("\n--- Busca Sequencial no Vetor ---\n");
    printf("Nome do item: ");
    lerString(nomeBusca, MAX_NOME);

    comparacoesSeqVetor = 0; // zera o contador antes da busca

    for (int i = 0; i < totalItens; i++) {
        comparacoesSeqVetor++; // conta cada comparacao realizada

        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            printf("\n[v] Item encontrado na posicao %d!\n", i + 1);
            printf("  Nome      : %s\n", mochila[i].nome);
            printf("  Tipo      : %s\n", mochila[i].tipo);
            printf("  Quantidade: %d\n", mochila[i].quantidade);
            printf("  Comparacoes realizadas: %d\n", comparacoesSeqVetor);
            return;
        }
    }

    printf("\n[!] Item \"%s\" nao encontrado.\n", nomeBusca);
    printf("  Comparacoes realizadas: %d\n", comparacoesSeqVetor);
}

// -----------------------------------------------------------------------------
// ORDENAR VETOR (BUBBLE SORT)
// Ordena os itens em ordem alfabetica crescente pelo nome.
// A cada passagem, elementos "maiores" borbulham para o final do vetor.
// Complexidade: O(n²) no pior caso.
//
// Por que ordenar?
//   A busca binaria exige que os dados estejam ordenados para poder
//   dividir o espaco de busca ao meio a cada iteracao.
// -----------------------------------------------------------------------------

void ordenarVetor() {
    if (totalItens < 2) {
        printf("\n[!] Precisa de ao menos 2 itens para ordenar.\n");
        return;
    }

    Item temp;
    int  trocas = 0;

    // Bubble Sort: compara pares adjacentes e troca se estiverem fora de ordem
    for (int i = 0; i < totalItens - 1; i++) {
        for (int j = 0; j < totalItens - 1 - i; j++) {
            if (strcmp(mochila[j].nome, mochila[j + 1].nome) > 0) {
                temp           = mochila[j];
                mochila[j]     = mochila[j + 1];
                mochila[j + 1] = temp;
                trocas++;
            }
        }
    }

    printf("\n[*] Vetor ordenado por nome! Trocas realizadas: %d\n", trocas);
    listarItensVetor();
}

// -----------------------------------------------------------------------------
// BUSCA BINARIA NO VETOR
// Exige que o vetor esteja ordenado (use a opcao 5 antes).
// A cada iteracao, compara o elemento do meio com o alvo e descarta
// metade do espaco de busca.
// Complexidade: O(log n) - muito mais rapida que a sequencial para n grande.
//
// Diferenca pratica:
//   Busca sequencial em 10 itens: ate 10 comparacoes.
//   Busca binaria   em 10 itens: ate  4 comparacoes.
// -----------------------------------------------------------------------------

void buscarBinariaVetor() {
    if (totalItens == 0) {
        printf("\n[!] Vetor vazio! Nada para buscar.\n");
        return;
    }

    char nomeBusca[MAX_NOME];
    printf("\n--- Busca Binaria no Vetor ---\n");
    printf("(Certifique-se de ter ordenado o vetor antes!)\n");
    printf("Nome do item: ");
    lerString(nomeBusca, MAX_NOME);

    comparacoesBinVetor = 0; // zera o contador antes da busca

    int inicio = 0;
    int fim    = totalItens - 1;
    int meio;
    int resultado;

    while (inicio <= fim) {
        meio   = (inicio + fim) / 2;
        comparacoesBinVetor++; // conta cada comparacao realizada

        resultado = strcmp(mochila[meio].nome, nomeBusca);

        if (resultado == 0) {
            // Item encontrado exatamente no meio atual
            printf("\n[v] Item encontrado na posicao %d!\n", meio + 1);
            printf("  Nome      : %s\n", mochila[meio].nome);
            printf("  Tipo      : %s\n", mochila[meio].tipo);
            printf("  Quantidade: %d\n", mochila[meio].quantidade);
            printf("  Comparacoes (binaria) : %d\n", comparacoesBinVetor);
            printf("  Comparacoes (seq est) : %d  (estimativa se fosse sequencial)\n", meio + 1);
            return;
        } else if (resultado < 0) {
            inicio = meio + 1; // alvo esta na metade direita
        } else {
            fim = meio - 1;    // alvo esta na metade esquerda
        }
    }

    printf("\n[!] Item \"%s\" nao encontrado.\n", nomeBusca);
    printf("  Comparacoes realizadas: %d\n", comparacoesBinVetor);
}

// =============================================================================
// ==================== OPERACOES DA LISTA ENCADEADA ===========================
// =============================================================================

// -----------------------------------------------------------------------------
// INSERIR ITEM NA LISTA ENCADEADA
// Aloca um novo no dinamicamente e o insere no INICIO da lista.
// Inserir no inicio e O(1): basta ajustar o ponteiro da cabeca.
//
// Diferenca em relacao ao vetor:
//   No vetor, inserir no inicio exigiria deslocar todos os elementos.
//   Na lista, e apenas um redirecionamento de ponteiro.
// -----------------------------------------------------------------------------

void inserirItemLista() {
    Item novoItem;
    printf("\n--- Cadastrar Item na Lista Encadeada ---\n");

    printf("Nome do item         : ");
    lerString(novoItem.nome, MAX_NOME);

    printf("Tipo (arma/municao/cura/ferramenta): ");
    lerString(novoItem.tipo, MAX_TIPO);

    printf("Quantidade           : ");
    scanf("%d", &novoItem.quantidade);
    while (getchar() != '\n');

    if (novoItem.quantidade <= 0) {
        printf("\n[!] Quantidade invalida! Item nao cadastrado.\n");
        return;
    }

    // Aloca dinamicamente um novo no
    No *novoNo = (No *) malloc(sizeof(No));
    if (novoNo == NULL) {
        printf("\n[!] Erro de memoria! Nao foi possivel alocar o no.\n");
        return;
    }

    novoNo->dados   = novoItem;
    novoNo->proximo = listaHead; // aponta para o antigo primeiro no
    listaHead       = novoNo;    // o novo no passa a ser o primeiro

    printf("\n[+] Item \"%s\" inserido na lista com sucesso!\n", novoItem.nome);
    listarItensLista();
}

// -----------------------------------------------------------------------------
// REMOVER ITEM DA LISTA ENCADEADA
// Percorre a lista buscando o no cujo nome coincide com o alvo.
// Ao encontrar, ajusta os ponteiros e libera a memoria do no removido.
//
// Casos tratados:
//   1. No e o primeiro (cabeca): atualiza listaHead.
//   2. No e intermediario/ultimo: anterior->proximo aponta para o seguinte.
// -----------------------------------------------------------------------------

void removerItemLista() {
    if (listaHead == NULL) {
        printf("\n[!] Lista vazia! Nada para remover.\n");
        return;
    }

    char nomeBusca[MAX_NOME];
    printf("\n--- Remover Item da Lista ---\n");
    printf("Nome do item: ");
    lerString(nomeBusca, MAX_NOME);

    No *atual    = listaHead;
    No *anterior = NULL;

    // Percorre a lista buscando o no com o nome desejado
    while (atual != NULL) {
        if (strcmp(atual->dados.nome, nomeBusca) == 0) {
            // No encontrado: ajusta os ponteiros
            if (anterior == NULL) {
                listaHead = atual->proximo; // era o primeiro no
            } else {
                anterior->proximo = atual->proximo; // pula o no removido
            }
            free(atual); // libera a memoria alocada dinamicamente
            printf("\n[-] Item \"%s\" removido da lista com sucesso!\n", nomeBusca);
            listarItensLista();
            return;
        }
        anterior = atual;
        atual    = atual->proximo;
    }

    printf("\n[!] Item \"%s\" nao encontrado na lista.\n", nomeBusca);
}

// -----------------------------------------------------------------------------
// LISTAR ITENS DA LISTA ENCADEADA
// Percorre a lista do primeiro ao ultimo no exibindo os dados de cada item.
// Tambem conta e exibe o total de nos (tamanho dinamico).
// -----------------------------------------------------------------------------

void listarItensLista() {
    printf("\n======== MOCHILA - LISTA ENCADEADA ========\n");

    if (listaHead == NULL) {
        printf("  [ Lista vazia ]\n");
        printf("===========================================\n");
        return;
    }

    No  *atual = listaHead;
    int  contador = 1;

    while (atual != NULL) {
        printf("  [%d] %-20s | %-12s | Qtd: %d\n",
               contador,
               atual->dados.nome,
               atual->dados.tipo,
               atual->dados.quantidade);
        atual = atual->proximo;
        contador++;
    }
    printf("  Total de itens: %d\n", contador - 1);
    printf("===========================================\n");
}

// -----------------------------------------------------------------------------
// BUSCA SEQUENCIAL NA LISTA ENCADEADA
// Percorre os nos do inicio ao fim comparando cada nome com o alvo.
// Nao e possivel busca binaria na lista encadeada pois nao ha acesso
// direto por indice — para ir ao elemento do meio, e necessario percorrer
// metade da lista a partir da cabeca.
// Complexidade: O(n) sempre.
// -----------------------------------------------------------------------------

void buscarSequencialLista() {
    if (listaHead == NULL) {
        printf("\n[!] Lista vazia! Nada para buscar.\n");
        return;
    }

    char nomeBusca[MAX_NOME];
    printf("\n--- Busca Sequencial na Lista ---\n");
    printf("Nome do item: ");
    lerString(nomeBusca, MAX_NOME);

    comparacoesSeqLista = 0; // zera o contador antes da busca

    No *atual = listaHead;

    while (atual != NULL) {
        comparacoesSeqLista++; // conta cada comparacao realizada

        if (strcmp(atual->dados.nome, nomeBusca) == 0) {
            printf("\n[v] Item encontrado!\n");
            printf("  Nome      : %s\n", atual->dados.nome);
            printf("  Tipo      : %s\n", atual->dados.tipo);
            printf("  Quantidade: %d\n", atual->dados.quantidade);
            printf("  Comparacoes realizadas: %d\n", comparacoesSeqLista);
            return;
        }
        atual = atual->proximo;
    }

    printf("\n[!] Item \"%s\" nao encontrado.\n", nomeBusca);
    printf("  Comparacoes realizadas: %d\n", comparacoesSeqLista);
}

// -----------------------------------------------------------------------------
// LIBERAR LISTA ENCADEADA
// Percorre todos os nos e libera a memoria de cada um.
// Deve ser chamada ao encerrar o programa para evitar vazamento de memoria.
// -----------------------------------------------------------------------------

void liberarLista() {
    No *atual = listaHead;
    No *proximo;

    while (atual != NULL) {
        proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }

    listaHead = NULL;
}

// =============================================================================
// --- FUNCAO AUXILIAR: lerString ---
// Substitui fgets + strcspn para capturar strings com espacos
// e remover automaticamente o '\n' inserido pelo fgets.
// =============================================================================

void lerString(char *destino, int tamanho) {
    fgets(destino, tamanho, stdin);
    destino[strcspn(destino, "\n")] = '\0';
}