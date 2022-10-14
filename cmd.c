#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tubesem.h"
#include "shell_function.h"

typedef struct
{
    char *key;
    int val;
} CMD;
//

CMD table_commandes[] = {
    {"menu",SHELL_MENU},
    {"lister", LISTER}, {"fsupprimer", FSUPPRIMER}, 
    {"copier", COPIER}, { "afficher" , AFFICHER},
    {"deplacer", DEPLACER}, { "cd" , CD},
    {"dcreer", DCREER},{"courant", COURANT}, 
     {"fcreer", FCREER},{ "quitter" , QUTTER},
     { "help" , HELP},{ "edit" , EDIT},
     { "dsupprimer" , DSUPPRIMER}, {"clear" , CLEAR}};

#define n (sizeof(table_commandes) / sizeof(CMD))

int cmnd_keys(char *key) //Il recoit un string de line de commande et trouve le valeur associer
{
    int i;
    for (i = 0; i < n ; i++)
    {
        CMD sys = table_commandes[i];
        if (strcmp(sys.key, key) == 0)
            return sys.val;
    }
    return NOCMD;
}



void execArgs(char** parsed) 
{ 
    // Forking a child 
    pid_t pid = fork();  
  
    if (pid == -1) { 
        printf("\nFailed forking child.."); 
        return; 
    } else if (pid == 0) { 
        if (execvp(parsed[0], parsed) < 0) { 
            printf("\nCould not execute command.."); 
        } 
        exit(0); 
    } else { 
        // waiting for child to terminate 
        wait(NULL);  
        return; 
    } 
} 
int main(int argc, char **argv)
{
    
    int status;
    char *cmd = argv[1];
    char inputString[MAXCOM], *parsedArgs[MAXLIST]; 
    char* parsedArgsPiped[MAXLIST]; 
    const char *chd =argv[2];


    
        switch (cmnd_keys(cmd))
    {
    case SHELL_MENU:
    menu();
    break;
    case LISTER:
        execlp("ls", "ls", argv[2],argv[3], NULL);
        perror("Erreur lors de l'exécution la commande lister.");
        break;
    case FSUPPRIMER:
        execlp("rm", "rm", "-r", argv[2], NULL);
        perror("Erreur lors de l'exécution de la commande fsupprimer.");
        break;
    case COPIER:
        execlp("cp", "cp", "-R", argv[2],argv[3], NULL);
        perror("Erreur lors de l'exécution de la commande copier.");
        break;
    case DEPLACER:
        execlp("mv", "mv", argv[2],argv[3], NULL);
        perror("Erreur lors de l'exécution de la commande deplacer.");
        break;
    case COURANT:
        execlp("pwd", "pwd", NULL);
        perror("Erreur lors de l'exécution de la commande cournat.");
        break;
    case DCREER:
        execlp("mkdir", "mkdir", argv[2], NULL);
        perror("Erreur lors de l'exécution de la commande dcreer.");
        break;
    case FCREER:
        execlp("touch", "touch", argv[2], NULL);
        perror("Erreur lors de l'exécution de la commande fcreer.");
        break;

    case CD:
             chdir(chd);
             execlp("pwd", "pwd", NULL);
              perror("Erreur lors de l'exécution de la commande cd.");
             break;

    case DSUPPRIMER :
        execlp("rmdir", "rmdir", argv[2], NULL);
        perror("Erreur lors de l'exécution de la commande.");
        break;

    case AFFICHER:
        execlp("cat", "cat", argv[2], NULL);
        perror("Erreur lors de l'exécution de la commande.");
        break;
    
    case QUTTER:
                kill(getppid(),SIGINT);
                break;
    case EDIT:
        execlp("vi", "vi", argv[2], NULL);
        perror("Erreur lors de l'exécution de la commande.");
        break;
    
     case HELP:
                if(argv[2]!=NULL){
                    char *choice = getChoice(cmnd_keys(argv[2]));
                    execlp("man", "man", choice, NULL);
                    perror("Erreur lors de l'exécution de la commande.");
                    break;
                }
                else{
                        printf("inserer les arguments SVP \n");
                        break;
                }
      
    
     
    case CLEAR:
        clear();
        break;
    case NOCMD:
         
            printf("commande introuvable (tapez menu).\n");
                
        break;
    }
   
    return 0;
}
