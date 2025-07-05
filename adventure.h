#ifndef ADVENTURE_H
#define ADVENTURE_H

#define MAX_ITENS 5
#define MAX_DESC 128
#define MAX_NOME 32
#define MAX_ENIGMA 128
#define MAX_RESPOSTA 64
#define MAX_SALAS 10
#include "Grafo.h"

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

// Funcoes de inicializacao para cada nivel
typedef struct {
    Grafo* grafo;
    Sala* salas;
    Enigma* enigmas;
    int num_salas;
    int num_enigmas;
    int sala_saida;
} Labirinto;

// Funções básicas
void inicializa_salas(Sala salas[], Enigma enigmas[]);
void mostrar_sala(Sala *sala);
void coletar_item(Sala *sala, Jogador *jogador, const char *nome_item);
void resolver_enigma(Sala *sala);
void mostrar_inventario(Jogador *jogador);
int mover(Grafo *gr, int sala_atual, const char *direcao);
Labirinto inicializa_facil();
Labirinto inicializa_medio();
Labirinto inicializa_dificil();

// Novas funções para encapsulamento
Jogador inicializa_jogador();
Labirinto inicializa_labirinto(int nivel);
void processar_comando(char *comando, Labirinto *lab, Jogador *jogador);
int verificar_saida(Labirinto *lab, Jogador *jogador);
void mostrar_ajuda();
void liberar_labirinto(Labirinto *lab);

#endif // ADVENTURE_H