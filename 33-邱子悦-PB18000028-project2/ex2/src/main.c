#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define PAI 3.14159265

// global variables
int N;
double array[256];

complex double *RECURSIVE_FFT(complex double a[], int n){
    if(n == 1)
        return a;
    complex double w_n = cos(2*PAI/n) + sin(2*PAI/n)*_Complex_I;
    complex double w = 1;
    complex double *a_0 = (complex double *)malloc(sizeof(complex double)*n/2);
    complex double *a_1 = (complex double *)malloc(sizeof(complex double)*n/2);
    for(int i = 0; i < n/2; i++){
        a_0[i] = a[2*i];
        a_1[i] = a[2*i+1];
    }
    complex double *y_0 = RECURSIVE_FFT(a_0, n/2);
    complex double *y_1 = RECURSIVE_FFT(a_1, n/2);
    complex double *y = (complex double *)malloc(sizeof(complex double)*n);
    for(int k = 0; k < n/2; k++){
        y[k] = y_0[k] + w*y_1[k];
        y[k+n/2] = y_0[k] - w*y_1[k];
        w *= w_n;
    }
    free(a_0);
    free(a_1);
    return y;
}

int main() {
    char buf[40];

    clock_t start_t, end_t;
    double total_t;

    FILE *fp = fopen("../input/2_2_input.txt", "r");
    //FILE *fp = fopen("../input/test.txt", "r");
    FILE *fp1 = fopen("../output/time.txt", "w");
    FILE *fp2 = fopen("../output/result.txt", "w");

    while(fscanf(fp, "%d", &N)!=EOF){
        complex double *array_complex = (complex double*)malloc(sizeof(complex double)*N);
        for(int i = 0; i < N; i++){
            fscanf(fp, "%lf", &array[i]);
            array_complex[i] = array[i];
        }
        // time.txt
        start_t = clock();
        complex double *y = RECURSIVE_FFT(array_complex, N);
        end_t = clock();
        total_t = (double) (end_t - start_t) / CLOCKS_PER_SEC;
        printf("n=%d, time=%fs\n", N, total_t);
        memset(buf, '\0', 40 * sizeof(char));
        sprintf(buf, "%lf\n", total_t);
        fprintf(fp1, "%s", buf);
        // result.txt
        for(int i = 0; i < N-1; i++){
            memset(buf, '\0', 40 * sizeof(char));
            sprintf(buf, "%f ", creal(y[i]));
            fprintf(fp2, "%s", buf);
            if(N == 8){
                printf("%f ", creal(y[i]));
            }
        }
        memset(buf, '\0', 40 * sizeof(char));
        sprintf(buf, "%f\n", creal(y[N-1]));
        fprintf(fp2, "%s", buf);
        if(N == 8){
            printf("%f\n", creal(y[N-1]));
        }
    }
    fclose(fp);
    fclose(fp1);
    fclose(fp2);
    return 0;
}