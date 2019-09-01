#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define LIMIT 255
#define DIJ 1
#define MIN 0

enum {
	NIL = 0,
	SHORT,
};

enum {
	NOT_MST = 0,
	MST,
};

enum {
	MENU_MST = 1,
	MENU_SST,
	MENU_EXIT,
};



void SpanningTree(short ch, int p[], int d[], int n, int **ppGraph);
void min_spanning_tree(int **ppGraph, int nNode, int r);
void Dijkstra(int **ppGraph, int nNode, int S);
void print(int **ppGraph, int nNode);
int menu(int **ppGraph, int nNode);
int ExtractMin(int key[], int mstSet[], int nNode);
int **createGraph(FILE* fp, int* nNode, int **ppGraph);
void printTranslate(char* ch, int **ppGraph, int nNode) //��ȯ�ߴٴ°� �����ֱ�
{
	system("cls");
	print(ppGraph, nNode);
	printf("%s\n", ch);
	printf("\n \t\t�� \n\n");
}


int main()
{
	FILE*				file;				//����
	int					nfile_state;		//���� �ݱ� ����ó��
	int					nNode = 0;			//���� �� 
	int					i, j;
	int**				ppGraph = NULL;		//2���� �迭 �����Ҵ�


	/* ���� ���� */
	file = fopen("indat.bat", "r");	//indat.dat ���� ����
	if( file == NULL)
		printf("���� ���� ����! \n");

	//���� �б�
	ppGraph = createGraph(file, &nNode, ppGraph);

	/* ������ �Է��� ��� ���*/ 
	printf("%d x %d Array\n", nNode, nNode);
	print(ppGraph, nNode);

	for(;;)
	{
		switch(menu(ppGraph, nNode))
		{
		case MENU_MST:
			printTranslate("Minimum Spanning Tree Create!\n", 
				ppGraph, nNode);
			min_spanning_tree(ppGraph, nNode, 0);	//Prim
			break;
		case MENU_SST:
			printTranslate("Shortest Path Create!\n", 
				ppGraph, nNode);
			Dijkstra(ppGraph, nNode, 0);			//Dijkstra
			break;
		case MENU_EXIT:
			system("cls");
			printf("�����մϴ�.\n");
			exit(0);
			break;
		default:
			printTranslate("Input Error!\n", 
				ppGraph, nNode);
			break;
		}
	}

	//Dijkstra�˰���
	Dijkstra(ppGraph, nNode, 0);

	/* ���� �ݱ� */
	nfile_state = fclose(file);
	if(nfile_state == EOF)
		printf("���� �ݱ� ����! \n");


	/* �޸� ���� */
	for(j=0; j<nNode; j++)
	{
		free(ppGraph[j]);
	}
	free(ppGraph);

	return 0;
}


/* ���� ����*/
int **createGraph(FILE* fp, int* nNode, int **ppGraph)
{
	int i, j;
	while(!feof(fp))
	{
		fscanf(fp, "%d", nNode);	//���� ���� �ҷ�����

		/*2���� �迭 �Ҵ�*/
		ppGraph = (int **)malloc(sizeof(int*) * (*nNode));
		for(j=0; j<*nNode; j++)
		{
			ppGraph[j] = (int *)malloc(sizeof(int)*(*nNode));
		}

		for(i = 0; i < *nNode; i++)
		{
			for(j = 0; j < *nNode; j++)
			{
				fscanf(fp, "%d", &ppGraph[i][j]);
			}
		}
	}
	return ppGraph;
}


/* �޴� */
int menu(int **ppGraph, int nNode)
{
	int nSelect = 0;

	/* �޴� */
	printf("1. Minimum Spanning Tree Create\n");
	printf("2. Shortest Path Algorithm Create\n");
	printf("3. Exit\n");
	printf(" Select : ");
	scanf("%d", &nSelect);

	return nSelect;
}


/*
������ ��忡�� �ּ� ���� ã�� �Լ�
*/
int ExtractMin(int key[], int mstSet[], int nNode)
{
	int min = LIMIT;
	int idx;
	int v = 0;

	//������ ������ ���鼭 Ű���� �ּ��� ���� ����
	while (v < nNode) {
		if (mstSet[v] == NIL && key[v] <= min) {
			min = key[v];
			idx = v;
		}
		v++;
	}

	return idx;
}


/*
MinimumSpanningTree
*/
void min_spanning_tree(int **ppGraph, int nNode, int r)
{
	int *p = (int *)malloc(sizeof(int)* nNode);
	int *key = (int *)malloc(sizeof(int)* nNode);
	int *mst = (int *)malloc(sizeof(int)* nNode);
	int i, v;

	for (i = 0; i < nNode; i++) {
		key[i] = LIMIT;			//��� ��带 ���Ѵ�� ����
		mst[i] = NOT_MST;		//��� ��带 MST�� �ƴ϶�� ����
	}

	key[r] = 0;			//�������� ����
	p[r] = NULL;		//ù��° ���� ������ MST��� ����

	//key��� ���� ������ ������ŭ ����
	for (i = 0; i < nNode-1; i++) {
		//������ ������ ���� ���� ���� ��ȯ
		int u = ExtractMin(key, mst, nNode);

		//mst�� �Ǵ�
		mst[u] = MST;

		for (v = 0; v < nNode; v++) {
			//u�� ������ ������ ���鼭 Ű���� ���̰��� ����
			if (mst[v] == NOT_MST && ppGraph[u][v] <  key[v] && ppGraph[u][v] ) {
				p[v]  = u;	//������ ����, �� �θ�(���� ��)
				key[v] = ppGraph[u][v];
			}
		}
	}

	//���д� Ʈ�� ��̷� ���
	SpanningTree(MIN, p, NULL, nNode, ppGraph);
}



/*
Dijkstra Algorithm
*/
void Dijkstra(int **ppGraph, int nNode, int S)
{
	int i, v;
	int *p = (int *)malloc(sizeof(int)* nNode);
	int *d = (int *)malloc(sizeof(int)* nNode);
	int *spp = (int *)malloc(sizeof(int)* nNode);

	/* �ʱ�ȭ */
	for (i = 0; i < nNode; i++) {
		d[i] = LIMIT;			//��� ��带 ���Ѵ�� ����
		spp[i] = NIL;		//��� ��带 SHORT�� �ƴ϶�� ����
	}
	d[S] = 0;
	p[S] = 0;
	/* �ʱ�ȭ */

	i = 0;
	//������ �ϳ��� �����ؼ� ��� ������ spp�� ���ԵǸ� �˰��� ���� 
	while( i < nNode-1 )
	{
		//���� S�� �������� �ʴ� ���� �߿��� Dist�� �ּ��� ��带 ã�´� 
		int u = ExtractMin(d, spp, nNode);

		//�ּҰ��� ���� S�� ���Խ�Ų��
		spp[u] = SHORT;

		//S�� ������ �ʴ� ��� v��忡 ���ؼ��� 
		for( v = 0; v < nNode; v++)
		{
			//�������� �Ѵ�.
			//spp�� ���� ��ġ�� �ʰ�, ���� �����ϰ�, 
			if( spp[v] == NIL && ppGraph[u][v] && d[u] != LIMIT
				&& d[v] > ppGraph[u][v] + d[u] ) {
					d[v] = d[u] + ppGraph[u][v];
					p[v] = u;	//���� ��
			}
		}
		i++;
	}
	//���д� Ʈ�� ��̷� ���
	SpanningTree(DIJ, p, d, nNode, NULL);
}


/*
SpanningTree ����
*/
void SpanningTree(short ch, int p[], int d[], int n, int **ppGraph)
{
	int i , j;
	//�ٽ� ������� ���������� ����
	int **ppAfter;
	ppAfter = (int **)malloc(sizeof(int*) * (n));
	for(j=0; j<n; j++)
	{
		ppAfter[j] = (int *)malloc(sizeof(int)*(n));
	}

	//0���� �ʱ�ȭ
	for(i = 0 ; i < n; i++)
	{
		for(j = 0; j < n; j++) 
		{
			ppAfter[i][j] = 0;
		}
	}

	/* ��� ���� */
	switch( ch )
	{
	case MIN:	//Prim�˰����̸�
		//�迭�� ������ش�
		for (i = 1; i < n; i++) {
			ppAfter[p[i]][i] = ppGraph[i][p[i]];
		}
		break;
	case DIJ:	//DIJ�˰����̸�
		for(i = 0; i < n; i++){
			ppAfter[p[i]][i] = d[i];
		}
		break;
	default:
		printf(" ��� ���� ���� ! \n");
		break;
	}

	//��� ���
	print(ppAfter, n);


	//Free
	for(j=0; j<n; j++)
	{
		free(ppAfter[j]);
	}
	free(ppAfter);
}





/* ��̷� ������ִ� �Լ�*/
void print(int **ppGraph, int nNode)
{
	int i = 0, j = 0;

	printf("\t");
	for(i = 0; i < nNode; i++)
	{
		printf("//%d\t", i);
	}
	printf("\n");


	for(i = 0 ; i < nNode; i++)
	{
		printf("//%d", i);
		for(j = 0; j < nNode; j++) 
		{
			printf("\t%d", ppGraph[i][j]);
		}
		printf("\n");
	}

	printf("\n\n");
}