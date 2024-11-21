#include "exceptions.h"
#include <ctime>

/**
 * @brief Конструктор класса Exception.
 * @param message Сообщение об ошибке.
 * @param func Имя функции, в которой возникло исключение (по умолчанию "Undefended").
 * @param is_critical Флаг критичности исключения (по умолчанию false).
 */
Exception::Exception(const std::string &message, const std::string &func, bool is_critical)
    : message(message), func(func), is_critical(is_critical), name("Exception") {
    std::time_t now = std::time(nullptr);
    char buf[80];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    time = "[" + std::string(buf) + "]";
}

/**
 * @brief Метод для получения сообщения об ошибке.
 * @return Сообщение об ошибке.
 */
const char *Exception::what() const noexcept {
    std::string critical = this->is_critical ? "[Critical: True]" : "[Critical: False]";
    this->what_message = critical + "\t" + time + "\t[" + name + " in " + func + "]\t[Message: " + message + "]\n";
    return this->what_message.c_str();
}

/**
 * @brief Получение имени исключения.
 * @return Имя исключения.
 */
std::string Exception::getName() const {
    return name;
}

/**
 * @brief Получение имени функции, в которой возникло исключение.
 * @return Имя функции.
 */
std::string Exception::getFunc() const {
    return func;
}

/**
 * @brief Получение сообщения об ошибке.
 * @return Сообщение об ошибке.
 */
std::string Exception::getMessage() const {
    return message;
}

/**
 * @brief Получение времени возникновения исключения.
 * @return Время возникновения исключения.
 */
std::string Exception::getTime() const {
    return time;
}

/**
 * @brief Проверка флага критичности исключения.
 * @return true, если исключение критическое, иначе false.
 */
bool Exception::isCritical() const {
    return is_critical;
}

// Конструкторы для дочерних классов
RWException::RWException(const std::string &message, const std::string &func, bool is_critical)
    : Exception(message, func, is_critical) {
    name = "RWException";
}

ArgsDecodeException::ArgsDecodeException(const std::string &message, const std::string &func, bool is_critical)
    : Exception(message, func, is_critical) {
    name = "ArgsDecodeException";
}

DataDecodeException::DataDecodeException(const std::string &message, const std::string &func, bool is_critical)
    : Exception(message, func, is_critical) {
    name = "DataDecodeException";
}

AuthException::AuthException(const std::string &message, const std::string &func, bool is_critical)
    : Exception(message, func, is_critical) {
    name = "AuthException";
}

NetworkException::NetworkException(const std::string &message, const std::string &func, bool is_critical)
    : Exception(message, func, is_critical) {
    name = "NetworkException";
}
