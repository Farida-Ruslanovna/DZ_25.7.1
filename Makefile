# Компилятор
CXX = g++
# Флаги компиляции
CXXFLAGS = -std=c++17 -Wall -Wextra

# Линковка с необходимыми библиотеками (например, MySQL, если используете)
LDFLAGS = -lmysqlclient

# Объектные файлы
OBJS = main.o server.o db_manager.o network.o user.o message.o chat.o

# Цель сборки
TARGET = chat_server

# По умолчанию — сборка
all: $(TARGET)

$(TARGET): $(OBJS)
    $(CXX) -o $@ $^ $(LDFLAGS)

# Правила для компиляции
main.o: main.cpp
    $(CXX) $(CXXFLAGS) -c main.cpp

server.o: server.cpp server.h
    $(CXX) $(CXXFLAGS) -c server.cpp

db_manager.o: db_manager.cpp db_manager.h
    $(CXX) $(CXXFLAGS) -c db_manager.cpp

network.o: network.cpp network.h
    $(CXX) $(CXXFLAGS) -c network.cpp

user.o: user.cpp user.h
    $(CXX) $(CXXFLAGS) -c user.cpp

message.o: message.cpp message.h
    $(CXX) $(CXXFLAGS) -c message.cpp

chat.o: chat.cpp chat.h
    $(CXX) $(CXXFLAGS) -c chat.cpp

# Очистка
clean:
    rm -f *.o $(TARGET)