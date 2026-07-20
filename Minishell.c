#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    
    //criação da variável que vai servir para nortear a condição do while
    int executando = 1;

    //criação do laço while baseado na variável 'executando'
    while(executando){
        
        //vetor que vai receber a entrada do teclado
        char linha[1024];
        
        //prompt
        printf("mini-shell> ");

        //fgets usado para captar a entrada do teclado para a variável 'linha', de modo a já ter seu tamanho delimitado de acordo com o tamanho da entrada 
        fgets(linha, sizeof(linha), stdin);

        //variável para qual será transferida a entrada do teclado
        char *argumentos[100];

        //variável auxiliar para se 'passear' por dentro do vetor 'argumentos'
        int n=0;

        //ponteiro que funcionará de 'ponte' entre a entrada do teclado e o vetor 'argumentos'
        char *token;
       
        //função strtok acionada para transferir os segmentos da entrada delimitados por espaço ou fim de linha, para o ponteiro token
        //um detalhe importante aqui, é que a strtok foi usada para separar comandos da espécie de 'ls -la downloads' (por exemplo), que são espaçados, mas interessa todos estarem no vetor desejado
        //a partir dai, poderiamos usar somente a delimitação por espaço vazio, porém, esse mecanismo deve estar preparado para comandos de uma palavra só, tais quais 'pwd', 'cd' etc., por isso a delimitação também para final de arquivo/quebra de linha
        token=strtok(linha, " \n");

        //um loop para passar adequadamente todos os comandos para o vetor; a condição é caso o token seja NULL, ou seja, se não for passada informação alguma na entrada ou a linhade comando for lida por completo, o token vira NULL e o loop se encerra
        while(token!=NULL){

            //passagem do valor atual de token pro ponteiro
            argumentos[n]=token;

            //incremento da variável n, para progredirmos dentro do vetor durante o loop
            n++;

            //a continuação da passagem dos comandos da linha de comando para o ponteiro token, dando prosseguimento ao loop
            token=strtok(NULL, " ");
        }

        //uma vez encerrado o loop, a última 'casa' do vetor deve ser ocupada por NULL
        //caso o programa sequer entre no loop, esse n será igual a zero, e significa também que não foi passado nenhuma informação na linha de comando, então o vetor deve receber, em sua primeira e única casa, um valor nulo
        argumentos[n]=NULL;

        //condição para checar se foi passada alguma informação na linha de comando, caso não, pula para o próximo passo do loop, habilitando o usuário a dar um novo comando para o minishell
        if(argumentos[0]==NULL) continue;

        //condição para analisar se a linha de comando recebeu o comando 'exit', que é para fechar o minishell
        //utilizei o 'strcmp' para conseguir checar se o primeiro argumento do veto é equivalente ao comando 'exit'
        if(strcmp(argumentos[0],"exit")==0){
            
            //iguala a variável da condição do while principal a zero, valor que quebrará o loop
            executando=0;
            
            //aciona o 'continue' para pular para o próximo passo do loop, que não existirá, pois quando ele checar que a condição do loop não bate (executando==1), o programa sairá do loop
            continue;
        }

        //declaração da variável pid, que receberá o valor da função fork()
        //aciono a função fork(), me respaldando para o eventual 'execvp()' que vou utilizar mais adiante, a fim de que o minishell não se encerre com a execução da função
        //com o fork(), eu mantenho o minishell no processo pai, por mais que o processo filho vá se perder com o 'execvp()'
        int pid = fork();

        //laço para verificar qual processo estamos lidando
        //processo com pid<0 significa que deu algum erro no programa
        if(pid < 0){

            //citamos o erro para o usuário
            printf("Erro. \n");

            //encerramos o programa
            return -1;

        //segunda condição a se testar, se o processo em questão é o filho (pid=0)
        } else if(pid==0) {

            //sendo o processo filho, executamos o execvp, com a primeira casa do vetor 'argumentos' sendo o parâmetro que receberá o argv, que no caso, vai ser o vetor 'argumentos' em si
            execvp(argumentos[0], argumentos);

        //restada a última condição não testada, se o processo for pai, ou seja, herdou o pid do filho, logo (pid>0)
        } else {

            //caso seja o processo pai, esse é o que desejamos ser nossa salvaguarda para restaurar o minishell, então só o deixamos aguardando
            wait(NULL);
        }


    }

    //hora de encerrar o programa
    return 0;
}
