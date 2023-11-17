#include "symtable.h"


int max(int n1, int n2){
    if(n1>n2){
        return n1;
    }else{
        return n2;
    }
}

int bHeightBS(bStrom *root){
    if(root != NULL){
        return(max(bHeightBS(root->lPtr), bHeightBS(root->rPtr))+1);
    }else{
        return 0;
    }
}

//vracia 0 ak je uzol vyvazeni 1 ked lavi je vetci a -1 ked pravi je vetsi
int bVyvazenost(bStrom *root){
    return (bHeightBS(root->lPtr)-bHeightBS(root->rPtr));
}

bStrom *bCreate(int key, void *data){
    bStrom *newElement = (bStrom*)malloc(sizeof(bStrom));
    if( newElement == NULL){
        fprintf(stderr, "Error in malloc(pridanie noveho prvku do bstrom): %d\n", 99);
        return NULL;
    }
    newElement->data = data;
    newElement->key = key;
    newElement->height = 1;
    newElement->lPtr = NULL;
    newElement->rPtr = NULL;

    return (newElement);
}

bool bChecknext(bStrom *root){
    return ((root != NULL) ? true : false);
}

bStrom *bRightRotate(bStrom *root){
    bStrom *tmp = root->lPtr;
    root->lPtr = tmp->rPtr;
    tmp->rPtr = root;
    //oprava vyze prohozenich uzlu
    root->height = max(bHeightBS(root->lPtr), bHeightBS(root->rPtr))+1;
    tmp->height = max(bHeightBS(tmp->lPtr), bHeightBS(tmp->rPtr))+1;
    return tmp;

}

bStrom *bLeftRotate(bStrom *root){
    bStrom *tmp = root->rPtr;
    root->rPtr = tmp->lPtr;
    tmp->lPtr = root;
    root->height = max(bHeightBS(root->lPtr), bHeightBS(root->rPtr))+1;
    tmp->height = max(bHeightBS(tmp->lPtr), bHeightBS(tmp->rPtr))+1;
    return tmp;
}


bStrom *bInsert(bStrom *root, int key, void *data){
    if( root == NULL){
        return bCreate(key, data);
    }else{
        if( key < root->key){
            root->lPtr = bInsert(root->lPtr, key, data);
        }else {
            if( key > root->key){
                root->rPtr = bInsert(root->rPtr, key, data);
            }else{
                root->data = data;
            }
        }

        root->height = (max(bHeightBS(root->lPtr), bHeightBS(root->rPtr))+1);

        int balanc = bVyvazenost(root);
        
        if(balanc < -1){ //pravi podstrom je vetsi
            if( key > root->rPtr->key){ //RR
                return bLeftRotate(root);
            }else{//RL
                root->rPtr = bRightRotate(root->rPtr);
                return bLeftRotate(root);
            }
        }else if( balanc > 1){//lavi podstrom je vetsi
            if( key < root->lPtr->key){ //LL                
                return bRightRotate(root);
            }else{//LR            
                root->lPtr = bLeftRotate(root->lPtr);
                return bRightRotate(root);
            }
        }
        return root;
    }
  
}

bStrom *bMinL(bStrom *root){
    if(root->lPtr == NULL){
        
        return root;
    }else{
        
        return bMinL(root->lPtr);
    }
}

bStrom *bMinR(bStrom *root){
    if(root->rPtr == NULL){
        return root;
    }else{
        return bMinR(root->rPtr);
    }
}




bStrom *bDeleteOne(bStrom *root, int key) {
    if (root == NULL) {
        return NULL;
    } else {
        if (key < root->key) {
            root->lPtr = bDeleteOne(root->lPtr, key);
        } else if (key > root->key) {
            root->rPtr = bDeleteOne(root->rPtr, key);
        } else { // nasli sme
            if (root->lPtr == NULL || root->rPtr == NULL) {
                bStrom *tmp = (root->lPtr != NULL) ? root->lPtr : root->rPtr;

                if (tmp == NULL) { // iba jedno dieta
                    tmp = root;
                    root = NULL;
                } else { // ziaden dieta
                    *root = *tmp; // Copy the contents of the non-empty child
                }
                free(tmp);
            } else { // dve deti
                bStrom *min = (bVyvazenost(root) == -1) ? bMinL(root->rPtr) : bMinR(root->lPtr);
                root->key = min->key;
                root->data = min->data;
                root->lPtr = bDeleteOne(root->lPtr, min->key);
                root->rPtr = bDeleteOne(root->rPtr, min->key);
            }
        }

        if (root == NULL) {
            return root;
        }

        // opravime vysku
        root->height = max(bHeightBS(root->lPtr), bHeightBS(root->rPtr)) + 1;

        // kontrola vyvazenosti
        int balance = bVyvazenost(root);

        // lava je tazsia
        if (balance > 1) {
            if (bVyvazenost(root->lPtr) >= 0) {
                return bRightRotate(root);
            } else {
                root->lPtr = bLeftRotate(root->lPtr);
                return bRightRotate(root);
            }
        }

        // prava je tazsia
        if (balance < -1) {
            if (bVyvazenost(root->rPtr) <= 0) {
                return bLeftRotate(root);
            } else {
                root->rPtr = bRightRotate(root->rPtr);
                return bLeftRotate(root);
            }
        }

        return root;
    }
}

void bDestroyR(bStrom *ptr){
    if(ptr != NULL){
        bDestroyR(ptr->lPtr);
        bDestroyR(ptr->rPtr);
        free(ptr);
    }
}





void bPreOrder(bStrom *root){
  if(root != NULL){
    char *a = (char *)root->data;
    printf("Data: %s\n", a);
    bPreOrder(root->lPtr);
    bPreOrder(root->rPtr);
  }
}

void bInOrder(bStrom *root){
    if(root != NULL){
        char *a = (char *)root->data;
        bInOrder(root->lPtr);
        printf("Data: %s\n", a);
        bInOrder(root->rPtr);
    }
}

void bPostOrder(bStrom *root){
    if(root != NULL){
        char *a = (char *)root->data;
        bPostOrder(root->lPtr);
        bPostOrder(root->rPtr);
        printf("Data: %s\n", a);
    }
}
