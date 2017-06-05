#include "RailroadsServer.h"

using namespace std;

RailroadsServer::RailroadsServer(std::string ip, int port, RailsGraph* graph, RailroadsCanvas* canvas) :
    ipStr(localIP), portNum(por)
{
    tcpServer = new QTcpServer();
    addr = QHostAddress(ipStr);

    connected = false;
    exitFlag = false;
    waitingFlag = false;

    canvas = canvas;
    graph = graph;

    //connect(tcpServer, &QTcpServer::newConnection, this, &RailroadsServer::whenConnected);
}

bool RailroadsServer::startListening(){
  //Habilitando o servidor a receber conexoes do cliente
  bool listened = tcpServer->listen(ipStr, portNum);
  if (!listened)
  {
      log("SERVER", std::string("Failed to listen() on ") + ipStr.toStdString() +
          std::string("::") + std::to_string(portNum));
      return false;
  }else{
      return true;
  }
}

void RailroadsServer::start(){
  if(startListening() == false){
    return;
  }
  int nSeconds = 5;
  log("SERVER", "Waiting 5s for client to connect...");
  waitingFlag = true;
  bool connected = tcpServer->waitForNewConnection(5000);
  client = tcpServer->nextPendingConnection();
  if(!connected || client == NULL){
      log("SERVER", "No client connected to the server.");
      waitingFlag = false;
      connected = false;
      exitFlag = true;
      return;
  }
  connect(client, &QTcpSocket::disconnected, this, &clientDisconnected);
  whenConnected();
}

void RailroadsServer::whenConnected(){
    connected = true;
    exitFlag = false;
    waitingFlag = false;
    QtConcurrent::run(this, &receive);
    msgTreatmentThread();
    connected = false;
    exitFlag = true;
}

bool RailroadsServer::isConnected(){
  return connected || !exitFlag;
}

std::string RailroadsServer::getMessage(){
  std::string *msg = messages.pop();
  if(msg == NULL){
    return "";
  }else{
    return *msg;
  }
}

void RailroadsServer::stop(){
  //std::cout << "Server auto stopping itself\n";
  exitFlag = true;
  client->close();
  stopAllTrainThreads();
}

void RailroadsServer::stopAllTrainThreads(){
    for(TrainThread* train : trainThreads){
        train->stop();
    }
}

bool RailroadsServer::isWaiting(){
  return waitingFlag;
}

void RailroadsServer::msgTreatmentThread(){
    log("SERVER", "RailroadsServer is ON");

    while(!exitFlag){
        std::string msg = getMessage();
        if(msg.length() >= minMessage)
        {
            treatMessage(msg);
        }else if (msg.length() >= 2)
        {
            log("[SERVER]", std::string("Message is too short, ignore: \n") + msg);
        }
    }

    log("SERVER", "RailroadsServer is OFF");
}

void RailroadsServer::clientDisconnected(){
    exitFlag = true;
    connected = false;
}

void RailroadsServer::receive(){
    log("SERVER", "Waiting for messages");
    char* receivedData = NULL;
    string* line = NULL;
    while(!exitFlag){
        receivedData = client->readLine();
        if(strlen(receivedData) >= minMessage-1){
            messages.push(new string(receivedData));
        }
    }
    log("SERVER", "Connection to client finished, not receiving anymore");
}

int RailroadsServer::putMessage(std::string msgToSend){
    if(!connected){
        return 0;
    }
    int strLen = strlen(msgToSend.c_str());
    this->m.relock();
    int bytesSent = client->write(msgToSend.c_str(), strLen);
    m.unlock();
    if (bytesSent == 0)
    {
        log("SERVER", "Message sent: \nZero bytes, client finished connection");
        return bytesSent;
    }
    else if(bytesSent<0)
    {
        error("SERVER", "ERROR: send returned an error " + std::to_string(bytesSent));
        return bytesSent;
    }else if (bytesSent < strLen){
        log("SERVER", std::string("Message sent with less characters: \n") + msgToSend);
    }else if (bytesSent > strLen){
        log("SERVER", std::string("Message sent with extra characters: \n")
            + msgToSend + std::string("+") + std::to_string(bytesSent-strLen));
    }else{
        log("SERVER", std::string("Message sent: \n") + msgToSend);
    }
    return bytesSent;
}

void RailroadsServer::treatMessage(std::string message){
    log("SERVER", std::string("Treating '") + message + std::string("'"));
    //int posPOS = message.find_first_of("POS");
    std::vector<std::string> words;
    boost::split(words, message, boost::is_any_of("_ "));
    if(words.size() > 0){
        std::string first = words[0];
        words.erase(words.begin());
        std::vector<std::string> params = words;
        if(first=="POS"){
            POS(params);
        }else if(first=="REG"){
            REG(params);
        }else{
            log("SERVER", std::string("Message is not a POS or REG: ") + message);
        }
    }else{
        log("SERVER", std::string("No words in message: ") + message);
    }
}

void RailroadsServer::REG(std::vector<std::string> words){
    log("SERVER-REG", vectorToStr(words));
    string id = words[0];
    words.erase(words.begin());
    registerNewTrain(id, words);
}

void RailroadsServer::POS(std::vector<std::string> words){
    log("SERVER-POS", vectorToStr(words));
    string id = words[0];
    words.erase(words.begin());
    StringQueue* q = Events::getQueue(id);
    if(q != NULL){
        q->push(new string(words[1].c_str()));
    }
}

void RailroadsServer::sendDenyToID(string id){
    log("SERVER", string("DENYING TO ") + id);
    putMessage(string("DENY ")+id+string("\n"));
}

void RailroadsServer::sendAllowToID(string id, vector<int> lengths){
    log("SERVER", string("Allowing ") + id);
    putMessage(string("ALLOW ")+ id + string(" ") + vectorToStr(lengths) + string("\n"));
}

void RailroadsServer::sendGoToRailMessage(string id, string rail){
    string message = "GOTO ";
    message += id;
    message += " ";
    message += rail;
    message += "\n";
    log("SERVER", message);
    putMessage(message);
}

vector<bool> RailroadsServer::negativePaths(vector<string> path){
    vector<bool> negative;
    for(int i = 0; i < path.size(); i++){
        string name = path[i];
        bool found = false;
        if(name.length() >= 2){
            if(name[0] == '-'){
                found = true;
            }
        }
        negative.push_back(found);
    }
    return negative;
}

vector<string> RailroadsServer::pathWithoutNegativeSign(vector<string> path, vector<bool> negative){
    vector<string> noNegative;
    for(int i = 0; i < path.size(); i++){
        string name = path[i];
        if(negative[i]){
            name = name.substr(1);
        }
        noNegative.push_back(name);
    }
    return noNegative;
}

bool RailroadsServer::allRailsInGraph(vector<string> vec){
    for(string p : vec){
        if (graph->railInGraph(p) == false){
            return false;
        }
    }
    return true;
}

vector<int> RailroadsServer::lengthsOfPath(vector<string> path){
    vector<int> lengths;
    for(int i = 0; i < path.size(); i++){
        lengths.push_back(graph->getRail(path[i])->length);
    }
    return lengths;
}

bool RailroadsServer::registerNewTrain(string id, vector<string> path){
    StringQueue* q = Events::registerQueue(id);
    if(q == NULL){
        sendDenyToID(id);
        return false;
    }
    auto negative = negativePaths(path);
    auto noNegativeSign = pathWithoutNegativeSign(path, negative);
    if(!allRailsInGraph(noNegativeSign)){
        error("SERVER", string("Some of the rails in the given path could not be found")
              + vectorToStr(path));
        sendDenyToID(id);
        return false;
    }
    auto lengths = lengthsOfPath(noNegativeSign);
    TrainThread* train = new TrainThread(id, q, noNegativeSign, negative,
                                         lengths, this->graph, this->canvas, this);
    sendAllowToID(id, lengths);
    train->start();
    trainThreads.insert(train);
    return true;
}

