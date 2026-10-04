#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int k;
    char color;
    struct node *p;
    struct node *left;
    struct node *right;
} node;

typedef struct tree {
    node *root;
    node *nil;
} tree;

node *Search(tree *t, int k){
    node *x = t->root;
    while(x != t->nil && x->k != k){
        if(k < x->k){
            x = x->left;
        }
        else{
            x = x->right;
        }
    }
    return x;
}

node *Maximum(tree *t, node* x){
    while(x->right != t->nil){
        x = x->right;
    }
    return x;
}

node *Minimum(tree *t, node* x){
    while(x->left != t->nil){
        x = x->left;
    }
    return x;
}

tree *CreateTree(int k) {
    tree *t = (tree *)malloc(sizeof(tree));
    if (t == NULL) return NULL;
    
    t->nil = (node *)malloc(sizeof(node));
    if (t->nil == NULL) {
        free(t);
        return NULL;
    }

    t->nil->k = -1;
    t->nil->color = 'b'; 
    t->nil->p = t->nil;
    t->nil->left = t->nil;
    t->nil->right = t->nil;

    t->root = (node *)malloc(sizeof(node));
    if (t->root == NULL) {
        free(t->nil);
        free(t);
        return NULL;
    }
    
    t->root->k = k;
    t->root->color = 'b'; 
    t->root->p = t->nil;
    t->root->left = t->nil;
    t->root->right = t->nil;
    return t;
}

void RightRotate(tree *t, node *x) {
    node *aux = x->left;
    x->left = aux->right;
    if (aux->right != t->nil){
        aux->right->p = x;
    }
    aux->p = x->p;
    if (aux->p == t->nil){
        t->root = aux;
    } else if (x == aux->p->right){
        aux->p->right = aux;
    } else {
        aux->p->left = aux;
    }
    aux->right = x;
    x->p = aux;
}

void LeftRotate(tree *t, node *x) {
    node *aux = x->right;
    x->right = aux->left;
    if (aux->left != t->nil){
        aux->left->p = x;
    }
    aux->p = x->p;
    if (aux->p == t->nil){
        t->root = aux;
    } else if (x == aux->p->left){
        aux->p->left = aux;
    } else {
        aux->p->right = aux;
    }
    aux->left = x;
    x->p = aux;
}

void InsertFixup(tree *t, node *v) {
    while (v->p->color == 'r') {
        if (v->p == v->p->p->left) {
            node *u = v->p->p->right;

            if (u->color == 'r') {
                    v->p->color = 'b';
                    u->color = 'b';
                    v->p->p->color = 'r';
                    v = v->p->p;
            } else {
                if (v == v->p->right) {
                    v = v->p;
                    LeftRotate(t, v);
                }
                v->p->color = 'b';
                v->p->p->color = 'r';
                RightRotate(t, v->p->p);
            }
        }
        else {
            node *u = v->p->p->left;

            if (u->color == 'r') {
                v->p->color = 'b';
                u->color = 'b';
                v->p->p->color = 'r';
                v = v->p->p;
            } else {
                if (v == v->p->left) {
                    v = v->p;
                    RightRotate(t, v);
                }
                v->p->color = 'b';
                v->p->p->color = 'r';
                LeftRotate(t, v->p->p);
            }
        }
    }
    t->root->color = 'b';
}

node *Insert(tree *t, int k) {//pag 268
    node *newNode = (node *)malloc(sizeof(node));
    if (newNode == NULL) return NULL;
    newNode->k = k;
    node *x = t->root, *y = t->nil;
    while (x != t->nil) {
        y = x;
        if (newNode->k < x->k) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    newNode->p = y;
    if (y == t->nil) {
        t->root = newNode;
    } else if (newNode->k < y->k) {
        y->left = newNode;
    } else {
        y->right = newNode;
    }
    newNode->left = t->nil;
    newNode->right = t->nil;
    newNode->color = 'r';  
    InsertFixup(t, newNode);

    return newNode;
}

void Transplant(tree *t, node *u, node *v){
    if (u->p == t->nil){
        t->root = v;
    }
    else if(u == u->p->left){
        u->p->left = v;
    }
    else{
        u->p->right = v;
    }
    v->p = u->p;
}

void DeleteFixup(tree *t, node *x){ // pag 326
    node *w;
    while(x != t->root && x->color == 'b'){
        if(x == x->p->left){
            w = x->p->right;
            if(w->color == 'r'){
                w->color = 'b';
                x->p->color = 'r';
                LeftRotate(t, x->p);
                w = x->p->right;
            }
            if(w->left->color == 'b' && w->right->color == 'b'){
                w->color = 'r';
                x = x->p;
            }
            else{
                if(w->right->color == 'b'){
                    w->left->color = 'b';
                    w->color = 'r';
                    RightRotate(t, w);
                    w = x->p->right;
                }
                w->color = x->p->color;
                x->p->color = 'b';
                w->right->color = 'b';
                LeftRotate(t, x->p);
                x = t->root;
            }
        }
        else{
            w = x->p->left;
            if(w->color == 'r'){
                w->color = 'b';
                x->p->color = 'r';
                RightRotate(t, x->p);
                w = x->p->left;
            }
            if(w->right->color == 'b' && w->left->color == 'b'){
                w->color = 'r';
                x = x->p;
            }
            else{
                if(w->left->color == 'b'){
                    w->right->color = 'b';
                    w->color = 'r';
                    LeftRotate(t, w);
                    w = x->p->left;
                }
                w->color = x->p->color;
                x->p->color = 'b';
                w->left->color = 'b';
                RightRotate(t, x->p);
                x = t->root;
            }
        }
    }
    x->color = 'b';
}

void Delete(tree *t, node *z){ //pag 275
    node *y = z, *x;
    char y_og_color = y->color;

    if (z->left == t->nil){
        x = z->right;
        Transplant(t, z, z->right);
    }
    else if(z->right == t->nil){
        x = z->left;
        Transplant(t, z, z->left);
    }
    else{
        y = Maximum(t, z->left);
        y_og_color = y->color;
        x = y->left;
        if(y->p == z){
            x->p = y;
        }
        else{
            Transplant(t, y, y->left);
            y->left = z->left;
            y->left->p = y;
        }
        Transplant(t,z,y);
        y->right = z->right;
        y->right->p = y;
        y->color = z->color;
    }
    if(y_og_color == 'b'){
        DeleteFixup(t, x);
    }
    free(z);
}

void printTree(node* n){
    if(n == n->left){
        return;
    }
    if(n->color == 'r'){
        printf("%d RED\n",n->k);
    }else{
        printf("%d BLACK\n", n->k);
    }
    printTree(n->left);
    printTree(n->right);
    return;
}



int main() {
    int n;
    scanf("%d", &n);
    tree *t = CreateTree(n);

    while(scanf("%d", &n) != EOF){
        if(n == -1){
            Delete(t, Minimum(t, t->root));
        }
        else{
            Insert(t, n);
        }
    }

    printTree(t->root);
    
    return 0;
}