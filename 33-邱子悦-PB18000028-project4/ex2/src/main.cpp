#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdbool.h> // C99

#include <algorithm>
#include <iostream>
#include <vector>
#include <random>
#include <chrono>

#define BUF_LEN 20
const int N_list[4] = {27, 81, 243, 729};

typedef struct ArcNode{
    int from, to, w;
    struct ArcNode *next;
}ArcNode;

typedef struct VertexNode{
    int id;
    int key; // for BellmanFord's d
    VertexNode *p; // for BellmanFord's parent
    ArcNode *firstArc;
}VertexNode;

typedef struct Graph{
    int VertexNum;
    VertexNode *Vertex;
}Graph;

Graph G;

void rand_input(FILE *fp, int N, int type){
    char buf[BUF_LEN];

    int edgeNum;
    if(type == 0) {
        edgeNum = log(N)/log(5);
    }
    else if(type == 1)
        edgeNum = log(N)/log(7);
    else{
        printf("error!\n");
        exit(1);
    }

    int *p = (int *)malloc(sizeof(int)*N);
    memset(p, 0, sizeof(int)*N);
    int w, from, to;
    std::vector<int> v;
    ArcNode *ptr, *ptr_save;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    for(int i = 0; i < N; i++)
        v.push_back(i);
    for(int i = 0; i < N; i++){
        std::shuffle(v.begin(), v.end(), std::default_random_engine(seed));
        G.Vertex[i].firstArc = (ArcNode *)malloc(sizeof(ArcNode));
        ptr = G.Vertex[i].firstArc;
        ptr->next = NULL;
        while(p[i] < edgeNum){
            //w = -10+rand()%61;
            w = rand()%51;
            from = i;
            to = v[p[i]];
            p[i] = p[i]+1;
            memset(buf, '\0', BUF_LEN * sizeof(char));
            sprintf(buf, "%d %d %d\n", from, to, w);
            fprintf(fp, "%s", buf);
            ptr->from = from;
            ptr->to = to;
            ptr->w = w;
            ptr->next = (ArcNode *)malloc(sizeof(ArcNode));
            ptr_save = ptr;
            ptr = ptr->next;
        }
        free(ptr_save->next);
        ptr_save->next = NULL;
    }
}

void write_input_file(char *array, int len, int i, int j){
    char buf[20];
    memset(buf, '\0', 20* sizeof(char));
    memset(array, '\0', sizeof(char)*len);
    strcpy(array, "../input/input");
    sprintf(buf, "%d%d", i, j);
    strcat(array, buf);
    strcat(array, ".txt");
    //printf("%s\n", array);
    return;
}

void write_result_file(char *array, int len, int i, int j){
    char buf[20];
    memset(buf, '\0', 20* sizeof(char));
    memset(array, '\0', sizeof(char)*len);
    strcpy(array, "../output/result");
    sprintf(buf, "%d%d", i, j);
    strcat(array, buf);
    strcat(array, ".txt");
    //printf("%s\n", array);
    return;
}

void InitGraph(int VertexNum){
    G.VertexNum = VertexNum;
    G.Vertex = (VertexNode *)malloc(sizeof(VertexNode)*G.VertexNum);
    for(int i = 0; i < G.VertexNum; i++)
        G.Vertex[i].firstArc = NULL;
}

void printGraph(){
    ArcNode *arc;
    for(int i = 0; i < G.VertexNum; i++){
        printf("Point %d\n", i);
        arc = G.Vertex[i].firstArc;
        while(arc != NULL){
            printf("(%d->%d %d) ", arc->from, arc->to, arc->w);
            arc = arc->next;
        }
        printf("\n");
    }
}

void FreeGraph(){
    ArcNode *p, *q;
    if(G.Vertex == NULL)
        return;
    for(int i = 0; i < G.VertexNum; i++){
        p = G.Vertex[i].firstArc;
        while(p != NULL){
            q = p->next;
            free(p);
            p = q;
        }
    }
    free(G.Vertex);
}

void FreeD(int **d, int size){
    for(int i = 0; i < size; i++){
        free(d[i]);
    }
    free(d);
}

void INITIALIZE_SINGLE_SOURCE(int index){
    for(int i = 0; i < G.VertexNum; i++){
        G.Vertex[i].key = INT_MAX >> 2;
        G.Vertex[i].p = NULL;
        G.Vertex[i].id = i;
    }
    G.Vertex[index].key = 0;
}

void RELAX(ArcNode *arc){
    if(G.Vertex[arc->to].key > G.Vertex[arc->from].key+arc->w){
        G.Vertex[arc->to].key = G.Vertex[arc->from].key+arc->w;
        G.Vertex[arc->to].p = &G.Vertex[arc->from];
    }
}

bool printPath(VertexNode *src, VertexNode *des, FILE *fp){
    char buf[BUF_LEN];
    bool flag;
    if(src == des){
        //printf("%d,", src->id);
        //fflush(stdout);
        memset(buf, '\0', BUF_LEN * sizeof(char));
        sprintf(buf, "%d ", src->firstArc->from);
        fprintf(fp, "%s", buf);
    }
    else if(des->p == NULL){
        //printf("no path");
        //fflush(stdout);
        memset(buf, '\0', BUF_LEN * sizeof(char));
        sprintf(buf, "no path");
        fprintf(fp, "%s", buf);
        return false;

    }
    else{
        flag = printPath(src, des->p, fp);
        if(!flag)
            return flag;
        //printf("%d ", des->id);
        //fflush(stdout);
        memset(buf, '\0', BUF_LEN * sizeof(char));
        sprintf(buf, "%d ", des->firstArc->from);
        fprintf(fp, "%s", buf);
    }
    return true;
}

/*
void __printPath(VertexNode *src, VertexNode *des){
    if(src == des){
        printf("%d->", src->id);
        fflush(stdout);
    }
    else if(des->p == NULL){
        printf("no path");
        fflush(stdout);
    }
    else{
        __printPath(src, des->p);
        printf("%d->", des->id);
        fflush(stdout);
    }
}*/

bool BELLMAN_FORD(){
    INITIALIZE_SINGLE_SOURCE(G.VertexNum-1);
    //printGraph();
    for(int i = 0; i < G.VertexNum-1; i++){
        for(int j = 0; j < G.VertexNum; j++)
            for(ArcNode *arc = G.Vertex[j].firstArc; arc != NULL; arc = arc->next)
                RELAX(arc);
    }
    for(int j = 0; j < G.VertexNum; j++)
        for(ArcNode *arc = G.Vertex[j].firstArc; arc != NULL; arc = arc->next){
            if(G.Vertex[arc->to].key > G.Vertex[arc->from].key+arc->w)
                return false;
        }
    // for debug
    //printGraph();

    return true;
}

/*
bool cmp(VertexNode *node1, VertexNode *node2){
    return node1->key > node2->key;
}

void Dijkstra(int index){
    INITIALIZE_SINGLE_SOURCE(index);
    //printGraph();
    std::vector<VertexNode *> queue;
    for(int i = 0; i < G.VertexNum-1; i++)
        queue.push_back(&G.Vertex[i]);
    std::make_heap(queue.begin(), queue.end(), cmp);
    while(!queue.empty()){
        VertexNode *node = queue.front();
        std::pop_heap(queue.begin(), queue.end(), cmp);
        queue.pop_back();
        for(ArcNode *arc = node->firstArc; arc != NULL; arc = arc->next)
            RELAX(arc);
        std::make_heap(queue.begin(), queue.end(), cmp);
    }
}*/

int PARENT(int i){
    return (i-1)/2;
}
int LEFT(int i){
    return 2*i+1;
}
int RIGHT(int i){
    return 2*i+2;
}

void MIN_HEAPIFY(VertexNode** A, int i, int heapsize){
    int l, r, largest;
    VertexNode *t;
    l = LEFT(i);
    r = RIGHT(i);
    if(l < heapsize && A[l]->key < A[i]->key)
        largest = l;
    else
        largest = i;
    if(r < heapsize && A[r]->key < A[largest]->key)
        largest = r;
    if(largest != i){
        t = A[i];
        A[i] = A[largest];
        A[i]->id = i;
        A[largest] = t;
        A[largest]->id = largest;
        MIN_HEAPIFY(A, largest, heapsize);
    }
}

VertexNode* HEAP_EXTRACT_MIN(VertexNode** A, int heapsize){
    if(heapsize < 1){
        printf("heap underflow\n");
        return NULL;
    }
    VertexNode *min = A[0];
    A[0] = A[heapsize-1];
    heapsize--;
    MIN_HEAPIFY(A, 0, heapsize);
    return min;
}

void HEAP_DECREASE_KEY(VertexNode** A, int i, int key){
    VertexNode *t;
    if(key > A[i]->key){
        printf("new key is larger than current key\n");
        return;
    }
    A[i]->key = key;
    while(i > 0 & A[PARENT(i)]->key > A[i]->key){
        t = A[i];
        A[i] = A[PARENT(i)];
        A[i]->id = i;
        A[PARENT(i)] = t;
        A[PARENT(i)]->id = PARENT(i);
        i = PARENT(i);
    }
}

void BUILD_MIN_HEAP(VertexNode** A, int heapsize){
    for(int i = heapsize/2; i >= 0; i--)
        MIN_HEAPIFY(A, i, heapsize);
}

void Dijkstra(int index){
    INITIALIZE_SINGLE_SOURCE(index);
    //printGraph();
    VertexNode **queue = (VertexNode**)malloc(sizeof(VertexNode*)*(G.VertexNum-1));
    for(int i = 0; i < G.VertexNum-1; i++)
        queue[i] = &G.Vertex[i];

    BUILD_MIN_HEAP(queue, G.VertexNum-1);
    for(int i = 0; i < G.VertexNum-1; i++){
        VertexNode *node = HEAP_EXTRACT_MIN(queue, G.VertexNum-1);

        for(ArcNode *arc = node->firstArc; arc != NULL; arc = arc->next)
            if(G.Vertex[arc->to].key > G.Vertex[arc->from].key+arc->w){
                //G.Vertex[arc->to].key = G.Vertex[arc->from].key+arc->w;
                HEAP_DECREASE_KEY(queue, G.Vertex[arc->to].id, G.Vertex[arc->from].key+arc->w);
                G.Vertex[arc->to].p = &G.Vertex[arc->from];
            }
    }
}

int ** Johnson(int f_i, int f_j){
    char buf[BUF_LEN];
    write_input_file(buf, BUF_LEN, f_i+1, f_j+1);
    FILE *fp1 = fopen(buf, "w");
    InitGraph(N_list[f_i]+1);
    rand_input(fp1, N_list[f_i], f_j);
    fclose(fp1);

    write_result_file(buf, BUF_LEN, f_i+1, f_j+1);
    FILE *fp2 = fopen(buf, "w");

    // for debug
    //printGraph();

    ArcNode *ptr;
    int N = G.VertexNum-1;

    // source id = N
    G.Vertex[N].firstArc = (ArcNode *)malloc(sizeof(ArcNode));
    ptr = G.Vertex[N].firstArc;
    for(int k = 0; k < N-1; k++){
        ptr->from = N;
        ptr->to = k;
        ptr->w = 0;
        ptr->next = (ArcNode *)malloc(sizeof(ArcNode));
        ptr = ptr->next;
    }
    ptr->from = N;
    ptr->to = N-1;
    ptr->w = 0;
    ptr->next = NULL;

    // BELLMAN-FORD
    /*while(!BELLMAN_FORD()){
        printf("false!\n");
        FreeGraph();
        write_input_file(buf, BUF_LEN, f_i+1, f_j+1);
        fp1 = fopen(buf, "w");
        InitGraph(N_list[f_i]+1);
        rand_input(fp1, N_list[f_i], f_j);
        fclose(fp1);
    }*/
    if(!BELLMAN_FORD()){
        printf("false!\n");
    }

    int *h = (int *)malloc(sizeof(int)*G.VertexNum);
    for(int i = 0; i < G.VertexNum; i++)
        h[i] = G.Vertex[i].key;
    for(int i = 0; i < G.VertexNum; i++){
        ArcNode *arc = G.Vertex[i].firstArc;
        for(; arc != NULL; arc = arc->next)
            arc->w = arc->w+h[arc->from]-h[arc->to];
    }
    // make nxn matrix
    int **d = (int **)malloc(sizeof(int *)*N);
    for(int i = 0; i < N; i++)
        d[i] = (int *)malloc(sizeof(int)*N);
    for(int i = 0; i < N; i++){
        Dijkstra(i);
        for(int j = 0; j < N; j++){
            d[i][j] = G.Vertex[j].key+h[j]-h[i];
            //printf("(%d->%d %d)\n", i, j, d[i][j]);
        }
        for(int j = 0; j < N; j++) {
            if (j == i)
                continue;
            memset(buf, '\0', BUF_LEN * sizeof(char));
            sprintf(buf, "(");
            fprintf(fp2, "%s", buf);

            bool flag = printPath(&G.Vertex[i], &G.Vertex[j], fp2);

            //printf("\n");
            memset(buf, '\0', BUF_LEN * sizeof(char));
            if (flag)
                sprintf(buf, "%d)\n", d[i][j]);
            else
                sprintf(buf, ")\n");
            fprintf(fp2, "%s", buf);
        }
    }
    /*
    for(int index = 0; index < G.VertexNum-1; index++)
        for(int i = 0; i < G.VertexNum; i++){
            if(index == i)
                continue;
            memset(buf, '\0', BUF_LEN * sizeof(char));
            sprintf(buf, "(");
            fprintf(fp2, "%s", buf);

            bool flag = printPath(&G.Vertex[index], &G.Vertex[i], fp2);

            //printf("\n");
            memset(buf, '\0', BUF_LEN * sizeof(char));
            if(flag)
                sprintf(buf, "%d)\n", d[index][i]);
            else
                sprintf(buf, ")\n");
            fprintf(fp2, "%s", buf);
        }*/

    fclose(fp2);

    FreeGraph();
    free(h);
    return d;
}

int main(){
    char buf[BUF_LEN];
    clock_t start_t, end_t;
    double total_t;
    FILE *fp2 = fopen("../output/time.txt", "w");

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 2; j++){
            start_t = clock();

            int **d = Johnson(i, j);

            end_t = clock();

            FreeD(d, N_list[i]);

            total_t = (double) (end_t - start_t) / CLOCKS_PER_SEC;
            memset(buf, '\0', BUF_LEN * sizeof(char));
            sprintf(buf, "%f\n", total_t);
            fprintf(fp2, "%s", buf);
        }
    fclose(fp2);
}