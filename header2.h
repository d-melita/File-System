#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RETURN_ZERO 0

#define HELP "help"
#define HELP_DESC "Imprime os comandos disponíveis.\n"
#define QUIT "quit"
#define QUIT_DESC "Termina o programa.\n"
#define SET "set"
#define SET_DESC "Adiciona ou modifica o valor a armazenar.\n"
#define PRINT "print"
#define PRINT_DESC "Imprime todos os caminhos e valores.\n"
#define FIND "find"
#define FIND_DESC "Imprime o valor armazenado.\n"
#define LIST "list"
#define LIST_DESC "Lista todos os componentes imediatos de um sub-caminho.\n"
#define SEARCH "search"
#define SEARCH_DESC "Procura o caminho dado um valor.\n"
#define DELETE "delete"
#define DELETE_DESC "Apaga um caminho e todos os subcaminhos.\n"

#define NOT_FOUND "not found\n"
#define NO_DATA_ERROR "no data\n"
#define FOUND "found"

#define NO_MEMORY "no memory\n"

#define MAX_CHARS 65535
#define MAX_COMANDO 7
#define PATH 2

#define MAX_CHARS 65535
#define MAX_COMANDO 7


/*ATS*/
typedef struct Node* link;

/*cada node da lista tem a seguinte informacao*/
typedef struct Node{
    char *valor;
    char *dir;
    struct Node *next;
    struct Node *prev;
}Node;

typedef struct DL{
    link primeiro;
    link ultimo;
}DL;

typedef struct AVLTree* ligacao;
typedef struct info* informacao;

struct AVLTree{
    ligacao l, r;
    informacao info; /*informacao de cada node */
};

struct info{
    char *valor;
    char *dir;
    int height;
    /*cada no tem um pointer para a sua propira avl e lista*/
    DL *subdir; 
    ligacao* tree;
};

/*prototipos das funcoes dos ficheiros avl.c e dll.c*/
void setfunc(ligacao* AVL, DL *lista);
void insere(ligacao* AVL, DL *lista, char *dir, char *valor, informacao info);
void STinsert(ligacao* head, informacao info, char* dir, char* valor);
ligacao insert(ligacao h, informacao info, char* dir, char* valor);
informacao newNode(char* dir, char* valor);
ligacao novo(informacao info, ligacao l, ligacao r);
ligacao AVLbalance(ligacao h);
int balance(ligacao h);
ligacao rotRL(ligacao h);
ligacao rotLR(ligacao h);
ligacao rotR(ligacao h);
ligacao rotL(ligacao h);
int altura(ligacao h);
void traverse(ligacao h);
void visit(ligacao h);
int lookup(link head, char* dir, char* value);
link insereElemento(DL *lista, char *valor, char *dir);
link removeElemento(link head, char* dir);
DL* init(DL *lista);
void AVLinit(ligacao* head);
informacao search(ligacao h, char* dir);
ligacao max(ligacao h);
ligacao freeAVL(ligacao h);
link freeDL(link head);
ligacao freeNode(ligacao h);
ligacao freeR(ligacao h);
void STfree(ligacao* head);
ligacao deleteAVLNode(informacao info, ligacao tree);

/*prototipos das funcoes do ficheiro proj2.c*/
void helpfunc();
void leValor(char* string);
char* correto(char* string);
void setfunc(ligacao* AVL, DL* lista);
void findfunc(ligacao* AVL);
void listfunc(ligacao* AVL);
void printfunc(ligacao* AVL, DL *lista, link t, char* path);
int searchfunc(ligacao* AVL, DL *list, link t, char* path, char* val);
void searchaux(ligacao* AVL, DL *lista, link t, char* path);
void auxdelete(ligacao* AVL, DL* lista);
void deleteroot(ligacao* AVL, DL* lista);


/*----------------------------ESTRUTURA DO SISTEMA----------------------------*/

/*cada diretoria e introduzida numa avl e numa lista especifica. Imaginemos que
fazemos "set iaed projeto". A diretoria iaed sera introduzida na avl e na lista
da root sendo que o seu node da avl possui um pointer para a sua propria avl e 
para a sua propira lista que contem os seus componentes diretos. As avls ficam 
ordenadas por ordem alfabetica do nome da sua diretoria enquanto que as listas
estao ordenadas por ordem de criacao dos componentes.
Caso facamos "set iaed/teste um", a diretoria iaed sera introduzida na avl e na
lista da root e nao tera valor associado, ou caso ja existia nao se altera o 
seu valor. Por sua vez, a diretoria teste sera introduzida apenas na avl e na
lista da diretoria iaed uma vez que esta diretoria e a sua mae. 
Este processo sera repetido para cada diretoria. Deste modo, basta percorrer as
listas para saber a ordem de criacao dos componetnes e fazer traverse in order 
de uma avl para ter os componentes ordenados por ordem alfabetica*/

/*os comentarios nao contem caracteres nao ascii*/