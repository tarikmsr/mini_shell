#include "shell_function.h"
#include "tubesem.h"



Semaphore S1; 
void chrono (int sig) { 
    
   
    execlp("open", "open", "https://github.com/tarikmsr/mini_shell", NULL);
        perror("Erreur lors de l'exécution de la commande.");
    } 

void getSignal(int sig)
{
    if (sig == SIGINT) {
    printf("\nMerci pour utiliser mon shell\n");
    sleep(2); }
    else if (sig == SIGTSTP){
    signal(SIGALRM, chrono); 
    printf("\nouvrir dans 3 secondes\n"); 
    printf("Merci pour utiliser mon shell\n"); 
    printf("***************Tarik MSR*****************\n"); 
    alarm(3); 
    sleep(3);
    }
    exit(0);
}


void init() 
{ 
  // print welcome message

	printf("\nMini Projet Creation d'une shell\n");
    sleep(2); 
    clear(); 
} 

void cmd_execute(char **args) 
{
  if (fork() == 0)
    {
        P(S1);
       
        execlp("home/Tarik/Desktop/mini_shell/cmd", "home/Tarik/Desktop/mini_shell/cmd", args[0], args[1], args[2], NULL);
        perror("Erreur lors de l'exécution de la commande.");
        exit(0);
    }
    wait(0);
    V(S1);
}
void printPath() 
{ 
    char cwd[1024]; 
    getcwd(cwd, sizeof(cwd)); 
    printf("\nrépertoire courant: %s", cwd); 
} 
  
void start_shell(void)
{
  char *line;
  char **args;
  
    Initsem(S1, 1);
    signal(SIGINT, getSignal);
    signal(SIGTSTP,getSignal);
    while (1)
    {
        printf("%s#",getenv("USER"));
        line = read_line();
        args = split_line(line);
        cmd_execute(args);
          if(strcmp(args[0],"cd")==0 || strcmp(args[0],"enter")==0){
          chdir(args[1]);

        }
        free(line);
        free(args);
    }
}

/*
 ============================================================================

	Main method of this program

 ============================================================================
 */


int main(int argc, char **argv)
  {

  
    // Chargez le fichier de configuration. 
      init(); 
      while (1) { 
            printPath(); // print répertoire actuel
      printf(" \n ");
      start_shell(); // lancer le shell
    

      return 0;
    }
}
void Initsem(Semaphore S, int N)
{
    int i;
    char c = 'a';
    pipe(S);
    for (i = 1; i <= N; i++)
        write(S[1], &c, 1);
}

/* P sur le semaphore S, prendre un jeton 'a' */
void P(Semaphore S)
{
    char c;
    read(S[0], &c, 1);
}
/* V sur le semaphore S, remettre un jeton 'a' */
void V(Semaphore S)
{
    char c = 'a';
    write(S[1], &c, 1);
}
/* Attendre un nombre aleatoire de secondes entre 0 et N-1*/
void attente(int N)
{
    sleep(rand() % N);
}
