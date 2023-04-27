#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef enum Color { RED, BLACK } Color;

struct Node {
    int key;
    Color color;
    struct Node* left;
    struct Node* right;
    struct Node* parent;
};

struct RBTree {
    struct Node* root;
    struct Node* nil;
};

// Creating Node
struct Node* createNode(int key, Color color, struct Node* nil) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->key = key;
    node->color = color;
    node->left = nil;
    node->right = nil;
    node->parent = nil;
    return node;
}

// Creating RB Tree
struct RBTree* createRBTree() {
    struct RBTree* tree = (struct RBTree*)malloc(sizeof(struct RBTree));
    struct Node* nil = (struct Node*)malloc(sizeof(struct Node));
    nil->key = -1;
    nil->color = BLACK;
    nil->left = nil->right = nil->parent = nil;
    tree->nil = nil;
    tree->root = nil;
    return tree;
}

// Left Rotate
void leftRotate(struct RBTree* tree, struct Node* x) {
    struct Node* y = x->right;
    x->right = y->left;
    if (y->left != tree->nil) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == tree->nil) {
        tree->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

// Right Rotate
void rightRotate(struct RBTree* tree, struct Node* y) {
    struct Node* x = y->left;
    y->left = x->right;
    if (x->right != tree->nil) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == tree->nil) {
        tree->root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }
    x->right = y;
    y->parent = x;
}

// RB Insert Fixup using algorithm on page 316
void RBInsertFixup(struct RBTree* tree, struct Node* z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            struct Node* y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(tree, z->parent->parent);
            }
        } else {
            struct Node* y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }   else {
                if  (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = BLACK;
}

// RB Insert Using Algorithm on page 315
void RBInsert(struct RBTree* tree, struct Node* z) {
    struct Node* y = tree->nil;
    struct Node* x = tree->root;
    while (x != tree->nil) {
        y = x;
        if (z->key < x->key) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    z->parent = y;
    if (y == tree->nil) {
        tree->root = z;
    } else if (z->key < y->key) {
        y->left = z;
    } else {
        y->right = z;
    }
    z->left = z->right = tree->nil;
    z->color = RED;
    RBInsertFixup(tree, z);
}

// RB Transplant Using Algorithm on page 323
void RBTransplant(struct RBTree* tree, struct Node* u, struct Node* v) {
    if (u->parent == tree->nil) {
        tree->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

// find the minimum node
struct Node* TreeMinimum(struct Node* x,struct RBTree* tree) {
    while (x->left != tree->nil) {
        x = x->left;
    }
    return x;
}

struct Node* Successor(struct RBTree* tree, struct Node* x) {
    if (x->right != tree->nil) {
        return TreeMinimum(x->right,tree);
    } else {
        struct Node* y = x->parent;
        while (y != tree->nil && x == y->right) {
            x = y;
            y = y->parent;
        }
        return y;
    }
}
// RB DELETE FIXUP algorithm using page 326
void RBDeleteFixup(struct RBTree* tree, struct Node* x) {
    while (x != tree->root && x->color == BLACK) {
        if (x == x->parent->left) {
            struct Node* w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(tree, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else if (w->right->color == BLACK) {
                w->left->color = BLACK;
                w->color = RED;
                rightRotate(tree, w);
                w = x->parent->right;
            }
            w->color = x->parent->color;
            x->parent->color = BLACK;
            w->right->color = BLACK;
            leftRotate(tree, x->parent);
            x = tree->root;
        } else {
            struct Node* w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(tree, x->parent);
                w = x->parent->left;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else if (w->left->color == BLACK) {
                w->right->color = BLACK;
                w->color = RED;
                leftRotate(tree, w);
                w = x->parent->left;
            }
            w->color = x->parent->color;
            x->parent->color = BLACK;
            w->left->color = BLACK;
            rightRotate(tree, x->parent);
            x = tree->root;
        }
    }
    x->color = BLACK;
}


//RB Delete using algorithm on page 326
void RBDelete(struct RBTree* tree, struct Node* z) {
    struct Node* y = z;
    int y_original_color = y->color;
    struct Node* x = NULL;
    if (z->left == tree->nil) {
        x = z->right;
        RBTransplant(tree, z, z->right);
    } else if (z->right == tree->nil) {
        x = z->left;
        RBTransplant(tree, z, z->left);
    } else {
        y = TreeMinimum(z->right,tree);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            RBTransplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        RBTransplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    if (y_original_color == BLACK) {
        RBDeleteFixup(tree, x);
    }
}


int getNodeHeight(struct Node* node, struct Node* nil) {
    int height = 0;
    while (node != nil ) {
        height++;
        node = node->parent;
    }
    return height;
}

int getMaxHeight(struct Node* node, struct Node* nil) {
    if (node == nil) {
        return 0;
    }
    int leftHeight = getMaxHeight(node->left, nil);
    int rightHeight = getMaxHeight(node->right, nil);
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

int getLeafNum(struct RBTree* tree, struct Node* node){
    int current_num = 0;
    if(node == tree->root){
        return 1;
    } else{
        if(node == (node->parent)->left){
            current_num = 2*getLeafNum(tree,node->parent)-1;
        } else{
            current_num = 2*getLeafNum(tree,node->parent);
        }
    }
    return current_num;
}

int countsize(struct Node* store_node[8][128],int row){
    int i,j = 0;
    for(i = 0; i<15;i++){
        if(store_node[row][i] != NULL){
            j++;  
        }
    }
    return j;
}

int count_prespace(int h,int max_h){
    return pow(2,max_h-h)-1;
}

int count_two_elements_space(int h, int max_h){
    return pow(2,max_h-h+1)-1;
}

void preorderWalk(struct RBTree* tree, struct Node* node,struct Node* store_node[8][128]) {\

    if (node != tree->nil) {
        int h = getNodeHeight(node, tree->nil);
        int max_height = getMaxHeight(tree->root, tree->nil);
        int col = count_prespace(h,max_height) + (getLeafNum(tree,node) -1) * count_two_elements_space(h,max_height) +(getLeafNum(tree,node)-1);
        store_node[h-1][col] = node;
        preorderWalk(tree, node->left,store_node);
        preorderWalk(tree, node->right,store_node);
    }
}

void InitStore(struct Node* store_node[8][128]){
    int i,j;
    for(i = 0; i < 8;i++){
        for(j = 0; j < 128; j++){
            store_node[i][j] = NULL;
        }

    }
}

void printspace(int num){
    int i;
    for(i = 0; i<num;i++){
        printf(" ");
    }
}

struct Node* SEARCH(struct Node* x, int k){
    if( x== NULL || k == x->key){
        return x; 
    } 
    if (k < x->key){
        return SEARCH(x->left,k);
    } else{
        return SEARCH(x->right,k);
    }
}

struct Node* SEARCH_2(struct Node* x, int k,struct RBTree* tree) {
    while (x != tree->nil && k != x->key) {
        if (k < x->key) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    return x;
}

void InitEdge(char edge[128]){
    int i;
    for(i = 0;i<128;i++){
        edge[i] = ' ';
    }
}

void printBST(struct RBTree* tree){
    int max_height = getMaxHeight(tree->root, tree->nil),i,j,k,l;

    struct Node* store[8][128];
    char edge[128];
    InitEdge(edge);
    InitStore(store);
    preorderWalk(tree,tree->root,store);
    for(i = 0; i < max_height; i++){
        InitEdge(edge);
        for(j = 0; j < 128; j++){

            if(store[i][j] == NULL){
                printf("  ");
            } else{
                if(store[i][j]->left != tree->nil){
                    k = count_prespace(i+1,max_height) + (getLeafNum(tree,store[i][j]) -1) * count_two_elements_space(i+1,max_height) +(getLeafNum(tree,store[i][j])-1);
                    l = count_prespace(i+2,max_height) + (getLeafNum(tree,store[i][j]->left) -1) * count_two_elements_space(i+2,max_height) +(getLeafNum(tree,store[i][j]->left)-1);
                    edge[(l+k)/2] = '/';
                }
                switch (store[i][j]->color) {
                    case 0:
                        printf("\033[31m");
                        printf("%d", store[i][j]->key);
                        printf("\033[0m");
                        break;
                    case 1:
                        printf("\033[30m");
                        printf("%d", store[i][j]->key);
                        printf("\033[0m");
                        break;
                }
                if(store[i][j]->right != tree->nil){
                    k = count_prespace(i+1,max_height) + (getLeafNum(tree,store[i][j]) -1) * count_two_elements_space(i+1,max_height) +(getLeafNum(tree,store[i][j])-1);
                    l = count_prespace(i+2,max_height) + (getLeafNum(tree,store[i][j]->right) -1) * count_two_elements_space(i+2,max_height) +(getLeafNum(tree,store[i][j]->right)-1);
                    edge[(l+k)/2] = '\\';
                }
            }
        }
        printf("\n");
        for(k = 0; k < 128; k ++){
            printf("%c ",edge[k]);
        }
        printf("\n");
    }

}

// Judge whether value is in arr[]
int judge_contain(int arr[], int size, int value){
    for (int i = 0; i < size; i++) {
        if (arr[i] == value) {
            return 1;
        }
    }
    return 0;
}

void print_Node(struct Node* node,struct RBTree* tree){
    if(node != tree->nil){
        printf("Find Value:%d\n",node->key);
    } else{
        printf("NIL\n");
    }
}

int main() {
    int i, A[15],new_value;

    // Question 2
    struct RBTree* tree = createRBTree();
    srand(time(NULL));
    for(i = 0; i < 15; i++){
        new_value = rand() % 50;
        while(judge_contain(A,i+1,new_value)){
            new_value = rand() % 50;
        }
        A[i] = new_value;
        RBInsert(tree,createNode(A[i], RED, tree->nil));
    }
    printf("This is the initial RB Tree:\n");
    printBST(tree);


    // Question 3
    printf("Add 12 to the RB Tree\n");
    RBInsert(tree,createNode(12, RED, tree->nil));
    printBST(tree);
        
    printf("Add 15 to the RB Tree\n");
    RBInsert(tree,createNode(15, RED, tree->nil));
    printBST(tree);

    printf("Add 41 to the RB Tree\n");
    RBInsert(tree,createNode(41, RED, tree->nil));
    printBST(tree);

    printf("Add 43 to the RB Tree\n");
    RBInsert(tree,createNode(43, RED, tree->nil));
    printBST(tree);

    printf("Add 5 to the RB Tree\n");
    RBInsert(tree,createNode(5, RED, tree->nil));
    printBST(tree);
    
    // Question 4
    print_Node(SEARCH_2(tree->root,9,tree),tree);
    print_Node(SEARCH_2(tree->root,42,tree),tree);
    print_Node(SEARCH_2(tree->root,26,tree),tree);
    print_Node(SEARCH_2(tree->root,3,tree),tree);
    print_Node(SEARCH_2(tree->root,11,tree),tree);

    // Question 5
    int random = rand()%14;
    int random2 = rand()%14;
    while(random == random2){
        random2 = rand()%14;
    }
    printf("Delete 15 to the RB Tree\n");
    RBDelete(tree,SEARCH_2(tree->root,15,tree));
    printBST(tree);

    printf("Delete %d to the RB Tree\n",A[random]);
    RBDelete(tree,SEARCH_2(tree->root,A[random],tree));
    printBST(tree);

    printf("Delete 43 to the RB Tree\n");
    RBDelete(tree,SEARCH_2(tree->root,43,tree));
    printBST(tree);

    printf("Delete 12 to the RB Tree\n");
    RBDelete(tree,SEARCH_2(tree->root,12,tree));
    printBST(tree);


    printf("Delete %d to the RB Tree\n",A[random2]);
    RBDelete(tree,SEARCH_2(tree->root,A[random2],tree));
    printBST(tree);
    return 0;
}
