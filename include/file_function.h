#include <stdio.h>
#include <stdlib.h>

#include <sys/stat.h>
#include <time.h>

char* read_line(FILE* f);

char** difference_file(const char* file1_s, const char* file2_s);

/**
*Confronta il contenuto di due file.
*@return ritorna -1 se i file sono uguali. ritorna l'indice della posizione fino a dove erano uguali altrimenti.
*/
int file_equal(const char* file1_s, const char* file2_s);

/**
*Funzione che copia il contenuto di un file ad un altro, controllando che essi siano
*diversi.
*@param destination il path del file in cui verrà copiato il contenuto di source
*@param source il path del file copiato
*/
void copy_file_to_t(const char* destination, const char* source);

/**
*Funzione che copia il contenuto di un file ad un altro,se non esiste lo crea.
*@param destination il path del file in cui verrà copiato il contenuto di source
*@param source il path del file copiato
*/
void copy_file_to(const char* destination, const char* source);

/**
*Funzione che confronta l'ultima data di modifica di due file.
*@param path1 il path del primo file da controllare
*@param path2 il path del secondo file da controllare
*@return ritorna 0 se sono uguali. ritorna -1 se il secondo è stato modificato
*                dopo il primo e 1 se il primo è stato modficato dopo il secondo
*/
int compare_file_mtime(char* path1, char* path2);
