#ifndef CHAT_H
#define CHAT_H

#include <vector>
#include <string>
#include "message.h"
#include <fstream>
#include <sys/stat.h> // для chmod
#include <unistd.h>   // для access

// Класс Chat для управления коллекцией сообщений
class Chat {
private:
    std::vector<Message> messages; // Вектор сообщений

public:
    // Добавить сообщение
    void addMessage(const Message& msg) {
        messages.push_back(msg);
    }

    // Вывести все сообщения
    void showMessages() const {
        for (const auto& msg : messages) {
            std::cout << "От: " << msg.getSender()
                << " Кому: " << msg.getReceiver()
                << "\nСообщение: " << msg.getText() << "\n\n";
        }
    }

    // Сохранить все сообщения в файл
    void saveToFile(const std::string& filename) const {
        std::ofstream ofs(filename, std::ios::trunc);
        if (!ofs)
            return;

        for (const auto& msg : messages) {
            ofs << msg.getText() << '\n'
                << msg.getSender() << '\n'
                << msg.getReceiver() << '\n';
        }

        // Установка прав 600
        if (chmod(filename.c_str(), S_IRUSR | S_IWUSR) != 0) {
            // Можно логировать ошибку
        }
    }

    // Загрузить сообщения из файла
    void loadFromFile(const std::string& filename) {
        messages.clear();

        std::ifstream ifs(filename);
        if (!ifs)
            return;

        // Читаем по три строки на сообщение
        while (true) {
            std::string text, sender, receiver;
            if (!std::getline(ifs, text))
                break; // если не удалось прочитать, выходим
            if (text.empty()) // можно пропускать пустые
                break;

            if (!std::getline(ifs, sender)) break;
            if (!std::getline(ifs, receiver)) break;

            messages.emplace_back(text, sender, receiver);
        }
    }
};

#endif