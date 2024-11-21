#ifndef RW_H
#define RW_H

#include <map>
#include <string>
#include "exceptions.h" // Подключение нашего файла с исключениями

/**
 * @file rw.h
 * @brief Определение функций для работы с файлами.
 * @details Этот файл содержит определения функций для чтения базы данных и записи логов.
 * @date 23.11.2024
 * @version 1.0
 * @author Бусыгин Г. С.
 */

namespace rw
{
    /**
     * @brief Функция для чтения базы данных.
     * @param path Путь к файлу базы данных.
     * @return Возвращает map, содержащий данные из базы данных.
     * @throw RWException Если произошла ошибка при чтении файла.
     */
    std::map<std::string, std::string> readDataBase(const std::string &path);

    /**
     * @brief Функция для записи лога.
     * @param path Путь к файлу логов.
     * @param e Исключение, которое нужно записать в лог.
     * @throw RWException Если произошла ошибка при записи файла.
     */
    void writeLog(const std::string &path, const Exception &e);
}

#endif // RW_H
