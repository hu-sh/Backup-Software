#include <unistd.h>
#include <dirent.h>

#include "file_function.h"


/**
*Copia una stringa e aggiunge uno "/" in fondo.
*@param to la stringa in cui copiare la secona e aggiungere lo slash
*@param from la stringa che viene copiata.
*@return puntatore alla stringa in cui si è copiato
*/
char* cpy_slash(char* to, const char* from);

/**
*Cerca un elemento in elem, con nome key. Se viene trovato si ritorna l'indice altrimenti -1.

*@param key l'elemento da cercare
*@param elem l'array in cui cercare
*@param elem_dim la dimensione dell'array su cui cercare
*@return l'indice
*/
int find_in(const char* key, const struct dirent **elem, size_t elem_size);

/**
*Funzione che compara due directory. Se nella prima mancano dei file rispetto alla
*seconda, vi vengono copiati dalla seconda. Se dei file della prima risultano vecchi
*(confronto di ultima modifica e del contenuto) rispetto
*alla seconda, vengono aggiornati.
*@param oldPath il percorso alla prima directory
*@param newPath percorso alla directory "nuova"
*/
int update_dir(const char* oldPath, const char* newPath);

void delete_more(const char* dir_checked, const char* compare_with);

void delete_dir(const char* path);

/**
* Funzione di comparazione per prendere gli elementi di una directory escludendo "."
* e ".."
*/
int noThis_noPrev_noCheck(const struct dirent* elem);

/**
* Funzione che stampa a video l'albero della directory passata fino a una certa profondità.
*/
int print_dir(const char* dir_path, const unsigned max_depth);
