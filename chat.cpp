#include "chat.h"

// добавление нового сообщения
void Chat::addMessage(const Message& msg) {
    messages.push_back(msg);
}

// вывод всех сообщений на экран
void Chat::showMessages() const {
    for (const auto& msg : messages) {
        std::cout << "От: " << msg.getSender()
            << " Кому: " << msg.getReceiver()
            << "\nСообщение: " << msg.getText() << "\n\n";
    }
}

// сохранение всех сообщений в файл
void Chat::saveToFile(const std::string& filename) const {
    std::ofstream ofs(filename, std::ios::trunc);
    if (!ofs)
        return;

    for (const auto& msg : messages) {
        ofs << msg.getText() << '\n'
            << msg.getSender() << '\n'
            << msg.getReceiver() << '\n';
    }

    // Установка прав доступа 600
    if (chmod(filename.c_str(), S_IRUSR | S_IWUSR) != 0) {
      
    }
}

// Загрузка сообщений из файла
void Chat::loadFromFile(const std::string& filename) {
    messages.clear();

    std::ifstream ifs(filename);
    if (!ifs)
        return;

    
    while (true) {
        std::string text, sender, receiver;
        if (!std::getline(ifs, text))
            break;
        if (text.empty())
            break;

        if (!std::getline(ifs, sender)) break;
        if (!std::getline(ifs, receiver)) break;

        messages.emplace_back(text, sender, receiver);
    }

}
