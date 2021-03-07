#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> // C99

#define RED true
#define BLACK false
#define BUF_LEN 20

typedef struct Interval {
    int low, high;
} Interval;

// red_black_tree
typedef struct rbtnode{
    bool color;
    Interval interval;
    int max;
    struct rbtnode *left, *right, *p;
} RBTNode;
typedef struct rbt{
    RBTNode *nil, *root;
} RBT;

// function declarations
RBTNode *INTERVAL_INIT_KEY(RBT *T, FILE *fp);
void LEFT_ROTATE(RBT *T, RBTNode *x);
void RIGHT_ROTATE(RBT *T, RBTNode *x);
void RB_INSERT_FIXUP(RBT *T, RBTNode *z);
void RB_INSERT(RBT *T, RBTNode *z);
void InOrderTraverse(RBTNode *T, RBTNode *nil, FILE *fp);
int Max(int a, int b, int c);
void MAX_FIXUP(RBT *T, RBTNode *z);
RBTNode *INTERVAL_SEARCH(RBT *T, Interval i);
void RB_TRANSPLANT(RBT *T, RBTNode *u, RBTNode *v);
RBTNode *TREE_MINIMUM(RBT *T, RBTNode *x);
void RB_DELETE(RBT *T, RBTNode *z);
void RB_DELETE_FIXUP(RBT *T, RBTNode *x);

RBTNode *INTERVAL_INIT_KEY(RBT *T, FILE *fp){
    RBTNode *z = (RBTNode *)malloc(sizeof(RBTNode));
    fscanf(fp, "%d", &z->interval.low);
    fscanf(fp, "%d", &z->interval.high);
    z->left = z->right = z->p = T->nil;
    z->color = RED;
    z->max = z->interval.high;
    return z;
}

int Max(int a, int b, int c){
    if(a > b)
        b = a;
    if(b > c)
        return b;
    else
        return c;
}

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
    // about max
    x->max = Max(x->interval.high, x->left->max, x->right->max);
    y->max = Max(y->interval.high, y->left->max, y->right->max);
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
    // about max
    x->max = Max(x->interval.high, x->left->max, x->right->max);
    y->max = Max(y->interval.high, y->left->max, y->right->max);
}

void MAX_FIXUP(RBT *T, RBTNode *z){
    while(z != T->nil){
        z->max = Max(z->interval.high, z->left->max, z->right->max);
        z = z->p;
    }
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

void RB_INSERT(RBT *T, RBTNode *z){
    RBTNode *y = T->nil, *x = T->root;
    while(x != T->nil){
        y = x;
        if(z->interval.low < x->interval.low)
            x = x->left;
        else
            x = x->right;
    }
    z->p = y;
    if(y == T->nil)
        T->root = z;
    else if(z->interval.low < y->interval.low)
        y->left = z;
    else
        y->right = z;
    z->left = T->nil;
    z->right = T->nil;
    z->color = RED;
    MAX_FIXUP(T, z);
    RB_INSERT_FIXUP(T, z);
}

bool Overlap(Interval a, Interval b){
    if(a.low < b.low)
        if(a.high < b.low)
            return false;
        else
            return true;
    else if(a.low == b.low)
        return true;
    else
    if(b.high < a.low)
        return false;
    else
        return true;
}

RBTNode *INTERVAL_SEARCH(RBT *T, Interval i){
    RBTNode *x = T->root;
    while(x != T->nil && !Overlap(x->interval, i)){
        if(x->left != T->nil && x->left->max >= i.low)
            x = x->left;
        else
            x = x->right;
    }
    return x;
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

RBTNode *RB_SEARCH(RBT *T, RBTNode *x, int key){
    if(x == T->nil || key == x->interval.low)
        return x;
    else if(key < x->interval.low)
        return RB_SEARCH(T, x->left, key);
    else
        return RB_SEARCH(T, x->right, key);
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
    MAX_FIXUP(T, x);
    if(y_original_color == BLACK)
        RB_DELETE_FIXUP(T, x);
    return;
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

void InOrderTraverse(RBTNode *T, RBTNode *nil, FILE *fp){
    char buf[BUF_LEN];
    if(T != nil){
        InOrderTraverse(T->left, nil, fp);
        memset(buf, '0', BUF_LEN * sizeof(char));
        sprintf(buf, "%d %d %d\n", T->interval.low, T->interval.high, T->max);
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

int main() {
    FILE *fp1 = fopen("../input/input.txt", "r");
    FILE *fp2 = fopen("../output/inorder.txt", "w");
    FILE *fp3 = fopen("../input/delete_data.txt", "r");
    FILE *fp4 = fopen("../output/delete_data.txt", "w");
    FILE *fp5 = fopen("../output/search.txt", "w");
    FILE *fp6 = fopen("../input/search_interval.txt", "r");

    char buf[BUF_LEN];

    // initialization
    RBT *T;
    T = (RBT *)malloc(sizeof(RBT));
    T->nil = (RBTNode *)malloc(sizeof(RBTNode));
    T->nil->color = BLACK;
    T->nil->interval.low = T->nil->interval.high = -1;
    T->nil->left = T->nil->right = T->nil->p = T->nil;
    T->nil->max = -1;
    T->root = T->nil;

    // insert
    RBTNode *x;
    for(int i = 0; i < 30; i++){
        x = INTERVAL_INIT_KEY(T, fp1);
        RB_INSERT(T, x);
    }
    InOrderTraverse(T->root, T->nil, fp2);

    // search
    Interval interval;
    for(int i = 0; i < 3; i++){
        fscanf(fp6, "%d", &interval.low);
        fscanf(fp6, "%d", &interval.high);
        x = INTERVAL_SEARCH(T, interval);
        if(x == T->nil)
            fprintf(fp5, "Null\n");
        else
            fprintf(fp5, "%d %d\n", x->interval.low, x->interval.high);
    }

    // delete
    for(int i = 0; i < 3; i++){
        fscanf(fp3, "%d", &interval.low);
        RBTNode *p = RB_SEARCH(T, T->root, interval.low);
        fprintf(fp4, "%d %d\n", p->interval.low, p->interval.high);
        RB_DELETE(T, p);
    }
    InOrderTraverse(T->root, T->nil, fp4);

    // free
    PostOrderTraverse(T->root, T->nil);
    free(T->nil);
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);
    fclose(fp5);
    fclose(fp6);
    return 0;
}