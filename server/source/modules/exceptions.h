#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

/**
 * @file exceptions.h
 * @brief Определение классов исключений.
 * @details Этот файл содержит определения классов исключений для обработки различных ошибок в программе.
 * @date 23.11.2024
 * @version 1.0
 * @author Бусыгин Г. С.
 */

/**
 * @brief Базовый класс для исключений.
 */
class Exception : public std::exception {
public:
    /**
     * @brief Конструктор класса Exception.
     * @param message Сообщение об ошибке.
     * @param func Имя функции, в которой возникло исключение (по умолчанию "Undefended").
     * @param is_critical Флаг критичности исключения (по умолчанию false).
     */
    Exception(const std::string &message, const std::string &func = "Undefended", bool is_critical = false);

    std::string getName() const;
    std::string getFunc() const;
    std::string getMessage() const;
    std::string getTime() const;
    bool isCritical() const;

    /**
     * @brief Метод для получения сообщения об ошибке.
     * @return Сообщение об ошибке.
     */
    const char *what() const noexcept override;

protected:
    std::string name; ///< Имя исключения.
    std::string func; ///< Имя функции, в которой возникло исключение.
    std::string message; ///< Сообщение об ошибке.
    std::string time; ///< Время возникновения исключения.
    bool is_critical; ///< Флаг критичности исключения.
    mutable std::string what_message; ///< Полное сообщение об ошибке.
};

/**
 * @brief Класс для исключений ввода-вывода.
 */
class RWException : public Exception {
public:
    RWException(const std::string &message, const std::string &func = "Undefended", bool is_critical = false);
};

/**
 * @brief Класс для исключений при декодировании аргументов.
 */
class ArgsDecodeException : public Exception {
public:
    ArgsDecodeException(const std::string &message, const std::string &func = "Undefended", bool is_critical = false);
};

/**
 * @brief Класс для исключений при декодировании данных.
 */
class DataDecodeException : public Exception {
public:
    DataDecodeException(const std::string &message, const std::string &func = "Undefended", bool is_critical = false);
};

/**
 * @brief Класс для исключений аутентификации.
 */
class AuthException : public Exception {
public:
    AuthException(const std::string &message, const std::string &func = "Undefended", bool is_critical = false);
};

/**
 * @brief Класс для сетевых исключений.
 */
class NetworkException : public Exception {
public:
    NetworkException(const std::string &message, const std::string &func = "Undefended", bool is_critical = false);
};

#endif // EXCEPTIONS_H
