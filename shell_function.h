#include <unistd.h>    
#include <sys/types.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

//
// les commandes
#define NOCMD -1
#define LISTER 1
#define FSUPPRIMER 2
#define COPIER 3
#define DEPLACER 4
#define COURANT 5
#define DCREER 6
#define FCREER 7
#define QUTTER 8
#define HELP 9
#define EDIT 10
#define CD 11 
#define DSUPPRIMER 12 
#define SHELL_MENU 14
#define  AFFICHER 13 // afficher le contenu d'une fichier cat
#define CLEAR 15


#define clear() printf("\033[H\033[J") 
#define TOK_BUFSIZE 64
#define SPACE " \t\r\n\a"
#define MAXLIST 100 
#define MAXCOM 1000 


char *read_line(void) //Lire une ligne d'entrée de stdin.
{
  char *line = NULL;
  ssize_t bufsize = 0;  //  allouer un buffer 

  if (getline(&line, &bufsize, stdin) == -1){
    if (feof(stdin)) {
      exit(0); 
    } else  {
      perror("readline error");
      exit(1);
    }
  }

  return line;
}

char **split_line(char *line) //Divisez une ligne en tokens
{
  int bufsize = TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token, **tokens_backup;

  if (!tokens) {
    fprintf(stderr, "shell: Erreur d'allocation\n");
    exit(1);
  }

  token = strtok(line, SPACE);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += TOK_BUFSIZE;
      tokens_backup = tokens;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
		free(tokens_backup);
        fprintf(stderr, "shell: Erreur d'allocation\n");
        exit(1);
      }
    }

    token = strtok(NULL, SPACE);
  }
  tokens[position] = NULL;
  return tokens;
}
//this function executes command with ">"
int executeFileOutCommand(char * commandName, char * argv[], char * filename) {
	int def = dup(1);

    //First, we're going to open a file
    int file = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU | S_IRGRP | S_IROTH);
    if(file < 0)    return 1;

    //Now we redirect standard output to the file using dup2
    if(dup2(file,1) < 0)    return 1;
    int pid;
    if( pid = fork() == 0) {
    	close(file);
    	close(def);
    	execve(commandName, argv,0);
    	return 0;
    }
    dup2(def, 1);
    close(file);
    close(def);
    wait(NULL);
 	close(file);
	return 0;
}


// this function executes command with "<"
int executeFileInCommand(char * commandName, char * argv[], char * filename) {
	int pipefd[2];

	if(pipe(pipefd)) {
		perror("pipe");
		exit(2);
	}

	switch(fork()) {
	case -1:
		perror("fork()");
		exit(2);
	case 0:
        close(pipefd[0]);  /* the other side of the pipe */
        dup2(pipefd[1], 1);
        close(pipefd[1]);  
 	   FILE * pFile;
 	   char mystring;

 	   pFile = fopen (filename , "r");
 	   if (pFile == NULL) perror ("Error opening file");
 	   else {

 		while ((mystring=fgetc(pFile)) != EOF) {
 				putchar(mystring); /* print the character */
 			}
 	     fclose (pFile);
 	   }
 	  exit(EXIT_SUCCESS);

    default:

        close(pipefd[1]);  /* the other side of the pipe */
        dup2(pipefd[0], 0);  /* automatically closes previous fd 0 */
        close(pipefd[0]);  /* cleanup */

        execve(commandName, argv, 0);
        perror(commandName);
        exit(3);

	}

	return 0;
}


char *getChoice(int cmd){

        switch (cmd)
    {
   
    case LISTER:
       return "ls";
       perror("Erreur lors de l'exécution de la commande.");
        break;
    case FSUPPRIMER:
       return "rm";
       perror("Erreur lors de l'exécution de la commande.");
        break;
    case COPIER:
       return "cp";
       perror("Erreur lors de l'exécution de la commande.");
        break;
    case DEPLACER:
        return "mv";
        perror("Erreur lors de l'exécution de la commande.");
        break;
    case COURANT:
        return "pwd";
        perror("Erreur lors de l'exécution de la commande.");
        break;
    case DCREER:
        return"mkdir";
        perror("Erreur lors de l'exécution de la commande.");
        break;
    case FCREER:
        return"touch";
        perror("Erreur lors de l'exécution de la commande.");
        break;

    case CD:
        return "cd";
        perror("Erreur lors de l'exécution de la commande.");
             break;

    case DSUPPRIMER :
        return "rm";
        perror("Erreur lors de l'exécution de la commande.");
        break;

    case AFFICHER:
        return"cat";
        perror("Erreur lors de l'exécution de la commande.");
        break;
    
    case QUTTER:
                kill(getppid(),SIGINT);
                perror("Erreur lors de l'exécution de la commande.");
                break;
    case EDIT:
    
                return"vi";
                 break;
     case HELP:
       return "man";
      
     break;
     
       }



}

void menu(){
   printf("----------------------------Commandes valable---------------------------------\n\n");
    printf("\t\t\t\tShell Creer par amineRB\n\n");
    printf("lister :   une commande qui liste les fichier et les dossiers (toutes les options de ls valabe)\n");
    printf("fsupprimer:   une commande pour supprimer les fichiers (tout les option de rm sont valable)\n");
    printf("copier :   une commande pour copier les fichiers utilasant le fichier comme 1er argument et le dossier cible comme 2eme (tous les options de cp sont valable)\n");
     printf("deplacer : une commande pour couper les fichiers utilasant le fichier comme 1er argument et le dossier cible comme 2eme (tous les options de mv sont valable)\n");
    printf("courant:   une commande pour afficher le dossier courant\n");
    printf("dcreer:   une commande pour creer les dossier (tous les option de mkdir sont valabe)\n");
    printf("dcreer:   une commande pour creer les fichiers (tous les option de touch sont valabe)\n");
    printf("edit:   une commande pour edit les fichiers(tous les option de vi sont valable)\n");
     printf("dsupprimer:   une commande pour supprimer les dossiers (tout les option de rmdir sont valable)\n");
    printf("afficher:   une commande pour afficher le contenu d'un fichier\n");
    printf("cd:  pour changer les directories\n");
    printf("clear: pour effacer l'ecran\n");
    printf("\n**pour acceder au github repository click CTRL-Z**\n\n");
    printf("*****************pour plus d'info tapez : help NomCommande************\n");
    
    }




