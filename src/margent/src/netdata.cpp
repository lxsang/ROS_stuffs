/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2016  Guillaume L. <guillaume.lozenguez@mines-douai.fr>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

// pour le client uniquement:
#include <netdb.h>

#include "netdata.h"

using namespace::mia;
using namespace::std;

NetData::NetData( int port ):
    a_port(port)
{
    a_error= "";
    
    a_server_sockfd = socket(AF_INET, SOCK_STREAM, 0);// TCP socket
    if (a_server_sockfd < 0)
    {
        a_error= std::string("ERROR opening socket") + strerror(errno);
    }

    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(a_port);
    if (bind(a_server_sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        a_error= std::string("ERROR on binding :") + strerror(errno);
    }

    listen(a_server_sockfd, 5);
}

NetData::~NetData()
{
  shutdown(a_server_sockfd, SHUT_RDWR);
  close(a_server_sockfd);
}

bool initialize_server(){
  
}

bool NetData::send( const std::string target, const Data & d ) {

    cout << "send :" << endl;

    int sockfd;
    struct hostent *server;

    // Connection to server :
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        a_error= "SEND ERROR: opening socket";
	close(sockfd);
        return false;
    }

    server = gethostbyname( target.c_str() );
    if (server == NULL)
    {
        a_error= "SEND ERROR: no such host";
	close(sockfd);
        return false;
    }
    
    struct sockaddr_in serv_addr;

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(a_port);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
        a_error= "SEND ERROR: connecting";
	close(sockfd);
        return false;
    }

    // Send meta_data :

    int meta[3];
    meta[0]= d.mesage_size();
    meta[1]= d.flag_size();
    meta[2]= d.value_size();

    cout << "\t meta : " << meta[0] << ", " << meta[1] << ", " << meta[2] << endl;

    if (write( sockfd, meta, 3*sizeof(int) ) < 0)
    {
        a_error= "SEND ERROR: writing to socket";
	close(sockfd);
        return false;
    }

    // Send msg :
    for( int i(0); i < d.a_mesage_size; i+= 1024)
    {
      int size= min(1024, d.a_mesage_size-i);
      
      if (write( sockfd, &(d.a_mesage[i]), size ) < 0)
      {
	  a_error= "SEND ERROR: writing to socket";
	  close(sockfd);
	  return false;
      }
    }
    
    // Send flag :
    for( int i(0); i < d.a_flag_size; i+= 256)
    {
      int size= min(256, d.a_flag_size-i);
      if (write( sockfd, &(d.a_flag[i]), size*sizeof(int) ) < 0)
      {
	  a_error= "SEND ERROR: writing to socket";
	  close(sockfd);
	  return false;
      }
    }

    // Send value :
    for( int i(0); i < d.a_value_size; i+= 256)
    {
      int size= min(256, d.a_value_size-i);
      if (write( sockfd, &(d.a_value[i]), size*sizeof(float) ) < 0)
      {
	  a_error= "SEND ERROR: writing to socket";
	  close(sockfd);
	  return false;
      }
    }
    
    char buffer[256];
    bzero(buffer,256);
    if ( read(sockfd,buffer,255) < 0 )
    {
        a_error= "SEND ERROR: reading from socket";
	close(sockfd);
        return false;
    }

    printf("%s\n",buffer);
    
    shutdown(sockfd, SHUT_RDWR);
    close(sockfd);

    a_error= "";
    return true;
}

bool NetData::wait(Data & d)
{
    cout << "wait :" << endl;
    int newsockfd;
    socklen_t clilen;
    struct sockaddr_in cli_addr;


    clilen = sizeof(cli_addr);
    newsockfd = accept(a_server_sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0)
    {
        a_error= std::string("ERROR on accept") + strerror(errno);
        return false;
    }

    // read meta :
    int meta[3];
    if ( read( newsockfd, meta, 3*sizeof(int) ) < 0)
    {
        a_error= "ERROR reading meta from socket";
        return false;
    }

    cout << "\t meta : " << meta[0] << ", " << meta[1] << ", " << meta[2] << endl;
    
    // Initialize data d
    d.initialize( meta[0], meta[1], meta[2] );
    
    // read msg :
    bzero( d.a_mesage,  d.a_mesage_size+1 );
    for( int i(0); i < d.a_mesage_size; i+= 1024)
    {
      int size= min(1024, d.a_mesage_size-i);
      if ( read(newsockfd, &(d.a_mesage[i]), size) < 0)
      {
	  a_error= "ERROR reading data from socket";
	  return false;
      }
    }

    // read flag :
    for( int i(0); i < d.a_flag_size; i+= 256)
    {
      int size= min(256, d.a_flag_size-i);
      if ( read( newsockfd, &(d.a_flag[i]), size*sizeof(int) ) < 0)
      {
        a_error= "ERROR reading data from socket";
        return false;
      }
    }
           
    // read values :
    for( int i(0); i < d.a_value_size; i+= 256)
    {
      int size= min(256, d.a_value_size-i);
      if (read( newsockfd, &(d.a_value[i]), size*sizeof(float) ) < 0)
      {
	  a_error= "ERROR reading data from socket";
	  return false;
      }
    }

    // Send data :
    if (write(newsockfd, "ok", 4) < 0)
    {
        a_error= "ERROR writing to socket";
        return false;
    }
    
    shutdown(newsockfd, SHUT_RDWR);
    close(newsockfd);

    a_error= "";
    return true;
}
