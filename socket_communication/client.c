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

int main(void){ 

    int s; 
    struct sockaddr_in sa,csa; 
    char buf[MAX_BUF_SIZE + 1]; 
    int len; 

    int port = 4000; //接続先ポート番号 
    char ipaddr[1024] = {"172.27.80.69"}; //IPアドレス設定
    
    
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) { /* socket create */ 
        perror("socket error"); 
        exit(1); 
    }    
    
    bzero(&sa, sizeof(sa)); 
    sa.sin_family = AF_INET; 
    sa.sin_port = htons(port); 
    if (inet_pton(AF_INET, ipaddr, &sa.sin_addr) <= 0) { 
        perror("inet_pton error"); 
        exit(1); 
    } 
    
    if (connect(s, (struct sockaddr *)&sa, sizeof(sa)) < 0) { 
        perror("connect error"); 
        exit(1); 
    } 

    while(1){ 

        scanf("%s",buf); 

        write(s, buf, strlen(buf)); 

        if(strcmp(buf,"quit") == 0){ 
            close(s); 
            break; 
        } 

        len = read(s, buf, MAX_BUF_SIZE); 
        buf[len] = '\0'; 

        printf("%s\n",buf); 
        fflush(stdout); 

        if (len < 0) { 
            perror("read error"); 
        } 

        if(strcmp(buf,"quit") == 0){ 
            close(s); 
            break; 
        } 

    } 
}

/*
 4000番で待ち受けてるサーバーとチャットをするプログラム
 どちらかが「quit」を入力すると終了する
 */
