#ifndef SERVER_H
#define SERVER_H

#include "network.h"
#include "db_manager.h"
#include <string>
#include <vector>

class Server {
private:
    int listenSocket;
    DBManager db;
public:
    Server();
    ~Server();
    bool start(int port);
    void run();
    void handleClient(int clientSocket);
};

#endif