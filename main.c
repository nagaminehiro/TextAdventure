#include <stdio.h>
#include <stdlib.h>
#include "Grafo.h"
#include "adventure.h"
#include <string.h>

// Funcao para inicializar as salas, itens e enigmas
static void inicializa_salas(Sala salas[], Enigma enigmas[]){
    // Sala 0
    strcpy(salas[0].nome, "Entrada");
    strcpy(salas[0].descricao, "Voce esta na entrada do labirinto. Ha uma porta ao norte.");
    salas[0].qtd_itens = 1;
    strcpy(salas[0].itens[0].nome, "Lanterna");
    strcpy(salas[0].itens[0].descricao, "Uma lanterna velha, mas funcional.");
    salas[0].itens[0].coletado = 0;
    salas[0].enigma = NULL;
    // Sala 1
    strcpy(salas[1].nome, "Corredor Escuro");
    strcpy(salas[1].descricao, "Um corredor escuro. Voce sente uma brisa vindo do leste.");
    salas[1].qtd_itens = 0;
    salas[1].enigma = &enigmas[0];
    // Sala 2
    strcpy(salas[2].nome, "Sala do Enigma");
    strcpy(salas[2].descricao, "Uma sala com inscricoes nas paredes.");
    salas[2].qtd_itens = 1;
    strcpy(salas[2].itens[0].nome, "Chave");
    strcpy(salas[2].itens[0].descricao, "Uma pequena chave dourada.");
    salas[2].itens[0].coletado = 0;
    salas[2].enigma = &enigmas[1];
    // Sala 3
    strcpy(salas[3].nome, "Sala Circular");
    strcpy(salas[3].descricao, "Uma sala circular com portas em todas as direcoes.");
    salas[3].qtd_itens = 0;
    salas[3].enigma = NULL;
    // Sala 4
    strcpy(salas[4].nome, "Saida");
    strcpy(salas[4].descricao, "Voce ve a luz do lado de fora. Precisa de uma chave para sair.");
    salas[4].qtd_itens = 0;
    salas[4].enigma = NULL;
    // Enigmas
    strcpy(enigmas[0].pergunta, "O que e, o que e: quanto mais se tira, maior fica?");
    strcpy(enigmas[0].resposta, "buraco");
    enigmas[0].resolvido = 0;
    strcpy(enigmas[1].pergunta, "Tenho cidades, mas nao casas. Tenho montanhas, mas nao arvores. O que sou eu?");
    strcpy(enigmas[1].resposta, "mapa");
    enigmas[1].resolvido = 0;
}

// Funcao para mostrar a sala atual
static void mostrar_sala(Sala *sala) {
    printf("\n== %s ==\n%s\n", sala->nome, sala->descricao);
    if (sala->qtd_itens > 0) {
        printf("Itens visiveis: ");
        for (int i = 0; i < sala->qtd_itens; i++) {
            if (!sala->itens[i].coletado)
                printf("%s ", sala->itens[i].nome);
        }
        printf("\n");
    }
    if (sala->enigma && !sala->enigma->resolvido) {
        printf("Ha um enigma aqui!\n");
    }
}

// Funcao para coletar item
static void coletar_item(Sala *sala, Jogador *jogador, const char *nome_item) {
    for (int i = 0; i < sala->qtd_itens; i++) {
        if (!sala->itens[i].coletado && strcmp(sala->itens[i].nome, nome_item) == 0) {
            if (jogador->qtd_inventario < MAX_ITENS) {
                jogador->inventario[jogador->qtd_inventario++] = sala->itens[i];
                sala->itens[i].coletado = 1;
                printf("Voce coletou: %s\n", nome_item);
                return;
            } else {
                printf("Inventario cheio!\n");
                return;
            }
        }
    }
    printf("Item nao encontrado.\n");
}

// Funcao para tentar resolver enigma
static void resolver_enigma(Sala *sala) {
    if (sala->enigma && !sala->enigma->resolvido) {
        char resposta[MAX_RESPOSTA];
        printf("Enigma: %s\nSua resposta: ", sala->enigma->pergunta);
        fgets(resposta, sizeof(resposta), stdin);
        resposta[strcspn(resposta, "\n")] = 0;
        if (strcmp(resposta, sala->enigma->resposta) == 0) {
            printf("Correto! Enigma resolvido.\n");
            sala->enigma->resolvido = 1;
        } else {
            printf("Resposta incorreta.\n");
        }
    } else {
        printf("Nao ha enigma para resolver aqui.\n");
    }
}

// Funcao para mostrar inventario
static void mostrar_inventario(Jogador *jogador) {
    printf("\nInventario: ");
    if (jogador->qtd_inventario == 0) {
        printf("vazio\n");
        return;
    }
    for (int i = 0; i < jogador->qtd_inventario; i++) {
        printf("%s ", jogador->inventario[i].nome);
    }
    printf("\n");
}

// Funcao para mover o jogador
static int mover(Grafo *gr, int sala_atual, const char *direcao) {
    // 0=norte, 1=leste, 2=sul, 3=oeste
    int direcao_idx = -1;
    if (strcmp(direcao, "norte") == 0) direcao_idx = 0;
    else if (strcmp(direcao, "leste") == 0) direcao_idx = 1;
    else if (strcmp(direcao, "sul") == 0) direcao_idx = 2;
    else if (strcmp(direcao, "oeste") == 0) direcao_idx = 3;
    if (direcao_idx == -1) return -1;

    // Implementacao usando uma tabela de conexoes pre-definida com base no design do labirinto
    // Mapeamento de conexoes conforme os tres niveis do jogo

    // Nivel facil (5 salas)
    if (sala_atual == 0 && direcao_idx == 0) return 1; // Entrada -> Norte -> Corredor
    if (sala_atual == 1 && direcao_idx == 1) return 2; // Corredor -> Leste -> Sala do Enigma
    if (sala_atual == 1 && direcao_idx == 2) return 3; // Corredor -> Sul -> Sala Circular
    if (sala_atual == 3 && direcao_idx == 1) return 4; // Sala Circular -> Leste -> Saida

    // Nivel medio (8 salas)
    if (sala_atual == 0 && direcao_idx == 0) return 1; // Entrada -> Norte -> Sala 1
    if (sala_atual == 1 && direcao_idx == 1) return 2; // Sala 1 -> Leste -> Sala 2
    if (sala_atual == 1 && direcao_idx == 2) return 3; // Sala 1 -> Sul -> Sala 3
    if (sala_atual == 2 && direcao_idx == 0) return 4; // Sala 2 -> Norte -> Sala 4
    if (sala_atual == 3 && direcao_idx == 1) return 5; // Sala 3 -> Leste -> Sala 5
    if (sala_atual == 4 && direcao_idx == 1) return 6; // Sala 4 -> Leste -> Sala 6
    if (sala_atual == 5 && direcao_idx == 0) return 6; // Sala 5 -> Norte -> Sala 6
    if (sala_atual == 6 && direcao_idx == 0) return 7; // Sala 6 -> Norte -> Saida

    // Nivel dificil (12 salas)
    if (sala_atual == 0 && direcao_idx == 0) return 1; // Entrada -> Norte -> Sala 1
    if (sala_atual == 1 && direcao_idx == 1) return 2; // Sala 1 -> Leste -> Sala 2
    if (sala_atual == 1 && direcao_idx == 2) return 3; // Sala 1 -> Sul -> Sala 3
    if (sala_atual == 2 && direcao_idx == 0) return 4; // Sala 2 -> Norte -> Sala 4
    if (sala_atual == 3 && direcao_idx == 1) return 5; // Sala 3 -> Leste -> Sala 5
    if (sala_atual == 4 && direcao_idx == 1) return 6; // Sala 4 -> Leste -> Sala 6
    if (sala_atual == 5 && direcao_idx == 0) return 6; // Sala 5 -> Norte -> Sala 6
    if (sala_atual == 6 && direcao_idx == 0) return 7; // Sala 6 -> Norte -> Sala 7
    if (sala_atual == 7 && direcao_idx == 1) return 8; // Sala 7 -> Leste -> Sala 8
    if (sala_atual == 8 && direcao_idx == 0) return 9; // Sala 8 -> Norte -> Sala 9
    if (sala_atual == 9 && direcao_idx == 1) return 10; // Sala 9 -> Leste -> Sala 10
    if (sala_atual == 10 && direcao_idx == 0) return 11; // Sala 10 -> Norte -> Saida

    // Movimento contrario para permitir retornar (simplificado)
    if (sala_atual == 1 && direcao_idx == 3) return 0; // Voltar a entrada

    return -1; // Nao e possivel mover nesta direcao
}

// Funcoes de inicializacao para cada nivel
typedef struct {
    Grafo* grafo;
    Sala* salas;
    Enigma* enigmas;
    int num_salas;
    int num_enigmas;
    int sala_saida;
} Labirinto;

static Labirinto inicializa_facil() {
    Labirinto l;
    l.num_salas = 5;
    l.num_enigmas = 2;
    l.sala_saida = 4;
    l.grafo = cria_Grafo(l.num_salas, 4, 0);
    insereAresta(l.grafo, 0, 1, 1, 0); // 0-norte->1
    insereAresta(l.grafo, 1, 2, 1, 0); // 1-leste->2
    insereAresta(l.grafo, 1, 3, 1, 0); // 1-sul->3
    insereAresta(l.grafo, 3, 4, 1, 0); // 3-leste->4
    l.salas = malloc(sizeof(Sala) * l.num_salas);
    l.enigmas = malloc(sizeof(Enigma) * l.num_enigmas);
    inicializa_salas(l.salas, l.enigmas);
    return l;
}

static Labirinto inicializa_medio() {
    Labirinto l;
    l.num_salas = 8;
    l.num_enigmas = 3;
    l.sala_saida = 7;
    l.grafo = cria_Grafo(l.num_salas, 4, 0);
    // Ligacoes mais complexas
    insereAresta(l.grafo, 0, 1, 1, 0);
    insereAresta(l.grafo, 1, 2, 1, 0);
    insereAresta(l.grafo, 1, 3, 1, 0);
    insereAresta(l.grafo, 2, 4, 1, 0);
    insereAresta(l.grafo, 3, 5, 1, 0);
    insereAresta(l.grafo, 4, 6, 1, 0);
    insereAresta(l.grafo, 5, 6, 1, 0);
    insereAresta(l.grafo, 6, 7, 1, 0);
    l.salas = malloc(sizeof(Sala) * l.num_salas);
    l.enigmas = malloc(sizeof(Enigma) * l.num_enigmas);
    // Inicializacao das salas e enigmas para medio
    for (int i = 0; i < l.num_salas; i++) {
        sprintf(l.salas[i].nome, "Sala %d", i);
        sprintf(l.salas[i].descricao, "Voce esta na sala %d do labirinto.", i);
        l.salas[i].qtd_itens = 0;
        l.salas[i].enigma = NULL;
    }
    // Itens
    strcpy(l.salas[0].nome, "Entrada");
    strcpy(l.salas[0].descricao, "Entrada do labirinto medio. Portas ao norte.");
    l.salas[0].qtd_itens = 1;
    strcpy(l.salas[0].itens[0].nome, "Lanterna");
    strcpy(l.salas[0].itens[0].descricao, "Uma lanterna util.");
    l.salas[0].itens[0].coletado = 0;
    l.salas[2].qtd_itens = 1;
    strcpy(l.salas[2].itens[0].nome, "Chave");
    strcpy(l.salas[2].itens[0].descricao, "Chave prateada.");
    l.salas[2].itens[0].coletado = 0;
    l.salas[5].qtd_itens = 1;
    strcpy(l.salas[5].itens[0].nome, "Mapa");
    strcpy(l.salas[5].itens[0].descricao, "Um mapa do labirinto.");
    l.salas[5].itens[0].coletado = 0;
    // Enigmas
    l.enigmas[0].resolvido = 0;
    strcpy(l.enigmas[0].pergunta, "O que e, o que e: quanto mais se tira, maior fica?");
    strcpy(l.enigmas[0].resposta, "buraco");
    l.enigmas[1].resolvido = 0;
    strcpy(l.enigmas[1].pergunta, "Tenho cidades, mas nao casas. Tenho montanhas, mas nao arvores. O que sou eu?");
    strcpy(l.enigmas[1].resposta, "mapa");
    l.enigmas[2].resolvido = 0;
    strcpy(l.enigmas[2].pergunta, "O que esta sempre na sua frente, mas nao pode ser visto?");
    strcpy(l.enigmas[2].resposta, "futuro");
    l.salas[1].enigma = &l.enigmas[0];
    l.salas[4].enigma = &l.enigmas[1];
    l.salas[6].enigma = &l.enigmas[2];
    strcpy(l.salas[7].nome, "Saida");
    strcpy(l.salas[7].descricao, "Voce ve a saida, mas precisa da chave.");
    return l;
}

static Labirinto inicializa_dificil() {
    Labirinto l;
    l.num_salas = 12;
    l.num_enigmas = 4;
    l.sala_saida = 11;
    l.grafo = cria_Grafo(l.num_salas, 4, 0);
    // Ligacoes mais complexas
    insereAresta(l.grafo, 0, 1, 1, 0);
    insereAresta(l.grafo, 1, 2, 1, 0);
    insereAresta(l.grafo, 1, 3, 1, 0);
    insereAresta(l.grafo, 2, 4, 1, 0);
    insereAresta(l.grafo, 3, 5, 1, 0);
    insereAresta(l.grafo, 4, 6, 1, 0);
    insereAresta(l.grafo, 5, 6, 1, 0);
    insereAresta(l.grafo, 6, 7, 1, 0);
    insereAresta(l.grafo, 7, 8, 1, 0);
    insereAresta(l.grafo, 8, 9, 1, 0);
    insereAresta(l.grafo, 9, 10, 1, 0);
    insereAresta(l.grafo, 10, 11, 1, 0);
    l.salas = malloc(sizeof(Sala) * l.num_salas);
    l.enigmas = malloc(sizeof(Enigma) * l.num_enigmas);
    for (int i = 0; i < l.num_salas; i++) {
        sprintf(l.salas[i].nome, "Sala %d", i);
        sprintf(l.salas[i].descricao, "Voce esta na sala %d do labirinto.", i);
        l.salas[i].qtd_itens = 0;
        l.salas[i].enigma = NULL;
    }
    // Itens
    strcpy(l.salas[0].nome, "Entrada");
    strcpy(l.salas[0].descricao, "Entrada do labirinto dificil. Portas ao norte.");
    l.salas[0].qtd_itens = 1;
    strcpy(l.salas[0].itens[0].nome, "Lanterna");
    strcpy(l.salas[0].itens[0].descricao, "Uma lanterna potente.");
    l.salas[0].itens[0].coletado = 0;
    l.salas[2].qtd_itens = 1;
    strcpy(l.salas[2].itens[0].nome, "Chave");
    strcpy(l.salas[2].itens[0].descricao, "Chave dourada.");
    l.salas[2].itens[0].coletado = 0;
    l.salas[5].qtd_itens = 1;
    strcpy(l.salas[5].itens[0].nome, "Mapa");
    strcpy(l.salas[5].itens[0].descricao, "Um mapa detalhado.");
    l.salas[5].itens[0].coletado = 0;
    l.salas[8].qtd_itens = 1;
    strcpy(l.salas[8].itens[0].nome, "Cristal");
    strcpy(l.salas[8].itens[0].descricao, "Um cristal misterioso.");
    l.salas[8].itens[0].coletado = 0;
    // Enigmas
    l.enigmas[0].resolvido = 0;
    strcpy(l.enigmas[0].pergunta, "O que e, o que e: quanto mais se tira, maior fica?");
    strcpy(l.enigmas[0].resposta, "buraco");
    l.enigmas[1].resolvido = 0;
    strcpy(l.enigmas[1].pergunta, "Tenho cidades, mas nao casas. Tenho montanhas, mas nao arvores. O que sou eu?");
    strcpy(l.enigmas[1].resposta, "mapa");
    l.enigmas[2].resolvido = 0;
    strcpy(l.enigmas[2].pergunta, "O que esta sempre na sua frente, mas nao pode ser visto?");
    strcpy(l.enigmas[2].resposta, "futuro");
    l.enigmas[3].resolvido = 0;
    strcpy(l.enigmas[3].pergunta, "O que pode encher uma sala sem ocupar espaco?");
    strcpy(l.enigmas[3].resposta, "luz");
    l.salas[1].enigma = &l.enigmas[0];
    l.salas[4].enigma = &l.enigmas[1];
    l.salas[6].enigma = &l.enigmas[2];
    l.salas[9].enigma = &l.enigmas[3];
    strcpy(l.salas[11].nome, "Saida");
    strcpy(l.salas[11].descricao, "Voce ve a saida, mas precisa da chave.");
    return l;
}

int main() {
    printf("Escolha o nivel de dificuldade (1-Facil, 2-Medio, 3-Dificil): ");
    int nivel = 1;
    scanf("%d", &nivel);
    getchar(); // Limpa o enter
    Labirinto lab;
    if (nivel == 1) lab = inicializa_facil();
    else if (nivel == 2) lab = inicializa_medio();
    else lab = inicializa_dificil();
    Jogador jogador = {0};
    jogador.sala_atual = 0;
    jogador.qtd_inventario = 0;
    char comando[64];
    printf("Bem-vindo ao Labirinto Aventura!\nDigite 'ajuda' para comandos.\n");
    while (1) {
        mostrar_sala(&lab.salas[jogador.sala_atual]);
        printf("\nComando: ");
        fgets(comando, sizeof(comando), stdin);
        comando[strcspn(comando, "\n")] = 0;
        if (strcmp(comando, "sair") == 0) break;
        else if (strcmp(comando, "ajuda") == 0) {
            printf("Comandos: norte, sul, leste, oeste, coletar <item>, enigma, inventario, sair\n");
        } else if (strncmp(comando, "coletar ", 8) == 0) {
            coletar_item(&lab.salas[jogador.sala_atual], &jogador, comando+8);
        } else if (strcmp(comando, "enigma") == 0) {
            resolver_enigma(&lab.salas[jogador.sala_atual]);
        } else if (strcmp(comando, "inventario") == 0) {
            mostrar_inventario(&jogador);
        } else if (
            strcmp(comando, "norte") == 0 || strcmp(comando, "sul") == 0 ||
            strcmp(comando, "leste") == 0 || strcmp(comando, "oeste") == 0) {
            int prox = mover(lab.grafo, jogador.sala_atual, comando);
            if (prox != -1) {
                // Se for a sala de saida, exige chave
                if (prox == lab.sala_saida) {
                    int tem_chave = 0;
                    for (int i = 0; i < jogador.qtd_inventario; i++) {
                        if (strcmp(jogador.inventario[i].nome, "Chave") == 0) tem_chave = 1;
                    }
                    if (!tem_chave) {
                        printf("Voce precisa da chave para sair!\n");
                        continue;
                    }
                }
                jogador.sala_atual = prox;
            } else {
                printf("Nao e possivel ir nessa direcao.\n");
            }
        } else {
            printf("Comando desconhecido.\n");
        }
        // Vitoria
        if (jogador.sala_atual == lab.sala_saida) {
            printf("\nParabens! Voce escapou do labirinto!\n");
            break;
        }
    }
    libera_Grafo(lab.grafo);
    free(lab.salas);
    free(lab.enigmas);
    return 0;
}
