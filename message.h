#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <fstream>
#include <sys/stat.h> // для chmod
#include <unistd.h>   // для access

// Класс Message для хранения сообщений
class Message {
private:
    std::string _text;     // Текст сообщения
    std::string _sender;   // Отправитель
    std::string _receiver; // Получатель

public:
    Message() = default;

    // Конструктор с параметрами
    Message(const std::string& text, const std::string& sender, const std::string& receiver)
        : _text(text), _sender(sender), _receiver(receiver) {
    }

    // Геттеры
    std::string getText() const { return _text; }
    std::string getSender() const { return _sender; }
    std::string getReceiver() const { return _receiver; }

    // Сеттеры
    void setText(const std::string& text) { _text = text; }
    void setSender(const std::string& sender) { _sender = sender; }
    void setReceiver(const std::string& receiver) { _receiver = receiver; }

    // Сохранение сообщения в файл
    bool saveToFile(const std::string& filename) const {
        std::ofstream ofs(filename, std::ios::trunc);
        if (!ofs)
            return false;

        ofs << _text << '\n' << _sender << '\n' << _receiver << '\n';

        // Установка прав доступа 600
        if (chmod(filename.c_str(), S_IRUSR | S_IWUSR) != 0) {
          
            return false;
        }
        return true;
    }

    // Загрузка сообщения из файла
    bool loadFromFile(const std::string& filename) {
        if (access(filename.c_str(), F_OK) != 0)
            return false; // файл не существует

        std::ifstream ifs(filename);
        if (!ifs)
            return false;

    
        if (!std::getline(ifs, _text)) return false;
        if (!std::getline(ifs, _sender)) return false;
        if (!std::getline(ifs, _receiver)) return false;

        return true;
    }
};


#endif 
