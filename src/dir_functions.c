#include "../include/dir_functions.h"

int no_dot_doubleDot(const struct dirent* elem)
{
    if(elem!=NULL)
    {
        if(strcmp(elem->d_name, "..") && strcmp(elem->d_name, "."))
            return 1;
    }
    return 0;
}

int noThis_noPrev_noCheck(const struct dirent* elem)
{
    if(elem!=NULL)
    {
        if(strcmp(elem->d_name, "..") && strcmp(elem->d_name, "."))
            return 1;
    }
    return 0;
}

/**
*Cerca un elemento in elem, con nome key. Se viene trovato si ritorna l'indice altrimenti -1.

*@param key l'elemento da cercare
*@param elem l'array in cui cercare
*@param elem_dim la dimensione dell'array su cui cercare
*@return l'indice
*/
int find_in(const char* key, const struct dirent **elem, size_t elem_size)
{
    int i=0;

    while(i<elem_size)
    {
        if(0 == strcmp(elem[i]->d_name, key)) return i;
        else i++;
    }

    return -1;
}

/**
*Copia una stringa e aggiunge uno "/" in fondo.
*@param to la stringa in cui copiare la secona e aggiungere lo slash
*@param from la stringa che viene copiata.
*@return puntatore alla stringa in cui si è copiato
*/
char* cpy_slash(char* to, const char* from)
{
    strcpy(to, from);
    strcat(to, "/");

    return to;
}

/**
*Funzione che compara due directory. Se nella prima mancano dei file rispetto alla
*seconda, vi vengono copiati dalla seconda. Se dei file della prima risultano vecchi
*(confronto di ultima modifica e del contenuto) rispetto
*alla seconda, vengono aggiornati.
*@param updated il percorso alla prima directory
*@param from percorso alla directory "nuova"
*/
int update_dir(const char* updated, const char* from)
{   static updated_file=0;

    struct dirent **elem1;
    struct dirent **elem_new;

    size_t updated_size = scandir(updated, &elem1, noThis_noPrev_noCheck, alphasort);
    size_t from_size = scandir(from, &elem_new, noThis_noPrev_noCheck, alphasort);

    if(updated_size == -1){ perror("ERRORE"); return updated_size;  }
    if(from_size == -1)   { perror("ERRORE"); return from_size;     }


    for(size_t i=0; i<from_size; ++i)
    {
        char* elem_upt = malloc(strlen(updated) + strlen(elem_new[i]->d_name) + 2); //path di un elemento della directory da aggiornare
        char* upt_with = malloc(strlen(from) + strlen(elem_new[i]->d_name) + 2);    //path dello stesso elemento ma nella directory da cui si aggiorna

        cpy_slash(elem_upt, updated);
        cpy_slash(upt_with, from);

        strcat(upt_with, elem_new[i]->d_name);

        int ind_check = find_in(elem_new[i]->d_name, elem1, updated_size);           //controlla se l'elemento da aggiornare esiste o meno nella directory da aggionare

        if(elem_new[i]->d_type == DT_DIR)
        {
            if(ind_check != -1)                                                     // se la directory viene trovata allora ricorsivamente la aggiorniamo con update_dir
            {
                strcat(elem_upt, elem1[ind_check]->d_name);

                update_dir(elem_upt, upt_with);
            }
            else                                                                    // se la directory non viene trovata viene creata e aggiornata con update_dir
            {
                strcat(elem_upt, elem_new[i]->d_name);

                mkdir(elem_upt, 0700);

                update_dir(elem_upt, upt_with);
            }
        }
        else
        {
            if(ind_check != -1)
            {
                strcat(elem_upt, elem1[ind_check]->d_name);

                if(compare_file_mtime(elem_upt, upt_with) == -1)
                {
                    copy_file_to_t(elem_upt, upt_with);
                    printf("%d\n", ind_check);

                    updated_file++;
                }
            }
            else                         //aggiungi
            {
                strcat(elem_upt, elem_new[i]->d_name);
                strcat(upt_with, elem_new[i]->d_name);

                copy_file_to(elem_upt, upt_with);

                updated_file++;
            }
        }

        free(elem_upt);
        free(upt_with);
    }

    delete_more(updated, from);

    return updated_file;
}

/**
* Elimina gli elementi in più di una directory rispetto a un'altra directory.
*/
void delete_more(const char* dir_checked, const char* compare_with)
{
    struct dirent **elem_checked;
    struct dirent **elem_compared;

    size_t oldSize = scandir(dir_checked, &elem_checked, noThis_noPrev_noCheck, alphasort);
    size_t newSize = scandir(compare_with, &elem_compared, noThis_noPrev_noCheck, alphasort);

    if(oldSize == -1)
    {
        perror("ERRORE");
        return;
    }
    if(newSize == -1)
    {
        perror("ERRORE");
        return;
    }

    for(size_t i=0; i<oldSize; ++i)
    {
        char* elem_ch_cpy = malloc(strlen(dir_checked) + strlen(elem_checked[i]->d_name) + 2);
        cpy_slash(elem_ch_cpy, dir_checked);
        strcat(elem_ch_cpy, elem_checked[i]->d_name);

        if(-1 == find_in(elem_checked[i]->d_name, elem_compared, newSize))
        {
            if(elem_checked[i]->d_type == DT_DIR)
            {
                delete_dir(elem_ch_cpy);
            }
            else
            {
                unlink(elem_ch_cpy);
            }
        }
        else
        {
            if(elem_checked[i]->d_type == DT_DIR)
            {
                char* newPath_cpy = malloc(strlen(dir_checked) + strlen(elem_checked[i]->d_name) + 2);
                cpy_slash(newPath_cpy, compare_with);

                strcat(newPath_cpy, elem_checked[i]->d_name);
                delete_more(elem_ch_cpy, newPath_cpy);

                free(newPath_cpy);
            }
        }
        free(elem_ch_cpy);
    }
}

/**
* Elimina una directory non vuota a partire dal suo path se trova altre directory
* richiama la funzione per eliminarle.
*/
void delete_dir(const char* path)
{
    struct dirent **elem;

    size_t dir_size = scandir(path, &elem, noThis_noPrev_noCheck, alphasort);

    for(size_t i=0; i<dir_size; ++i)
    {
        char *elem_path = malloc(strlen(path) + strlen(elem[i]->d_name) + 2);
        cpy_slash(elem_path, path);
        strcat(elem_path, elem[i]->d_name);

        if(elem[i]->d_type == DT_DIR)
        {

            delete_dir(elem_path);

        }
        else
        {

            unlink(elem_path);

        }
        free(elem_path);
    }
    rmdir(path);
}



/**
* Funzione che stampa a video l'albero della directory passata fino a una certa profondità.
*/
int print_dir(const char* dir_path, const unsigned max_depth)
{
    static int depth = 0;
    depth++;

    DIR* dir = opendir(dir_path);
    struct dirent **elem;
    size_t dir_size;

    dir_size = scandir(dir_path, &elem, noThis_noPrev_noCheck, alphasort);

    if(dir_size==-1)
    {
        perror("ERRORE");
        return dir_size;
    }


    for(size_t i = 0; i < dir_size; ++i)
    {
        for(int j=0; j<depth-1; ++j)
            printf("\t");

        printf("%s\n", elem[i]->d_name);

        if (elem[i]->d_type == DT_DIR)
        {
            char *temp = malloc(strlen(dir_path) + elem[i]->d_reclen);
            strcpy(temp, dir_path);

            strcat(temp, "/");
            strcat(temp, elem[i]->d_name);

            if((depth)/max_depth<1)
            {
                print_dir(temp, max_depth-1);
                depth--;                        //viene incrementata in check_dir quindi la decremento
            }

            free(temp);
        }
    }

    closedir(dir);
    return dir_size;
}

