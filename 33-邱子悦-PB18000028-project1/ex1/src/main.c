#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define POW2_18 262144
#define POW2_15 32768

typedef void (*SORT)(int);

void INSERTION_SORT(int n);
void HEAP_SORT(int n);
void QUICK_SORT(int n);
void MERGE_SORT(int n);
void COUNTING_SORT(int n);
/* for test
#define POW2_5 32 */

int A[POW2_18]; // input array
int B[POW2_18]; // output array

const char * string_list[] = {"INSERTION_SORT",\
                            "HEAP_SORT",\
                            "QUICK_SORT",\
                            "MERGE_SORT",\
                            "COUNTING_SORT"};

// function list
SORT function_list[] = {INSERTION_SORT,\
                            HEAP_SORT,\
                            QUICK_SORT,\
                            MERGE_SORT,\
                            COUNTING_SORT};

/* write the txt and generate A[] */
void rand_15_18(){
    //  rand() between 0 and 2^15-1
    //printf("%d\n", time(NULL));
    srand((unsigned)time(NULL));
    FILE *fp = fopen("../input/input.txt", "w");
    char buf[20];
    int num;
    for(int i = 0; i < POW2_18; i++){
        num = rand() % POW2_15;
        A[i] = num;
        memset(buf, '0', 20* sizeof(char));
        sprintf(buf, "%d\n", num);
        //printf("%s", buf);
        fprintf(fp, "%s", buf);
    }
    fclose(fp);
    printf("successfully closed fp!\n");
    return;
}

/* before this function B should be the same as A */
void INSERTION_SORT(int n){
    int key, i;
    for(int j = 1; j < pow(2, n); j++){
        key = B[j];
        i = j-1;
        while(i >= 0 && B[i] > key){
            B[i+1] = B[i];
            i = i-1;
        }
        B[i+1] = key;
    }
    return;
}

/*int PARENT(int i){
    return (i-1)/2;
}*/
int LEFT(int i){
    return 2*i+1;
}
int RIGHT(int i){
    return 2*i+2;
}

void MAX_HEAPIFY(int i, int heapsize){
    int l, r, largest, t;
    l = LEFT(i);
    r = RIGHT(i);
    if(l < heapsize && B[l]>B[i])
        largest = l;
    else
        largest = i;
    if(r < heapsize && B[r]>B[largest])
        largest = r;
    if(largest != i){
        t = B[i];
        B[i] = B[largest];
        B[largest] = t;
        MAX_HEAPIFY(largest, heapsize);
    }
}

void BUILD_MAX_HEAP(int heapsize){
    for(int i = heapsize/2; i >= 0; i--)
        MAX_HEAPIFY(i, heapsize);
}

void HEAP_SORT(int n){
    int heapsize = pow(2, n);
    BUILD_MAX_HEAP(heapsize);
    int t;
    for(int i = pow(2, n)-1; i >= 1; i--){
        t = B[0];
        B[0] = B[i];
        B[i] = t;
        heapsize--;
        MAX_HEAPIFY(0, heapsize);
    }
}

int PARTITION(int p, int r){
    int x = B[r];
    int i = p-1;
    int t;
    for(int j = p; j < r; j++){
        if(B[j] <= x){
            i++;
            t = B[i];
            B[i] = B[j];
            B[j] = t;
        }
    }
    t = B[i+1];
    B[i+1] = B[r];
    B[r] = t;
    return i+1;
}

void QUICKSORT(int p, int r){
    if(p<r){
        int q = PARTITION(p, r);
        QUICKSORT(p, q-1);
        QUICKSORT(q+1, r);
    }
    return;
}

void QUICK_SORT(int n){
    QUICKSORT(0, pow(2, n)-1);
    return;
}

void MERGE(int p, int q, int r){
    int m = q-p+1;
    int n = r-q;
    int *ptr_m = (int*)malloc(sizeof(int)*(m+1));
    int *ptr_n = (int*)malloc(sizeof(int)*(n+1));
    int i, j;

    for(i = 0; i < m; i++)
        ptr_m[i] = B[p+i];
    for(j = 0; j < n; j++)
        ptr_n[j] = B[q+j+1];
    ptr_m[m] = POW2_15;
    ptr_n[n] = POW2_15;
    //printf("%d %d\n", ptr_m[m], ptr_n[n]);
    i = 0;
    j = 0;
    for(int k = p; k <= r; k++){
        if(ptr_m[i] <= ptr_n[j]){
            B[k] = ptr_m[i];
            i++;
        }
        else{
            B[k] = ptr_n[j];
            j++;
        }
    }
    free(ptr_m);
    free(ptr_n);
    return;
}

void MERGESORT(int p, int r){
    if(p < r){
       int q = (p+r)/2;
       MERGESORT(p, q);
       MERGESORT(q+1, r);
       MERGE(p, q, r);
    }
    return;
}

void MERGE_SORT(int n){
    MERGESORT(0, pow(2, n)-1);
    return;
}

void COUNTING_SORT(int n){
    /*
     * input n = pow(2,n) from 0 to k = 2^15-1
     */
    int C[POW2_15];
    memset(C, 0, sizeof(int)*POW2_15);
    int length = pow(2, n);
    for(int j = 0; j < length; j++)
        C[A[j]] = C[A[j]]+1;
    for(int i = 1; i < POW2_15; i++)
        C[i] += C[i-1];
    for(int j = length-1; j >= 0; j--){
        //printf("%d", C[A[j]]);
        B[C[A[j]]-1] = A[j];
        C[A[j]] = C[A[j]]-1;
    }
    //printf("\n");
    return;
}

void write_output_time(char *array, int len, int i){
    memset(array, '\0', sizeof(char)*len);
    strcpy(array, "../output/");
    strcat(array, string_list[i]);
    strcat(array, "/time.txt");
    //printf("%s\n", array);
    return;
}

void write_output_result(char *array, int len, int i){
    memset(array, '\0', sizeof(char)*len);
    strcpy(array, "../output/");
    strcat(array, string_list[i]);
    strcat(array, "/result_");
    //printf("%s\n", array);
    return;
}

int main(){
    printf("Let's start!\n");
    //printf("RAND_MAX = %d\n", RAND_MAX);
    // generate random numbers
    rand_15_18();

    int n, num;
    clock_t start_t, end_t;
    double total_t;
    char buf[40], string[40];
    for(int i = 0; i < 5; i++){
        write_output_time(string, 40, i);
        FILE *fp1 = fopen(string, "w");

        for(n = 3; n <= 18; n = n+3) {
            // initialization
            memcpy(B, A, pow(2, n) * sizeof(int));
            // time
            start_t = clock();
            function_list[i](n);
            end_t = clock();
            total_t = (double) (end_t - start_t) / CLOCKS_PER_SEC;
            printf("n=%d, sort%d time=%fs\n", n, i, total_t);
            // fill output time.txt
            memset(buf, '0', 40 * sizeof(char));
            sprintf(buf, "%f\n", total_t);
            fprintf(fp1, "%s", buf);
            // generate output result.txt
            memset(buf, '0', 40 * sizeof(char));
            write_output_result(string, 40, i);
            sprintf(buf, "%s%d%s", string, n, ".txt");
            FILE *rp = fopen(buf, "w");
            for(int i = 0; i < pow(2, n); i++){
                num = B[i];
                memset(buf, '0', 40 * sizeof(char));
                sprintf(buf, "%d\n", num);
                fprintf(rp, "%s", buf);
            }
            fclose(rp);
            printf("successfully closed rp!\n");
        }
        fclose(fp1);
        printf("successfully closed fp1!\n");
    }
}

