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
void printTranslate(char* ch, int **ppGraph, int nNode) //변환했다는것 보여주기
{
	system("cls");
	print(ppGraph, nNode);
	printf("%s\n", ch);
	printf("\n \t\t↓ \n\n");
}


int main()
{
	FILE*				file;				//파일
	int					nfile_state;		//파일 닫기 예외처리
	int					nNode = 0;			//정점 수 
	int					i, j;
	int**				ppGraph = NULL;		//2차원 배열 동적할당


	/* 파일 열기 */
	file = fopen("indat.bat", "r");	//indat.dat 파일 오픈
	if( file == NULL)
		printf("파일 열기 오류! \n");

	//파일 읽기
	ppGraph = createGraph(file, &nNode, ppGraph);

	/* 유저가 입력한 어레이 출력*/ 
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
			printf("종료합니다.\n");
			exit(0);
			break;
		default:
			printTranslate("Input Error!\n", 
				ppGraph, nNode);
			break;
		}
	}

	//Dijkstra알고리즘
	Dijkstra(ppGraph, nNode, 0);

	/* 파일 닫기 */
	nfile_state = fclose(file);
	if(nfile_state == EOF)
		printf("파일 닫기 오류! \n");


	/* 메모리 해제 */
	for(j=0; j<nNode; j++)
	{
		free(ppGraph[j]);
	}
	free(ppGraph);

	return 0;
}


/* 파일 오픈*/
int **createGraph(FILE* fp, int* nNode, int **ppGraph)
{
	int i, j;
	while(!feof(fp))
	{
		fscanf(fp, "%d", nNode);	//정점 갯수 불러오기

		/*2차원 배열 할당*/
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


/* 메뉴 */
int menu(int **ppGraph, int nNode)
{
	int nSelect = 0;

	/* 메뉴 */
	printf("1. Minimum Spanning Tree Create\n");
	printf("2. Shortest Path Algorithm Create\n");
	printf("3. Exit\n");
	printf(" Select : ");
	scanf("%d", &nSelect);

	return nSelect;
}


/*
인접한 노드에서 최소 값을 찾는 함수
*/
int ExtractMin(int key[], int mstSet[], int nNode)
{
	int min = LIMIT;
	int idx;
	int v = 0;

	//인접한 노드들을 돌면서 키값이 최소인 값을 리턴
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
		key[i] = LIMIT;			//모든 노드를 무한대로 변경
		mst[i] = NOT_MST;		//모든 노드를 MST가 아니라고 변경
	}

	key[r] = 0;			//랜덤으로 설정
	p[r] = NULL;		//첫번째 노드는 무조건 MST라고 설정

	//key노드 빼고 나머지 노드수만큼 루프
	for (i = 0; i < nNode-1; i++) {
		//인접한 간선중 제일 적은 숫자 반환
		int u = ExtractMin(key, mst, nNode);

		//mst로 판단
		mst[u] = MST;

		for (v = 0; v < nNode; v++) {
			//u에 인접한 노드들을 돌면서 키값과 파이값을 갱신
			if (mst[v] == NOT_MST && ppGraph[u][v] <  key[v] && ppGraph[u][v] ) {
				p[v]  = u;	//간선의 시작, 즉 부모(파이 ㅠ)
				key[v] = ppGraph[u][v];
			}
		}
	}

	//스패닝 트리 어레이로 출력
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

	/* 초기화 */
	for (i = 0; i < nNode; i++) {
		d[i] = LIMIT;			//모든 노드를 무한대로 변경
		spp[i] = NIL;		//모든 노드를 SHORT가 아니라고 변경
	}
	d[S] = 0;
	p[S] = 0;
	/* 초기화 */

	i = 0;
	//노드들을 하나씩 선택해서 모든 노드들이 spp에 포함되면 알고리즘 종료 
	while( i < nNode-1 )
	{
		//집합 S에 포함하지 않는 노드들 중에서 Dist가 최소인 노드를 찾는다 
		int u = ExtractMin(d, spp, nNode);

		//최소값을 집합 S에 포함시킨다
		spp[u] = SHORT;

		//S에 속하지 않는 모든 v노드에 대해서는 
		for( v = 0; v < nNode; v++)
		{
			//릴렉스를 한다.
			//spp가 아직 거치지 않고, 값이 존재하고, 
			if( spp[v] == NIL && ppGraph[u][v] && d[u] != LIMIT
				&& d[v] > ppGraph[u][v] + d[u] ) {
					d[v] = d[u] + ppGraph[u][v];
					p[v] = u;	//파이 값
			}
		}
		i++;
	}
	//스패닝 트리 어레이로 출력
	SpanningTree(DIJ, p, d, nNode, NULL);
}


/*
SpanningTree 생성
*/
void SpanningTree(short ch, int p[], int d[], int n, int **ppGraph)
{
	int i , j;
	//다시 출력해줄 더블포인터 생성
	int **ppAfter;
	ppAfter = (int **)malloc(sizeof(int*) * (n));
	for(j=0; j<n; j++)
	{
		ppAfter[j] = (int *)malloc(sizeof(int)*(n));
	}

	//0으로 초기화
	for(i = 0 ; i < n; i++)
	{
		for(j = 0; j < n; j++) 
		{
			ppAfter[i][j] = 0;
		}
	}

	/* 어레이 생성 */
	switch( ch )
	{
	case MIN:	//Prim알고리즘이면
		//배열을 만들어준다
		for (i = 1; i < n; i++) {
			ppAfter[p[i]][i] = ppGraph[i][p[i]];
		}
		break;
	case DIJ:	//DIJ알고리즘이면
		for(i = 0; i < n; i++){
			ppAfter[p[i]][i] = d[i];
		}
		break;
	default:
		printf(" 어레이 생성 오류 ! \n");
		break;
	}

	//어레이 출력
	print(ppAfter, n);


	//Free
	for(j=0; j<n; j++)
	{
		free(ppAfter[j]);
	}
	free(ppAfter);
}





/* 어레이로 출력해주는 함수*/
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