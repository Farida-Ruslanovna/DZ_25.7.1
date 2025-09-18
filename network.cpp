#include "network.h"
#include <iostream>
#include <thread>
#include <string>
#include <cstdlib> // для std::exit

const int PORT = 12345;

int main() {
    if (!Network::initialize()) {
        std::cerr << "Ошибка инициализации сети.\n";
        return 1;
    }

    std::string serverIP;
    std::cout << "Введите IP сервера: ";
    std::getline(std::cin, serverIP);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Не удалось создать сокет.\n";
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

#ifdef _WIN32
    inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr);
#else
    inet_aton(serverIP.c_str(), &serverAddr.sin_addr);
#endif

    // Попытка подключиться к серверу
    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Ошибка соединения.\n";
#ifdef _WIN32
        closesocket(sock);
#else
        close(sock);
#endif
        return 1;
    }

    std::cout << "Подключено к серверу.\n";

    // Запускаем поток для получения сообщений
    std::thread recvThread([sock]() {
        char buffer[1024];
        while (true) {
            int bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0);
            if (bytesReceived <= 0) {
                std::cout << "Соединение закрыто.\n";
                break;
            }
            buffer[bytesReceived] = '\0';
            std::cout << "Получено: " << buffer << "\n";
        }
        });

    // Основной поток — отправка сообщений
    std::string message;
    while (true) {
        std::getline(std::cin, message);
        if (message == "exit") break; // команда выхода

        if (send(sock, message.c_str(), message.size(), 0) < 0) {
            std::cerr << "Ошибка отправки.\n";
            break;
        }
    }

    // Закрываем сокет
#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif

    // Ожидаем завершения потока
    if (recvThread.joinable())
        recvThread.join();

    Network::cleanup();
    return 0;
}