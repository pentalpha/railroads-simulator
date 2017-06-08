#include "RailroadsServer.h"
#include <thread>
using namespace std;

RailroadsServer::RailroadsServer(std::string ip, int port, RailsGraph* graph0, RailroadsCanvas* canvas0) :
    ipStr(ip.c_str()), portNum(port), m()
{
    tcpServer = new QTcpServer();
    addr = QHostAddress(ipStr);

    connected = false;
    exitFlag = false;
    waitingFlag = false;

    this->canvas = canvas0;
    this->graph = graph0;

    //connect(tcpServer, &QTcpServer::newConnection, this, &RailroadsServer::whenConnected);
    connect(tcpServer, &QTcpServer::newConnection, this, &RailroadsServer::newConnection);
    connect(this, &RailroadsServer::messageToSend, this, &RailroadsServer::sendAMessage);
}

bool RailroadsServer::startListening(){
  //Habilitando o servidor a receber conexoes do cliente

  bool listened = tcpServer->listen(addr, portNum);
  if (!listened)
  {
      log("SERVER", std::string("Failed to listen() on ") + ipStr.toStdString() +
          std::string("::") + std::to_string(portNum));
      return false;
  }else{
      return true;
  }
}

void RailroadsServer::newConnection(){
    this->client = tcpServer->nextPendingConnection();
    connected = true;
    exitFlag = false;
    waitingFlag = false;
    connect(client, &QTcpSocket::disconnected, this, &RailroadsServer::clientDisconnected);
    connect(client, &QTcpSocket::readyRead, this, &RailroadsServer::readAMessage);
    client->startTransaction();
    log("SERVER", string("Waiting for messages from ") + client->peerAddress().toString().toStdString());
    //QtConcurrent::run(this, &RailroadsServer::receiveFromClient);
    //std::thread treater(&RailroadsServer::msgTreatmentThread, this);
    //treater.detach();
    QtConcurrent::run(this, &RailroadsServer::msgTreatmentThread);
}

void RailroadsServer::readAMessage(){
    if(!client->canReadLine()){
        return;
    }
    int maxLen = 52;
    char* bytes = new char[maxLen];
    memset(bytes, ' ', maxLen);
    QMutexLocker locker(&m);
    int nRead = client->readLine(bytes, maxLen);
    locker.unlock();
    if(nRead > 0){
        string* line = NULL;
        line = new string(bytes);
        *line = line->substr(0, line->length()-1);
        log("SERVER", string("Received ") + *line);
        messages.push(line);
    }else if(nRead == -1){
        log("SERVER", "Received -1");
        exitFlag = true;
    }
    /*if(client->canReadLine()){
        readAMessage();
    }*/
}

void RailroadsServer::putMessage(std::string msgToSend){
    toSend.push(new string(msgToSend));
    log("SERVER-MAILBOX", string("Stored message to send later: ") + msgToSend);
    emit messageToSend();
}

void RailroadsServer::sendAMessage(){
    if(!connected){
        return;
    }else if (client == NULL){
        return;
    }else if (!client->isWritable()){
        error("SERVER", "Client is not writable");
        return;
    }
    string msgToSend = toSend.pop();
    if(msgToSend == ""){
        return;
    }
    int strLen = msgToSend.length();
    int bytesSent = client->write(msgToSend.c_str(), strLen);
    if (bytesSent == 0)
    {
        log("SERVER", "Message sent: \nZero bytes, client finished connection");
    }
    else if(bytesSent<0)
    {
        error("SERVER", "ERROR: send returned an error " + std::to_string(bytesSent));
    }
    else if (bytesSent < strLen)
    {
        log("SERVER", std::string("Message sent with less characters: \n") + msgToSend);
    }
    else if (bytesSent > strLen)
    {
        log("SERVER", std::string("Message sent with extra characters: \n")
            + msgToSend + std::string("+") + std::to_string(bytesSent-strLen));
    }
    else
    {
        log("SERVER", std::string("Message sent: \n") + msgToSend);
    }
    /*if(toSend.getElements() > 0){
        sendAMessage();
    }*/
}

void RailroadsServer::msgTreatmentThread(){
    log("SERVER", "RailroadsServer is ON");

    while(!exitFlag || messages.getElements() > 0)
    {
        std::string msg = getMessage();
        if(msg.length() >= minMessage)
        {
            treatMessage(msg);
        }
        else if (msg.length() >= 2)
        {
            log("[SERVER]", std::string("Message is too short, ignore: \n") + msg);
        }
    }

    log("SERVER", "RailroadsServer Message Treater is OFF");
}

void RailroadsServer::start(){
  if(startListening() == false){
    return;
  }

  int nSeconds = 1;
  log("SERVER", "Waiting 1s for client to connect...");
  waitingFlag = true;
  tcpServer->waitForNewConnection(nSeconds*1000);
  log("SERVER", "...Finished Waiting for Client!");
}

bool RailroadsServer::isConnected(){
  return connected || !exitFlag;
}

std::string RailroadsServer::getMessage(){
  std::string msg = messages.pop();
  return msg;
}

void RailroadsServer::stop(){
  //std::cout << "Server auto stopping itself\n";
  exitFlag = true;
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

void RailroadsServer::clientDisconnected(){
    log("SERVER", "Client disconnected");
    exitFlag = true;
    connected = false;
}

void RailroadsServer::treatMessage(std::string message){
    //log("SERVER", std::string("Treating '") + message + std::string("'"));
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
    //log("SERVER-POS", vectorToStr(words));
    string id = words[0];
    words.erase(words.begin());
    StringQueue* q = Events::getQueue(id);
    if(words[0].length() <= 0){
        error("SERVER", string("Invalid pos for ") + id + string(": ") + string(words[0]));
    }else if(q != NULL){
        q->push(new string(words[0].c_str()));
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

string RailroadsServer::allRailsInGraph(vector<string> vec){
    for(string p : vec){
        if (graph->railInGraph(p) == false){
            return p;
        }
    }
    return "";
}

vector<int> RailroadsServer::lengthsOfPath(vector<string> path){
    vector<int> lengths;
    for(int i = 0; i < path.size(); i++){
        string key = path[i];
        Rail* rail = graph->getRail(key);
        int len = rail->length;
        lengths.push_back(len);
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
    string absentStr = allRailsInGraph(noNegativeSign);
    if(absentStr != ""){
        error("SERVER", string("Some of the rails in the given path could not be found ")
              + vectorToStr(noNegativeSign) + string(": ") + absentStr);
        sendDenyToID(id);
        return false;
    }else{
        log("SERVER", string("All rails informed by ") + id + string(" are valid."));
    }
    vector<int> lengths = lengthsOfPath(noNegativeSign);
    log("SERVER", string("Creating train thread for ") + id);
    TrainThread* train = new TrainThread(id, q, noNegativeSign, negative,
                                         lengths, this->graph, this->canvas, this);
    sendAllowToID(id, lengths);
    log("SERVER", string("Starting train thread for ") + id);
    train->start();
    trainThreads.insert(train);
    return true;
}

