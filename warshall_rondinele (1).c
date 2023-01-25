#include <stdio.h>
int n = 16;
#define INF 9999
#include "time.h"

void printMatrix(int matrix[][n], int l);
void printPath(int graph[][n], int p[][n], int start);

void pathPrintexato(int path[][n], int start, int end)
{
	int aux = end;
	if(path[start][end] != start){
		end = path[start][end];
		pathPrintexato(path,start,end);
	}
	printf("->%d", aux+1);
}

void printBetterPath(int path[][n], int start, int end){
	int aux = end;
    printf("%d", start+1);
    end = path[start][end];
    pathPrintexato(path, start,end);
    printf("->%d", aux+1);
}

void Warshall_method1(int paths[][n], int graph[][n], int list[][n], int pr){
    int i, j, k ,p[n][n], aux[1][n], pesoAux = INF, ver;
	//printf("matriz de preï¿½os\n");
	//printMatrix(list,pr);
	aux[0][0] = 0;
	for(i=1;i<n;i++) //Iniciando matriz de caminhos e produtos
	{
		aux[0][i] = list[0][i];
		for(k=1;k<pr;k++){
			aux[0][i] = aux[0][i]+list[k][i];
		}
	}
    for(j=0; j<n; j++) //criando a matriz de paths
    {
        for(k=0; k<n; k++)
        {
            if(graph[j][k] < INF)
            {
                p[j][k] = j;
            }
            else
            {
                p[j][k] = graph[j][k];
            }
        }
    }
	for(i = 1;i<n;i++){
		if(graph[0][i]+aux[0][i]<pesoAux)
		{
			pesoAux = graph[0][i]+aux[0][i];
			ver = i;
		}
	}
	printf("\n\n METODO 1: Produtos em uma unica loja\n\n");
	for(i=0;i<pr;i++){
		printf("produto: %d\tpreco Produto: %d\n", i+1, list[i][ver]);
	}
	printf("cuato total: %d\tvertice de destino:: %d\n", pesoAux, ver);
}

void Warshall_method2(int paths[][n], int graph_weigth[][n], int list[][n], int pr, int v){
    ///computando os caminhos sem levar em conta precedencia
	int aux[pr][2],i,j,k;

	for(i=0;i<pr;i++) //Iniciando matriz de caminhos e produtos
	{
		for(k=0;k<2;k++)
			aux[i][k] = INF;
	}

	for(i=1 ; i<n ; i++)//Pesquisando os caminhos com melhor custo beneficio
	{
		for(j=0 ; j<pr ; j++)
		{
			if(aux[j][0] > (graph_weigth[0][i]+list[j][i])){
				aux[j][0] = graph_weigth[0][i]+list[j][i];
				aux[j][1] = i;
			}
		}
	}

	printf("\n\n METODO 2: Solucao nao levando em conta a rota\n\n\n");
	printf("\nmatriz de custo beneficio\n");
	int peso, vertice, prod, custo_total = 0;
	for(i=0;i<pr;i++){
		
		vertice = aux[i][1];
		prod = list[i][vertice];
		custo_total += prod;
		peso = graph_weigth[0][aux[i][1]];
		printf("preco Produto: %d\tdistancia: %d\tVertice: %d\n", prod, peso, vertice+1);
	}
    printf("\n");
	printf("\tCusto total: %d\n\n", custo_total);

	int ori=0, end = 0, count[pr];
    printf("rota da lista de compras\n");
    for(i=0;i<pr;i++){
    	printBetterPath(paths, ori, end);
    	ori = aux[i][1];
	}
	printf("%d",end);
}

void Warshall_method3(int paths[][n], int graph_weigth[][n], int list[][n], int QTProd, int v){
    /// define os melhores caminhos custo beneficio levando em conta a precedencia
	int aux[QTProd][2], aux2[QTProd][2], i,j,k, crf;
	for(i=0;i<QTProd;i++) //Iniciando matriz de caminhos e produtos
	{
		for(k=0;k<2;k++)
			aux[i][k] = INF;
			aux2[i][k] = INF;
	}
	int pesoAUX = 0;
    int pesoMIN = INF;
    int aux_path = 0;
	for(i=1 ; i<n ; i++)//Pesquisando os caminhos com melhor custo beneficio
	{
		pesoAUX = 3*INF;
		for(crf=0;crf<QTProd;crf++){
			aux[crf][0] = INF;
			aux[crf][1] = INF;
		}
		crf = i;
		for(j=0 ; j<QTProd ; j++) //Cada interação verifica um produto
		{
			do{
				if(aux[j][0] > (graph_weigth[0][crf]+list[j][crf])){
					pesoAUX = pesoAUX-aux[j][0];
					aux[j][0] = graph_weigth[0][crf]+list[j][crf];
					aux[j][1] = crf;
					pesoAUX = pesoAUX+aux[j][0];
				}
				crf = paths[0][crf];
			}while(crf!=0);
			crf=i;
		}
		if(pesoAUX<pesoMIN){
			pesoMIN=pesoAUX;
			aux_path = i;
			for(crf=0;crf<QTProd;crf++){
				aux2[crf][0] = aux[crf][0];
				aux2[crf][1] = aux[crf][1];
				}
		}
	}
	printf("\n\n METODO 3: Solucao levando em conta a rota\n\n\n");
	printf("\nmatriz de custo beneficio\n");
	int peso, vertice, prod, custo_total = 0;
	for(i=0;i<QTProd;i++){
		
		vertice = aux2[i][1];
		prod = list[i][vertice];
		custo_total += prod;
		peso = graph_weigth[0][aux2[i][1]];
		printf("produto: %d\tpreco Produto: %d\tdistancia: %d\tVertice: %d\n",i+1, prod, peso, vertice+1);
	}
	custo_total += graph_weigth[0][aux_path];
    printf("\n");
	printf("Vertice de destino: %d\tCusto total: %d\n\n", aux_path+1, custo_total);
	printBetterPath(paths,0,aux_path);
}



void floydWarshall_p(int graph[][n], int list[][n], int pr)
{
    int i, j, k, matrix_lista[n][n],p[n][n];
	printf("matriz de preï¿½os\n");
	printMatrix(list,pr);

    for(j=0; j<n; j++) //criando a matriz de paths
    {
        for(k=0; k<n; k++)
        {
            if(graph[j][k] < INF)
            {
                p[j][k] = j;
            }
            else
            {
                p[j][k] = graph[j][k];
            }
        }
    }

    for (k = 0; k < n; k++) //calculando os caminhos mais curtos
    {
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                if (graph[i][k] + graph[k][j] < graph[i][j])//se a distancia pelo vertice k for menor, subtitua
                {
                    graph[i][j] = graph[i][k] + graph[k][j];
                    p[i][j] = p[k][j];//adciona o o caminho atalho mais curto
                }

            }
        }
    }
	printf("matriz de adjacencia\n");
    printMatrix(p,n);
	printf("matriz de distancias\n");
    printMatrix(graph,n);
    //printPath(graph,p,1);
    Warshall_method1(p,graph,list,pr);
    Warshall_method2(p,graph,list, pr, 0);
    Warshall_method3(p,graph,list,pr, 0);
}



void printMatrix(int matrix[][n], int l)
{
    printf("   ");
    int x, i, j, nv = n;
    for(x = 0; x<nv ; x++)
    {
        printf(" [%d]", x);
    }
    printf("\n");
    for ( i = 0; i < l; i++)
    {
        printf("[%d]", i);
        for ( j = 0; j < n; j++)
        {
            if (matrix[i][j] == INF)
            {
                printf("%4s", "INF");
            }
            else
                printf("%4d", matrix[i][j]);
        }
        printf("\n");
    }

}

void printPath(int graph[][n], int p[][n], int start){
	int i,j,aux;
    for(i=0; i<start; i++)
    {
        for(j=0; j<n; j++)
        {
            if(i==j)
            {

            }
            else
            {
                printf("Caminho otimo de %d para %d\n", i, j);
                printf("Custo: %d\n", graph[i][j]);
                aux = j;
                printf("%d", aux);
                while(p[i][aux]!=aux)
                {
                    aux = p[i][aux];
                    printf("<-%d", aux);
                }
                printf("\n\n");
            }
        }
    }
}

int main()
{
    int list_prod_loja[3][16] = {
        {INF ,12, 9, 5, 14, 9, 13, 8, 7, 9, 10, 10, 6, 12, 6, 6},
		{INF, 7, 12, 11, 6, 9, 7, 4, 7, 7, 6, 11, 13, 4, 5, 11},
		{INF, 13, 14, 7, 2, 14, 3, 9, 8, 6, 7, 8, 8, 9, 3, 6},
    };
    int pr = 3;
    int graph[16][16] = {
{0,   11,   8,    8,    INF,   11,7,INF,INF,INF,INF,INF,INF,INF,INF,22},
{11,  0,    INF,  INF,  INF,   INF,INF,INF,INF,7,INF,11,INF,INF,INF,INF},
{8,   INF,  0,    INF,  INF,   INF,6,INF,INF,8,INF,INF,INF,INF,INF,INF },
{8,   INF,  INF,  0,    INF,   INF,INF,INF,INF,INF,INF,INF,INF,INF,6,INF},
{INF, INF,  INF,  INF,  0,     6,INF,INF,INF,INF,INF,INF,INF,INF,7,INF},
{11,  INF,  INF,  INF,  6,     0,INF,INF,INF,INF,1,INF,INF,INF,INF,INF},
{7,   INF,  6,    INF,  INF,   INF,0,INF,INF,INF,3,INF,INF,INF,INF,INF},
{INF, INF,  INF,  INF,  INF,   INF,INF,0,INF,INF,INF,INF,INF,INF,3,7},
{INF, INF,  INF,  INF,  INF,   INF,INF,INF,0,INF,INF,8,1,INF,INF,7},
{INF, 7,    8,    INF,  INF,   INF,INF,INF,INF,0,INF,3,INF,INF,INF,INF},
{INF, INF,  INF,  INF,  INF,   1,3,INF,INF,INF,0,INF,INF,INF,INF,INF},
{INF, 11,   INF,  INF,  INF,   INF,INF,INF,8,3,INF,0,INF,8,INF,INF},
{INF, INF,  INF,  INF,  INF,   INF,INF,INF,1,INF,INF,INF,0,INF,INF,7},
{INF, INF,  INF,  INF,  INF,   INF,INF,INF,INF,INF,INF,8,INF,0,INF,6},
{INF, INF,  INF,  6,    7,     INF,INF,3,INF,INF,INF,INF,INF,INF,0,INF},
{22,  INF,  INF,  INF,  INF,   INF,INF,7,7,INF,INF,INF,7,6,INF,0}
};

	
	
    int i, j, k;
	n = 16;
    for(i=0;i<n;i++){
    	for(k=0;k<n;k++){
    		graph[i][k] = graph[i][k]/3;
		}
	}
    floydWarshall_p(graph, list_prod_loja, pr);
    return 0;
}
