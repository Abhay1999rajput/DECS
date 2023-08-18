#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>
#include <fcntl.h> 
#define MAX_INPUTSIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64
int tokenNo=0;
int array[64];
int curr=0;
int fg=0;
char *addr="";

 int count_c=0;
 
void sig_handler(int signum) 
{ 

        
         if(fg!=0)
         {//   printf("check11");
         kill(fg,SIGTERM);
         printf("\nfg_killed-%d\n",fg);
       // fg  
         count_c=0;
         }
         else
         {
          //signal(signum,SIG_IGN);
           count_c=0;
           printf("\n");
         } //  printf("No fg to kilslee
         //exircount_c=0;
     
     return;
} 
 

/*void sig_handler(int signum)
{
    signal(signum,SIG_IGN);
    
    kill(fg,SIGKILL);

}*/
char ** tokenizer(char * line)
{
    char **tokens =(char**)malloc(MAX_NUM_TOKENS*sizeof(char *));
    char *token=(char*)malloc(MAX_TOKEN_SIZE*sizeof(char ));
    int i=0,tokenIndex=0;

    tokenNo=0;
for(int i=0;i<strlen(line);i++)
    {
        char c = line[i];
        if(c=='\n'||c==' '||c=='\t') {          
            token[tokenIndex]='\0';
            if(tokenIndex!=0)
            {
             
                tokens[tokenNo]=(char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
                strcpy(tokens[tokenNo++],token);
                  tokenIndex=0;
                
            }
        }
        else
        {
            token[tokenIndex++]=c;
            
        }
  
    }

    free(token);
    tokens[tokenNo]=NULL;

return tokens;
    
}
void add_in_array(int pid)
{
    for(int i=0;i<64;i++)
    {

        if(array[i]==-2)
        {
            array[i]=pid;
            break;
        }   

    }

}
void remove_fro_array(int pid)
{
    for(int i=0;i<64;i++)
    {

        if(array[i]==pid)
        {
            array[i]=-2;
            break;
        }   

    }

}
void bg_process(char *tokens[])
{

     int fc=fork();
    if(fc<0)
        {
            fprintf(stderr,"%s\n","unable to create child Process!!\n");
        }
    else if(fc==0)
        {
           
             execvp(tokens[0],tokens);
            printf("command failed!! \n");
            exit(0);

        }
     else{ 

        setpgid(fc,fc);
        // perror("setpid"); 

       
          //fg=fc;
    printf("%d process in in bg\n",fc);
        add_in_array(fc);
        curr++;
     }
       
       
}

void fg_pro(char *tokens[])
{

     count_c=1;
     int fc=fork();
    if(fc<0)
        {
            fprintf(stderr,"%s\n","unable to create child Process!!\n");
        }
     else if(fc==0)
        {
               
             execvp(tokens[0],tokens);
            printf("command failed!! \n");
            exit(0);

        }
       else 
        {
            
        
            setpgid(fc,fc) == 0; //perror("setpid"); 
             fg=fc;

          int  wc=wait(NULL);
        
           //if(w)
          
           while(wc!=fc)
           { printf(" process is removed %d %d \n",wc,fc);
               remove_fro_array(wc);
              wc=wait(NULL);
              

           }
          

        }
}
  
void go_exit(char * tokens[])    //for exit command 
{
    //printf("w");
        for(int i=0;i<64;i++)     // check till last elemnt of array in PID of Bg stored
    {
        //printf("%d ",array[i]);

        if(array[i]>0)
        kill(array[i],SIGTERM);
        wait(NULL);      // kill runing bg process
    }
    for(int i=0;tokens[i]!=NULL;i++)
{
    free(tokens[i]);
}
free(tokens); // free memory

    
   return;
    
}
void remove_add()   //for removing last address from string
{
    int l=strlen(addr);
    for(int i=l-2;i>=0;i--)
    {
        if(addr[i]=='/')      //check till backslash
        {
            break;
        }
        else
        {
            addr[i]='\0';
        }
    }
}

int main(int argc,char * argv)
{
   
    addr="";
     signal(SIGINT,sig_handler); 
   for(int i=0;i<64;i++)
   {
    array[i]=-2;
   }

    char line[MAX_INPUTSIZE];
    char **tokens;
    int i;
    char str[MAX_INPUTSIZE]="";
      char prev[MAX_INPUTSIZE]="";
      int c=0;
    addr=(char*)malloc(MAX_INPUTSIZE*sizeof(char ));
    strcpy(addr,"\0");
   // str="";
    while (1)
    {
        fg=0;
        bzero(line,sizeof(char)*MAX_INPUTSIZE);
        printf("%s",addr);
        printf("$ ");
        scanf("%[^\n]",line);
        getchar();
        int n=strlen(line);
       line[n]='\n';
        tokens=tokenizer(line);
        if(tokens[0]==NULL)
        continue;
       
           if(strcmp(tokens[0],"exit")==0 )
           {
            go_exit(tokens);
          // continue;
            return 0;
           }
       
        if(strcmp(tokens[0],"cd")==0 )
        {
                  
                  
                
                if(strcmp(tokens[1],"..")==0 )
                {
                   if(strcmp(addr,"")!=0){
                 if(chdir(tokens[1])!=0){
                    printf("invalid Directory\n");
                 continue;
                    }if(addr[0]=='\0')
                    continue;
                    
                  remove_add();}
                  // str[0]='\0';
                   //strcpy(str,prev);
                }
                else
                   {
                    if(chdir(tokens[1])!=0){
                    printf("invalid Directory\n");
                    continue;
                    }
                    
                    strcat(addr,tokens[1]);
                    strcat(addr,"/");

                    //strcpy(prev,str);
                   // strcat(str,tokens[1]);
                   // strcat(str,"/");
                    
                    
                }

                    
                
                

                
            continue;
        }
       
       
        if(strcmp(tokens[tokenNo-1],"&")==0)
       { 
        tokens[tokenNo-1]=0;
        bg_process(tokens);     // call for background process
        
        continue;
       }
       else
       {
        fg_pro(tokens);
       }
     
       
   
    }
      

                

for(int i=0;tokens[i]!=NULL;i++)
{
    free(tokens[i]);
}
free(tokens);

    
    return 0;
    

}
