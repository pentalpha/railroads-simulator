#include "Server.h"
#include "logging.h"

Server::Server(const char* localIP, int port){
  this->localIP = std::string(localIP);
  this->port = port;
  sizeAddressClient = sizeof(struct sockaddr);
  msg = new char[MAXMSG+1];
  /*
   * Configurações do endereço
  */
  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  //address.sin_addr.s_addr = INADDR_ANY;
  address.sin_addr.s_addr = inet_addr(localIP);
  connected = false;
  exitFlag = false;
  waitingFlag = false;
}

bool Server::start(){
  if(getSocket() == false){
    return false;
  }
  if(doBind() == false){
    return false;
  }
  if(startListening() == false){
    return false;
  }
  waitingFlag = false;
  return true;
}

bool Server::getSocket(){
  /*
   * Criando o Socket
   *
   * PARAM1: AF_INET ou AF_INET6 (IPV4 ou IPV6)
   * PARAM2: SOCK_STREAM ou SOCK_DGRAM
   * PARAM3: protocolo (IP, UDP, TCP, etc). Valor 0 escolhe automaticamente
  */
  socketId = socket(AF_INET, SOCK_STREAM, 0);
  //Verificar erros
  if (socketId == -1)
  {
      log("SERVER", "Failed to create socket()");
      return false;
  }
  return true;
}

bool Server::doBind(){
  //Conectando o socket a uma porta. Executado apenas no lado servidor
  if (bind (socketId, (struct sockaddr *)&address, sizeof(struct sockaddr)) == -1)
  {
    log("SERVER", std::string("Failed to bind() a port (") + std::to_string(port));
    return false;
  }else{
    return true;
  }
}

bool Server::startListening(){
  //Habilitando o servidor a receber conexoes do cliente
  if (listen( socketId, 10 ) == -1)
  {
      log("SERVER", std::string("Failed to listen() on ") + localIP + std::string("::") + std::to_string(port));
      return false;
  }else{
      return true;
  }
}

bool Server::isConnected(){
  return connected;
}

int Server::putMessage(std::string msgToSend){
    if(!connected){
        return 0;
    }
    //std::string* bytes = new std::string(msgToSend.c_str());
    //sendQueue.push(bytes);
    m.lock();
    const char* bytesToSend = msgToSend.c_str();
    int bytesSent = send(connectionClientId, bytesToSend, strlen(bytesToSend), 0);
    if (bytesSent == 0)
    {
        log("SERVER", "Message sent: \nZero bytes, client finished connection");
        return bytesSent;
    }
    else if(bytesSent<0)
    {
        error("SERVER", "ERROR: send returned an error" + std::to_string(errno));
        //cerr << "ERROR: send returned an error "<<errno<< endl; // this case is triggered
        return bytesSent;
    }else if (bytesSent < strlen(bytesToSend)){
        log("SERVER", std::string("Message sent with less characters: \n") + std::string(bytesToSend, bytesSent));
    }else if (bytesSent > strlen(bytesToSend)){
        log("SERVER", std::string("Message sent with extra characters: \n")
            + std::string(bytesToSend) + std::string("+") + std::to_string(bytesSent-strlen(bytesToSend)));
    }else{
        log("SERVER", std::string("Message sent: \n") + std::string(bytesToSend));
    }
    m.unlock();
    return bytesSent;
}

std::string Server::getMessage(){
  std::string *msg = messages.pop();
  if(msg == NULL){
    return "";
  }else{
    return *msg;
  }
}

void Server::startWaiting(std::thread* toThrow){
  exitFlag = false;
  waitingFlag = true;
  std::thread theThread = std::thread(&Server::waitForClientAndReceive, this, toThrow);
  theThread.detach();
}

void Server::stop(){
  //std::cout << "Server auto stopping itself\n";
  exitFlag = true;
}

bool Server::isWaiting(){
  return waitingFlag;
}

void Server::waitForClientAndReceive(std::thread* toThrow){
  waitingFlag = true;
  //Servidor fica bloqueado esperando uma conexão do cliente
  log("SERVER", "Waiting for client...");
  connectionClientId = accept( socketId,(struct sockaddr *) &addressClient,&sizeAddressClient );

  log("SERVER", std::string("Client connected: ") + std::string(inet_ntoa(addressClient.sin_addr)));
  waitingFlag = false;
  connected = true;
  //Verificando erros
  if ( connectionClientId == -1)
  {
      log("SERVER", "Failed to accept() a client");
      return;
  }
  if(toThrow != NULL){
      toThrow->detach();
  }
  std::thread recvThread(&Server::receive, this);
  recvThread.detach();
}

void Server::receive(){
    int nBytes = minMessage+1;
    //receber uma msg do cliente
    //std::cout << "Server waiting for a message...\n";
    std::stringstream* msgBuilder = new std::stringstream();
    char* bytesread = new char[nBytes];
    int nRead = 0;
    log("SERVER", std::string("Client id: ") + std::to_string(connectionClientId));
    int bufferingChars = 0;
    while(!exitFlag){
          //memset (bytesread,(char)0,nBytes);
          nRead = recv(connectionClientId,bytesread,nBytes,0);
          if (nRead > 0)
          {
              //log("SERVER", std::string("bytesread == ") + std::string(bytesread, nRead));
              for(int i = 0; i < nBytes; i++)
              {
                  //log("SERVER", std::string("msgBuilder << ") + std::to_string((int)bytesread[i]));
                  *msgBuilder << bytesread[i];
                  if((int)bytesread[i] == 10 && bufferingChars){
                      std::string msg;
                      *msgBuilder >> msg;
                      //log("SERVER", std::string("Message received: \n") + msg);
                      std::string *s = new std::string(msg);
                      messages.push(s);
                      msgBuilder =  new std::stringstream();
                      bufferingChars = 0;
                  }else{
                      bufferingChars++;
                  }
              }
          }
          else if (nRead == 0)
          {
              //std::cout << "recv() == 0" << std::endl;
              log("SERVER", "Message received: \nZero bytes, client finished connection");
              exitFlag = true;
          }
          else
          {
              if(nRead == -1){
                  log("SERVER", "Message received: \nBroken message | Failed to recv() (=-1)");
                  error("SERVER", std::to_string(errno));
              }else{
                  log("SERVER", std::string("Message received: \nBroken message | Failed to receive\n recv() =")
                      + std::to_string(nRead));
                  error("SERVER", std::to_string(errno));
              }
              exitFlag = true;
          }
    }

    waitingFlag = false;
    connected = false;
    close(connectionClientId);
    log("SERVER", "Connection to client finished, not receiving anymore");
}

/*void Server::sendAll(){
    while(!exitFlag){
        std::string* toSend = sendQueue.pop();
        if(toSend != NULL){
            const char* bytesToSend = toSend->c_str();
            int bytesSent = send(connectionClientId, bytesToSend, MAXMSG, 0);
            if (bytesSent == 0)
            {
                log("SERVER", "Message sent: \nZero bytes, client finished connection");
                break;
            }
            else if(bytesSent<0)
            {
                error("SERVER", "ERROR: send returned an error" + std::to_string(errno));
                //cerr << "ERROR: send returned an error "<<errno<< endl; // this case is triggered
                //return n;
            }else{
                log("SERVER", std::string("Message sent: \n") + std::string(bytesToSend, MAXMSG));
            }
            delete toSend;
            delete bytesToSend;
        }
    }
    log("SERVER", "Connection to client finished, not sending anymore");
}*/
