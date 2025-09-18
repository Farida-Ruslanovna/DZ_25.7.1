// DZ_25.7.1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "network.h"
#include "server.h"
#include <string>

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "");
    if (argc < 2) {
        std::cout << "Usage:\n  " << argv[0] << " server <port>\n  " << argv[0] << " client\n";
        return 1;
    }

    std::string mode = argv[1];

    if (mode == "server") {
        int port = 12345; // Значение по умолчанию
        if (argc >= 3) port = std::stoi(argv[2]);
        // Реализация класса Server не предоставлена, можно добавить
        std::cout << "Запуск сервера на порту " << port << " — не реализовано в этом примере.\n";
    }
    else if (mode == "client") {
        std::cout << "Запуск клиента — не реализован в этом примере.\n";
        // Можно добавить код клиента или оставить как заглушку
    }
    else {
        std::cerr << "Неизвестный режим.\n";
        return 1;
    }

    return 0;
}