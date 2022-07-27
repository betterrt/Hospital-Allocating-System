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

int main(void)
{
    int sockfd;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);// create UDP socket
    if(sockfd == -1)
    {
        sys_err("socket error");
    }
    char buf[1024];
    int bdfd, ret;
    struct hospinfo
    {
        char name;
        int capacity;
        int occupancy;
    };
    char x,y;
    hospinfo hospA, hospB, hospC;

    struct sockaddr_in serv_addr, clie_addr;
    socklen_t clie_addr_len;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(33706);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);// set scheduler address

    bdfd = bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if(bdfd == -1)
    {
        sys_err("bind error");
    }
    clie_addr_len = sizeof(clie_addr);
    
    
     int pfd, bfd, lfd, cfd;
     char buff[1024];
     struct sockaddr_in tserv_addr, clit_addr;
     socklen_t clit_addr_len;
     
     tserv_addr.sin_family = AF_INET;
     tserv_addr.sin_port = htons(34706);
     tserv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

     struct sockaddr_in hospA_addr, hospB_addr, hospC_addr;
     bzero(&hospA_addr, sizeof(hospA_addr));
     hospA_addr.sin_family = AF_INET;
     hospA_addr.sin_port = htons(30706);
     inet_pton(AF_INET, "127.0.0.1", &hospA_addr.sin_addr.s_addr);// set hospital A address

     bzero(&hospB_addr, sizeof(hospB_addr));
     hospB_addr.sin_family = AF_INET;
     hospB_addr.sin_port = htons(31706);
     inet_pton(AF_INET, "127.0.0.1", &hospB_addr.sin_addr.s_addr);// set hospital B address

     bzero(&hospC_addr, sizeof(hospC_addr));
     hospC_addr.sin_family = AF_INET;
     hospC_addr.sin_port = htons(32706);
     inet_pton(AF_INET, "127.0.0.1", &hospC_addr.sin_addr.s_addr);// set hospital C address

     int ret2;
    struct hospscore
    {
        char name;
        double score;
        double distance;
    };
    hospscore hospscoreA, hospscoreB, hospscoreC;
     
     pfd = socket(AF_INET, SOCK_STREAM, 0);// create TCP socket
     if(pfd == -1)
     {
         sys_err("socket error");
     }
   
     bfd = bind(pfd, (struct sockaddr *)&tserv_addr, sizeof(tserv_addr));
     if(bfd == -1)
     {
        sys_err("bind error");
     }
     lfd = listen(pfd, 128);
     if (lfd == -1)
     {
         sys_err("listen error");
     }
     
     clit_addr_len = sizeof(clit_addr);

     cout << "The Scheduler is up and runnning" << endl;
     
    
 while(1)
 {
        memset(&buf, 0, sizeof(buf));
        for(int i = 0; i < 3; i++)
      {
        ret = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&clie_addr, &clie_addr_len);
        x = buf[0];
        if(ret == -1)
        {
            sys_err("reveive error");
        }
        else
        {
            if(x == 'A')
            {
                sscanf(buf, "%c %d %d", &hospA.name, &hospA.capacity, &hospA.occupancy);
                memset(&buf, 0, sizeof(buf));
            }
            else if(x == 'B')
            {
                sscanf(buf, "%c %d %d", &hospB.name, &hospB.capacity, &hospB.occupancy);
                memset(&buf, 0, sizeof(buf));
            }
            else if(x == 'C')
            {
                sscanf(buf, "%c %d %d", &hospC.name, &hospC.capacity, &hospC.occupancy);
                memset(&buf, 0, sizeof(buf));
            }
        }
      }//receive initial information from the hospital
    cout <<"The Scheduler has received information from Hospital A: total capacity is " << hospA.capacity <<" and initial occupancy is "<< hospA.occupancy << endl;
    cout <<"The Scheduler has received information from Hospital B: total capacity is " << hospB.capacity <<" and initial occupancy is "<< hospB.occupancy << endl;
    cout <<"The Scheduler has received information from Hospital C: total capacity is " << hospC.capacity <<" and initial occupancy is "<< hospC.occupancy << endl;


     cfd = accept(pfd, (struct sockaddr *)&clit_addr, &clit_addr_len);// TCP child socket
     if(cfd == -1)
     {
         sys_err("accept error");
     }
     
     memset(&buff, 0, sizeof(buff));
     recv(cfd, buff, sizeof(buff), 0);
    //receive initial information from the client
    cout <<"The Scheduler has received client at location " << buff << " from the client using TCP over port 34706" <<endl;
    memset(&buf, 0, sizeof(buf));
    strcpy(buf, buff);// pass the received client location to buf, prepare to send it to the hospital

    if(hospA.capacity >= hospA.occupancy)
    {
      sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&hospA_addr, sizeof(hospA_addr));
      cout << "The Scheduler has sent client location to Hospital A using UDP over port 33706" << endl;
    }
    if(hospB.capacity >= hospB.occupancy)
    {
      sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&hospB_addr, sizeof(hospB_addr));
      cout << "The Scheduler has sent client location to Hospital B using UDP over port 33706" << endl;
    }
    if(hospC.capacity >= hospC.occupancy)
    {
      sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&hospC_addr, sizeof(hospC_addr));
      cout << "The Scheduler has sent client location to Hospital C using UDP over port 33706" << endl;
    }
    //forward the client location inforamtion to hospitals through UDP
   
    memset(&buf, 0, sizeof(buf));
    
    for(int i = 0; i < 3; i++)
    {
      ret2 = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&clie_addr, &clie_addr_len);
      y = buf[0];
      if(ret == -1)
        {
            sys_err("reveive error");
        }
      else
        {
         if(y == 'A')
            {
                sscanf(buf, "%c %lf %lf", &hospscoreA.name, &hospscoreA.score, &hospscoreA.distance);
                memset(&buf, 0, sizeof(buf));
            }
            else if(y == 'B')
            {
                sscanf(buf, "%c %lf %lf", &hospscoreB.name, &hospscoreB.score, &hospscoreB.distance);
                memset(&buf, 0, sizeof(buf));
            }
            else if(y == 'C')
            {
                sscanf(buf, "%c %lf %lf", &hospscoreC.name, &hospscoreC.score, &hospscoreC.distance);
                memset(&buf, 0, sizeof(buf));
            }
        }
    }// receive hospital's score and distance through UDP, and store them in the structure
    
    if(hospscoreA.distance == -1)
    {
      cout << "The Scheduler has received map information from Hospital A, the score = None and the distance = None" << endl;
    }
    else if(hospscoreA.distance != -1 && hospscoreA.score == -1)
    {
      cout << "The Scheduler has received map information from Hospital A, the score = None and the distance = " << hospscoreA.distance << endl;
    }
    else if(hospscoreA.distance != -1 && hospscoreA.score != -1)
    {
      cout << "The Scheduler has received map information from Hospital A, the score = "<< hospscoreA.score << " and the distance = " << hospscoreA.distance << endl;
    }

     if(hospscoreB.distance == -1)
    {
      cout << "The Scheduler has received map information from Hospital B, the score = None and the distance = None" << endl;
    }
    else if(hospscoreB.distance != -1 && hospscoreB.score == -1)
    {
      cout << "The Scheduler has received map information from Hospital B, the score = None and the distance = " << hospscoreB.distance << endl;
    }
    else if(hospscoreB.distance != -1 && hospscoreB.score != -1)
    {
      cout << "The Scheduler has received map information from Hospital B, the score = "<< hospscoreB.score << " and the distance = " << hospscoreB.distance << endl;
    }

     if(hospscoreC.distance == -1)
    {
      cout << "The Scheduler has received map information from Hospital C, the score = None and the distance = None" << endl;
    }
    else if(hospscoreC.distance != -1 && hospscoreC.score == -1)
    {
      cout << "The Scheduler has received map information from Hospital C, the score = None and the distance = " << hospscoreC.distance << endl;
    }
    else if(hospscoreC.distance != -1 && hospscoreC.score != -1)
    {
      cout << "The Scheduler has received map information from Hospital C, the score = "<< hospscoreC.score << " and the distance = " << hospscoreC.distance << endl;
    }
    


    memset(buff, 0, sizeof(buff));

    if(hospscoreA.distance == -1 || hospscoreB.distance == -1 || hospscoreC.distance == -1 || (hospscoreA.score == -1 && hospscoreB.score == -1 && hospscoreC.score == -1))
    {
        if(hospscoreA.distance == -1 || hospscoreB.distance == -1 || hospscoreC.distance == -1 )
        { 
         if(hospscoreA.distance == -1 && hospscoreB.distance == -1 && hospscoreC.distance == -1)
          {
            sprintf(buff, "%s", "None");
            send(cfd, buff, sizeof(buff), 0);
            sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospA_addr, sizeof(hospA_addr));
            sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospB_addr, sizeof(hospB_addr));
            sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospC_addr, sizeof(hospC_addr));
            cout << "The location of the client is not in the map, no Hospital will be assigned to it" << endl;
            cout << "The Scheduler has sent the result to client using TCP over port 34706" << endl;
            cout << "The Scheduler has sent the result to all the Hospitals using UDP over port 33706" << endl;
           }//client location is not in the map
         else
          {
            sprintf(buff, "%s", "Same");
            send(cfd, buff, sizeof(buff), 0);
            sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospA_addr, sizeof(hospA_addr));
            sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospB_addr, sizeof(hospB_addr));
            sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospC_addr, sizeof(hospC_addr));
            cout << "The location of the client is the same as Hospital, no Hospital will be assigned to it" << endl;
            cout << "The Scheduler has sent the result to client using TCP over port 34706" << endl;
            cout << "The Scheduler has sent the result to all the Hospitals using UDP over port 33706" << endl;
          }//client location is the same as hospital
        }//client location is illegal 
        else if(hospscoreA.distance != -1 && hospscoreB.distance != -1 && hospscoreC.distance != -1 && hospscoreA.score == -1 && hospscoreB.score == -1 && hospscoreC.score == -1)
        {
         sprintf(buff, "%s", "Zero");
         send(cfd, buff, sizeof(buff), 0);
         sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospA_addr, sizeof(hospA_addr));
         sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospB_addr, sizeof(hospB_addr));
         sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospC_addr, sizeof(hospC_addr));
         cout << "All the Hospitals have the score = None, no Hospital will be assigned" << endl;
         cout << "The Scheduler has sent the result to client using TCP over port 34706" << endl;
         cout << "The Scheduler has sent the result to all the Hospitals using UDP over port 33706" << endl;
        }//client location is legal but all the hospitals have score = None, i.e. all hospitals are fully occupied
    }
    else// client location is legal and at least one hospital has score != None
    {
       if(hospscoreA.score >= hospscoreB.score && hospscoreA.score >= hospscoreC.score)// hospitalA has the highest score
       {
          if(hospscoreA.score == hospscoreB.score && hospscoreA.score != hospscoreC.score)//hospital A and B have the same score
          {
              if(hospscoreA.distance < hospscoreB.distance)
              {
                  sprintf(buff, "%s", "A");
                  send(cfd, buff, sizeof(buff), 0);
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospA_addr, sizeof(hospA_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospB_addr, sizeof(hospB_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospC_addr, sizeof(hospC_addr));
                  cout << "The Scheduler has assigned Hospital A to the client" << endl;
                  cout << "The Scheduler has sent the result to client using TCP over port 34706" << endl;
                  cout << "The Scheduler has sent the result to Hospital A using UDP over port 33706" << endl;
              }
              else if(hospscoreA.distance > hospscoreB.distance)
              {
                  sprintf(buff, "%s", "B");
                  send(cfd, buff, sizeof(buff), 0);
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospA_addr, sizeof(hospA_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospB_addr, sizeof(hospB_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospC_addr, sizeof(hospC_addr));
                  cout << "The Scheduler has assigned Hospital B to the client" << endl;
                  cout << "The Scheduler has sent the result to client using TCP over port 34706" << endl;
                  cout << "The Scheduler has sent the result to Hospital B using UDP over port 33706" << endl;
              }
          }
          else if(hospscoreA.score == hospscoreC.score && hospscoreA.score != hospscoreB.score)//hospital A and C have the same score
          {
              if(hospscoreA.distance < hospscoreC.distance)
              {
                  sprintf(buff, "%s", "A");
                  send(cfd, buff, sizeof(buff), 0);
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospA_addr, sizeof(hospA_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospB_addr, sizeof(hospB_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospC_addr, sizeof(hospC_addr));
                  cout << "The Scheduler has assigned Hospital A to the client" << endl;
                  cout << "The Scheduler has sent the result to client using TCP over port 34706" << endl;
                  cout << "The Scheduler has sent the result to Hospital A using UDP over port 33706" << endl;
              }
              else if(hospscoreA.distance > hospscoreC.distance)
              {
                  sprintf(buff, "%s", "C");
                  send(cfd, buff, sizeof(buff), 0);
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospA_addr, sizeof(hospA_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospB_addr, sizeof(hospB_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospC_addr, sizeof(hospC_addr));
                  cout << "The Scheduler has assigned Hospital C to the client" << endl;
                  cout << "The Scheduler has sent the result to client using TCP over port 34706" << endl;
                  cout << "The Scheduler has sent the result to Hospital C using UDP over port 33706" << endl;
              }
          }
          else if(hospscoreA.score == hospscoreB.score && hospscoreA.score == hospscoreC.score)//hospital A and B and C have the same score
          {
              if(hospscoreA.distance < hospscoreB.distance && hospscoreA.distance < hospscoreC.distance )
              {
                  sprintf(buff, "%s", "A");
                  send(cfd, buff, sizeof(buff), 0);
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospA_addr, sizeof(hospA_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospB_addr, sizeof(hospB_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospC_addr, sizeof(hospC_addr));
                  cout << "The Scheduler has assigned Hospital A to the client" << endl;
                  cout << "The Scheduler has sent the result to client using TCP over port 34706" << endl;
                  cout << "The Scheduler has sent the result to Hospital A using UDP over port 33706" << endl;
              }
              else if(hospscoreB.distance < hospscoreA.distance && hospscoreB.distance < hospscoreC.distance )
              {
                  sprintf(buff, "%s", "B");
                  send(cfd, buff, sizeof(buff), 0);
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospA_addr, sizeof(hospA_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospB_addr, sizeof(hospB_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospC_addr, sizeof(hospC_addr));
                  cout << "The Scheduler has assigned Hospital B to the client" << endl;
                  cout << "The Scheduler has sent the result to client using TCP over port 34706" << endl;
                  cout << "The Scheduler has sent the result to Hospital B using UDP over port 33706" << endl;
              }
            else if(hospscoreC.distance < hospscoreB.distance && hospscoreC.distance < hospscoreA.distance )
              {
                  sprintf(buff, "%s", "C");
                  send(cfd, buff, sizeof(buff), 0);
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospA_addr, sizeof(hospA_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospB_addr, sizeof(hospB_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospC_addr, sizeof(hospC_addr));
                  cout << "The Scheduler has assigned Hospital C to the client" << endl;
                  cout << "The Scheduler has sent the result to client using TCP over port 34706" << endl;
                  cout << "The Scheduler has sent the result to Hospital C using UDP over port 33706" << endl;
              }
          }
          else if(hospscoreA.score != hospscoreB.score && hospscoreA.score != hospscoreC.score)//No tie for highest score
          {
               sprintf(buff, "%s", "A");
               send(cfd, buff, sizeof(buff), 0);
               sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospA_addr, sizeof(hospA_addr));
               sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospB_addr, sizeof(hospB_addr));
               sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospC_addr, sizeof(hospC_addr));
               cout << "The Scheduler has assigned Hospital A to the client" << endl;
               cout << "The Scheduler has sent the result to client using TCP over port 34706" << endl;
               cout << "The Scheduler has sent the result to Hospital A using UDP over port 33706" << endl;
          }
       }
       else if(hospscoreB.score >= hospscoreA.score && hospscoreB.score >= hospscoreC.score)//hospitalB has the highest score
       {
          if(hospscoreB.score == hospscoreA.score && hospscoreB.score != hospscoreC.score)//hospital B and A have the same score
          {
              if(hospscoreB.distance < hospscoreA.distance)
              {
                  sprintf(buff, "%s", "B");
                  send(cfd, buff, sizeof(buff), 0);
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospA_addr, sizeof(hospA_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospB_addr, sizeof(hospB_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospC_addr, sizeof(hospC_addr));
                  cout << "The Scheduler has assigned Hospital B to the client" << endl;
                  cout << "The Scheduler has sent the result to client using TCP over port 34706" << endl;
                  cout << "The Scheduler has sent the result to Hospital B using UDP over port 33706" << endl;
              }
              else if(hospscoreB.distance > hospscoreA.distance)
              {
                  sprintf(buff, "%s", "A");
                  send(cfd, buff, sizeof(buff), 0);
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospA_addr, sizeof(hospA_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospB_addr, sizeof(hospB_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospC_addr, sizeof(hospC_addr));
                  cout << "The Scheduler has assigned Hospital A to the client" << endl;
                  cout << "The Scheduler has sent the result to client using TCP over port 34706" << endl;
                  cout << "The Scheduler has sent the result to Hospital A using UDP over port 33706" << endl;
              }
          }
          else if(hospscoreB.score == hospscoreC.score && hospscoreB.score != hospscoreA.score)//hospital B and C have the same score
          {
              if(hospscoreB.distance < hospscoreC.distance)
              {
                  sprintf(buff, "%s", "B");
                  send(cfd, buff, sizeof(buff), 0);
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospA_addr, sizeof(hospA_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospB_addr, sizeof(hospB_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospC_addr, sizeof(hospC_addr));
                  cout << "The Scheduler has assigned Hospital B to the client" << endl;
                  cout << "The Scheduler has sent the result to client using TCP over port 34706" << endl;
                  cout << "The Scheduler has sent the result to Hospital B using UDP over port 33706" << endl;
              }
              else if(hospscoreB.distance > hospscoreC.distance)
              {
                  sprintf(buff, "%s", "C");
                  send(cfd, buff, sizeof(buff), 0);
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospA_addr, sizeof(hospA_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospB_addr, sizeof(hospB_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospC_addr, sizeof(hospC_addr));
                  cout << "The Scheduler has assigned Hospital C to the client" << endl;
                  cout << "The Scheduler has sent the result to client using TCP over port 34706" << endl;
                  cout << "The Scheduler has sent the result to Hospital C using UDP over port 33706" << endl;
              }
          }
          else if(hospscoreB.score == hospscoreC.score && hospscoreB.score == hospscoreA.score)//hospital A and B and C have the same score
          {
              if(hospscoreA.distance < hospscoreB.distance && hospscoreA.distance < hospscoreC.distance )
              {
                  sprintf(buff, "%s", "A");
                  send(cfd, buff, sizeof(buff), 0);
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospA_addr, sizeof(hospA_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospB_addr, sizeof(hospB_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospC_addr, sizeof(hospC_addr));
                  cout << "The Scheduler has assigned Hospital A to the client" << endl;
                  cout << "The Scheduler has sent the result to client using TCP over port 34706" << endl;
                  cout << "The Scheduler has sent the result to Hospital A using UDP over port 33706" << endl;
              }
              else if(hospscoreB.distance < hospscoreA.distance && hospscoreB.distance < hospscoreC.distance )
              {
                  sprintf(buff, "%s", "B");
                  send(cfd, buff, sizeof(buff), 0);
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospA_addr, sizeof(hospA_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospB_addr, sizeof(hospB_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospC_addr, sizeof(hospC_addr));
                  cout << "The Scheduler has assigned Hospital B to the client" << endl;
                  cout << "The Scheduler has sent the result to client using TCP over port 34706" << endl;
                  cout << "The Scheduler has sent the result to Hospital B using UDP over port 33706" << endl;
              }
            else if(hospscoreC.distance < hospscoreB.distance && hospscoreC.distance < hospscoreA.distance )
              {
                  sprintf(buff, "%s", "C");
                  send(cfd, buff, sizeof(buff), 0);
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospA_addr, sizeof(hospA_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospB_addr, sizeof(hospB_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospC_addr, sizeof(hospC_addr));
                  cout << "The Scheduler has assigned Hospital C to the client" << endl;
                  cout << "The Scheduler has sent the result to client using TCP over port 34706" << endl;
                  cout << "The Scheduler has sent the result to Hospital C using UDP over port 33706" << endl;
              }
          }
          else if(hospscoreB.score != hospscoreC.score && hospscoreB.score != hospscoreA.score)//No tie for highest score
          {
               sprintf(buff, "%s", "B");
               send(cfd, buff, sizeof(buff), 0);
               sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospA_addr, sizeof(hospA_addr));
               sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospB_addr, sizeof(hospB_addr));
               sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospC_addr, sizeof(hospC_addr));
               cout << "The Scheduler has assigned Hospital B to the client" << endl;
               cout << "The Scheduler has sent the result to client using TCP over port 34706" << endl;
               cout << "The Scheduler has sent the result to Hospital B using UDP over port 33706" << endl;
          }
       }
       else if(hospscoreC.score >= hospscoreB.score && hospscoreC.score >= hospscoreA.score)//hospitalC has the highest score
       {
          if(hospscoreC.score == hospscoreB.score && hospscoreC.score != hospscoreA.score)//hospital C and B have the same score
          {
              if(hospscoreC.distance < hospscoreB.distance)
              {
                  sprintf(buff, "%s", "C");
                  send(cfd, buff, sizeof(buff), 0);
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospA_addr, sizeof(hospA_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospB_addr, sizeof(hospB_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospC_addr, sizeof(hospC_addr));
                  cout << "The Scheduler has assigned Hospital C to the client" << endl;
                  cout << "The Scheduler has sent the result to client using TCP over port 34706" << endl;
                  cout << "The Scheduler has sent the result to Hospital C using UDP over port 33706" << endl;
              }
              else if(hospscoreC.distance > hospscoreB.distance)
              {
                  sprintf(buff, "%s", "B");
                  send(cfd, buff, sizeof(buff), 0);
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospA_addr, sizeof(hospA_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospB_addr, sizeof(hospB_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospC_addr, sizeof(hospC_addr));
                  cout << "The Scheduler has assigned Hospital B to the client" << endl;
                  cout << "The Scheduler has sent the result to client using TCP over port 34706" << endl;
                  cout << "The Scheduler has sent the result to Hospital B using UDP over port 33706" << endl;
              }
          }
          else if(hospscoreC.score == hospscoreA.score && hospscoreC.score != hospscoreB.score)//hospital C and A have the same score
          {
              if(hospscoreC.distance < hospscoreA.distance)
              {
                  sprintf(buff, "%s", "C");
                  send(cfd, buff, sizeof(buff), 0);
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospA_addr, sizeof(hospA_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospB_addr, sizeof(hospB_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospC_addr, sizeof(hospC_addr));
                  cout << "The Scheduler has assigned Hospital C to the client" << endl;
                  cout << "The Scheduler has sent the result to client using TCP over port 34706" << endl;
                  cout << "The Scheduler has sent the result to Hospital C using UDP over port 33706" << endl;
              }
              else if(hospscoreC.distance > hospscoreA.distance)
              {
                  sprintf(buff, "%s", "A");
                  send(cfd, buff, sizeof(buff), 0);
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospA_addr, sizeof(hospA_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospB_addr, sizeof(hospB_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospC_addr, sizeof(hospC_addr));
                  cout << "The Scheduler has assigned Hospital A to the client" << endl;
                  cout << "The Scheduler has sent the result to client using TCP over port 34706" << endl;
                  cout << "The Scheduler has sent the result to Hospital A using UDP over port 33706" << endl;
              }
          }
          else if(hospscoreC.score == hospscoreB.score && hospscoreC.score == hospscoreA.score)//hospital A and B and C have the same score
          {
              if(hospscoreA.distance < hospscoreB.distance && hospscoreA.distance < hospscoreC.distance )
              {
                  sprintf(buff, "%s", "A");
                  send(cfd, buff, sizeof(buff), 0);
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospA_addr, sizeof(hospA_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospB_addr, sizeof(hospB_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospC_addr, sizeof(hospC_addr));
                  cout << "The Scheduler has assigned Hospital A to the client" << endl;
                  cout << "The Scheduler has sent the result to client using TCP over port 34706" << endl;
                  cout << "The Scheduler has sent the result to Hospital A using UDP over port 33706" << endl;
              }
              else if(hospscoreB.distance < hospscoreA.distance && hospscoreB.distance < hospscoreC.distance )
              {
                  sprintf(buff, "%s", "B");
                  send(cfd, buff, sizeof(buff), 0);
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospA_addr, sizeof(hospA_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospB_addr, sizeof(hospB_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospC_addr, sizeof(hospC_addr));
                  cout << "The Scheduler has assigned Hospital B to the client" << endl;
                  cout << "The Scheduler has sent the result to client using TCP over port 34706" << endl;
                  cout << "The Scheduler has sent the result to Hospital B using UDP over port 33706" << endl;
              }
            else if(hospscoreC.distance < hospscoreB.distance && hospscoreC.distance < hospscoreA.distance )
              {
                  sprintf(buff, "%s", "C");
                  send(cfd, buff, sizeof(buff), 0);
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospA_addr, sizeof(hospA_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospB_addr, sizeof(hospB_addr));
                  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospC_addr, sizeof(hospC_addr));
                  cout << "The Scheduler has assigned Hospital C to the client" << endl;
                  cout << "The Scheduler has sent the result to client using TCP over port 34706" << endl;
                  cout << "The Scheduler has sent the result to Hospital C using UDP over port 33706" << endl;
              }
          }
          else if(hospscoreC.score != hospscoreB.score && hospscoreC.score != hospscoreA.score)//No tie for highest score
          {
               sprintf(buff, "%s", "C");
               send(cfd, buff, sizeof(buff), 0);
               sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospA_addr, sizeof(hospA_addr));
               sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospB_addr, sizeof(hospB_addr));
               sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&hospC_addr, sizeof(hospC_addr));
               cout << "The Scheduler has assigned Hospital C to the client" << endl;
               cout << "The Scheduler has sent the result to client using TCP over port 34706" << endl;
               cout << "The Scheduler has sent the result to Hospital C using UDP over port 33706" << endl;
          }
       }
    }
    close(cfd);
    cout << "**************************************************************************************" << endl;
 }
}


