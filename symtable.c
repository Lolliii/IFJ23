/*
IFJ Projekt 2023

SYMBOL TABLE

@author Jakub Valeš
@author Milan Takáč
@author Jakub Dyrčík
@author Thu Tra Phamová
*/

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

bStrom *bCreate(char *key, void *data, int data_type){
    bStrom *newElement = (bStrom*)malloc(sizeof(bStrom));
    if( newElement == NULL){
        fprintf(stderr, "Error in malloc(pridanie noveho prvku do bstrom): %d\n", 99);
        return NULL;
    }
    //newElement->data = data;
    newElement->key = (char *)malloc(strlen(key) + 1);
    if (newElement->key == NULL) {
        fprintf(stderr, "Error in malloc for key: %d\n", 99);
        free(newElement);
        return NULL;
    }

    // Copy the key value
    strcpy(newElement->key, key);
    newElement->height = 1;
    newElement->data_typ = data_type;
    newElement->lPtr = NULL;
    newElement->rPtr = NULL;
        
        switch (data_type) {
        case 1: // T_param
            newElement->data = malloc(sizeof(T_param));
            if (newElement->data == NULL) {
                fprintf(stderr, "Error in malloc for T_param data: %d\n", 99);
                free(newElement->key);
                free(newElement);
                return NULL;
            }
            memcpy(newElement->data, data, sizeof(T_param));
            break;
        case 2: // T_func
            newElement->data = malloc(sizeof(T_func));
            if (newElement->data == NULL) {
                fprintf(stderr, "Error in malloc for T_func data: %d\n", 99);
                free(newElement->key);
                free(newElement);
                return NULL;
            }
            memcpy(newElement->data, data, sizeof(T_func));
            break;
        case 3: // T_id
            newElement->data = malloc(sizeof(T_id));
            if (newElement->data == NULL) {
                fprintf(stderr, "Error in malloc for T_id data: %d\n", 99);
                free(newElement->key);
                free(newElement);
                return NULL;
            }
            memcpy(newElement->data, data, sizeof(T_id));
            break;
        default:
            fprintf(stderr, "Unknown data type: %d\n", data_type);
            free(newElement->key);
            free(newElement);
            return NULL;
    }

    return newElement;
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


bStrom *bInsert(bStrom *root, char *key, void *data, int data_type){
    if( root == NULL){
        return bCreate(key, data, data_type);
    }else{
        int compR = strcmp(key, root->key);
        // key < root->key
        if( compR < 0){
            root->lPtr = bInsert(root->lPtr, key, data, data_type);
        }else {
            //key > root->key
            if( compR > 0){
                root->rPtr = bInsert(root->rPtr, key, data, data_type);
            }else{
                root->data = data;
            }
        }

        root->height = (max(bHeightBS(root->lPtr), bHeightBS(root->rPtr))+1);

        int balanc = bVyvazenost(root);
        
        if(balanc < -1){ //pravi podstrom je vetsi
            int cmptmp = strcmp(key, root->rPtr->key);
            //key > root->rPtr->key
            if( cmptmp > 0){ //RR
                return bLeftRotate(root);
            }else{//RL
                root->rPtr = bRightRotate(root->rPtr);
                return bLeftRotate(root);
            }
        }else if( balanc > 1){//lavi podstrom je vetsi
            int cmptmp2 = strcmp(key, root->rPtr->key);
            //key < root->lPtr->key
            if( cmptmp2 < 0){ //LL                
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




bStrom *bDeleteOne(bStrom *root, char *key) {
    if (root == NULL) {
        return NULL;
    } else {
        int stcmp = strcmp(key, root->key);
        //key < root->key
        if ( stcmp < 0) {
            root->lPtr = bDeleteOne(root->lPtr, key);
        //key > root->key
        } else if ( stcmp > 0) {
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
                free(tmp->key);
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
        free(ptr->key);
        free(ptr);
    }
}


bStrom *bsearch_one(bStrom *root, char *search){
    if(root == NULL){
        return NULL;
    }
    int compare = strcmp(search, root->key);
    if(compare > 0){
        return bsearch_one(root->rPtr, search);
    }else if(compare < 0){
        return bsearch_one(root->lPtr, search);
    }else if(compare == 0){
        return root;
    }
        return NULL;
}

//prehodi nam act a vracia ten frame kde to najde inak null
ListElement *bSearch_all(Tlist *t, char*search){
    if( t == NULL || t->first == NULL){
        return NULL;
    }
    t->act = t->first;
    bStrom *tmp = bsearch_one(t->act->data, search);
    while( tmp == NULL && t->act != NULL){
        t->act = t->act->rPtr;
        if(t->act == NULL){//pozor 
            return NULL;
        }
        tmp = bsearch_one(t->act->data, search);
    }
    if(tmp != NULL){
        return t->act;
    }
    return NULL;
}

void bPreOrder(bStrom *root){
  if(root != NULL){
    if(root->data_typ == 1){
        T_param *a = (T_param*)root->data;
        printf("Data: %s\n", a->pName);
    }else if(root->data_typ == 2){
        T_func *b = (T_func*)root->data;
        printf("Data: %s\n", b->name);
    }else{
        T_id *c = (T_id*)root->data;
        printf("Data: %s\n", c->name);
    }
    
    bPreOrder(root->lPtr);
    bPreOrder(root->rPtr);
  }
}

void bInOrder(bStrom *root){
    if(root != NULL){
        
        bInOrder(root->lPtr);
        if(root->data_typ == 1){
            T_param *a = (T_param*)root->data;
            printf("Data: %s\n", a->pName);
        }else if(root->data_typ == 2){
            T_func *b = (T_func*)root->data;
            printf("Data: %s\n", b->name);
        }else{
            T_id *c = (T_id*)root->data;
            printf("Data: %s\n", c->name);
        }
        bInOrder(root->rPtr);
    }
}

void bPostOrder(bStrom *root){
    if(root != NULL){
        bPostOrder(root->lPtr);
        bPostOrder(root->rPtr);
        if(root->data_typ == 1){
            T_param *a = (T_param*)root->data;
            printf("Data: %s\n", a->pName);
        }else if(root->data_typ == 2){
            T_func *b = (T_func*)root->data;
            printf("Data: %s\n", b->name);
        }else{
            T_id *c = (T_id*)root->data;
            printf("Data: %s\n", c->name);
        }
    }
}




Tlist *init_list(){
    Tlist *t = (Tlist *)malloc(sizeof(Tlist));
    if( t == NULL){
        fprintf(stderr, "Error in malloc(init list): %d\n", 99);
        exit(99);
    }
    t->act = NULL;
    t->first = NULL;
    return t;
}

void add_to_Lil(Tlist *t, bStrom *data){
    ListElement *newElement = (ListElement *)malloc(sizeof(ListElement));
    if( newElement == NULL){
        fprintf(stderr, "Error in malloc(pridanie noveho prvku do LinkListu): %d\n", 99);
        exit(99);
    }
    newElement->data = data;
    newElement->rPtr = t->first;
    t->first = newElement;    
    printf("add_to ide dobre\n");
}

void set_act_first_Lil(Tlist *t){
    t->act = t->first;
}

bool isActive_Lil(Tlist *t){
    return t->act != NULL;
}

void InsertAfter_Lil(Tlist *t, bStrom *data){
    if(isActive_Lil(t)){
        ListElement *newElement = (ListElement *)malloc(sizeof(ListElement));
        if( newElement == NULL){
            fprintf(stderr, "Error in malloc(pridanie noveho prvku do LinkListu): %d\n", 99);
            exit(99);
        }
        newElement->data = data;
        newElement->rPtr = t->act->rPtr;
        t->act->rPtr = newElement;
    }
}

void set_value_Lil(Tlist *t, bStrom *data){
    if(isActive_Lil(t)){
        t->act->data = data;
    }
}

void DeleteAfter_Lil(Tlist *t){
    ListElement *tmp;
    if(isActive_Lil(t)){
        if(t->act->rPtr != NULL){
            tmp = t->act->rPtr;
            t->act->rPtr = tmp->rPtr;
            bDestroyR(tmp->data);
            free(tmp);
        }
    }
}

void next_Lil(Tlist *t){
    t->act = t->act->rPtr;
}

void destroy_LilLast(Tlist *t){
    if(isActive_Lil(t)){
        ListElement *store = NULL;
        while(t->act->rPtr != NULL){
            store = t->act;
            t->act = t->act->rPtr;
        }
        if(store != NULL){
            bDestroyR(store->data);
            free(store);
        }else{
            bDestroyR(t->act->data);
            free(t->act);
            t->act = NULL;
            t->first = NULL;
        }
    }
}

//will free even the Tlist *t
void destroy_Lilall(Tlist *t){
    t->act = NULL;
    while (t->first != NULL) {
        ListElement *tmp = t->first;
        t->first = t->first->rPtr;
        bDestroyR(tmp->data);
        free(tmp);
    }
    free(t);
}

