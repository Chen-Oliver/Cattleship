#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/shm.h>
void error_check( int i, char *s ) {
  if ( i < 0 ) {
    printf("%d\n", i);
    printf("[%s] socket error %d: %s\n", s, errno, strerror(errno) );
    exit(1);
  }
}

int server_setup() {

  int sd;
  int i;

  sd = socket( AF_INET, SOCK_STREAM, 0 );
  error_check( sd, "server socket" );

  struct sockaddr_in sock;
  sock.sin_family = AF_INET;
  sock.sin_addr.s_addr = INADDR_ANY;
  sock.sin_port = htons(9001);
  i = bind( sd, (struct sockaddr *)&sock, sizeof(sock) );
  error_check( i, "server bind" );
 //creates semaphore of value 2 to limit connections to 2 players
  int semkey = ftok("makefile",23);
  int semid = semget(semkey,1,IPC_CREAT|IPC_EXCL|0666);
  union semun su;
  su.val = 1;
  semctl(semid,0,SETVAL,su);
  int shmkey = ftok("makefile",6);
  int shmid = shmget(shmkey,3*sizeof(int),0666|IPC_CREAT);
  return sd;

}

int server_connect(int sd) {
  int connection, i;

  i = listen(sd, 1);
  error_check( i, "server listen" );

  struct sockaddr_in sock1;
  unsigned int sock1_len = sizeof(sock1);
  connection = accept( sd, (struct sockaddr *)&sock1, &sock1_len );
  error_check( connection, "server accept" );

  printf("[server] connected to %s\n", inet_ntoa( sock1.sin_addr ) );
  return connection;
}


int client_connect( char *host ) {
  int sd, i;

  sd = socket( AF_INET, SOCK_STREAM, 0 );
  error_check( sd, "client socket" );

  struct sockaddr_in sock;
  sock.sin_family = AF_INET;
  inet_aton( host, &(sock.sin_addr));
  sock.sin_port = htons(9001);

  printf("[client] connecting to: %s\n", host );
  //when a client connects, down semaphore by 1
  int semkey = ftok("makefile",23);
  int semid = semget(semkey,1,0);
  struct sembuf sb;
  sb.sem_op=-1;
  sb.sem_num=0;
  sb.sem_flg=SEM_UNDO;
  semop(semid,&sb,1);
  i = connect( sd, (struct sockaddr *)&sock, sizeof(sock) );
  error_check( i, "client connect");

  return sd;
}
