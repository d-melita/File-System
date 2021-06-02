 /* Diogo Godinho Melita - ist199202 - 2o Projeto IAED 2020/2021 */
#include "header2.h"

int main(){
    char *comando, *path;
    DL *lista;
    ligacao* AVL;
    comando = (char*)malloc(MAX_COMANDO * sizeof(char));
    path = (char*)malloc(PATH * sizeof(char));
    lista = (DL*)malloc(sizeof(DL));
    AVL = (ligacao*)malloc(sizeof(struct AVLTree));
    strcpy(path, "/");
    /*inicializar a AVL e a lista da root */
    AVLinit(AVL);
    lista = init(lista);
    scanf("%s", comando);
    while (strcmp(comando, QUIT) != 0){
        if (strcmp(comando, HELP) == 0)
            helpfunc();

        else if (strcmp(comando, SET) == 0)
            setfunc(AVL, lista);
        
        else if (strcmp(comando, FIND) == 0)
            findfunc(AVL);
        
        else if (strcmp(comando, LIST) == 0)
            listfunc(AVL);
        
        else if (strcmp(comando, PRINT) == 0)
            printfunc(AVL, lista, lista->primeiro, path);
        
        else if (strcmp(comando, SEARCH) == 0)
            searchaux(AVL, lista, lista->primeiro, path);
        
        else if(strcmp(comando, DELETE) == 0)
            auxdelete(AVL, lista);

        strcpy(path, "/");
        scanf("%s", comando);
    }
    /*se o comando for quit vai apagar tudo e sair*/
    deleteroot(AVL, lista);
    free(path);
    free(comando);
    free(AVL);
    free(lista);
    return RETURN_ZERO;
}

/* help: imprime todos os comandos disponiveis e a sua descricao */
void helpfunc(){
    printf("%s: %s%s: %s%s: %s%s: %s%s: %s%s: %s%s: %s%s: %s", 
    HELP, HELP_DESC, QUIT, QUIT_DESC, SET, SET_DESC, PRINT, PRINT_DESC, FIND, 
    FIND_DESC, LIST, LIST_DESC, SEARCH, SEARCH_DESC, DELETE, DELETE_DESC);
}
/*funcao auxiliar que le o valor indicado de um certo caminho */
void leValor(char* string){
    int c, i = 0, estado = 0;

    while((c = getchar()) != '\n'){
        if (c != ' ' && c != '\t')
            estado = 1;
        if (estado == 1)
            string[i++] = c;
    }
    string[i] = '\0';
}

/*funcao auxiliar que vai verificar qual e a diretoria onde se deve introduzir
o valor indicado. Por exemplo se o input for "a/b/c valor", a diretoria correta
onde se deve introduzir "valor" sera a diretoria c*/
char* correto(char* string){
    char *aux, *caminho;
    caminho = (char*)malloc(sizeof(char)*MAX_CHARS);
    aux = strtok(string, "/");
    while (aux != NULL){
        strcpy(caminho, aux);
        aux = strtok(NULL, "/");
    }
    return caminho;
}

/* set: adiciona ou modifica um caminho */
void setfunc(ligacao* AVL, DL *lista){
    char *caminho, *valor, *dir, *path, *aux, *auxiliar;
    informacao info;
    caminho = (char*)malloc(MAX_CHARS*sizeof(char));
    valor = (char*)malloc(MAX_CHARS*sizeof(char));
    auxiliar = (char*)malloc(MAX_CHARS*sizeof(char));
    scanf("%s", caminho);
    strcpy(auxiliar, caminho);
    leValor(valor);
    path = correto(auxiliar); /*saber qual e a diretoria correta */
    aux = "";
    /* inserir ordenadamente */
    dir = strtok(caminho,  "/");
    while(dir != NULL){
        if (strcmp(dir, path) == 0){
            /*se for a diretoria correta dependendo de ja existir ou nao a
            diretoria adicionamos ou mudamos o seu valor */
            info = search(*AVL, dir);
            if (info == NULL)
                info = newNode(dir, valor);
            insere(AVL, lista, dir, valor, info);
        }
        else{
            /*caso nao seja a diretoria correta, vamos apenas introduzir a 
            diretoria sem valor caso nao exista ainda no sistema */
            info = search(*AVL, dir);
            if (info == NULL)
                info = newNode(dir, aux);
            insere(AVL, lista, dir, aux, info);
        }
        AVL = info->tree;
        lista = info->subdir;
        dir = strtok(NULL, "/");         
    }
    free(caminho);
    free(valor);
    free(auxiliar);
    free(path);
}

/*find: imprime o valor armazenado */
void findfunc(ligacao* AVL){
    char* caminho, *path, *aux, *dir;
    informacao info;
    int i = 0; /*var de estado - 1: nao existe o caminho / 0: existe */
    caminho = (char*)malloc(MAX_CHARS*sizeof(char));
    aux = (char*)malloc(MAX_CHARS*sizeof(char));
    scanf("%s", caminho);
    strcpy(aux, caminho);
    path = correto(aux);
    dir = strtok(caminho, "/");
    while (dir != NULL){
        info = search(*AVL, dir);
        if (info == NULL){
            printf("%s", NOT_FOUND);
            i = 1;
            break; /*caso nao exista o caminho acaba a execucao do loop */
        }
        if (strcmp(dir, path) == 0)
            break;
        AVL = info->tree;
        dir = strtok(NULL, "/");
    }
    if (i == 0){ /*se existir o caminho, dependendo do conteudo do valor e
    impressa a mensagem certa*/
        if (strcmp(info->valor, "") == 0)
            printf("%s", NO_DATA_ERROR);
        else
            printf("%s\n", info->valor);
    }
    
    free(path);
    free(caminho);
    free(aux);
}

/*list:  lista todos os componentes de um caminho */
void listfunc(ligacao* AVL){
    char *caminho, *aux, *path, *dir; 
    informacao info;
    char c = getchar();
    caminho = (char*)malloc(MAX_CHARS*sizeof(char));
    aux = (char*)malloc(MAX_CHARS*sizeof(char));
    if (c == '\n' || c == EOF) /* se nao existir mais nada no input a seguir a
    list, sao impressos as diretorias da root*/
        traverse(*AVL);
    else{
        scanf("%s", caminho);
        strcpy(aux, caminho);
        path = correto(aux);
        dir = strtok(caminho, "/");
        while (dir != NULL){
            info = search(*AVL, dir); /*encontrar a avl do caminho que contem os
            seus compenentes ordenados por ordem alfabetica numa travesia in
            order*/
            if (info == NULL){ /*caso nao exista esse caminho, isto e, caso esse
            caminho nao contenha informacao e exibida a mensagem certa*/
                printf("%s", NOT_FOUND);
                break;
            }
            if (strcmp(dir, path) == 0)
                break;
            AVL = info->tree;
            dir = strtok(NULL, "/");
        }
        if (info != NULL){
            AVL = info->tree;
            traverse(*AVL);
        }
        free(path);
    }
    free(caminho);
    free(aux);
}

/*print: Imprime todos os caminhos e valor por ordem de criacao*/
void printfunc(ligacao* AVL, DL *lista, link t, char* path){
    char* caminho, *aux, *copia, *barra = "/";
    informacao info;
    if (t == NULL){
        return;
    }
    caminho = (char*)malloc((strlen(t->dir)+1)*sizeof(char));
    aux = (char*)malloc((strlen(path)+1)*sizeof(char));
    strcpy(caminho, t->dir);
    copia = (char*)malloc(sizeof(char)*(strlen(caminho)+strlen(path)+2));
    strcpy(aux, path);
    strcpy(copia, path);
    strcat(copia, caminho);
    info = search(*AVL, caminho);
    if (strcmp(info->valor, "") != 0) /*se o caminho tiver valor associado estes
    sao impressos no stdou*/
        printf("%s %s\n", copia, info->valor);

    /*verificar se o caminho tem diretorias associadas. Se nao tiver, passamos
    ao proximo caminho caso contrario vamos repetir o procedimento para as suas
    diretorias*/
    if (info->subdir->primeiro == NULL){
        free(caminho);
        printfunc(AVL, lista, t->next, aux);
        free(aux);
        free(copia);
    }
    else{
        strcat(copia, barra);
        printfunc(info->tree, info->subdir, info->subdir->primeiro, copia);
        printfunc(AVL, lista, t->next, aux);
        free(caminho);
        free(aux);
        free(copia);
    }
}

/*search: procura o caminho de um dado valor */
int searchfunc(ligacao* AVL, DL *list, link t, char* path, char* val){
    char* caminho, *aux, *copia, *barra = "/";
    informacao info;
    int i; /*1 = encontrou se o valor / 0 = nao se encontrou*/
    
    if (t == NULL){
        i = 0;
        return i;
    }
    caminho = (char*)malloc((strlen(t->dir)+1)*sizeof(char));
    aux = (char*)malloc((strlen(path)+1)*sizeof(char));
    strcpy(caminho, t->dir);
    copia = (char*)malloc((strlen(caminho)+strlen(path)+2)*sizeof(char));
    strcpy(aux, path);
    strcpy(copia, path);
    strcat(copia, caminho);
    info = search(*AVL, caminho);
    if (strcmp(info->valor, val) == 0){
        printf("%s\n", copia);
        i = 1;
        free(caminho);
        free(aux);
        free(copia);
        /*se for encontrado o valor acaba a execucao da funcao pois so interessa
        o primeiro caminho*/
        return i;
    }
    if (info->subdir->primeiro ==  NULL){
        /*caso o caminho nao tenha filhos passa se ao proximo elemento da lista
        e faz se a procura de igual forma, acabando a execucao da funcao caso se
        encontre o valor*/
        i = searchfunc(AVL, list, t->next, aux, val);
        if (i == 1){
            free(caminho);
            free(aux);
            free(copia);
            return i;
        }
    }
    else{
        /*caso o caminho tenha diretorias associadas, filhas, iremos procurar 
        nestas primeiro, seguindo o mesmo formato, caso se encontre o valor 
        pretendido acaba a execucao da funcao*/
        strcat(copia, barra);
        i = searchfunc(info->tree, info->subdir, info->subdir->primeiro, copia,
        val);
        if (i == 1){
            free(caminho);
            free(aux);
            free(copia);
            return i;
        }
        i = searchfunc(AVL, list, t->next, aux, val);
        if (i == 1){
            free(caminho);
            free(aux);
            free(copia);
            return i;
        }
        
    }
    free(copia);
    free(caminho);
    free(aux);
    return i;
}

/* funcao auxiliar que vai ler o valor do stdin para fazer a procura recorrendo
a funcao anterior*/
void searchaux(ligacao* AVL, DL *lista, link t, char* path){
    char* valor;
    int i;
    valor = (char*)malloc(MAX_CHARS*sizeof(char));
    leValor(valor);
    i = searchfunc(AVL, lista, t, path, valor);
    /*caso o valor retornado na funcao de procura for 0, sera mostrada a 
    mensagem de erro*/
    if (i == 0)
        printf("%s", NOT_FOUND);
    free(valor);
}

/*funcao que apaga todos os componentes do sistema*/
void deleteroot(ligacao* AVL, DL* lista){
    informacao info;
    while (lista->primeiro != NULL){
        info = search(*AVL, lista->primeiro->dir);
        *AVL = deleteAVLNode(info, *AVL);
        lista->primeiro = removeElemento(lista->primeiro, lista->primeiro->dir);
    }
}

/*delete: apaga todos os caminhos de um sub-caminho*/
void auxdelete(ligacao* AVL, DL* lista){
    char *caminho, *dir, *copy;
    char c = getchar();
    informacao info;
    ligacao* copiaAVL;
    DL* copialista;
    caminho = (char*)malloc(MAX_CHARS * sizeof(char));
    copy = (char*)malloc(MAX_CHARS * sizeof(char));
    
    /*caso o input seja apenas "delete" sao apagados todos os ficheiros do 
    sistema*/
    if (c == '\n' || c == EOF)
        deleteroot(AVL, lista);
    else{
        /*caso contrario le se o caminho do input e vai-se procurar o sitio
        correto para apagar o caminho*/
        scanf("%s", caminho);
        dir = strtok(caminho, "/");
        while (dir != NULL){
            strcpy(copy, dir);
            copiaAVL = AVL; /*copias que garantem que nao se perde a AVL e a*/
            copialista = lista; /*lista de onde se tem de apagar*/
            info = search(*AVL, dir);
            if (info == NULL){/*caso a informacao seja NULL, significa que o 
            caminho nao exista logo a execucao da funcao acaba e mostra-se a 
            mensagem de erro*/
                printf("%s", NOT_FOUND);
                free(copy);
                free(caminho);
                return;
            }
            AVL = info->tree;
            lista = info->subdir;
            dir = strtok(NULL, "/");   
        }
        /*uma vez encontrados os sitios corretos de onde apagar procedesse a 
        este processo*/
        *copiaAVL = deleteAVLNode(info, *copiaAVL);
        copialista->primeiro = removeElemento(copialista->primeiro, copy);
    }
    free(caminho);
    free(copy);
}