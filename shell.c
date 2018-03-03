//Dec 2017

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/wait.h> 
#include <unistd.h> 
#include <signal.h> 

void sig_fin(int sig); 
void sig_child(int sig); 

int now_pid; 
int status; 

int p_n=0; 
int p_id[1024]={}; 
char p_name[1024][1024]={}; 


int main(void) { 

  int i; 

  if(signal(SIGINT,sig_fin)==SIG_ERR){ 
    puts("error"); 
  } 


  if(signal(SIGCHLD,sig_child)==SIG_ERR){ 
    puts("error"); 
  } 

  while(1){ 

    now_pid = 0; 

    printf("$ "); 

    char str[1024]; 
    fgets(str,1024,stdin); 


    int n = strlen(str); 
    for(i = 0; i < n; i++) 
      if(str[i] == '\n') str[i] = '\0'; 


    if(strcmp(str, "exit") == 0 || strcmp(str, "quit") == 0){ 
      exit(0); 
    } 

    if(strcmp(str, "jobs") == 0){ 

      for(i = 0; i < p_n; i++){ 
	printf("pid:%d %s\n",p_id[i],p_name[i]); 
      } 
      continue; 

    } 

    strcpy(p_name[p_n], str); 

    char *arg[1024]; 
    int num = 1; 
    arg[0] = str; 
    for(i = 1; i < n; i++) 
      if(str[i] == ' ' && str[i-1] != '\0'){ 
	arg[num] = &str[i+1], num++; 
	str[i] = '\0'; 
      } 
    arg[num] = NULL; 

    int check = 0; 

    if(strcmp(arg[0],"fg") == 0){ 

      if(num == 1){ 
	puts("error"); 
	continue; 
      } 

      int fgpid = now_pid = atoi(arg[1]); 
      waitpid(fgpid,&status,0); 

      for(i = 0; p_id[i] != fgpid; i++); 
      for(; i+1 < p_n; i++){ 
	p_id[i] = p_id[i+1]; 
	strcpy(p_name[i],p_name[i+1]); 
      } 
      p_n--; 

      continue; 

    } 


    if(strcmp(arg[num-1],"&") == 0){ 
      check = 1; 
      arg[num-1] = NULL; 
      num--; 
    } 

    int p = now_pid = fork(); 

    if(p == -1){//error 

      printf("fork error\n"); 

    }elseif(p == 0){//child 

      setpgid(0,0); 

      if(arg[0][0]=='/'){ 

	if(execv(str,arg)==-1) 
	  printf("command not found\n"); 

      }else{ 

	char *s = getenv("PATH"); 
	char tmp[1024] = {}; 
	strcpy(tmp,arg[0]); 

	int index = 0, cnt = 0; 
	char path[50][1024] = {}; 
	for(i = index; s[i] != '\0'; i++) 
	  if(s[i] == ':'){ 
	    strcat(path[cnt],"/"); 
	    strcat(path[cnt],tmp); 
	    arg[0] = &path[cnt][0]; 

	    char strtmp[1024]={}; 
	    strcpy(strtmp,path[cnt]); 
	    if(execv(strtmp,arg)!=-1)break; 
	    cnt++; 
	    index = i+1; 
	  }else{ 
	    path[cnt][i-index] = s[i]; 
	  } 
	if(s[i] == '\0'){ 
	  strcat(path[cnt],"/"); 
	  strcat(path[cnt],tmp); 
	  arg[0]=&path[cnt][0]; 

	  char strtmp[1024]={}; 
	  strcpy(strtmp,path[cnt]); 
	  if(execv(strtmp,arg)==-1) 
	    printf("command not found\n"); 
	} 

      } 

      exit(0); 

    }else if(p > 0){//parent 

      if(check == 0){//fg 

	waitpid(p,&status,0); 

      }else{//bg 

	p_id[p_n] = p; 
	for(i = (int)strlen(p_name[p_n])-1; p_name[p_n][i] != '&'; i--); 
	p_name[p_n][i-1] = '\0'; 
	p_n++; 

      } 
    } 


  } 
} 


void sig_fin(int sig){ 
  if(now_pid == 0) return; 

  if(kill(now_pid,SIGINT)==-1){ 

    puts("kill error"); 

  }else{ 

    printf("finish pid: %d \n",now_pid); 

  } 

} 

void sig_child(int sig){ 
  int i; 
  int fin = waitpid(-1,&status,WNOHANG);
  if(fin > 0){ 

    for(i = 0; p_id[i] != fin; i++); 
    for(; i+1 < p_n; i++){ 
      p_id[i] = p_id[i+1]; 
      strcpy(p_name[i],p_name[i+1]); 
    } 
    p_n--; 

    printf("finish pid: %d \n",fin); 
  } 

}
