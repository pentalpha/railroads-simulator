#include "Server.h"
#include "logging.h"

Server::Server(const char* localIP, int port){
  this->localIP = string(localIP);
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
  waitingFlag =false;
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
    log("SERVER", string("Failed to bind() a port (") + to_string(port));
    return false;
  }else{
    return true;
  }
}

bool Server::startListening(){
  //Habilitando o servidor a receber conexoes do cliente
  if (listen( socketId, 10 ) == -1)
  {
      log("SERVER", string("Failed to listen() on ") + localIP + string("::") + to_string(port));
      return false;
  }else{
      return true;
  }
}

bool Server::isConnected(){
  return connected;
}

void Server::putMessage(std::string msgToSend){
    string* bytes = new string(msgToSend.c_str());
    sendQueue.push(bytes);
}

string Server::getMessage(){
  string *msg = messages.pop();
  if(msg == NULL){
    return "";
  }else{
    return *msg;
  }
}

void Server::startWaiting(){
  exitFlag = false;
  waitingFlag = true;
  thread theThread = thread(&Server::waitForClientAndReceive, this);
  theThread.detach();
}

void Server::stop(){
  //std::cout << "Server auto stopping itself\n";
  exitFlag = true;
}

bool Server::isWaiting(){
  return waitingFlag;
}

void Server::waitForClientAndReceive(){
  waitingFlag = true;
  //Servidor fica bloqueado esperando uma conexão do cliente
  log("SERVER", "Waiting for client...");
  connectionClientId = accept( socketId,(struct sockaddr *) &addressClient,&sizeAddressClient );

  log("SERVER", string("Client connected: ") + string(inet_ntoa(addressClient.sin_addr)));
  waitingFlag = false;
  connected = true;
  //Verificando erros
  if ( connectionClientId == -1)
  {
      log("SERVER", "Failed to accept() a client");
      return;
  }
  std::thread recvThread(&Server::receive, this);
  std::thread sendThread(&Server::sendAll, this);
  recvThread.detach();
  sendThread.detach();
}

void Server::receive(){
    while(!exitFlag){
      //receber uma msg do cliente
      //std::cout << "Server waiting for a message...\n";
      bytesread = recv(connectionClientId,msg,MAXMSG,0);
      if (bytesread == -1)
      {
          log("SERVER", "Message received: \nBroken message | Failed to recv()");
          break;
      }
      else if (bytesread == 0)
      {
          log("SERVER", "Message received: \nZero bytes, client finished connection");
          break;
      }
      //Inserir o caracter de fim de mensagem
      msg[bytesread] = '\0';
      log("SERVER", string("Message received: \n") + msg);
      string *s = new string(msg);
      messages.push(s);
      //close(connectionClientId);
    }
    waitingFlag = false;
    connected = false;
    close(connectionClientId);
    log("SERVER", "Connection to client finished, not receiving anymore");
}

void Server::sendAll(){
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
                log("SERVER", string("Message sent: \n") + string(bytesToSend, MAXMSG));
            }
            delete toSend;
            delete bytesToSend;
        }
    }
    log("SERVER", "Connection to client finished, not sending anymore");
}
