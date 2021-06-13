#include <stdio.h>

#include "include/dir_functions.h"

int main()
{
    puts("Effettua il backup degli elementi di una director,y\n"
         "in un'altra directory. Ora inserisci il path della directory di backup:");

    char backup[1000];// ="/home/hush/informatica/Sistemi/EserciziTecnologie/backups/prova_dir";
    fgets(backup, sizeof(backup), stdin);
    backup[strlen(backup)-1] = '\0';

    puts("Inserisci il path della directory di cui vuoi fare il backup: ");

    char backuped[1000];//="/home/hush/informatica/Sistemi/EserciziTecnologie/backups/Prova_dir";
    fgets(backuped, sizeof(backuped), stdin);
    backuped[strlen(backuped)-1] = '\0';

    printf("hai aggiornato %d elementi!\n\n", update_dir(backup, backuped));

    print_dir(backup, 5);

    return 0;
}
