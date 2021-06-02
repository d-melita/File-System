#include "header2.h"

/*inicializa a avl*/
void AVLinit(ligacao* head){
    *head = NULL;
}

/*funcao que mostra o nome da diretoria do node*/
void visit(ligacao h){
    printf("%s\n", h->info->dir);
    return;
}

/*traveser sorted da avl*/
void traverse(ligacao h){
    if (h == NULL){
        return;
    }
    traverse(h->l);
    visit(h);
    traverse(h->r);
}

int altura(ligacao h){
    if (h == NULL)
        return 0;
    return h->info->height;
}

/*rotacao a esquerda*/
ligacao rotL(ligacao h){ 
    int hleft, hright, xleft, xright;
    ligacao x = h->r;
    h->r = x->l;
    x->l = h;

    hleft = altura(h->l);
    hright = altura(h->r);
    h->info->height = hleft > hright ? hleft + 1 : hright + 1;

    xleft = altura(x->l);
    xright = altura(x->r);
    h->info->height = xleft > xright ?  xleft + 1 : xright + 1;

    return x;
}

/*rotacao a direita*/
ligacao rotR(ligacao h){
    int hleft, hright, xleft, xright;
    ligacao x = h->l;
    h->l = x->r;
    x->r = h;

    hleft = altura(h->l);
    hright = altura(h->r);
    h->info->height = hleft > hright ? hleft + 1 : hright + 1;

    xleft = altura(x->l);
    xright = altura(x->r);
    h->info->height = xleft > xright ?  xleft + 1 : xright + 1;

    return x;
}

/*rotacao a esquerda e depois a direita*/
ligacao rotLR(ligacao h){
    if (h == NULL)
        return h;
    h->l = rotL(h->l);
    return rotR(h);
}

/*rotacao a direita e depois a esquerda*/
ligacao rotRL(ligacao h){
    if (h == NULL)
        return h;
    h->r = rotR(h->r);
    return rotL(h);
}


int balance(ligacao h){
    if (h == NULL){
        return 0;
    }
    return altura(h->l) - altura(h->r);
}


/*funcao que vai fazer o equilibrio da avl*/
ligacao AVLbalance(ligacao h){
    int fator, hleft, hright;

    if (h == NULL){
        return h;
    }
    fator = balance(h);
    if (fator > 1){
        if (balance(h->l) >= 0)
            h = rotR(h);
        else
            h = rotLR(h);
    }
    else if (fator < -1){
        if (balance(h->r) <= 0)
            h = rotL(h);
        else
            h = rotRL(h);
    }
    else{
        hleft = altura(h->l);
        hright = altura(h->r);
        h->info->height = hleft > hright ? hleft + 1 : hright + 1;        
    }

    return h;
}

/*introduz um novo node na avl*/
ligacao novo(informacao info, ligacao l, ligacao r){
    ligacao x = (ligacao)malloc(sizeof(struct AVLTree));
    x->info = info;
    x->l = l;
    x->r = r;
    return x;
}

/*cria a informacao de um novo node*/
informacao newNode(char* dir, char* valor){
    DL *lista;
    informacao x = (informacao)malloc(sizeof(struct info));
    lista = (DL*)malloc(sizeof(DL));
    x->tree = (ligacao*)malloc(sizeof(struct AVLTree));
    x->valor = (char*)malloc(sizeof(char)*(strlen(valor)+1));
    x->dir = (char*)malloc(sizeof(char)*(strlen(dir)+1));
    strcpy(x->valor, valor);
    strcpy(x->dir, dir);
    x->height = 1;
    x->subdir = init(lista);
    AVLinit(x->tree);
    return x;
}

/*insere ou modifica um elemento na avl*/
ligacao insert(ligacao h, informacao info, char* dir, char* valor){
    if (h == NULL)
        return novo(info, NULL, NULL);

    if (strcmp(h->info->dir, dir) > 0)
        h->l = insert(h->l, info, dir, valor);
    
    else if (strcmp(h->info->dir, dir) == 0){
        if (strcmp(h->info->valor, "") == 0 || strcmp(valor, "") != 0){
            h->info->valor = (char*)realloc(h->info->valor, (strlen(valor) + 1));
            strcpy(h->info->valor, valor);
        }
        return h;
    }

    else
        h->r = insert(h->r, info, dir, valor);
    h = AVLbalance(h);
    return h;
}

void STinsert(ligacao* head, informacao info, char* dir, char* valor){
    *head = insert(*head, info, dir, valor);
}

/*funcao que vai inserir uma diretoria na avl e lista correta*/
void insere(ligacao* AVL, DL *lista, char *dir, char *valor, informacao info){
    int i;
    link head;
    head = lista->primeiro;
    i = lookup(head, dir, valor);
    if (i == 0)
        insereElemento(lista, valor, dir);
    STinsert(AVL, info, dir, valor);

}

/*funcao que procura a informacao de um elemento da avl. Caso este elemento nao
exista a funcao devovler null*/
informacao search(ligacao h, char* dir){
    if (h == NULL)
        return NULL;
    
    if (strcmp(h->info->dir, dir) == 0)
        return h->info;
    
    if (strcmp(h->info->dir, dir) > 0)
        return search(h->l, dir);
    else
        return search(h->r, dir);
}

ligacao max(ligacao h){
    if (h == NULL || h->r == NULL)
        return h;
    else
        return max(h->r);
}

/*funcao que vai apagar uma avl recursivamente*/
ligacao freeAVL(ligacao h){
    if (h != NULL){
        if (h->info != NULL) h = freeNode(h);
        if (h->l != NULL) h->l = freeAVL(h->l);
        if (h->r != NULL) h->r = freeAVL(h->r);
        free(h->l);
        h->l = NULL;
        free(h->r);
        h->r = NULL;
        free(h);
        h = NULL;
    }
    return h;
}

/*funcao que vai apagar todos os componentes de uma lista*/
link freeDL(link head){
    while (head != NULL){
        head = removeElemento(head, head->dir);
    }
    return head;
}

/*funcao que vai apagar a informacao de um node da avl*/
ligacao freeNode(ligacao h){
    if (h->info != NULL){
        if (h->info->subdir->primeiro != NULL)
            h->info->subdir->primeiro  = freeDL(h->info->subdir->primeiro);
        free(h->info->subdir);
        h->info->subdir = NULL;
        free(h->info->valor);
        h->info->valor = NULL;
        free(h->info->dir);
        h->info->dir = NULL;
        if (h->info->tree != NULL) 
            *h->info->tree = freeAVL(*h->info->tree); 
        free(h->info->tree);
        h->info->tree = NULL;
        free(h->info);
        h->info = NULL;
    }
    return h;
}

/*funcao que vai apagar um node da avl*/

ligacao deleteAVLNode(informacao info, ligacao tree){
    ligacao treeaux;
    informacao nodeaux;

    if (tree == NULL)
        return tree;
    
    else if(strcmp(info->dir, tree->info->dir) < 0)
        tree->l = deleteAVLNode(info, tree->l);
    else if (strcmp(info->dir, tree->info->dir) > 0)
        tree->r = deleteAVLNode(info, tree->r);
    
    else{
        if (tree->l != NULL && tree->r != NULL){
            treeaux = max(tree->l);
            nodeaux = tree->info;
            tree->info = treeaux->info;
            treeaux->info = nodeaux;
            tree->l = deleteAVLNode(treeaux->info, tree->l);
        }
        else{
            treeaux = tree;
            if (tree->l == NULL && tree->r == NULL)
                tree = NULL;
            else if(tree->l == NULL)
                tree = tree->r;
            else
                tree = tree->l;
            freeNode(treeaux);
            free(treeaux);
        }
    }
    tree = AVLbalance(tree);
    return tree;
}