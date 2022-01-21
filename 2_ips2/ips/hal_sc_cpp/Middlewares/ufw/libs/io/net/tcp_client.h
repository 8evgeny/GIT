/*
 * tcp_client.h - a part of CMppCore -an object engine for Cortex-M
 * microcontrollers.
 *
 *  Created on: 2 февр. 2021 г.
 *      Author: Dmitry Donskih
 *
 * Copyright (c) 2019-2021 by Dmitry Donskih
 * All rights reserved.
 *
 * Attention Software modification allowed only with
 * the permission of the author.
 */
#ifndef UFW_LIBS_IO_NET_TCP_CLIENT_H_
#define UFW_LIBS_IO_NET_TCP_CLIENT_H_

namespace ufw
{
  
  class tcpClient
  {
    public:
      tcpClient ();
      ~tcpClient ();

//      int sock = 0, valread;
//      struct sockaddr_in serv_addr;
//      char *hello = "Hello from client";
//      char buffer[1024] = {0};
//      if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
//      {
//          printf("\n Socket creation error \n");
//          return -1;
//      }
//
//      serv_addr.sin_family = AF_INET;
//      serv_addr.sin_port = htons(PORT);
//
//      // Convert IPv4 and IPv6 addresses from text to binary form
//      if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
//      {
//          printf("\nInvalid address/ Address not supported \n");
//          return -1;
//      }
//
//      if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
//      {
//          printf("\nConnection Failed \n");
//          return -1;
//      }
//      send(sock , hello , strlen(hello) , 0 );
//      printf("Hello message sent\n");
//      valread = read( sock , buffer, 1024);
//      printf("%s\n",buffer );
//      return 0;
//

    private:
      tcpClient (const tcpClient &other) = delete;
      tcpClient (tcpClient &&other) = delete;
      tcpClient& operator= (const tcpClient &other) = delete;
      tcpClient& operator= (tcpClient &&other) = delete;
  };

} /* namespace ufw */

#endif /* UFW_LIBS_IO_NET_TCP_CLIENT_H_ */
