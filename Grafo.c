#include <stdio.h>
#include <stdlib.h>
#include "Grafo.h" //inclui os Prot�tipos

//Defini��o do tipo Grafo
struct grafo
{
    int eh_ponderado; //0 indica que n�o � ponderado/valorado, e 1 indica que �
    int nro_vertices; //N�mero de v�rtices do grafo
    int grau_max; //N�mero m�ximo de conex�es entre v�rtices, tamanho das listas
    int** arestas; //Matriz para registrar os relacionamentos entre v�rtices, ou seja as arestas do grafo
    float** pesos; //Matriz para registrar o peso de cada aresta, se o grafo for ponderado/valorado
    int* grau; //Vetor que armazena o n�mero de conex�es (grau) de cada um dos v�rtices, ou seja, quantidade de elementos em cada lista
};

Grafo* cria_Grafo(int nro_vertices, int grau_max, int eh_ponderado)
{
    Grafo *gr;
    gr = (Grafo*) malloc(sizeof(struct grafo));
    if(gr != NULL)
    {
        int i;
        gr->nro_vertices = nro_vertices;
        gr->grau_max = grau_max;
        gr->eh_ponderado = (eh_ponderado != 0)?1:0; //Se as arestas tem peso, eh_ponderado deve ser 1
        gr->grau = (int*) calloc(nro_vertices,sizeof(int)); //Aloca mem�ria para o vetor que vai armazenar o grau de cada v�rtice

        //Aloca mem�ria para a matriz de arestas
        gr->arestas = (int**) malloc(nro_vertices * sizeof(int*));
        for(i=0; i<nro_vertices; i++)
        {
            gr->arestas[i] = (int*) malloc(grau_max * sizeof(int));
        }

        //Aloca mem�ria para a matriz de pesos
        if(gr->eh_ponderado)
        {
            gr->pesos = (float**) malloc(nro_vertices * sizeof(float*));
            for(i=0; i<nro_vertices; i++)
            {
                gr->pesos[i] = (float*) malloc(grau_max * sizeof(float));
            }
        }
    }
    return gr;
}

void libera_Grafo(Grafo* gr)
{
    if(gr != NULL)
    {
        int i;
        //Libera matriz de arestas
        for(i=0; i<gr->nro_vertices; i++)
        {
            free(gr->arestas[i]);
        }
        free(gr->arestas);

        //Libera matriz de pesos
        if(gr->eh_ponderado)
        {
            for(i=0; i<gr->nro_vertices; i++)
            {
                free(gr->pesos[i]);
            }
            free(gr->pesos);
        }
        free(gr->grau);
        free(gr);
    }
}

int insereAresta(Grafo* gr, int orig, int dest, int eh_digrafo, float peso)
{
    if(gr == NULL)
        return 0;
    //Verifica se v�rtices de origem e destino existem
    if(orig < 0 || orig >= gr->nro_vertices)
    {
        return 0;
    }
    if(dest < 0 || dest >= gr->nro_vertices)
    {
        return 0;
    }

    //Insere no final da linha
    gr->arestas[orig][gr->grau[orig]] = dest;
    if(gr->eh_ponderado)
    {
        gr->pesos[orig][gr->grau[orig]] = peso;
    }
    gr->grau[orig]++;

    //Insere outra aresta se n�o for d�grafo, pois neste caso o relacionamento � em ambos os sentidos
    if(eh_digrafo == 0)
    {
        insereAresta(gr,dest,orig,1,peso);
    }
    return 1;
}

int removeAresta(Grafo* gr, int orig, int dest, int eh_digrafo)
{
    if(gr == NULL)
    {
        return 0;
    }
    //Verifica se v�rtices de origem e destino existem
    if(orig < 0 || orig >= gr->nro_vertices)
    {
        return 0;
    }
    if(dest < 0 || dest >= gr->nro_vertices)
    {
        return 0;
    }

    //Busca o elemento a ser removido e armazena em i a posi��o
    int i = 0;
    while(i<gr->grau[orig] && gr->arestas[orig][i] != dest)
    {
        i++;
    }
    if(i == gr->grau[orig])//elemento nao encontrado
    {
        return 0;
    }

    //Diminui o grau do v�rtice removido, pois ele ter� um relacionamento a menos
    gr->grau[orig]--;
    //Substitui a aresta removida pela aresta que est� na �ltima posi��o da lista
    gr->arestas[orig][i] = gr->arestas[orig][gr->grau[orig]];

    //Se � ponderado atualiza a matriz de pesos
    if(gr->eh_ponderado)
    {
        gr->pesos[orig][i] = gr->pesos[orig][gr->grau[orig]];
    }

    //Se � d�grafo existe uma segunda aresta armazenada, ent�o � preciso remover
    if(eh_digrafo == 0)
    {
        removeAresta(gr,dest,orig,1);
    }
    return 1;
}

void imprime_Grafo(Grafo *gr)
{
    if(gr == NULL)
    {
        return;
    }

    int i, j;
    for(i=0; i < gr->nro_vertices; i++)
    {
        printf(".Vertice %d, vinculado a: ", i);
        for(j=0; j < gr->grau[i]; j++)
        {
            if(gr->eh_ponderado)
            {
                printf("%d(Peso %.2f), ", gr->arestas[i][j], gr->pesos[i][j]);
            }
            else
            {
                printf("%d, ", gr->arestas[i][j]);
            }
        }
        printf("\n");
    }
}

int procuraMenorDistancia(float *dist, int *visitado, int NV)
{
    int i, menor = -1, primeiro = 1;
    //Procura v�rtice com menor dist�ncia e que n�o tenha sido visitado
    for(i=0; i < NV; i++)
    {
        if(dist[i] >= 0 && visitado[i] == 0)
        {
            if(primeiro)
            {
                menor = i;
                primeiro = 0;
            }
            else
            {
                if(dist[menor] > dist[i])
                {
                    menor = i;
                }
            }
        }
    }
    return menor;
}

void menorCaminho_Grafo(Grafo *gr, int ini, int *ant, float *dist, int chave)
{
    int i, cont, NV, ind, *visitado, vert, encontrou = 0;
    cont = NV = gr->nro_vertices;
    //Cria vetor auxiliar e inicializa vetor de dist�ncias e anteriores
    visitado = (int*) malloc(NV * sizeof(int));
    for(i=0; i < NV; i++)
    {
        ant[i] = -1;
        dist[i] = -1;
        visitado[i] = 0;
    }

    //Procura v�rtice com menor dist�ncia e marca como visitado
    dist[ini] = 0;
    while(cont > 0)
    {
        vert = procuraMenorDistancia(dist, visitado, NV);

        if(vert == -1)
            break;

        if (vert == chave) //verifica se a chave est� no grafo
        {
            printf("\nChave %d procurada, encontra-se no grafo\n", vert);
            encontrou = 1;
        }
        visitado[vert] = 1;
        cont--;
        //Para cada v�rtice vizinho
        for(i=0; i<gr->grau[vert]; i++)
        {
            ind = gr->arestas[vert][i];
            if(dist[ind] < 0)
            {
                dist[ind] = dist[vert] + 1;//ou peso da aresta
                ant[ind] = vert;
            }
            else
            {
                if(dist[ind] > dist[vert] + 1)
                {
                    dist[ind] = dist[vert] + 1;//ou peso da aresta
                    ant[ind] = vert;
                }
            }
        }
    }

    free(visitado);
    if (!encontrou)
    {
        printf("\nChave %d procurada, nao encontra-se no grafo!\n\n", chave);
    }
}

int buscaProfundidade(Grafo *gr, int ini, int *visitado, int cont, int chave, int encontrou)
{
    int i;
    visitado[ini] = cont;
    if (ini == chave) //verifica se a chave est� no grafo
    {
        printf("\nChave %d procurada, encontra-se no grafo\n", ini);
        encontrou = 1;
    }
    for(i=0; i<gr->grau[ini]; i++)
    {
        if(!visitado[gr->arestas[ini][i]])
            encontrou = buscaProfundidade(gr,gr->arestas[ini][i],visitado,cont+1, chave, encontrou);
    }

    return encontrou;
}
void buscaProfundidade_Grafo(Grafo *gr, int ini, int *visitado, int chave)
{
    int i, cont = 1, encontrou = 0;
    for(i=0; i<gr->nro_vertices; i++)
    {
        visitado[i] = 0;
    }
    encontrou = buscaProfundidade(gr,ini,visitado,cont, chave, 0);

    for(i=0; i < gr->nro_vertices; i++)
    {
        printf(". Vertice %d -> Visita na iteracao %d\n",i,visitado[i]);
    }

    if (!encontrou)
    {
        printf("\nChave %d procurada, nao encontra-se no grafo!\n", chave);
    }
}

void buscaLargura_Grafo(Grafo *gr, int ini, int *visitado, int chave)
{
    int i, vert, NV, cont = 1, encontrou = 0;
    int *fila, IF = 0, FF = 0;
    for(i=0; i<gr->nro_vertices; i++)
    {
        visitado[i] = 0;
    }

    NV = gr->nro_vertices;
    fila = (int*) malloc(NV * sizeof(int));
    FF++;
    fila[FF] = ini;

    visitado[ini] = cont;
    printf("\nFila: \n");
    printf(".[%d]=%d \t", FF, fila[FF]);
    while(IF != FF)
    {
        IF = (IF + 1) % NV;
        vert = fila[IF];
        if (vert == chave) //verifica se a chave est� no grafo
        {
            printf("\nChave %d procurada, encontra-se no grafo", vert);
            encontrou = 1;
        }
        cont++;
        printf("\n");
        for(i=0; i<gr->grau[vert]; i++)
        {
            if(!visitado[gr->arestas[vert][i]])
            {
                FF = (FF + 1) % NV;
                fila[FF] = gr->arestas[vert][i];
                visitado[gr->arestas[vert][i]] = cont;
            }
            printf(".[%d]=%d \t", FF, fila[FF]);
        }
    }

    printf("\n\n");
    free(fila);

    for(i=0; i < gr->nro_vertices; i++)
    {
        printf(". Vertice %d -> Visita na iteracao %d\n",i,visitado[i]);
    }

    if (!encontrou)
    {
        printf("\nChave %d procurada, nao encontra-se no grafo!\n", chave);
    }

}
