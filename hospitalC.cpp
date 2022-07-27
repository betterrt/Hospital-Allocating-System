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

int VerticesN;
struct Vnames
{
    char name[100];
};

Vnames unvisited[2000], visited[2000], startV;
int unvisitedN = 0, visitedN = 0;

struct edge
{
    Vnames v1;
    Vnames v2;
    double weight;
};
edge edges[2000];
int EdgesN = 0;

struct minPath
{
    Vnames Vertex;
    Vnames Previous;
    double dist;
};

minPath minPaths[2000];

void getmap()
{
    FILE *input;
    input = fopen("map.txt", "r");
    while(!feof(input))
    {
        fscanf(input, "%s %s %lf", edges[EdgesN].v1.name, edges[EdgesN].v2.name, &edges[EdgesN].weight);
        EdgesN++;
    }
    fclose(input);
}// read the map and store the data into a structure array

int added(char name[])
{ 
    for(int i = 0; i < unvisitedN; i++)
 {
     if(strcmp(name, unvisited[i].name) == 0)
     {
         return 1;
     }
 }
 return 0;
}



void getUnvisitedVertices()
{
    for(int i = 0; i < EdgesN; i++)
    {
      if(!added(edges[i].v1.name))
      {
         strcpy(unvisited[unvisitedN].name, edges[i].v1.name);
         unvisitedN++;
      }
      if(!added(edges[i].v2.name))
      {
         strcpy(unvisited[unvisitedN].name, edges[i].v2.name);
         unvisitedN++;
      }
    }
}

void initializeMinPathTable()
{
    double initialDist = 1;
    for(int i = 0; i < EdgesN; i++)
    {
        initialDist = initialDist + edges[i].weight;
    }

    for(int i = 0; i < VerticesN; i++)
    {
        strcpy(minPaths[i].Vertex.name, unvisited[i].name);
        strcpy(minPaths[i].Previous.name, "null");
        if(strcmp(minPaths[i].Vertex.name, startV.name) == 0)
        {
            minPaths[i].dist = 0;
        }
        else
        {
            minPaths[i].dist = initialDist;
        }
    }
}// if the starting vertex is the same with the vertex we are interested in, then set the distance to zero, otherwise set the distance to a huge nunber.


double edgeWeight(char s1[], char s2[])
{
    for(int i = 0; i < EdgesN; i++)
    {
        if(strcmp(edges[i].v1.name, s1) == 0 && strcmp(edges[i].v2.name, s2) == 0)
        {
            return edges[i].weight;
        }
        if(strcmp(edges[i].v2.name, s1) == 0 && strcmp(edges[i].v1.name, s2) == 0)
        {
            return edges[i].weight;
        }
    }
}

int unvisitedIndexInPath(char s[])
{
    for(int i = 0; i < VerticesN; i++)
    {
        if(strcmp(s, minPaths[i].Vertex.name) == 0)
        {
            return i;
        }
    }
}

void dijkstra(char vName[])
{
    Vnames vName_unvisited_neighbors[100];
    int N = 0;

    for(int i = 0; i < EdgesN; i ++){
        if(strcmp(edges[i].v1.name, vName) == 0)
        {
            strcpy(vName_unvisited_neighbors[N].name, edges[i].v2.name);
            N++;
        }
        if(strcmp(edges[i].v2.name, vName) == 0)
        {
            strcpy(vName_unvisited_neighbors[N].name, edges[i].v1.name);
            N++;
        }
    }

    Vnames help[100];
    int x = 0, isUnvisited;
    for(int i = 0; i < N; i ++)
    {
        isUnvisited = 0;
        for(int j = 0; j < unvisitedN; j ++)
        {
            if(strcmp(vName_unvisited_neighbors[i].name, unvisited[j].name) == 0)
            {
                isUnvisited = 1;
            }
        }
        if(isUnvisited == 1)
        {
            strcpy(help[x].name, vName_unvisited_neighbors[i].name);
            x++;
        }
    }
    N = x;
    for(int i = 0; i < N; i ++)
    {
        strcpy(vName_unvisited_neighbors[i].name, help[i].name);
    }


    double vNameDist;
    for(int i = 0; i < VerticesN; i ++)
    {
        if(strcmp(vName, minPaths[i].Vertex.name) == 0)
        {
            vNameDist = minPaths[i].dist;
        }
    }

    double weight, distPlusWeight;
    for(int i = 0; i < N; i++)
    {
        weight = edgeWeight(vName, vName_unvisited_neighbors[i].name);
        distPlusWeight = weight + vNameDist;
        for(int j = 0; j < VerticesN; j ++)
        {
            if(strcmp(vName_unvisited_neighbors[i].name, minPaths[j].Vertex.name) == 0)
            {
                if(minPaths[j].dist > distPlusWeight)
                {
                    minPaths[j].dist = distPlusWeight;
                    strcpy(minPaths[j].Previous.name, vName);
                }
            }
        }
    }

    strcpy(visited[visitedN].name, vName);
    visitedN++;
    Vnames help1[100];
    int x1 = 0;
    for(int i  = 0; i < unvisitedN; i++)
    {
        if(strcmp(vName, unvisited[i].name) != 0)
        {
            strcpy(help1[x1].name, unvisited[i].name);
            x1++;
        }
    }
    unvisitedN = x1;
    for(int i = 0; i < unvisitedN; i++)
    {
        strcpy(unvisited[i].name, help1[i].name);
    }

    if(unvisitedN > 0)
    {
        int index, i2;
        double min;
        index = unvisitedIndexInPath(unvisited[0].name);
        min = minPaths[index].dist;
        for(int i = 0; i < unvisitedN; i++)
        {
            i2 = unvisitedIndexInPath(unvisited[i].name);
            if(min > minPaths[i2].dist)
            {
                index = i2;
                min = minPaths[index].dist;
            }
        }

        dijkstra(minPaths[index].Vertex.name);
    }
}//use the code from a Youtube video (https://www.youtube.com/watch?v=xme-ehbKelo) to implement Dijkstra algorithm


int main(int argc, char *argv[])
{
  getmap();
  int sockfd;
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);// create UDP socket
  if(sockfd == -1)
  {
      sys_err("socket error");
  }
  
  char buf[1024];
  int bfd, ret, s1, s2, r1;
  char mname[100];
  
  struct sockaddr_in serv_addr,clie_addr;
  socklen_t serv_addr_len;
  bzero(&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(33706);
  inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr.s_addr);//set scheduler address

  bzero(&clie_addr, sizeof(clie_addr));
  clie_addr.sin_family = AF_INET;
  clie_addr.sin_port = htons(32706);
  clie_addr.sin_addr.s_addr = htonl(INADDR_ANY);// set hospital C address because we want hospital C to use fixed port number 32706
  
  serv_addr_len = sizeof(serv_addr);

  bfd = bind(sockfd, (struct sockaddr *)&clie_addr, sizeof(clie_addr));
  if(bfd == -1)
  {
      sys_err("bind error");
  }

  double sdistance;
  double availability, score;
  struct hospinfo
  {
      int location;
      int capacity;
      int occupancy;
  };
  hospinfo hospC;
  hospC.location = atoi(argv[1]);
  hospC.capacity = atoi(argv[2]);
  hospC.occupancy = atoi(argv[3]);
  sprintf(mname, "%d", hospC.location);

  cout << "Hospital C is up and runnning using UDP on port 32706" << endl;
  cout << "Hospital C has total capacity " << hospC.capacity << " and initial occupancy " << hospC.occupancy << endl;

//loop
 while(1)
 {
   memset(&buf, 0, sizeof(buf));
   sprintf(buf, "C %d %d ", hospC.capacity, hospC.occupancy);

  
   s1 = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));//send initial information to scheduler through UDP
   if(s1 == -1)
   {
       sys_err("send error");
   }

   memset(&buf, 0, sizeof(buf));
   ret = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&serv_addr, &serv_addr_len);
   if(ret == -1)
   {
       sys_err("receive error");
   }
   //receive client location from scheduler
  
   strcpy(startV.name, buf);//set the client location as the starting vertex
   cout << "Hospital C has received input from client at location " << startV.name << endl;
   getUnvisitedVertices();
   VerticesN = unvisitedN;
   initializeMinPathTable();
   dijkstra(startV.name);
 
  
   int pand = 0;
   for(int i = 0; i < VerticesN; i++)
   {
       if(strcmp(startV.name, minPaths[i].Vertex.name) == 0)
       {
          pand = 1;
       }
   }//find if the client location is in the map
  
   if(pand == 0)
   {
       cout << "Hospital C does not have the location " << startV.name << " in map" << endl;
       cout << "Hospital C has sent location not found to the Scheduler " << endl;
   }

   if(strcmp(startV.name, mname) == 0 || pand == 0)
   {
       score = -1;
       sdistance = -1;
       availability = ((double)hospC.capacity - (double)hospC.occupancy)/(double)hospC.capacity;
       cout << "Hospital C has capacity = "<< hospC.capacity << ", occupation = " << hospC.occupancy << ", availability = "<< availability << endl;
       cout << "Hospital C has the distance = None" << endl;
       cout << "Hospital C has the score = None" << endl;

   }//if the client locaiton is the same with the hospital location or the client location is not in the map, set score and distance to None
   else 
   {
       for(int i = 0; i < VerticesN; i++)
     {
         if(strcmp(mname, minPaths[i].Vertex.name) == 0)
         {
            sdistance = minPaths[i].dist;
         }
     }
       availability = ((double)hospC.capacity - (double)hospC.occupancy)/(double)hospC.capacity;
       if(availability <= 0 || availability > 1)
       {
          score = -1;
          cout << "Hospital C has capacity = "<< hospC.capacity << ", occupation = " << hospC.occupancy << ", availability = None" << endl;
          cout << "Hospital C has found the shortest path to client, distance = " << sdistance << endl;
          cout << "Hospital C has the score = None" << endl;
       }
       else
       {
          score = 1.0/(sdistance * (1.1 - availability));
          cout << "Hospital C has capacity = "<< hospC.capacity << ", occupation = " << hospC.occupancy << ", availability = " << availability << endl;
          cout << "Hospital C has found the shortest path to client, distance = " << sdistance << endl;
          cout << "Hospital C has the score = " << score << endl;
       }
   }


   // calculate the score
   memset(&buf, 0, sizeof(buf));
   sprintf(buf, "C %lf %lf", score, sdistance);

   s2 = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));//send score and distance to scheduler through UDP
   if(s2 == -1)
   {
       sys_err("send error");
   }

   if(sdistance == -1)
   {
     cout << "Hospital C has sent score = None and distance = None to the Scheduler" << endl;
   }
   else if(sdistance != -1 && score == -1)
   {
     cout << "Hospital C has sent score = None and distance = " << sdistance << " to the Scheduler" << endl;
   }
   else if(sdistance != -1 && score != -1)
   {
     cout << "Hospital C has sent score = "<< score <<" and distance = " << sdistance << " to the Scheduler" << endl;
   }

   memset(&buf, 0, sizeof(buf));
   r1 = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&serv_addr, &serv_addr_len);
   if(r1 == -1)
   {
       sys_err("receive error");
   }
   char x;
   x = buf[0];
   if(x == 'C')
   {
      hospC.occupancy = hospC.occupancy + 1;
      availability = ((double)hospC.capacity - (double)hospC.occupancy)/(double)hospC.capacity;
      if(availability <= 0)
      {
       cout << "Hospital C has been assigned to a client, occupation is updated to " << hospC.occupancy << ", availability is updated to None" << endl;
      }
      else 
      {
       cout << "Hospital C has been assigned to a client, occupation is updated to " << hospC.occupancy << ", availability is updated to " << availability << endl;  
      }
   }
   cout << "**************************************************************************************" << endl;
 }//update hospital occupation information if it is assigned to a client
}