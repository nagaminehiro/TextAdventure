#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Grafo.h"
#include "adventure.h"

int main() {
    // Seleção de nível de dificuldade
    printf("Escolha o nivel de dificuldade (1-Facil, 2-Medio, 3-Dificil): ");
    int nivel = 1;
    scanf("%d", &nivel);
    getchar(); // Limpa o enter

    // Inicialização do labirinto e jogador
    Labirinto lab = inicializa_labirinto(nivel);
    Jogador jogador = inicializa_jogador();

    // Mensagem de boas-vindas
    printf("Bem-vindo ao Labirinto Aventura!\nDigite 'ajuda' para comandos.\n");

    // Loop principal do jogo
    char comando[64];
    char comando_principal[32];
    char argumento[32];

    while (1) {
        // Mostra a sala atual
        mostrar_sala(&lab.salas[jogador.sala_atual]);

        // Lê comando do jogador
        printf("\nComando: ");
        scanf("%s", comando_principal);

        // Verifica se é o comando de saída
        if (strcmp(comando_principal, "sair") == 0) break;

        // Se o comando for "coletar", precisa ler o argumento
        if (strcmp(comando_principal, "coletar") == 0) {
            scanf("%s", argumento);
            sprintf(comando, "%s %s", comando_principal, argumento);
        } else {
            strcpy(comando, comando_principal);
        }

        // Processa o comando
        processar_comando(comando, &lab, &jogador);
    }

    // Libera memória alocada
    liberar_labirinto(&lab);

    return 0;
}
