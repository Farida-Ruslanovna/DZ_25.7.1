#include "network.h"
#include "db_manager.h"
#include <iostream>
#include <thread>
#include <vector>
#include <sstream> // для разбора команд

const int PORT = 12345;

// Обработка клиента
void handleClient(int clientSocket, DBManager& db) {
    char buffer[1024];
    while (true) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived <= 0) break; // соединение закрыто или ошибка
        buffer[bytesReceived] = '\0';

        std::string request(buffer);
        std::istringstream iss(request);
        std::string command;
        iss >> command;

        if (command == "REGISTER") {
            std::string login, pass, name;
            iss >> login >> pass >> name;
            bool success = db.registerUser(login, pass, name);
            std::string response = success ? "OK\n" : "FAIL\n";
            send(clientSocket, response.c_str(), response.size(), 0);
        }
        else if (command == "LOGIN") {
            std::string login, pass;
            int userId;
            iss >> login >> pass;
            userId = db.loginUser(login, pass);
            std::string response = (userId >= 0) ? "OK " + std::to_string(userId) + "\n" : "FAIL\n";
            send(clientSocket, response.c_str(), response.size(), 0);
        }
        else if (command == "GET_USERS") {
            auto users = db.getUsers();
            std::string response;
            for (const auto& u : users) {
                response += std::to_string(u.first) + ":" + u.second + "\n";
            }
            send(clientSocket, response.c_str(), response.size(), 0);
        }
        else if (command == "GET_MESSAGES") {
            int userId1, userId2;
            iss >> userId1 >> userId2;
            auto msgs = db.getMessages(userId1, userId2);
            std::string response;
            for (const auto& msg : msgs) {
                response += msg + "\n";
            }
            send(clientSocket, response.c_str(), response.size(), 0);
        }
        else if (command == "SEND_MESSAGE") {
            int senderId, receiverId;
            std::string text;
            iss >> senderId >> receiverId;
            std::getline(iss, text);
            // Удаляем начальные пробелы
            if (!text.empty() && text[0] == ' ') text.erase(0, 1);
            bool success = db.sendMessage(senderId, receiverId, text);
            std::string response = success ? "OK\n" : "FAIL\n";
            send(clientSocket, response.c_str(), response.size(), 0);
        }
        else {
            std::string response = "UNKNOWN_COMMAND\n";
            send(clientSocket, response.c_str(), response.size(), 0);
        }
    }
    // Закрываем соединение
#ifdef _WIN32
    closesocket(clientSocket);
#else
    close(clientSocket);
#endif
}

int main() {
    if (!Network::initialize()) {
        std::cerr << "Ошибка инициализации сети.\n";
        return 1;
    }

    int listenSock = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSock < 0) {
        std::cerr << "Не удалось создать сокет.\n";
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listenSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) != 0) {
        std::cerr << "Ошибка привязки сокета.\n";
#ifdef _WIN32
        closesocket(listenSock);
#else
        close(listenSock);
#endif
        return 1;
    }

    if (listen(listenSock, 5) != 0) {
        std::cerr << "Ошибка прослушивания.\n";
#ifdef _WIN32
        closesocket(listenSock);
#else
        close(listenSock);
#endif
        return 1;
    }

    // Создаем и подключаемся к базе данных
    DBManager db;
    if (!db.connect("your_dsn", "user", "pass")) {
        std::cerr << "Не удалось подключиться к базе данных.\n";
#ifdef _WIN32
        closesocket(listenSock);
#else
        close(listenSock);
#endif
        return 1;
    }

    std::cout << "Сервер запущен и слушает порт " << PORT << "\n";

    while (true) {
        int clientSock = accept(listenSock, nullptr, nullptr);
        if (clientSock < 0) {
            std::cerr << "Ошибка accept.\n";
            continue; // продолжить принимать соединения
        }
        // Создаем поток для обработки клиента
        std::thread t(handleClient, clientSock, std::ref(db));
        t.detach(); // отделяем поток
    }

    // При завершении работы
    db.disconnect();

#ifdef _WIN32
    closesocket(listenSock);
#else
    close(listenSock);
#endif
    Network::cleanup();
}