#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <netdb.h>
#include <iostream>
using namespace std;

void sys_err(const char *str)
{
    perror(str);
    exit(1);
}

int main (int argc, char *argv[])
{
    int cfd, ret, s1, r1;
    struct sockaddr_in serv_addr;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(34706);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr.s_addr);//set server(scheduler) address
    
    char buff[1024];
    int clielocation;
    clielocation = atoi(argv[1]);
    memset(&buff, 0, sizeof(buff));
    sprintf(buff, "%d", clielocation);
    
    cout << "The client is up and running" << endl;

    cfd = socket(AF_INET, SOCK_STREAM, 0);// create TCP socket
    if(cfd == -1)
    {
        sys_err("socket error");
    }

    ret = connect(cfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if(ret != 0)
    {
        sys_err("connect error");
    }
       
     s1 = send(cfd, buff, sizeof(buff), 0);//send initial information to scheduler through TCP
     if(s1 == -1)
     {
         sys_err("send error");
     }
     else
     {
         cout<< "The client has sent query to Scheduler using TCP: client location " << clielocation << endl;
     }

     memset(buff, 0, sizeof(buff));
     r1 = recv(cfd, buff, sizeof(buff), 0);//receive the decision of hospital assignment from the scheduler through TCP
     if(r1 == -1)
     {
         sys_err("receive error");
     }

     char x;
     x = buff[0];
     if(x != 'Z' && x!= 'S' && x != 'N')
     {
         cout << "The client has received results from the Scheduler: assigned to Hospital " << buff << endl;
     }//normal hospital assignment
     else if(x == 'Z' || x == 'S')
     {
         cout << "The client has received results from the Scheduler: assigned to Hospital None " << endl;
         cout << "Score = None, No assignment" << endl;
     }//all the hospitals have the score = None or client location is the same as hospital
     else if(x == 'N')
     {
         cout << "The client has received results from the Scheduler: assigned to Hospital " << buff << endl;
         cout << "Location " << clielocation << " not found" << endl;
     }//client location is not in the map 
    close(cfd);
}