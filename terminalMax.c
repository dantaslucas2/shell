#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

//Tratando o diretodio de acordo com o sistema operacional.
    #ifdef _WIN32
        printf("Esse programa nao e compativel com windows\n");
    #elif __APPLE__
        char  demo[10] = "/sbin/";
    #elif __linux__
        char  demo[10] = "/bin/";
    #endif

//funcao principal do progrando, responsavel pelo codigo do shell
void terminal(){

    char comando[30];
    int n_argumentos;

    scanf("%s", comando);

    //concatenar diretorio e comando
    int tamanho1 = 0;
    int tamanho2 = 0;
    while (demo[tamanho1]) tamanho1++;
    while (comando[tamanho2]) tamanho2++;
    int tamanho3 = tamanho1 + tamanho2 + 1;
    char *diretorio = (char *) malloc(tamanho3);
    for (int i = 0; demo[i]; i++) {
        diretorio[i] = demo[i];
    }
    for (int i = 0; comando[i]; i++) {
        diretorio[i + tamanho1] = comando[i];
    }
    diretorio[tamanho1 + tamanho2] = 0;
    //fim concatenacao

    //Leitura da quantidade de argumentos da funcao
    printf("Quantos argumentos você quer digitar? \n");
    scanf("%d", &n_argumentos);
    n_argumentos = n_argumentos + 1;

    //criando vetor dinamico para armazenar os argumentos
    char **argumentos = malloc(n_argumentos *sizeof(char*));

    //Fork()
    pid_t child_pid = fork();

    if (child_pid == 0) {
        //inserindo o comando, no vetor de argumentos
        char *argumento = malloc(sizeof(comando) *sizeof(char));
        argumento = comando;
        argumentos[0] = argumento;
        //inserindo agumentos no vetor argumentos
        for (int i = 1; i < n_argumentos; i++) {
                printf("Digite o argumento %d \n", i);
                char *argumento = malloc(sizeof(argumento) *sizeof(char));
                scanf("%s", argumento);
                argumentos[i] = argumento;
        }

        // inserindo valor NULL ao fim do vetor argumentos
        argumentos[n_argumentos] = NULL;

        // verifica se o comando fornecido pelo usuario e valido
        int ver = execv(diretorio, argumentos);
        if (ver == -1){
            printf("Error!!  Comando invalido!\nQual comando quer executar? \n");
            terminal();
            return;
        }
    }else {
        wait(NULL);
        return;
    }
    return;
}

// funcao para tratar o sinal SIGUSR1
void sinal(int signo){
  if (signo == SIGUSR1){
    printf("\n!!Sinal Recebido, reiniciando!!\nQual comando quer executar? \n");
    terminal();
  }
}

int main(void){
    if (signal(SIGINT, sinal) == SIG_ERR)
        printf("Nao foi possivel capturar o sinal");

    printf("Qual comando quer executar? \n");
    terminal();
    return 0;
}
