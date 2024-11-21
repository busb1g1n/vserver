#include "network.h"
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdint>
#include <vector>

// Конструктор
Network::Network(
    const std::string &address,
    uint16_t port,
    const std::map<std::string, std::string> &database)
    : address(address),
      port(port),
      database(database),
      socket(-1),
      client_socket(-1)
{
    this->socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (this->socket == -1)
        throw NetworkException("Failed to create socket", __func__, true);

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(this->port);
    server_addr.sin_addr.s_addr = inet_addr(this->address.c_str());

    if (bind(this->socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        ::close(this->socket);
        throw NetworkException("Failed to bind socket", __func__, true);
    }

    if (listen(this->socket, 5) == -1)
    {
        ::close(this->socket);
        throw NetworkException("Failed to listen on socket", __func__, true);
    }
    std::cout << "Server listening on " << this->address << ":" << this->port << std::endl;
}

std::string &Network::getAddress()
{
    return this->address;
}

uint16_t &Network::getPort()
{
    return this->port;
}

std::map<std::string, std::string> &Network::getDatabase()
{
    return this->database;
}

// Метод для ожидания соединений
void Network::wait()
{
    this->client_socket = accept(this->socket, nullptr, nullptr);
    if (this->client_socket == -1)
    {
        throw NetworkException("Failed to accept connection", __func__, false);
    }
    std::cout << "Accepted connection from client" << std::endl;
}

// Метод для аутентификации
void Network::auth()
{
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    ssize_t bytes_read = recv(this->client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read <= 0)
    {
        throw NetworkException("Failed to read authentication data from client", __func__, false);
    }
    std::string auth_data(buffer);
    std::cout << "Received authentication data: " << auth_data << std::endl;

    if (auth_data.size() < 57) // user + 16 символов соли + 40 символов хеша
    {
        throw NetworkException("Invalid authentication data format", __func__, false);
    }

    std::string login = auth_data.substr(0, auth_data.size() - 56);    // извлекаем логин
    std::string salt = auth_data.substr(auth_data.size() - 56, 16);    // извлекаем соль
    std::string client_hash = auth_data.substr(auth_data.size() - 40); // извлекаем хеш

    std::cout << "Login: " << login << ", Salt: " << salt << ", Client Hash: " << client_hash << std::endl;

    auto it = this->database.find(login);
    if (it == this->database.end())
    {
        std::string error_message = "ERR";
        send(this->client_socket, error_message.c_str(), error_message.size(), 0);
        throw AuthException("Login not found in database: " + login, __func__, false);
    }

    std::string password = it->second;
    std::string data_to_hash = salt + password;
    std::string server_hash = crutils::calculateHash(data_to_hash);
    std::cout << "Calculated hash: " << server_hash << std::endl;

    if (client_hash != server_hash)
    {
        std::string error_message = "ERR";
        send(this->client_socket, error_message.c_str(), error_message.size(), 0);
        throw AuthException("Hash mismatch", __func__, false);
    }
    std::string success_message = "OK";
    send(this->client_socket, success_message.c_str(), success_message.size(), 0);
    std::cout << "Client authenticated successfully" << std::endl;
}

// Метод для вычисления среднего арифметического векторов
void Network::calc()
{
    uint32_t num_vectors;
    uint32_t num_values;
    int32_t value;

    // Чтение количества векторов от клиента
    ssize_t bytes_read = recv(this->client_socket, &num_vectors, sizeof(num_vectors), 0);
    if (bytes_read <= 0)
    {
        std::cerr << "Network error: Failed to read number of vectors from client\n";
        throw NetworkException("Failed to read number of vectors from client", __func__);
    }
    std::cout << "Received number of vectors: " << num_vectors << std::endl;

    // Обработка каждого вектора
    for (uint32_t i = 0; i < num_vectors; ++i)
    {
        // Чтение количества значений в векторе от клиента
        bytes_read = recv(this->client_socket, &num_values, sizeof(num_values), 0);
        if (bytes_read <= 0)
        {
            std::cerr << "Network error: Failed to read number of values from client, vector index: " << i << "\n";
            throw NetworkException("Failed to read number of values from client", __func__);
        }
        std::cout << "Received number of values for vector " << i + 1 << ": " << num_values << std::endl;

        int32_t sum = 0;
        for (uint32_t j = 0; j < num_values; ++j)
        {
            // Чтение значения вектора от клиента
            bytes_read = recv(this->client_socket, &value, sizeof(value), 0);
            if (bytes_read <= 0)
            {
                std::cerr << "Network error: Failed to read vector value from client, vector index: " << i << ", element index: " << j << "\n";
                throw NetworkException("Failed to read vector value from client", __func__);
            }
            std::cout << "Received value for vector " << i + 1 << ", element " << j + 1 << ": " << value << std::endl;

            sum += value;
        }

        double average = static_cast<double>(sum) / num_values;
        int32_t average_int = static_cast<int32_t>(average);

        // Отправка среднего арифметического обратно клиенту
        ssize_t bytes_sent = send(this->client_socket, &average_int, sizeof(average_int), 0);
        if (bytes_sent <= 0)
        {
            std::cerr << "Network error: Failed to send average to client, vector index: " << i << ", average: " << average_int << "\n";
            throw NetworkException("Failed to send average to client", __func__);
        }
        std::cout << "Sent average for vector " << i + 1 << ": " << average_int << std::endl;
    }

    std::cout << "Calculated and sent averages for all vectors" << std::endl;
}

// Метод для закрытия клиентского соединения
void Network::close()
{

    if (this->client_socket != -1)
    {
        if (::close(this->client_socket) == -1)
        {
            throw NetworkException("Failed to close client socket", __func__, true);
        }
        this->client_socket = -1;
    }
    std::cout << "Closed client connection" << std::endl;
}

// Метод для закрытия основного соединения
void Network::quit()
{
    if (this->socket != -1)
    {
        if (::close(this->socket) == -1)
        {
            throw NetworkException("Failed to close server socket", __func__, true);
        }
        this->socket = -1;
    }
    std::cout << "Closed server socket" << std::endl;
}
