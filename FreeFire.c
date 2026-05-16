//=====================================
// Desafio Free-Fire - Nivel Novato
// Sistema de Inventario - Mochila de Loot
//=====================================

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// =======================================
// --- DEFINICOES E CONSTANTES ---
// =======================================

#define MAX_ITENS 10     // Capacidade maxima da mochila
#define MAX_NOME  30     // Tamanho maximo do nome do item
#define MAX_TIPO  20     // Tamanho maximo do tipo do item

// =======================================
// --- STRUCT ITEM ---
// Representa cada objeto coletado pelo jogador.
// Cada Item possui:
//   - nome: identificador do item (ex: "AK-47", "Kit Medico")
//   - tipo: categoria do item (ex: "arma", "municao", "cura")
//   - quantidade: quantidade disponivel na mochila
// =======================================

typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int quantidade;
} Item;

// =======================================
// --- MOCHILA (VETOR DE STRUCTS) ---
// A mochila e um vetor estatico de Items.
// 'totalItens' controla quantos itens estao
// realmente armazenados no momento.
// =======================================
Item mochila[MAX_ITENS];
int totalItens = 0;

// =======================================
// --- PROTOTIPOS DAS FUNCOES ---
// =======================================
void inserirItem();
void removerItem();
void listarItens();
void buscarItem();
void menuPrincipal();

// =======================================
// --- FUNCAO PRINCIPAL ---
// =======================================
int main() {
    menuPrincipal();
    return 0;
}

// =======================================
// --- MENU PRINCIPAL ---
// Exibe as opcoes disponiveis ao jogador
// e direciona para a funcao correta.
// =======================================

void menuPrincipal() {
    int opcao;

    do {                                              
        printf("\n__________________________________\n");
        printf("|   MOCHILA DE LOOT - INVENTARIO   |\n");
        printf("|__________________________________|\n");
        printf("|  1. Cadastrar item               |\n");
        printf("|  2. Remover item                 |\n");
        printf("|  3. Listar itens                 |\n");
        printf("|  4. Buscar item                  |\n");
        printf("|  0. Sair                         |\n");
        printf("|__________________________________|\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        while (getchar() != '\n'); // limpa o buffer

        switch (opcao) {
            case 1: 
                inserirItem(); break;
            case 2:
                removerItem(); break;
            case 3:
                listarItens(); break;
            case 4:
                buscarItem(); break;
            case 0:
                printf("\n[ Encerrando o jogo... Ate a proxima! ]\n\n"); break;
            default: 
                printf("\n Opcao invalida! Tente novamente.\n");
            
        }

    } while (opcao != 0);
}

// =======================================
// --- INSERIR ITEM ---
// Cadastra um novo item na mochila.
// Verifica se a mochila esta cheia antes
// de permitir o cadastro.
// Usa fgets para capturar nomes com espacos
// e strcspn para remover o '\n' do fgets.
// =======================================

void inserirItem() {
    // Verifica se a mochila atingiu a capacidade maxima
    if (totalItens >= MAX_ITENS) {
        printf("\n[!] Mochila cheia! Remova um item antes de adicionar outro.\n");
        return;
    }

    Item novoItem;

    printf("\n--- Cadastrar Novo Item ---\n");

    // Leitura do nome (aceita espacos com fgets)
    printf("Nome do item : ");
    fgets(novoItem.nome, MAX_NOME, stdin);
    novoItem.nome[strcspn(novoItem.nome, "\n")] = '\0'; // remove '\n'

    // Leitura do tipo
    printf("Tipo (arma/municao/cura/ferramenta): ");
    fgets(novoItem.tipo, MAX_TIPO, stdin);
    novoItem.tipo[strcspn(novoItem.tipo, "\n")] = '\0';

    // Leitura da quantidade
    printf("Quantidade   : ");
    scanf("%d", &novoItem.quantidade);
    while (getchar() != '\n');

    // Validacao basica de quantidade
    if (novoItem.quantidade <= 0) {
        printf("\n[!] Quantidade invalida! O item nao foi cadastrado.\n");
        return;
    }

    // Insere o novo item na proxima posicao livre do vetor
    mochila[totalItens] = novoItem;
    totalItens++;

    printf("\n[+] Item \"%s\" cadastrado com sucesso!\n", novoItem.nome);

    // Lista os itens apos a operacao (requisito 4)
    listarItens();
}

// =======================================
// --- REMOVER ITEM ---
// Remove um item da mochila pelo nome.
// Percorre o vetor buscando o item e,
// ao encontra-lo, desloca os elementos
// seguintes para preencher o espaco vazio.
// =======================================
void removerItem() {
    if (totalItens == 0) {
        printf("\n[!] A mochila esta vazia! Nada para remover.\n");
        return;
    }

    char nomeBusca[MAX_NOME];
    printf("\n--- Remover Item ---\n");
    printf("Nome do item a remover: ");
    fgets(nomeBusca, MAX_NOME, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    int pos = -1; // posicao do item encontrado (-1 = nao encontrado)

    // Busca sequencial pelo nome do item
    for (int i = 0; i < totalItens; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            pos = i;
            break;
        }
    }

    // Item nao encontrado
    if (pos == -1) {
        printf("\n[!] Item \"%s\" nao encontrado na mochila!\n", nomeBusca);
        return;
    }

    // Desloca os elementos seguintes uma posicao para a esquerda
    // para fechar o espaco deixado pelo item removido
    for (int i = pos; i < totalItens - 1; i++) {
        mochila[i] = mochila[i + 1];
    }

    totalItens--;
    printf("\n[-] Item \"%s\" removido com sucesso!\n", nomeBusca);

    // Lista os itens apos a operacao (requisito 4)
    listarItens();
}

// =======================================
// --- LISTAR ITENS ---
// Exibe todos os itens cadastrados
// na mochila com seus respectivos dados.
// Chamada automaticamente apos cada operacao.
// =======================================
void listarItens() {
    printf("\n========= MOCHILA ATUAL =========\n");

    if (totalItens == 0) {
        printf("  [ Mochila vazia ]\n");
        printf("=================================\n");
        return;
    }

    // Percorre o vetor e imprime os dados de cada item
    for (int i = 0; i < totalItens; i++) {
        printf("  [%d] Nome: %-20s | Tipo: %-12s | Qtd: %d\n",
            i + 1,
            mochila[i].nome,
            mochila[i].tipo,
            mochila[i].quantidade);
    }

    printf("  Itens na mochila: %d/%d\n", totalItens, MAX_ITENS);
    printf("=================================\n");
}

// =======================================
// --- BUSCAR ITEM ---
// Realiza busca sequencial pelo nome.
// Percorre o vetor do inicio ao fim ate
// encontrar o item ou chegar ao final.
// Exibe os dados completos se encontrado.
// =======================================
void buscarItem() {
    if (totalItens == 0) {
        printf("\n[!] A mochila esta vazia! Nada para buscar.\n");
        return;
    }

    char nomeBusca[MAX_NOME];
    printf("\n--- Buscar Item ---\n");
    printf("Nome do item: ");
    fgets(nomeBusca, MAX_NOME, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    // Busca sequencial: percorre do indice 0 ate totalItens-1
    for (int i = 0; i < totalItens; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            printf("\n[✓] Item encontrado!\n");
            printf("  Nome      : %s\n", mochila[i].nome);
            printf("  Tipo      : %s\n", mochila[i].tipo);
            printf("  Quantidade: %d\n", mochila[i].quantidade);
            return;
        }
    }

    // Se chegou aqui, o item nao foi encontrado
    printf("\n[!] Item \"%s\" nao encontrado na mochila.\n", nomeBusca);
}