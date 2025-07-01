# Labirinto Aventura - Jogo de Aventura Textual

## Descrição
Este é um jogo de aventura textual em C, onde o jogador explora um labirinto representado por um grafo, coleta itens, resolve enigmas e busca alcançar a saída. O labirinto muda de tamanho e dificuldade conforme o nível escolhido.

## Como Jogar

### 1. Compilação
Compile o projeto normalmente com um compilador C. Exemplo usando gcc:

```
gcc main.c Grafo.c -o labirinto
```

### 2. Iniciando o Jogo
Execute o programa:

```
./labirinto
```

### 3. Escolha do Nível
Ao iniciar, escolha o nível de dificuldade:
- 1: Fácil (labirinto pequeno)
- 2: Médio (labirinto médio)
- 3: Difícil (labirinto grande)

Digite o número correspondente e pressione Enter.

### 4. Objetivo
O objetivo é encontrar a sala de saída do labirinto. Para isso, você deve explorar as salas, coletar itens (como a chave) e resolver enigmas para avançar.

### 5. Comandos Disponíveis
Digite os comandos abaixo para interagir:

- `norte`, `sul`, `leste`, `oeste`: Move o jogador na direção desejada, se houver passagem.
- `coletar <item>`: Coleta um item visível na sala (exemplo: `coletar Lanterna`).
- `enigma`: Tenta resolver o enigma da sala atual, se houver.
- `inventario`: Mostra os itens coletados.
- `ajuda`: Mostra a lista de comandos.
- `sair`: Encerra o jogo.

### 6. Dicas
- Para sair do labirinto, normalmente é necessário encontrar e coletar a chave.
- Resolva os enigmas para desbloquear o avanço em algumas salas.
- Explore todas as direções possíveis em cada sala.

### 7. Condição de Vitória
Você vence ao chegar na sala de saída com a chave no inventário.

---

## Créditos
Inspirado em jogos clássicos de aventura textual como Zork. Desenvolvido para fins didáticos utilizando grafos em C.

