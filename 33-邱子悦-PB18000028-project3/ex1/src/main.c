#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h> // C99

#define MAX_LEN 100
#define BUF_LEN 20
#define RED true
#define BLACK false

// global variables
int array[MAX_LEN];
int delete_data[MAX_LEN/4];

// function declaration
void rand_input(int n, FILE *fp1);

/*
void rand_input(int n, FILE *fp1){
    srand((unsigned)time(NULL));
    char buf[BUF_LEN];
    int num;
    for(int i = 0; i < n-1; i++){
        num = rand() % 256;
        array[i] = num;
        memset(buf, '\0', BUF_LEN * sizeof(char));
        sprintf(buf, "%d ", num);
        fprintf(fp1, "%s", buf);
    }
    num = rand() % 1024;
    array[n-1] = num;
    memset(buf, '\0', BUF_LEN* sizeof(char));
    sprintf(buf, "%d\n", num);
    fprintf(fp1, "%s", buf);

    return;
}*/
void rand_input(int n, FILE *fp){
    for(int i = 0; i < n; i++){
        fscanf(fp, "%d", &array[i]);
    }
    return;
}

// red_black_tree
typedef struct rbtnode{
    bool color;
    int key;
    struct rbtnode *left, *right, *p;
} RBTNode;
typedef struct rbt{
    RBTNode *nil, *root;
} RBT;

void LEFT_ROTATE(RBT *T, RBTNode *x);
void RIGHT_ROTATE(RBT *T, RBTNode *x);
void RB_INSERT(RBT *T, RBTNode *z);
void RB_INSERT_FIXUP(RBT *T, RBTNode *z);
void RB_INSERT_KEY(RBT *T, int key);
void InOrderTraverse(RBTNode *T, RBTNode *nil, FILE *fp);

void RB_TRANSPLANT(RBT *T, RBTNode *u, RBTNode *v);
RBTNode *TREE_MINIMUM(RBT *T, RBTNode *x);
void RB_DELETE(RBT *T, RBTNode *z);
void RB_DELETE_FIXUP(RBT *T, RBTNode *x);
void rand_delete(int n, FILE *fp);
void RB_DELETE_KEY(RBT *T, int key);
RBTNode *RB_SEARCH(RBT *T, RBTNode *x, int key);

void LEFT_ROTATE(RBT *T, RBTNode *x){
    RBTNode *y = x->right;
    x->right = y->left;
    if(y->left != T->nil)
        y->left->p = x;
    y->p = x->p;
    if(x->p == T->nil)
        T->root = y;
    else if(x == x->p->left)
        x->p->left = y;
    else
        x->p->right = y;
    y->left = x;
    x->p = y;
}

void RIGHT_ROTATE(RBT *T, RBTNode *x){
    RBTNode *y = x->left;
    x->left = y->right;
    if(y->right != T->nil)
        y->right->p = x;
    y->p = x->p;
    if(x->p == T->nil)
        T->root = y;
    else if(x == x->p->left)
        x->p->left = y;
    else
        x->p->right = y;
    y->right = x;
    x->p = y;
}

void RB_INSERT(RBT *T, RBTNode *z){
    RBTNode *y = T->nil, *x = T->root;
    while(x != T->nil){
        y = x;
        if(z->key < x->key)
            x = x->left;
        else
            x = x->right;
    }
    z->p = y;
    if(y == T->nil)
        T->root = z;
    else if(z->key < y->key)
        y->left = z;
    else
        y->right = z;
    z->left = T->nil;
    z->right = T->nil;
    z->color = RED;
    RB_INSERT_FIXUP(T, z);
}

void RB_INSERT_KEY(RBT *T, int key){
    RBTNode *z = (RBTNode *)malloc(sizeof(RBTNode));
    z->key = key;
    z->left = z->right = z->p = T->nil;
    z->color = RED; // wait to check
    RB_INSERT(T, z);
}

void RB_INSERT_FIXUP(RBT *T, RBTNode *z){
    RBTNode *y;
    //while(z != T->root & z->p->color == RED){
    while(z->p->color == RED){
        if(z->p == z->p->p->left){
            y = z->p->p->right;
            if(y->color == RED){
                z->p->color = BLACK;
                y->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
            }
            else {
                if(z == z->p->right){
                    z = z->p;
                    LEFT_ROTATE(T, z);
                }
                z->p->color = BLACK;
                z->p->p->color = RED;
                RIGHT_ROTATE(T, z->p->p);
            }
        }
        else{ // wait to check
            y = z->p->p->left;
            if(y->color == RED){
                z->p->color = BLACK;
                y->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
            }
            else {
                if (z == z->p->left) {
                    z = z->p;
                    RIGHT_ROTATE(T, z);
                }
                z->p->color = BLACK;
                z->p->p->color = RED;
                LEFT_ROTATE(T, z->p->p);
            }
        }
    }
    T->root->color = BLACK;
}

void InOrderTraverse(RBTNode *T, RBTNode *nil, FILE *fp){
    char buf[BUF_LEN];
    if(T != nil){
        InOrderTraverse(T->left, nil, fp);
        printf("%d ", T->key);
        memset(buf, '0', BUF_LEN * sizeof(char));
        sprintf(buf, "%d ", T->key);
        fprintf(fp, "%s", buf);
        InOrderTraverse(T->right, nil, fp);
    }
    return;
}

void PostOrderTraverse(RBTNode *T, RBTNode *nil){
    if(T != nil){
        PostOrderTraverse(T->left, nil);
        PostOrderTraverse(T->right, nil);
        free(T);
    }
    return;
}

void RB_TRANSPLANT(RBT *T, RBTNode *u, RBTNode *v){
    if(u->p == T->nil)
        T->root = v;
    else if(u == u->p->left)
        u->p->left = v;
    else
        u->p->right = v;
    //if(v!=T->nil)
    v->p = u->p;
    return;
}

RBTNode *TREE_MINIMUM(RBT *T, RBTNode *x){
    while(x->left != T->nil)
        x = x->left;
    return x;
}

void RB_DELETE(RBT *T, RBTNode *z){
    RBTNode *y = z, *x;
    bool y_original_color = y->color;
    if(z->left == T->nil){
        x = z->right;
        RB_TRANSPLANT(T, z, z->right);
    }
    else if(z->right == T->nil){
        x = z->left;
        RB_TRANSPLANT(T, z, z->left);
    }
    else{
        y = TREE_MINIMUM(T, z->right);
        y_original_color = y->color;
        x = y->right;
        if(y->p == z)
            x->p = y;
        else{
            RB_TRANSPLANT(T, y, y->right);
            y->right = z->right;
            y->right->p = y;
        }
        RB_TRANSPLANT(T, z, y);
        y->left = z->left;
        y->left->p = y;
        y->color = z->color;
    }
    if(y_original_color == BLACK)
        RB_DELETE_FIXUP(T, x);
    return;
}

RBTNode *RB_SEARCH(RBT *T, RBTNode *x, int key){
    if(x == T->nil || key == x->key)
        return x;
    else if(key < x->key)
        return RB_SEARCH(T, x->left, key);
    else
        return RB_SEARCH(T, x->right, key);
}

void RB_DELETE_KEY(RBT *T, int key){
    RBTNode *p = RB_SEARCH(T, T->root, key);
    RB_DELETE(T, p);
}

void RB_DELETE_FIXUP(RBT *T, RBTNode *x){
    RBTNode *w;
    while(x != T->root & x->color == BLACK){
        if(x == x->p->left){
            w = x->p->right;
            if(w->color == RED){
                w->color = BLACK;
                x->p->color = RED;
                LEFT_ROTATE(T, x->p);
                w = x->p->right;
            }
            if(w->left->color == BLACK & w->right->color == BLACK){
                w->color = RED;
                x = x->p;
            }
            else{
                if(w->right->color == BLACK){
                    w->left->color = BLACK;
                    w->color = RED;
                    RIGHT_ROTATE(T, w);
                    w = x->p->right;
                }
                w->color = x->p->color;
                x->p->color = BLACK;
                w->right->color = BLACK;
                LEFT_ROTATE(T, x->p);
                x = T->root;
            }
        }
        else {
           w = x->p->left;
            if(w->color == RED){
                w->color = BLACK;
                x->p->color = RED;
                RIGHT_ROTATE(T, x->p);
                w = x->p->left;
            }
            if(w->left->color == BLACK & w->right->color == BLACK){
                w->color = RED;
                x = x->p;
            }
            else{
                if(w->left->color == BLACK){
                    w->right->color = BLACK;
                    w->color = RED;
                    LEFT_ROTATE(T, w);
                    w = x->p->left;
                }
                w->color = x->p->color;
                x->p->color = BLACK;
                w->left->color = BLACK;
                RIGHT_ROTATE(T, x->p);
                x = T->root;
            } 
        }
    }
    x->color = BLACK;
    return;
}

void rand_delete(int n, FILE *fp){
    int index;
    for(int i = 0; i < n; i++){
        fscanf(fp, "%d", &index);
        delete_data[i] = array[index];
    }
    return;
}

int main() {
    // time
    clock_t start_t, end_t;
    double total_t;
    RBT *T;

    FILE *fp1 = fopen("../input/input.txt", "r");
    FILE *fp2 = fopen("../output/time1.txt", "w");
    FILE *fp3 = fopen("../output/inorder.txt", "w");
    FILE *fp4 = fopen("../output/delete_data.txt", "w");
    FILE *fp5 = fopen("../input/delete_index.txt", "r");
    FILE *fp6 = fopen("../output/time2.txt", "w");
    int n;
    char buf[BUF_LEN];
    for(int i = 0; i < 5; i++){
        n = (i+1)*20;
        // initialization
        T = (RBT *)malloc(sizeof(RBT));
        T->nil = (RBTNode *)malloc(sizeof(RBTNode));
        T->nil->color = BLACK;
        T->nil->key = -1;
        T->nil->left = T->nil->right = T->nil->p = T->nil;
        T->root = T->nil;
        // write the input.txt
        rand_input(n, fp1);
        // array is ready!
        start_t = clock();
        for(int j = 0; j < n; j++){
            RB_INSERT_KEY(T, array[j]);
        }
        end_t = clock();
        total_t = (double) (end_t - start_t) / CLOCKS_PER_SEC;
        // fill output time1.txt
        memset(buf, '0', BUF_LEN * sizeof(char));
        sprintf(buf, "%f\n", total_t);
        fprintf(fp2, "%s", buf);
        // inorder.txt
        printf("n = %d insert\n", n);
        InOrderTraverse(T->root, T->nil, fp3);
        printf("\n");
        fprintf(fp3, "\n");
        // deleta_data
        rand_delete(n/4, fp5);
        start_t = clock();
        for(int j = 0; j < n/4; j++){
            RB_DELETE_KEY(T, delete_data[j]);
        }
        end_t = clock();
        total_t = (double) (end_t - start_t) / CLOCKS_PER_SEC;
        memset(buf, '0', BUF_LEN * sizeof(char));
        sprintf(buf, "%f\n", total_t);
        fprintf(fp6, "%s", buf);
        // delete_data.txt
        printf("n = %d delete\n", n);
        InOrderTraverse(T->root, T->nil, fp4);
        printf("\n");
        fprintf(fp4, "\n");
        // free
        PostOrderTraverse(T->root, T->nil);
        free(T->nil);
    }

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);
    fclose(fp5);
    fclose(fp6);
    return 0;
}