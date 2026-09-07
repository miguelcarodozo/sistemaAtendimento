#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int k;
    int color; // red == 1, black == 0
    struct node *dad;
    struct node *left;
    struct node *right;
} node;

typedef struct Tree {
    node *root;
} Tree;

void RR(Tree *t, node *d) {
    node *aux = d->left;
    if (aux == NULL){ //
        return;
    }
    d->left = aux->right;
    if (aux->right != NULL) {
        aux->right->dad = d;
    }
    aux->right = d;
    aux->dad = d->dad;
    d->dad = aux;

    if (aux->dad == NULL) {
        t->root = aux;
    } else if (d == aux->dad->left) {
        aux->dad->left = aux;
    } else {
        aux->dad->right = aux;
    }
}

void LL(Tree *t, node *d) {
    node *aux = d->right;
    if (aux == NULL) {//
        return;
    }
    
    d->right = aux->left;
    if (aux->left != NULL) {
        aux->left->dad = d;
    }
    aux->left = d;
    aux->dad = d->dad;
    d->dad = aux;

    if (aux->dad == NULL) {
        t->root = aux;
    } else if (d == aux->dad->left) {
        aux->dad->left = aux;
    } else {
        aux->dad->right = aux;
    }
}

void RL(Tree *t, node *d) {
    RR(t, d->right);
    LL(t, d);
}

void LR(Tree *t, node *d) {
    LL(t, d->left);
    RR(t, d);
}

void insertFixUp(Tree *t, node *v) {
    while (v != NULL && v->dad != NULL && v->dad->color == 1) {
        if (v->dad == v->dad->dad->left) {
            node *u = v->dad->dad->right;

            if (u != NULL && u->color == 1) {
                v->dad->color = 0;
                u->color = 0;
                v->dad->dad->color = 1;
                v = v->dad->dad;
            } else {
                if (v == v->dad->right) {
                    v = v->dad;
                    LL(t, v);
                }

                node *g = v->dad->dad;
                RR(t, g);

                v->dad->color = 0;
                g->color = 1;
            }
        } else {
            node *u = v->dad->dad->left;

            if (u != NULL && u->color == 1) {
                v->dad->color = 0;
                u->color = 0;
                v->dad->dad->color = 1;
                v = v->dad->dad;
            } else {
                if (v == v->dad->left) {
                    v = v->dad;
                    RR(t, v);
                }

                node *g = v->dad->dad;
                LL(t, g);

                v->dad->color = 0;
                g->color = 1;
            }
        }
    }

    if (t->root != NULL) {
        t->root->color = 0;
    }
}
void insert(Tree *t, node *v) {
    node *y = NULL;
    node *x = t->root;
    while (x != NULL) {
        y = x;
        if (v->k < x->k) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    v->dad = y;
    if (y == NULL) {
        t->root = v;
    } else if (v->k < y->k) {
        y->left = v;
    } else {
        y->right = v;
    }
    v->left = NULL;
    v->right = NULL;
    v->color = 1;  
    insertFixUp(t, v);
}

node *createNode(Tree *t, int value) {
    node *newNode = (node *)malloc(sizeof(node));
    if (newNode == NULL) return NULL;
    newNode->k = value;
    newNode->left = NULL;
    newNode->right = NULL;
    insert(t, newNode);
    return newNode;
}

Tree *createTree(int value) {
    Tree *t = (Tree *)malloc(sizeof(Tree));
    if (t == NULL) return NULL;
    
    t->root = (node *)malloc(sizeof(node));
    if (t->root == NULL) {
        free(t);
        return NULL;
    }
    
    t->root->k = value;
    t->root->color = 0; 
    t->root->dad = NULL;
    t->root->left = NULL;
    t->root->right = NULL;
    return t;
}

void printTree(node* n){
    if(n == NULL){
        return;
    }
    printTree(n->left);
    if(n->color){
        printf("%d: VERMELHO\n",n->k);
    }else{
        printf("%d: PRETO\n", n->k);
    }
    printTree(n->right);
    return;
}

int main() {
    Tree *t = createTree(5);
    printTree(t->root);
    printf("\n");
    createNode(t, 6);
    printTree(t->root);
    printf("\n");
    createNode(t, 7);
    printTree(t->root);
    printf("\n");
    createNode(t, 8);
    createNode(t, 9);
    printTree(t->root);
    printf("\n");
    return 0;
} 