#include "message.h"

// Сохранение сообщения в файл
bool Message::saveToFile(const std::string& filename) const {
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
bool Message::loadFromFile(const std::string& filename) {
    if (access(filename.c_str(), F_OK) != 0)
        return false; // файл не существует

    std::ifstream ifs(filename);
    if (!ifs)
        return false;

    // Читаем строки файла — предполагается правильный формат
    if (!std::getline(ifs, _text)) return false;
    if (!std::getline(ifs, _sender)) return false;
    if (!std::getline(ifs, _receiver)) return false;

    return true;

}
