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

void *listening(void *) ;
void getlocalip(char *);
//void sending(struct sockaddr_in *);
pthread_t threads[MAX_THREADS];

int main(int argc, char *argv[]){
	
	int serversocket;//, listensocket;
	char serverip[15];
	char serverport[8];
	int server_port;
	char username[128];
	char userID[128];
	char ownport[8];
	int own_port;
	char buffer[128];
	int threadnumber=1;
	char receiverip[128];

	
	serversocket= socket(PF_INET, SOCK_STREAM, 0);
	//listensocket = socket(PF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in server_addr;
	struct sockaddr_in my_addr;
	
	memset(&server_addr,0,sizeof(server_addr));
	
	printf("請輸入server ip: ");
	gets(serverip);
	printf("serverip = %s\n",serverip);
	printf("請輸入server port: ");
	memset(serverport,'\0',strlen(serverport));
	gets(serverport);
	server_port = atoi(serverport);
	printf("server port = %d\n",server_port);
		
	server_addr.sin_family = PF_INET;
	server_addr.sin_addr.s_addr = inet_addr(serverip);
	server_addr.sin_port = htons(server_port);
	
	if((connect(serversocket , (struct sockaddr*)&server_addr , sizeof(server_addr)))==-1){
		perror("connect");
		exit(1);
	}
	
	printf("enter username: ");
	gets(username);
	printf("the port you want to use: ");
	gets(ownport);
	own_port = atoi(ownport);
	
	memset(&my_addr,0,sizeof(my_addr));
	
// start get ip********************************************************************************************
    struct ifaddrs *ifaddr, *ifa;
    int family, s;
    char host[NI_MAXHOST];
    char *a;

    if (getifaddrs(&ifaddr) == -1) 
    {
        perror("getifaddrs");
        //exit(EXIT_FAILURE);
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) 
    {
        if (ifa->ifa_addr == NULL){
        	continue;
        }

        family = ifa->ifa_addr->sa_family;

        if (family == AF_INET) 
        {
            s = getnameinfo(ifa->ifa_addr,sizeof(struct sockaddr_in),
                    host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
        }
        
	}
	a = host;
    freeifaddrs(ifaddr);
//end get ip *************************************************************************************	
	
	my_addr.sin_family = PF_INET;
	my_addr.sin_port = htons(own_port);
	my_addr.sin_addr.s_addr = inet_addr(a);

	pthread_create(&(threads[threadnumber]),NULL,listening,(void*)&my_addr);//listen
	threadnumber++;
	
	strcpy(userID,username);
	strcat(username,"#");
	strcat(username,ownport);
	
	send(serversocket,username,strlen(username),0);
	
	
	char choose;
	char damnenter;
	int flag=0;
	
	while(flag==0){
		
		printf("a for list,b for chat,c for leave :\n");
		scanf("%d",&choose);
		
		choose=getchar();
		scanf("%c",&damnenter);
		
		if(choose == 'a'){
			send(serversocket,"List",strlen("List"),0);
			memset(&buffer , 0 , strlen(buffer));
			recv(serversocket, buffer, sizeof(buffer), 0);
			printf("%s",buffer);

		}
		else if(choose == 'b'){//not fin yet
			
			char wanttoconnectport[8];
			//int int_wanttoconnectport;
			struct sockaddr_in receiver_addr;
			
			send(serversocket,"List",strlen("List"),0);
			memset(&buffer , 0 , strlen(buffer));
			recv(serversocket, buffer, sizeof(buffer), 0);
			printf("%s",buffer);
			
			//****************** enter name get ip start
			char b[128];
			char r[128];
			char s[128];
			char *loc;
			int i;
			int int_wanttoconnectport=0;
			memset(r,'\0',strlen(r));
			memset(s,'\0',strlen(s));
			printf("輸入想交談的使用者ID：");
		    gets(b);
			loc = strstr(buffer, b);
			if(loc == NULL) {
		        printf("找不到符合的子字串\n");
		    }
		    else {
		       // printf("在索引位置 %d 處找到子字串\n", loc - buffer);
		        //for(i=0;i<;i++)
		
				for(i=(loc - buffer + strlen(b) + 1);buffer[i]!='#';i++){
					r[int_wanttoconnectport]=buffer[i];
					int_wanttoconnectport++;
					
					//printf("%c",buffer[i]);
				}
			//	printf("\n");
				
		    }
		    //printf("ip=%s\n",r);
		    
			strcat(b,"#");
			strcat(b,r);
			//printf("b now = %s\n",b);
			
			loc = strstr(buffer, b);
			int_wanttoconnectport=0;
			if(loc == NULL) {
		        printf("找不到符合的子字串\n");
		    }
		    else {
		        //printf("在索引位置 %d 處找到子字串\n", loc - buffer);
		        
				for(i=(loc - buffer + strlen(b) + 1);i<(loc - buffer + strlen(b) + 1 + strlen(b));i++){
					s[int_wanttoconnectport]=buffer[i];
					int_wanttoconnectport++;
					//printf("%c",buffer[i]);
				}
				//printf("\n");
				
		    }
		    int_wanttoconnectport=atoi(s);
		    //printf("int_wanttoconnectport=%d\n",int_wanttoconnectport);
			//****************** enter name get ip end
			memset(&receiver_addr , 0 , sizeof(receiver_addr));
			//give the ip destnation
			/*
			printf("請輸入receiver ip: ");
			gets(receiverip);
			printf("%s\n",receiverip);
			
			
			printf("choose the user u want to connect: ");
			memset(wanttoconnectport,'\0',strlen(wanttoconnectport));
			
			gets(wanttoconnectport);
			
			int_wanttoconnectport = atoi(wanttoconnectport);
			printf("port = %d\n",int_wanttoconnectport);*/
			
			receiver_addr.sin_family = PF_INET;
			receiver_addr.sin_port = htons(int_wanttoconnectport);
			receiver_addr.sin_addr.s_addr = inet_addr(r);	
			
			//sending(&receiver_addr);
			
			int sendsocket;
			sendsocket = socket(PF_INET, SOCK_STREAM, 0);
			char buffer2[128];
			
			printf("enter the message here : ");
			memset(&buffer2 , 0 , strlen(buffer2));
			
			gets(buffer2);
			
			strcat(buffer2," ( sent form ");
			strcat(buffer2,userID);
			strcat(buffer2," )");
			
			printf("in sending now\n\n");
			
			if(connect(sendsocket , (struct sockaddr*)&receiver_addr , sizeof(receiver_addr))==-1){
				printf("others logout\n");
				perror("connect");
				}
		
			
			send(sendsocket,buffer2,strlen(buffer2),0);
			memset(&buffer2 , 0 , strlen(buffer2));
			close(sendsocket);
			
			/*pthread_create(&(threads[threadnumber]),NULL,sending,(void*)&receiver_addr);
			threadnumber++;*/
		}
		else if(choose == 'c'){
			send(serversocket,"Exit",strlen("Exit"),0);
			memset(&buffer , 0 , strlen(buffer));
			recv(serversocket, buffer, sizeof(buffer), 0);
			printf("%s",buffer);
			flag=1;
			
		}
		else{
			printf("plz enter again :\n");
			choose='\0';
			choose=getchar();
		}
	    choose='\0';
	}
	close(serversocket);
	printf("logout success!\n");
	printf("press crtl+c to leave\n");
	
}

void *listening(void *my_addr){
	
	int listensocket;
	listensocket = socket(PF_INET, SOCK_STREAM, 0);
	struct sockaddr_in my = *(struct sockaddr_in*)my_addr;
	struct sockaddr_in other_addr;//接其他人的位置 
	char buffer1[128];

	if(bind(listensocket, (struct sockaddr *)&my, sizeof(struct sockaddr))==-1){
		perror("bind");
		exit(1);
	}
	    if(listen(listensocket, 10)==-1){
		perror("listen");
		exit(1);
	}
	printf("listening now ˊˇˋ\n");
	
	while(1){
		
		int new_socket;
		
		int size = sizeof(struct sockaddr_in);
		
		if((new_socket = accept(listensocket, (struct sockaddr*)&other_addr, &size))==-1){
			perror("accept");
            continue;
		}
		
		recv(new_socket,buffer1,sizeof(buffer1),0);
		printf("%s \n",buffer1);
		
		memset(&buffer1 , 0 , sizeof(buffer1));
		
		close(new_socket);
	}
	close(listensocket);
}



/*
void sending(struct sockaddr_in * receiver_addr){
	
	int sendsocket;
	sendsocket = socket(PF_INET, SOCK_STREAM, 0);
	//struct sockaddr_in receiver = *(struct sockaddr_in*)receiver_addr;
	char buffer2[128];
	
	printf("enter the message here : ");
	memset(&buffer2 , 0 , strlen(buffer2));
	
	gets(buffer2);
	
	printf("in sending now\n");
	
	if(connect(sendsocket , (struct sockaddr*)&receiver_addr , sizeof(receiver_addr))==-1){
		printf("others logout\n");
		perror("connect");
	}

	
	send(sendsocket,buffer2,strlen(buffer2),0);
	memset(&buffer2 , 0 , strlen(buffer2));
	close(sendsocket);
}
*/   
