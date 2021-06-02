#include "header2.h"

/*funcoes relacionadas com a double linked list */

/*inicaliza a lista*/
DL* init(DL *lista){
    lista->primeiro = NULL;
    lista->ultimo = NULL;
    return lista;
}

/*funcao que remove um certo elemento da lista*/
link removeElemento(link head, char* dir){
    link t, prev;
    for (t = head, prev = NULL; t != NULL; prev = t, t = t->next){
        if (strcmp(t->dir, dir) == 0){
            if (t == head)
                head = t->next;
            else
                prev->next = t->next;
            free(t->dir);
            free(t->valor);
            free(t);
            break;
        }
    }
    return head;
}

/*funcao que insere um elemento no final da lista*/
link insereElemento(DL *lista, char *valor, char *dir){
    link novo;
    novo = (link)malloc(sizeof(struct Node));
    novo->valor = (char*)malloc(sizeof(char) * (strlen(valor)+1));
    novo->dir = (char*)malloc(sizeof(char) * (strlen(dir)+1));
    strcpy(novo->valor, valor);
    strcpy(novo->dir, dir);
    novo->next = NULL;
    novo->prev = lista->ultimo;

    /* verificar se a lista esta vazia */
    if (lista->primeiro == NULL){
        lista->primeiro = novo;
    }
    
    else{
        lista->ultimo->next = novo;
    }
    lista->ultimo = novo;
    return lista->primeiro;
}

/*funcao que vai verificar se um elemento ja existe e caso afirmativo e
dependendo do conteudo do argumento value, altera um dos componentes do node*/
int lookup(link head, char* dir, char* value){
    link t;
    int estado = 0; /*0 = nao existe / 1 = existe */
    for (t = head; t != NULL; t = t->next){
        if (strcmp(t->dir, dir) == 0){
            estado = 1;
            break;
        }
    }
    if (estado == 1){
        if (strcmp(t->valor, "") == 0 || strcmp(value, "") != 0)
        t->valor = (char*)realloc(t->valor, (strlen(value) + 1));
        strcpy(t->valor, value);
    }
    return estado;
}