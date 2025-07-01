#ifndef ADVENTURE_H
#define ADVENTURE_H

#define MAX_ITENS 5
#define MAX_DESC 128
#define MAX_NOME 32
#define MAX_ENIGMA 128
#define MAX_RESPOSTA 64
#define MAX_SALAS 10

// Estrutura de Item
typedef struct {
    char nome[MAX_NOME];
    char descricao[MAX_DESC];
    int coletado;
} Item;

// Estrutura de Enigma
typedef struct {
    char pergunta[MAX_ENIGMA];
    char resposta[MAX_RESPOSTA];
    int resolvido;
} Enigma;

// Estrutura de Sala
typedef struct {
    char nome[MAX_NOME];
    char descricao[MAX_DESC];
    Item itens[MAX_ITENS];
    int qtd_itens;
    Enigma *enigma; // Pode ser NULL
} Sala;

// Estrutura do Jogador
typedef struct {
    int sala_atual; // índice da sala no grafo
    Item inventario[MAX_ITENS];
    int qtd_inventario;
} Jogador;

#endif // ADVENTURE_H

