#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

    int executando = 1;

    while(executando){

        char linha[1024];

        printf("mini-shell> ");

        fgets(linha, sizeof(linha), stdin);

        char *argumentos[100];

        int n=0;

        char *token;
        
        token=strtok(linha, " ");

        while(token!=NULL){

            argumentos[n]=token; 

            n++;

            token=strtok(NULL, " ");
        }

        argumentos[n]=NULL;

        int pid = fork();

        if(pid < 0){
            
            printf("Erro. \n");

            return -1;

        } else if(pid==0){

            execvp();

        }else{

            wait();
        }

        for(int i=0; i<n; i++){
            printf(argumentos[i]);
            printf(" ");
        }

        if(strcmp(argumentos[0],"exit\n")==0){ 
            executando=0;
        }

    }

    return 0;
}