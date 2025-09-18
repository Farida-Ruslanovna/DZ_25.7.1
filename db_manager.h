#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include <string>
#include <vector>
#include <sql.h>
#include <sqlext.h>

// Класс для работы с базой данных через ODBC
class DBManager {
private:
    SQLHENV env;    // Окружение ODBC
    SQLHDBC dbc;    // соединение с базой данных

public:
    // Конструктор — инициализация окружения
    DBManager();
    // Деструктор — освобождение ресурсов
    ~DBManager();

    // Подключение к базе данных через DSN, логин и пароль
    bool connect(const std::string& dsn, const std::string& user, const std::string& password);
    // Отключение от базы данных
    void disconnect();

    // Регистрация нового пользователя
    bool registerUser(const std::string& login, const std::string& pass, const std::string& name);
    // Вход пользователя — возвращает ID или -1, если не найден
    int loginUser(const std::string& login, const std::string& pass);
    // Получение списка всех пользователей (ID и имя)
    std::vector<std::pair<int, std::string>> getUsers();

    // Получение истории сообщений между двумя пользователями
    std::vector<std::string> getMessages(int userId1, int userId2);
    // Отправка сообщения от одного пользователя другому
    bool sendMessage(int senderId, int receiverId, const std::string& text);
};

#endif

