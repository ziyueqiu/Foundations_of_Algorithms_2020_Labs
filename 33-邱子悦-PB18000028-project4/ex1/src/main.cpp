#include <iostream>
#include <algorithm>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdbool.h> // C99

#define BUF_LEN 20
#define MAX 512

int N_list[4] = {8, 64, 128, 512};
const int MaxEdge = 512*256;
int father[MAX];
int Rank[MAX];

struct EdgeType{
    int i;
    int j;
    int w;
};

struct EdgeGraph{
    int vertexNum;
    int edgeNum;
    struct EdgeType edge[MaxEdge];
};
struct EdgeGraph G;

bool find_edge(int i, int j);
void rand_input(FILE *fp, int N);
void write_input_file(char *array, int len, int i);
void write_output_file(char *array, int len, int i);

bool find_edge(int i, int j){
    for(int k = 0; k < G.edgeNum; k++){
        if((G.edge[k].i == i && G.edge[k].j == j) ||\
        (G.edge[k].i == j && G.edge[k].j == i))
            return true;
    }
    return false;
}

void rand_input(FILE *fp, int N){
    // i, j, w_ij~(1, 20)
    // 1+rand()%(N/2)
    // num for every i
    srand((unsigned)time(NULL));
    int num, w, i, j;
    char buf[BUF_LEN];
    int *p = (int *)malloc(sizeof(int)*N);
    int max_num = (int)(N/2);
    int place = 0;
    memset(p, 0, sizeof(int)*N);
    for(i = 0; i < N; i++){
        num = 1+rand()% max_num;
        if(p[i]+num > max_num)
            num = max_num-p[i];
        for(int k = 0; k < num; k++){
            do{
                j = rand()%N;
            }while(j == i);
            if(find_edge(i, j)) {
                //printf("find one\n");
                continue;
            }
            if(p[j] >= max_num)
                continue;
            w = 1+rand()%20;
            p[j] = p[j]+1;
            p[i] = p[i]+1;
            memset(buf, '\0', BUF_LEN * sizeof(char));
            sprintf(buf, "%d %d %d\n", i, j, w);
            //printf("%d %d %d\n", i, j, w);
            fprintf(fp, "%s", buf);
            G.edge[place].i = i;
            G.edge[place].j = j;
            G.edge[place].w = w;
            place++;
            G.edgeNum++;
        }
    }
    free(p);
    /*printf("N:%d\n", N);
    for(int i = 0; i < N; i++)
        printf("%d ", p[i]);
    printf("\n");*/
}

void write_input_file(char *array, int len, int i){
    char buf[20];
    memset(buf, '\0', 20* sizeof(char));
    memset(array, '\0', sizeof(char)*len);
    strcpy(array, "../input/input");
    sprintf(buf, "%d", i);
    strcat(array, buf);
    strcat(array, ".txt");
    //printf("%s\n", array);
    return;
}

void write_output_file(char *array, int len, int i){
    char buf[20];
    memset(buf, '\0', 20* sizeof(char));
    memset(array, '\0', sizeof(char)*len);
    strcpy(array, "../output/result");
    sprintf(buf, "%d", i);
    strcat(array, buf);
    strcat(array, ".txt");
    //printf("%s\n", array);
    return;
}

bool comp(EdgeType x, EdgeType y){
    return x.w < y.w;
}


/*
void UNION(int *list, int u, int v){
    // fix the group id
    for(int i = 0; i < G.vertexNum; i++)
        if(list[i] == u)
            list[i] = v;
}*/

void MAKE_SET(int x){
    father[x] = x;
    Rank[x] = 0;
}

int FIND_SET(int x){
    if(x != father[x])
        father[x] = FIND_SET(father[x]);
    return father[x];
}

void UNION(int x, int y){
    if(x == y)
        return;
    if(Rank[x] > Rank[y])
        father[y] = x;
    else{
        if(Rank[x] == Rank[y])
            Rank[y]++;
        father[x] = y;
    }
}

EdgeType * MST_KRUSKAL(){
    EdgeType *A = (EdgeType *)malloc(sizeof(EdgeType)*(G.vertexNum-1));
    int place = 0;

    for(int i = 0; i < G.vertexNum; i++)
        MAKE_SET(i);

    sort(G.edge, G.edge+G.edgeNum, comp);
    int from, to;
    for(int i = 0; i < G.edgeNum; i++){
        from = FIND_SET(G.edge[i].i);
        to = FIND_SET(G.edge[i].j);
        if(from != to){
            A[place] = G.edge[i];
            place++;
            UNION(from, to);
        }
    }
    return A;
}

void result(FILE *fp, int N, EdgeType *A){
    char buf[BUF_LEN];
    int sum = 0;
    for(int i = 0; i < N-1; i++){
        //printf("(%d %d %d)\n", A[i].i, A[i].j, A[i].w);
        memset(buf, '\0', BUF_LEN * sizeof(char));
        sprintf(buf, "(%d %d %d)\n", A[i].i, A[i].j, A[i].w);
        fprintf(fp, "%s", buf);
        sum+= A[i].w;
    }
    memset(buf, '\0', BUF_LEN * sizeof(char));
    sprintf(buf, "%d\n", sum);
    fprintf(fp, "%s", buf);
}

int main(){
    char buf[BUF_LEN];
    clock_t start_t, end_t;
    double total_t;
    FILE *fp3 = fopen("../output/time.txt", "w");
    for(int i = 0; i < 4; i++){
        write_input_file(buf, BUF_LEN, i+1);
        FILE *fp1 = fopen(buf, "w");
        write_output_file(buf, BUF_LEN, i+1);
        FILE *fp2 = fopen(buf, "w");

        // G initialization
        G.edgeNum = 0;
        G.vertexNum = N_list[i];

        rand_input(fp1, N_list[i]);

        start_t = clock();
        EdgeType *A = MST_KRUSKAL();
        end_t = clock();
        total_t = (double) (end_t - start_t) / CLOCKS_PER_SEC;
        memset(buf, '\0', BUF_LEN * sizeof(char));
        sprintf(buf, "%f\n", total_t);
        fprintf(fp3, "%s", buf);

        result(fp2, N_list[i], A);

        free(A);
        fclose(fp1);
        fclose(fp2);
    }
    fclose(fp3);
}