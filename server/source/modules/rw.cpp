#include "rw.h"
#include "exceptions.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace rw
{
    // Функция для чтения базы данных пользователей из файла
    std::map<std::string, std::string> readDataBase(const std::string &path)
    {
        std::map<std::string, std::string> database; // Словарь для хранения пар "пользователь:пароль"
        std::ifstream file(path); // Открыть файл для чтения
        
        if (!file.is_open()) // Проверить, удалось ли открыть файл
        {
            throw RWException("Failed to open the database file: " + path, __func__, true); // Исключение при ошибке открытия файла
        }

        std::string line;
        while (std::getline(file, line)) // Читаем каждую строку из файла
        {
            std::istringstream iss(line); // Создаем вводный поток для разбора строки
            std::string user, pass;
            if (std::getline(iss, user, ':') && std::getline(iss, pass)) // Разделяем пользователя и пароль
            {
                database[user] = pass; // Сохраняем в словарь
            }
            else
            {
                // Логируем неверный формат строки и выбрасываем исключение DataDecodeException
                throw DataDecodeException("Invalid line format: " + line, __func__, true); // Исключение при неверном формате
            }
        }

        file.close(); // Закрыть файл после чтения
        return database; // Вернуть словарь с данными
    }

    // Функция для записи информации об исключениях в лог-файл
    void writeLog(const std::string &path, const Exception &e)
    {
        std::ofstream logFile(path, std::ios::app); // Открыть файл лога в режиме добавления
        
        if (!logFile.is_open()) // Проверить, удалось ли открыть файл
        {
            throw RWException("Failed to open the log file: " + path, __func__, true); // Исключение при ошибке открытия файла
        }

        // Записываем сообщение об ошибке и имя функции, в которой возникло исключение
        logFile << e.what(); 
        logFile.close(); // Закрыть файл после записи
    }
}

