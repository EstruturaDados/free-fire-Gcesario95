//=============================================================================
// Desafio Free-Fire - Sistema de Inventario - Mochila de Loot- Torre de Fuga
// - Adiciona struct Componente (nome, tipo, prioridade)
// - Implementa Bubble Sort por nome, Insertion Sort por tipo e Selection Sort por prioridade
// - Adiciona medicao de tempo com clock() e contador de comparacoes por algoritmo
// - Implementa busca binaria por nome (requer ordenacao previa)
// - Adiciona montagem final com confirmacao do componente-chave
// - Integra novo modulo ao menu principal como opcao 3
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
// --- CONSTANTES DO MODULO TORRE DE FUGA ---
// =============================================================================

#define MAX_COMPONENTES 20   // Capacidade maxima de componentes da torre

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
// --- STRUCT COMPONENTE ---
// Representa cada peca necessaria para montar a Torre de Fuga.
//   nome      : identificador da peca   (ex: "chip central")
//   tipo      : categoria da peca       (ex: "controle", "suporte", "propulsao")
//   prioridade: ordem de importancia na montagem (1 = menor, 10 = critica)
// =============================================================================

typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int  prioridade;
} Componente;

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
// --- VARIAVEIS GLOBAIS - MODULO TORRE DE FUGA ---
// =============================================================================

Componente torre[MAX_COMPONENTES]; // vetor de componentes da torre
int totalComponentes = 0;          // quantidade de componentes cadastrados
int torreOrdenadaPorNome = 0;      // flag: 1 = ordenada por nome (habilita busca binaria)
int comparacoesTorre = 0;          // contador de comparacoes dos algoritmos de ordenacao

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

// -- Torre de Fuga --
void cadastrarComponente();
void mostrarComponentes(Componente v[], int n);
void bubbleSortNome(Componente v[], int n);
void insertionSortTipo(Componente v[], int n);
void selectionSortPrioridade(Componente v[], int n);
void medirTempo(void (*algoritmo)(Componente[], int), Componente v[], int n, const char *nomeAlg);
void buscaBinariaPorNome(Componente v[], int n, char alvo[]);
void menuTorre();

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
        printf("║  3. Torre de Fuga (Missao Final)     ║\n");
        printf("║  0. Sair                             ║\n");
        printf("╚══════════════════════════════════════╝\n");
        printf("Escolha a estrutura: ");
        scanf("%d", &opcao);
        while (getchar() != '\n');

        switch (opcao) {
            case 1: menuVetor(); break;
            case 2: menuLista(); break;
            case 3: menuTorre(); break;
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
// ==================== MODULO TORRE DE FUGA ===================================
// =============================================================================

// -----------------------------------------------------------------------------
// MENU TORRE DE FUGA
// Sub-menu dedicado ao modulo da missao final.
// Apresenta opcoes de cadastro, ordenacao, busca e montagem final.
// -----------------------------------------------------------------------------

void menuTorre() {
    int opcao;
    do {
        printf("\n╔══════════════════════════════════════════╗\n");
        printf("║    TORRE DE FUGA - MISSAO FINAL          ║\n");
        printf("╠══════════════════════════════════════════╣\n");
        printf("║  1. Cadastrar componente                 ║\n");
        printf("║  2. Listar componentes                   ║\n");
        printf("║  3. Bubble Sort  (ordenar por nome)      ║\n");
        printf("║  4. Insertion Sort (ordenar por tipo)    ║\n");
        printf("║  5. Selection Sort (ordenar prioridade)  ║\n");
        printf("║  6. Busca binaria por nome               ║\n");
        printf("║  7. Montagem final da torre              ║\n");
        printf("║  0. Voltar ao menu principal             ║\n");
        printf("╚══════════════════════════════════════════╝\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        while (getchar() != '\n');

        switch (opcao) {
            case 1: cadastrarComponente(); break;
            case 2:
                mostrarComponentes(torre, totalComponentes);
                break;
            case 3:
                if (totalComponentes < 2) { printf("\n[!] Cadastre ao menos 2 componentes.\n"); break; }
                medirTempo(bubbleSortNome, torre, totalComponentes, "Bubble Sort (por nome)");
                torreOrdenadaPorNome = 1;
                mostrarComponentes(torre, totalComponentes);
                break;
            case 4:
                if (totalComponentes < 2) { printf("\n[!] Cadastre ao menos 2 componentes.\n"); break; }
                medirTempo(insertionSortTipo, torre, totalComponentes, "Insertion Sort (por tipo)");
                torreOrdenadaPorNome = 0; // nao ordenada por nome, desabilita busca binaria
                mostrarComponentes(torre, totalComponentes);
                break;
            case 5:
                if (totalComponentes < 2) { printf("\n[!] Cadastre ao menos 2 componentes.\n"); break; }
                medirTempo(selectionSortPrioridade, torre, totalComponentes, "Selection Sort (por prioridade)");
                torreOrdenadaPorNome = 0;
                mostrarComponentes(torre, totalComponentes);
                break;
            case 6: {
                if (totalComponentes == 0) { printf("\n[!] Nenhum componente cadastrado.\n"); break; }
                if (!torreOrdenadaPorNome) {
                    printf("\n[!] Ordene por nome (Bubble Sort) antes de usar a busca binaria!\n");
                    break;
                }
                char alvo[MAX_NOME];
                printf("\nNome do componente-chave: ");
                lerString(alvo, MAX_NOME);
                buscaBinariaPorNome(torre, totalComponentes, alvo);
                break;
            }
            case 7: {
                // Montagem final: exibe componentes ordenados e confirma componente-chave
                if (totalComponentes == 0) { printf("\n[!] Nenhum componente cadastrado.\n"); break; }
                printf("\n");
                printf("╔══════════════════════════════════════════╗\n");
                printf("║         MONTAGEM FINAL DA TORRE          ║\n");
                printf("╚══════════════════════════════════════════╝\n");
                mostrarComponentes(torre, totalComponentes);

                char chave[MAX_NOME];
                printf("\nInforme o componente-chave para ativar a torre: ");
                lerString(chave, MAX_NOME);

                // Busca sequencial para confirmar presenca da chave
                int encontrado = 0;
                for (int i = 0; i < totalComponentes; i++) {
                    if (strcmp(torre[i].nome, chave) == 0) {
                        encontrado = 1;
                        printf("\n[*] Componente-chave \"%s\" confirmado!\n", chave);
                        printf("    Tipo      : %s\n", torre[i].tipo);
                        printf("    Prioridade: %d\n", torre[i].prioridade);
                        printf("\n>>> TORRE ATIVADA! FUGA INICIADA! BOA SORTE! <<<\n");
                        break;
                    }
                }
                if (!encontrado)
                    printf("\n[X] Componente-chave nao encontrado! Torre nao pode ser ativada.\n");
                break;
            }
            case 0: break;
            default: printf("\n[!] Opcao invalida!\n");
        }
    } while (opcao != 0);
}

// -----------------------------------------------------------------------------
// CADASTRAR COMPONENTE
// Lê nome, tipo e prioridade de um novo componente e o insere no vetor.
// Valida prioridade no intervalo [1, 10].
// -----------------------------------------------------------------------------

void cadastrarComponente() {
    if (totalComponentes >= MAX_COMPONENTES) {
        printf("\n[!] Limite de %d componentes atingido!\n", MAX_COMPONENTES);
        return;
    }

    Componente c;
    printf("\n--- Cadastrar Componente ---\n");

    printf("Nome       : ");
    lerString(c.nome, MAX_NOME);

    printf("Tipo (controle/suporte/propulsao/energia): ");
    lerString(c.tipo, MAX_TIPO);

    printf("Prioridade (1-10): ");
    scanf("%d", &c.prioridade);
    while (getchar() != '\n');

    if (c.prioridade < 1 || c.prioridade > 10) {
        printf("\n[!] Prioridade invalida! Use valores entre 1 e 10.\n");
        return;
    }

    torre[totalComponentes] = c;
    totalComponentes++;
    printf("\n[+] Componente \"%s\" cadastrado! (%d/%d)\n", c.nome, totalComponentes, MAX_COMPONENTES);
}

// -----------------------------------------------------------------------------
// MOSTRAR COMPONENTES
// Exibe o vetor de componentes formatado com indice, nome, tipo e prioridade.
// Usada apos cada operacao para feedback visual imediato.
// -----------------------------------------------------------------------------

void mostrarComponentes(Componente v[], int n) {
    printf("\n======== COMPONENTES DA TORRE (%d/%d) ========\n", n, MAX_COMPONENTES);
    if (n == 0) {
        printf("  [ Nenhum componente cadastrado ]\n");
        printf("=============================================\n");
        return;
    }
    for (int i = 0; i < n; i++) {
        printf("  [%2d] %-22s | %-12s | Prioridade: %2d\n",
               i + 1, v[i].nome, v[i].tipo, v[i].prioridade);
    }
    printf("=============================================\n");
}

// -----------------------------------------------------------------------------
// BUBBLE SORT POR NOME
// Compara pares adjacentes pelo campo nome (string) e troca se necessario.
// O elemento "maior" (maior nome lexicografico) borbulha para o final.
// Complexidade: O(n²) — didatico, simples de entender e implementar.
// Contador: incrementa comparacoesTorre a cada strcmp executado.
// -----------------------------------------------------------------------------

void bubbleSortNome(Componente v[], int n) {
    comparacoesTorre = 0;
    Componente temp;

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            comparacoesTorre++;
            if (strcmp(v[j].nome, v[j + 1].nome) > 0) {
                temp     = v[j];
                v[j]     = v[j + 1];
                v[j + 1] = temp;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// INSERTION SORT POR TIPO
// Percorre o vetor e, para cada elemento, insere-o na posicao correta
// em relacao aos elementos ja ordenados antes dele (por tipo, string).
// Eficiente para vetores quase ordenados: O(n) no melhor caso, O(n²) no pior.
// Contador: incrementa comparacoesTorre a cada strcmp executado.
// -----------------------------------------------------------------------------

void insertionSortTipo(Componente v[], int n) {
    comparacoesTorre = 0;
    Componente chave;
    int j;

    for (int i = 1; i < n; i++) {
        chave = v[i]; // elemento a ser inserido na posicao correta
        j = i - 1;

        // Desloca elementos maiores que a chave uma posicao a frente
        while (j >= 0) {
            comparacoesTorre++;
            if (strcmp(v[j].tipo, chave.tipo) > 0) {
                v[j + 1] = v[j];
                j--;
            } else {
                break;
            }
        }
        v[j + 1] = chave; // insere a chave na posicao correta
    }
}

// -----------------------------------------------------------------------------
// SELECTION SORT POR PRIORIDADE
// A cada passagem, encontra o elemento de MAIOR prioridade no subvetor
// restante e o coloca no inicio (ordenacao decrescente: 10 → 1).
// Assim o componente mais critico aparece primeiro na listagem.
// Complexidade: O(n²) — numero de comparacoes fixo independente dos dados.
// Contador: incrementa comparacoesTorre a cada comparacao de prioridade.
// -----------------------------------------------------------------------------

void selectionSortPrioridade(Componente v[], int n) {
    comparacoesTorre = 0;
    int    idxMax;
    Componente temp;

    for (int i = 0; i < n - 1; i++) {
        idxMax = i;
        for (int j = i + 1; j < n; j++) {
            comparacoesTorre++;
            if (v[j].prioridade > v[idxMax].prioridade) {
                idxMax = j; // novo maximo encontrado
            }
        }
        // Troca o maior encontrado com o da posicao atual
        if (idxMax != i) {
            temp    = v[i];
            v[i]    = v[idxMax];
            v[idxMax] = temp;
        }
    }
}

// -----------------------------------------------------------------------------
// MEDIR TEMPO
// Recebe um ponteiro para funcao de ordenacao, executa e mede o tempo
// com clock() (resolucao em CLOCKS_PER_SEC, tipicamente microsegundos).
// Exibe comparacoes e tempo apos a execucao.
//
// Parametros:
//   algoritmo : ponteiro para a funcao de ordenacao a ser chamada
//   v         : vetor de componentes
//   n         : tamanho do vetor
//   nomeAlg   : nome do algoritmo (para exibicao)
// -----------------------------------------------------------------------------

void medirTempo(void (*algoritmo)(Componente[], int), Componente v[], int n, const char *nomeAlg) {
    comparacoesTorre = 0;

    clock_t inicio = clock();
    algoritmo(v, n); // chama o algoritmo passado como ponteiro
    clock_t fim = clock();

    double tempoMs = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0;

    printf("\n[*] %s concluido!\n", nomeAlg);
    printf("    Comparacoes realizadas : %d\n", comparacoesTorre);
    printf("    Tempo de execucao      : %.4f ms\n", tempoMs);
}

// -----------------------------------------------------------------------------
// BUSCA BINARIA POR NOME
// Exige que o vetor esteja ordenado por nome (use Bubble Sort antes).
// Divide o espaco de busca ao meio a cada iteracao: O(log n).
// Exibe o resultado e o numero de comparacoes para comparacao didatica
// com a busca sequencial.
//
// Parametros:
//   v    : vetor de componentes ja ordenado por nome
//   n    : tamanho do vetor
//   alvo : nome do componente a localizar
// -----------------------------------------------------------------------------

void buscaBinariaPorNome(Componente v[], int n, char alvo[]) {
    int inicio = 0, fim = n - 1, meio, comparacoes = 0, resultado;

    printf("\n--- Busca Binaria por Nome ---\n");

    while (inicio <= fim) {
        meio = (inicio + fim) / 2;
        comparacoes++;
        resultado = strcmp(v[meio].nome, alvo);

        if (resultado == 0) {
            printf("\n[v] Componente-chave \"%s\" encontrado na posicao %d!\n", alvo, meio + 1);
            printf("    Tipo      : %s\n", v[meio].tipo);
            printf("    Prioridade: %d\n", v[meio].prioridade);
            printf("    Comparacoes (binaria)     : %d\n", comparacoes);
            printf("    Comparacoes (seq. estimada): %d\n", meio + 1);
            printf("\n>>> COMPONENTE-CHAVE CONFIRMADO! TORRE PRONTA PARA ATIVAR! <<<\n");
            return;
        } else if (resultado < 0) {
            inicio = meio + 1; // alvo esta na metade direita
        } else {
            fim = meio - 1;    // alvo esta na metade esquerda
        }
    }

    printf("\n[!] Componente \"%s\" nao encontrado.\n", alvo);
    printf("    Comparacoes realizadas: %d\n", comparacoes);
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