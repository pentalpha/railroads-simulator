#include "semaforo.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

/*Construtor
Parametros:
  key_t key: chave para criação dos semaforos
  int valorInicial: valor inicial do contador do semaforo (1)
  int flags: flags de permissao
 */
Semaforo::Semaforo(key_t key, int valorInicial, int flags){
  //Alocando apenas 1 semaforo
  semaforoId = semget(key,1,IPC_CREAT|0600);
  //Verificando erros
  if (semaforoId == -1){
    perror("Error ao criar semaforo\n");
    exit(1);
  }
  //Modificando o valor da estrutura de configuracao do semaforo
  arg.val = valorInicial;

  //Atualizando o contador do semaforo atraves do comando SETVAL
  semctl(semaforoId,0,SETVAL,arg);
}

/*Destrutor
*/

Semaforo::~Semaforo(){
  semctl(semaforoId,0,IPC_RMID,arg);
}

/*
Função utilizada para entrada na região critica.
Decrementa o contador do semaforo.
*/

void Semaforo::P(){
  estruturaOperacao.sem_num = 0; //o grupo é formado por apenas 1 semaforo
  estruturaOperacao.sem_op = -1; //P()
  //Ao finalizar o processo, outros processos que estao pendentes pelo semaforo sao desbloqueados
  estruturaOperacao.sem_flg = SEM_UNDO;
  semop(semaforoId,&estruturaOperacao,1);
}

/*
Função utilizada para saida na região critica.
Incrementa o contador do semaforo.
*/

void Semaforo::V(){
  estruturaOperacao.sem_num = 0; //o grupo é formado por apenas 1 semaforo
  estruturaOperacao.sem_op = 1; //P()
  //Ao finalizar o processo, outros processos que estao pendentes pelo semaforo sao desbloqueados
  estruturaOperacao.sem_flg = SEM_UNDO;
  semop(semaforoId,&estruturaOperacao,1);
}

/*
  Retornar o atributo privado semaforoId
*/
int Semaforo::getSemaforoId(){
  return semaforoId;
}


/*
  Retornar o atributo privado referente ao contador do semaforo
*/
int Semaforo::getContador(){
  return semctl(semaforoId,0,GETVAL,arg);
}



