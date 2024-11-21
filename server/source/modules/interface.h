#ifndef INTERFACE_H
#define INTERFACE_H

#include <map>
#include <string>
#include "exceptions.h"

/**
 * @file interface.h
 * @brief Определение класса интерфейса.
 * @details Этот файл содержит определения классов для обработки параметров командной строки, чтения базы данных паролей и запуска сервера.
 * @date 23.11.2024
 * @version 1.0
 * @author Бусыгин Г. С.
 */

/**
 * @brief Класс для управления интерфейсом программы.
 */
class Interface
{
public:
    /**
     * @brief Конструктор принимает параметры командной строки.
     * @param argc Количество аргументов командной строки.
     * @param argv Аргументы командной строки.
     * @throw ArgsDecodeException Если аргументы командной строки некорректны.
     */
    Interface();

    /**
     * @brief Метод для вывода справки.
     */
    void parseArgs(int argc, char *argv[]);

    void showHelp() const;

    /**
     * @brief Метод для получения пути к файлу базы данных.
     * @return Путь к файлу базы данных.
     */
    std::string &getPathToDB();

    /**
     * @brief Метод для получения пути к файлу логов.
     * @return Путь к файлу логов.
     */
    std::string &getPathToLog();

    /**
     * @brief Метод для получения адреса.
     * @return Адрес.
     */
    std::string &getAddress();

    /**
     * @brief Метод для получения порта.
     * @return Порт.
     */
    int getPort() const;

private:
    std::string pathToDB;  ///< Путь к файлу базы данных.
    std::string pathToLog; ///< Путь к файлу логов.
    std::string address;   ///< Адрес.
    int port;              ///< Порт.
};

#endif // INTERFACE_H
