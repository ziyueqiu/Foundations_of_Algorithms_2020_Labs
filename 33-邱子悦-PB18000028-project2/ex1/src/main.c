#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

// global variables
long int array[26];
int N;
long int m[25][25];
int s[24][25];

void MATRIX_CHAIN_ORDER(long int p[], int len){
    int n = len-1;
    int i, l, j, k;
    long int q;
    for(i = 0; i < n; i++)
        m[i][i] = 0;
    for(l = 1; l < n; l++)
        for(i = 0; i<n-l+1; i++){
            j = i+l;
            m[i][j] = 0x7fffffffffffffff; // max
            //printf("%ld\n", m[i][j]);
            for(k = i; k <= j-1; k++){
                q = p[i]*p[k+1]*p[j+1]+m[i][k]+m[k+1][j];
                if(q < m[i][j]){
                    m[i][j] = q;
                    s[i][j] = k;
                }
            }
        }
    return;
}

void PRINT_OPTIMAL_PARENS(int i, int j, FILE *fp){
    if(i == j)
        fprintf(fp, "A%d", i+1);
    else{
        fprintf(fp, "(");
        PRINT_OPTIMAL_PARENS(i, s[i][j], fp);
        PRINT_OPTIMAL_PARENS(s[i][j]+1, j, fp);
        fprintf(fp, ")");
    }
}


int main() {
    clock_t start_t, end_t;
    double total_t;
    char buf[40];
    char parens[100];
    FILE *fp = fopen("../input/2_1_input.txt", "r");
    FILE *fp1 = fopen("../output/time.txt", "w");
    FILE *fp2 = fopen("../output/result.txt", "w");
    while(fscanf(fp, "%d", &N)!=EOF){
        for(int i = 0; i<=N; i++){
            fscanf(fp, "%ld", &array[i]);
        }
        // time
        start_t = clock();
        MATRIX_CHAIN_ORDER(array, N+1);
        end_t = clock();
        total_t = (double) (end_t - start_t) / CLOCKS_PER_SEC;
        //printf("n=%d, time=%fs\n", N, total_t);
        if(N == 5){
            for(int i = 0; i < N; i++) {
                for (int j = i; j < N; j++)
                    printf("%ld ", m[i][j]);
                printf("\n");
            }
        }
        memset(buf, '\0', 40 * sizeof(char));
        sprintf(buf, "%f\n", total_t);
        fprintf(fp1, "%s", buf);
        // result
        memset(buf, '\0', 40 * sizeof(char));
        sprintf(buf, "%ld\n", m[0][N-1]);
        fprintf(fp2, "%s", buf);
        // parens
        memset(parens, '\0', 100* sizeof(char));
        PRINT_OPTIMAL_PARENS(0, N-1, fp2);
        fprintf(fp2,"\n");
    }

    fclose(fp);
    fclose(fp1);
    fclose(fp2);
    //printf("successfully closed fp!\n");
    return 0;
}