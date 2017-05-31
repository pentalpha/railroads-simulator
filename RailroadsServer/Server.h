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
#include "global.h"

#define MAXMSG 1024
#define PORTNUM 50001

using namespace std;

class Server{
public:
  Server(const char* localIP, int port=PORTNUM);

  bool start();
  void startWaiting();
  bool isConnected();
  bool isWaiting();
  void stop();
  string getMessage();
  void putMessage(std::string msgToSend);

private:
  void waitForClientAndReceive();
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
  string localIP;
  int port;
  bool connected;
  //variáveis relacionadas com as conexões clientes
  struct sockaddr_in addressClient;
  socklen_t sizeAddressClient;
  int connectionClientId;
  //received data
  char *msg;
  int bytesread;

  StringQueue messages, sendQueue;
};

#endif
