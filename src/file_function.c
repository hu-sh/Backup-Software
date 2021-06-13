#include "../include/file_function.h"

/**
*Confronta il contenuto di due file.
*@return ritorna -1 se i file sono uguali. ritorna l'indice della posizione fino a dove erano uguali altrimenti.
*/
int file_equal(const char* file1_s, const char* file2_s){
    FILE* file1 = fopen(file1_s, "rb");
    FILE* file2 = fopen(file2_s, "rb");

    if(!file1){ perror("ERRORE");             exit(1);}
    if(!file2){ perror("ERRORE"); fclose(file1); exit(1);  }

    unsigned char byte1;
    unsigned char byte2;

    size_t elem1 = 1;
    size_t elem2 = 1;
    int cont = 1;

    while (elem1==1 && elem2==1) {
        elem1 = fread(&byte1, 1, 1, file1);
        elem2 = fread(&byte2, 1, 1, file2);

        if (byte1 != byte2) return -(cont);                                  //  file 2 diverso di file
        if((elem1==1 && elem2!=1) || (elem1!=1 && elem2==1)) return cont;    // posizione fino alla quale i file erano uguali

        cont++;
    }

    fclose(file1);
    fclose(file2);

    return 0;
}

/**
* Metodo che legge una riga da file e ritorna NULL se ilo file è finito.
* @param f il puntatore al file da leggere.
*/
char* read_line(FILE* f){
    char ch = getc(f);
    char *line=malloc(2);
    *line=ch;
    size_t lenght=1;
    while(ch != EOF){
        line = realloc(line, lenght+1+1);
        line[lenght++] = ch;
        if(ch == '\n')
            break;
        ch = getc(f);
    }
    if(lenght==1){
        free(line);
        return NULL;
    }
    line[lenght]='\0';
    return line;
}

/**
* Funzione che rileva le differenze tra due file.
*/
char** difference_file(const char* file1_s, const char* file2_s){
    FILE* file1 = fopen(file1_s, "r");
    FILE* file2 = fopen(file2_s, "r");

    if(!file1){ perror("ERRORE"); exit(1);}
    if(!file2){ perror("ERRORE"); exit(1);}

    //while(((line1=read_line(f1)) != NULL) && ((line2=read_line(f2)) != NULL))

    fclose(file1);
    fclose(file2);
}

/**
*Funzione che copia il contenuto di un file ad un altro, controllando che essi siano
*diversi.
*@param destination il path del file in cui verrà copiato il contenuto di source
*@param source il path del file copiato
*/
void copy_file_to_t(const char* destination, const char* source){

    int pos = file_equal(destination, source);

    if(pos) {
        FILE* to;
        FILE* from;
        if(pos < 0){

            to = fopen(destination, "wb"); //file diverso a metà del file di destinazione
            from = fopen(source, "rb");     //riscrivi tutto

            if(to==NULL || from==NULL){ perror("ERRORE"); return; }

        }else{

            to = fopen(destination, "ab"); //file di detinazione più corto di file di sorgente
            from = fopen(source, "rb");

            if(to==NULL || from==NULL){ perror("ERRORE"); return; }

            fseek(from, pos-1, SEEK_SET);

        }


        unsigned char byte;

        while(fread(&byte, 1, 1, from) == 1){
            fwrite(&byte, 1, 1, to);
        }

        fclose(to);
        fclose(from);

    } /*else {
        FILE* to = fopen(destination, "ab");
        fclose(to);
    }*/
}
/**
*Funzione che copia il contenuto di un file ad un altro,se non esiste lo crea.
*@param destination il path del file in cui verrà copiato il contenuto di source
*@param source il path del file copiato
*/
void copy_file_to(const char* destination, const char* source){
    FILE* to;
    FILE* from;

    to = fopen(destination, "wb");
    from = fopen(source, "rb");

    if(to==NULL || from==NULL){ perror("ERRORE"); return; }

    unsigned char byte;

    while(fread(&byte, 1, 1, from) == 1){
        fwrite(&byte, 1, 1, to);
    }

    fclose(to);
    fclose(from);
}

/**
*Funzione che confronta l'ultima data di modifica di due file.
*@param path1 il path del primo file da controllare
*@param path2 il path del secondo file da controllare
*@return ritorna 0 se sono uguali. ritorna -1 se il secondo è stato modificato
*                dopo il primo e 1 se il primo è stato modficato dopo il secondo
*/
int compare_file_mtime(char* path1, char* path2){
    struct stat file1, file2;

    if(stat(path1, &file1)) { perror("\nERRORE"); return -2; }
    if(stat(path2, &file2)) { perror("\nERRORE"); return -2; }

    double temp = difftime(file1.st_mtim.tv_sec, file2.st_mtim.tv_sec);

    if(temp == 0.0) return 0;
    if(temp > 0.0){
        return 1;
    } else {
        return -1;
    }
}
