#ifndef _SERVER_
#define _SERVER_

#include <iostream>     //cout
#include <cstring>      //memset
#include <cstdlib>      //exit
#include <netinet/in.h> //htons
#include <arpa/inet.h>  //inet_addr
#include <sys/socket.h> //socket
#include <unistd.h>     //close
#include <thread>
#include "StringQueue.h"
#include <string>
#include <thread>
#include <mutex>
#include "logging.h"
#include <sstream>

#define MAXMSG 1024
#define PORTNUM 50001

class Server{
public:
  Server(const char* localIP, int port=PORTNUM);

  bool start();
  void startWaiting(std::thread* throwWhenConnectedThread = NULL);
  bool isConnected();
  bool isWaiting();
  void stop();
  std::string getMessage();
  int putMessage(std::string msgToSend);

private:
  void waitForClientAndReceive(std::thread* throwWhenConnectedThread = NULL);
  bool getSocket();
  bool doBind();
  bool startListening();
  void receive();
  void sendAll();

  //flags
  bool waitingFlag;
  bool exitFlag;
  //variáveis do servidor
  struct sockaddr_in address;
  int socketId;
  //metadata not c-related
  std::string localIP;
  int port;
  bool connected;
  //variáveis relacionadas com as conexões clientes
  struct sockaddr_in addressClient;
  socklen_t sizeAddressClient;
  int connectionClientId;
  //received data
  char *msg;
  int bytesread;

  std::mutex m;

  StringQueue messages;
};

#endif
