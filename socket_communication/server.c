// Jan 2018


#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <string.h> 

/*for 64bit OS*/ 
#include <arpa/inet.h> //inet_ntoa() 
#include <unistd.h> //write(),close() 

#define MAX_BUF_SIZE 256 
#define BACKLOG 5 

int main(int argc, char *argv[]) {

    int s,n,c; 
    struct sockaddr_in sa; 
    char buf[MAX_BUF_SIZE + 1]; 
    int len; 
    struct sockaddr_in csa; 
#ifndef NOREUSEADDR 
    int nonzero = 1; 
#endif 

    int port = 4000; // 待ち受けポート番号

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
        perror("socket error"); 
        exit(1); 
    } 

    bzero(&sa, sizeof(sa)); 
    sa.sin_family = AF_INET; 
    sa.sin_port = htons(port); 
    sa.sin_addr.s_addr = htonl(INADDR_ANY); 

#ifndef NOREUSEADDR 
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &nonzero, sizeof(nonzero)); 
#endif 

    if (bind(s, (struct sockaddr *)&sa, sizeof(sa)) < 0) { 
        perror("bind error"); 
        exit(1); 
    } 

    if (listen(s, BACKLOG) < 0) { 
        perror("listen error"); 
        exit(1); 
    } 

    n = sizeof(csa); 
    if ((c = accept(s, (struct sockaddr *)&csa, (socklen_t *)&n)) < 0) { 
        perror("connect error"); 
        exit(1); 
    } 

    while(1){ 

      len = read(c, buf, MAX_BUF_SIZE); 
      buf[len] = '\0'; 


      printf("%s\n",buf); 
      	fflush(stdout); 

      if(strcmp(buf,"quit")==0){{
          close(c); 
          return 0; 
        } 

      scanf("%s",buf); 

      write(c, buf, strlen(buf)); 

      if(strcmp(buf,"quit")==0){ 
          close(c); 
          return 0; 
        } 

    } 
} 

/*
  4000番ポートでclientからの通信を待ち受け
  チャットを繰り返す
  どちらかが「quit」を入力すると終了する
 */


