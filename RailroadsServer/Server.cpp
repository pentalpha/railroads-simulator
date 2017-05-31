#include "Server.h"

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
      std::cout << "[SERVER] Failed to create socket()\n";
      return false;
  }
  return true;
}

bool Server::doBind(){
  //Conectando o socket a uma porta. Executado apenas no lado servidor
  if (bind (socketId, (struct sockaddr *)&address, sizeof(struct sockaddr)) == -1)
  {
    std::cout << "[SERVER] Failed to bind() a port ("<< port << ")\n";
    return false;
  }else{
    return true;
  }
}

bool Server::startListening(){
  //Habilitando o servidor a receber conexoes do cliente
  if (listen( socketId, 10 ) == -1)
  {
      std::cout << "[SERVER] Failed to listen() on " << localIP << "::" << port << "\n";
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
  waitForClientAndReceive();
}

void Server::stop(){
  //std::cout << "Server auto stopping itself\n";
  exitFlag = true;
}

bool Server::isWaiting(){
  return waitingFlag;
}

void Server::waitForClientAndReceive(){
  //servidor ficar em um loop infinito
  std::cout << "[SERVER] Waiting for a client\n";

  waitingFlag = true;
  //Servidor fica bloqueado esperando uma conexão do cliente
  std::cout << "[SERVER] Waiting for client..." << std::endl;
  connectionClientId = accept( socketId,(struct sockaddr *) &addressClient,&sizeAddressClient );

  std::cout << "[SERVER] Client connected: " << inet_ntoa(addressClient.sin_addr) << "\n";
  waitingFlag = false;
  connected = true;
  //Verificando erros
  if ( connectionClientId == -1)
  {
      std::cout << "[SERVER] Failed to accept() a client\n";
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
          Global::log("SERVER", "Message received: \nBroken message | Failed to recv()");
          break;
      }
      else if (bytesread == 0)
      {
          Global::log("SERVER", "Message received: \nZero bytes, client finished connection");
          break;
      }
      //Inserir o caracter de fim de mensagem
      msg[bytesread] = '\0';
      Global::log("SERVER", string("Message received: \n") + msg);
      string *s = new string(msg);
      messages.push(s);
      //close(connectionClientId);
    }
    waitingFlag = false;
    connected = false;
    close(connectionClientId);
    Global::log("SERVER", "Connection to client finished, not receiving anymore");
}

void Server::sendAll(){
    while(!exitFlag){
        std::string* toSend = sendQueue.pop();
        if(toSend != NULL){
            const char* bytesToSend = toSend->c_str();
            int bytesSent = send(connectionClientId, bytesToSend, MAXMSG, 0);
            if (bytesSent == 0)
            {
                Global::log("SERVER", "Message sent: \nZero bytes, client finished connection");
                break;
            }
            else if(bytesSent<0)
            {
                Global::error("SERVER", "ERROR: send returned an error" + std::to_string(errno));
                //cerr << "ERROR: send returned an error "<<errno<< endl; // this case is triggered
                //return n;
            }else{
                Global::log("SERVER", string("Message sent: \n") + string(bytesToSend, MAXMSG));
            }
            delete toSend;
            delete bytesToSend;
        }
    }
    Global::log("SERVER", "Connection to client finished, not sending anymore");
}
