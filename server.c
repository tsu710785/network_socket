#include <arpa/inet.h>
#include <ifaddrs.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_THREADS 50
int threadnumber=0;
char list[1000][1000];//list[threadnumber][string]
void *receive(void*) ;

pthread_t threads[MAX_THREADS];

int main(int argc, char *argv[]){
    struct sockaddr_in my_addr;
	struct sockaddr_in other_addr;
    
    
    int listensocket;
    
    listensocket = socket(PF_INET, SOCK_STREAM, 0);
    my_addr.sin_family = PF_INET;
	my_addr.sin_addr.s_addr = INADDR_ANY;
	my_addr.sin_port = htons(atoi(argv[1]));
    
    if(bind(listensocket, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))==-1){
		perror("bind");
		exit(1);
	}
    if(listen(listensocket, 10)==-1){
		perror("listen");
		exit(1);
	}
	printf("listening now\n");
	
	while(1){
        int new_socket;
		
		socklen_t size = sizeof(struct sockaddr_in);
		
        if((new_socket =accept(listensocket, (struct sockaddr*)&other_addr, &size))==-1){
			perror("accept");
            continue;
		}
        pthread_create(&(threads[threadnumber]),NULL,receive,(void*)&new_socket);//listen
        
        
    }
}

void *receive(void* new_socket){
    
    char buffer1[1000];
    int new;
    new=*(int*)new_socket;
    recv(new,buffer1,sizeof(buffer1),0);
    printf("%s \n",buffer1);
    memset(&buffer1 , 0 , sizeof(buffer1));
    send(new,"100 OK\n",strlen("100 OK\n"),0);
    
    recv(new,list[threadnumber],sizeof(list[threadnumber]),0);
    printf("%s \n",list[threadnumber]);
    int id;
    id = threadnumber;
    threadnumber++;
    int i;
    char onlinelist[10000];
    for(i=0;i<=threadnumber;i++){
        if(list[i]!='\0'){
            strcat(onlinelist,list[i]);
            strcat(onlinelist,"\n");
        }
    }
    send(new,onlinelist,strlen(onlinelist),0);
    
    memset(&buffer1 , 0 , sizeof(buffer1));
    recv(new,buffer1,sizeof(buffer1),0);
    
    while(strcmp(buffer1,"Exit")!=0){
        
        if(strcmp(buffer1,"List")==0){
            
            memset(&onlinelist , 0 , sizeof(onlinelist));
            for(i=0;i<=threadnumber;i++){
                if(list[i][0]!='\0'){
                    strcat(onlinelist,list[i]);
                    strcat(onlinelist,"\n");
                }
            }
            send(new,onlinelist,strlen(onlinelist),0);
        }
        memset(&buffer1 , 0 , sizeof(buffer1));
        recv(new,buffer1,sizeof(buffer1),0);
    }
    send(new,"Bye",strlen("Bye"),0);
    
    memset(&list[id] , '\0' , sizeof(list[id]));
    printf("listid=%s=",list[id]);
    pthread_exit(0);
}
