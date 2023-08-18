#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<bits/stdc++.h>
#include"http_server.hh"
#include <netinet/in.h>
#include <pthread.h>
#include<signal.h>
using namespace std;

#define MAX_THREAD  30

void error(char *msg) {
  perror(msg);
  exit(1);
}

pthread_t thread_id[30];

queue<int> q_td;
pthread_mutex_t lo= PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond=PTHREAD_COND_INITIALIZER;



void handle_sigint(int sig)
{
  exit(0);
}



 
void *handler(void *);
int main(int argc, char *argv[]) {
  signal(SIGINT, handle_sigint);

int sockfd, newsockfd, portno;
  socklen_t clilen;

  struct sockaddr_in serv_addr, cli_addr;
  int n;

  if (argc < 2) {
    fprintf(stderr, "ERROR, no port provided\n");
    exit(1);
  }



  



  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    error("ERROR opening socket");

  /* fill in port number to listen on. IP address can be anything (INADDR_ANY)
   */

  bzero((char *)&serv_addr, sizeof(serv_addr));
  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  /* bind socket to this port number on this machine */

  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    error("ERROR on binding");

  /* listen for incoming connection requests */

  if(listen(sockfd, 90)<0)
  error("ERROR on binding");
  
  clilen = sizeof(cli_addr);
 
  /* accept a new request, create a newsockfd */


// int i;

 for(int i = 0; i <30; i++){
 pthread_create(&thread_id[i], NULL,handler, NULL);
 //pthread_detach(thread_id[i]);
 }

  while(1){
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
  
  if(newsockfd<0)
  {
     perror("error on accept");
     break;
   }
   // printf("connection stablished\n");

    pthread_mutex_lock(&lo);
    //while(q_td.size()==MAX_THREAD)
      //pthread_cond_wait(&cond,&lo);
     

     q_td.push(newsockfd);
  
     pthread_cond_signal(&cond);
     pthread_mutex_unlock(&lo);

    }
  
     
    
   
 close(sockfd);
   for(int i=0;i<30;i++)
  {
         pthread_join(thread_id[i], NULL);
  }

return 0;

}
void *handler(void *newsfd)
{
  int n;
	
  //int newsockfd = *(int*)newsfd;
  cout<<"Succesfully accepeted \n";

    
while(1)
{	
    
pthread_mutex_lock(&lo);
    while(q_td.empty())
       pthread_cond_wait(&cond,&lo);
    //cout<<q_td.size()<<"queue"<<endl;
      int pc= q_td.front();
      q_td.pop();
    pthread_cond_signal(&cond);
      pthread_mutex_unlock(&lo);
  if(pc){    
    char buffer[256];    
//cout<<"a ";
  bzero(buffer, 256);
  n = read(pc, buffer, 255);
 
  if (n < 0)
    error("ERROR reading from socket");
  //cout<<"Server Connected\n";
  HTTP_Response* Resp= handle_request(buffer);

//cout<<buffer<<endl<<endl;

  /* send reply to client */
char p[2048];

	int ll=Resp->body.length();
	for (int j = 0; j < ll; j++) {
		p[j] = Resp->body[j];
		//cout << p[i];
	}

  //cout<<p;
  n = write(pc,p,ll );
 
  if (n < 0)
    error("ERROR writing to socket");


 }

  close(pc);

}

 

 return 0;
}
