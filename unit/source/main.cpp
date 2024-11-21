/**
 * @file main.cpp
 * @brief Тесты для проверки функциональности различных компонентов.
 * @details Этот файл содержит тесты для проверки парсинга аргументов командной строки, генерации соли, вычисления хеша, и работы сетевого взаимодействия.
 * @date 23.11.2024
 * @version 1.0
 * @author Бусыгин Г. С.
 */

#include <UnitTest++/UnitTest++.h>
#include "../../server/source/modules/crutils.h"
#include "../../server/source/modules/network.h"
#include "../../server/source/modules/interface.h"
#include "../../server/source/modules/exceptions.h"
#include "../../server/source/modules/rw.h"
#include <iostream>
#include <fstream>

/**
 * @brief Тест для проверки совпадения хешей для одинаковых данных.
 */
TEST(CalculateHash_SameData)
{
    std::string data1 = "UnitTest";
    std::string data2 = "UnitTest";
    std::string hash1 = crutils::calculateHash(data1);
    std::string hash2 = crutils::calculateHash(data2);
    CHECK_EQUAL(hash1, hash2);
}

/**
 * @brief Тест для проверки различия хешей для разных данных.
 */
TEST(CalculateHash_DifferentData)
{
    std::string data1 = "UnitTest";
    std::string data2 = "Microsoft";
    std::string hash1 = crutils::calculateHash(data1);
    std::string hash2 = crutils::calculateHash(data2);
    CHECK(hash1 != hash2);
}

/**
 * @brief Тест для проверки инициализации атрибутов класса Exception.
 */
TEST(Exception_Attributes)
{
    std::string message = "An error occurred";
    std::string func = "TestFunction";
    bool is_critical = true;
    Exception e(message, func, is_critical);

    CHECK_EQUAL("An error occurred", e.getMessage());
    CHECK_EQUAL("TestFunction", e.getFunc());
    CHECK_EQUAL(true, e.isCritical());
    CHECK_EQUAL("Exception", e.getName());
    CHECK(!e.getTime().empty());
}

/**
 * @brief Тест для проверки успешного чтения базы данных.
 */
TEST(ReadDataBase_Success)
{
    std::string path = "test_database.txt";
    std::ofstream file(path);
    file << "user1:pass1\nuser2:pass2\n";
    file.close();

    std::map<std::string, std::string> database = rw::readDataBase(path);

    CHECK_EQUAL(2, database.size());
    CHECK_EQUAL("pass1", database["user1"]);
    CHECK_EQUAL("pass2", database["user2"]);

    std::remove(path.c_str());
}

/**
 * @brief Тест для проверки ошибки при открытии несуществующего файла базы данных.
 */
TEST(ReadDataBase_FileNotFound)
{
    std::string path = "non_existent_file.txt";

    CHECK_THROW(rw::readDataBase(path), RWException);
}

/**
 * @brief Тест для проверки ошибки при неверном формате строки в базе данных.
 */
TEST(ReadDataBase_InvalidLineFormat)
{
    std::string path = "test_database_invalid.txt";
    std::ofstream file(path);
    file << "user1:pass1\ninvalid_line_format\n";
    file.close();

    CHECK_THROW(rw::readDataBase(path), DataDecodeException);

    std::remove(path.c_str());
}

/**
 * @brief Тест для проверки успешной записи лога.
 */
TEST(WriteLog_Success)
{
    std::string path = "test_log.txt";
    Exception e("Test error message", "TestFunction", false);

    rw::writeLog(path, e);

    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string log_content = buffer.str();

    CHECK(log_content.find("Test error message") != std::string::npos);
    CHECK(log_content.find("TestFunction") != std::string::npos);

    std::remove(path.c_str());
}

/**
 * @brief Тест для проверки ошибки при открытии несуществующего файла лога.
 */
TEST(WriteLog_FileNotFound)
{
    std::string invalid_path = "/invalid_directory/test_log.txt";
    Exception e("Test error message", "TestFunction", false);

    CHECK_THROW(rw::writeLog(invalid_path, e), RWException);
}

/**
 * @brief Класс логгера для вывода детализированной информации о тестах.
 */
class MyTestReporter : public UnitTest::TestReporter
{
public:
    /**
     * @brief Сообщает о начале выполнения теста.
     * @param test Детали теста.
     */
    void ReportTestStart(UnitTest::TestDetails const &test) override
    {
        std::cout << "Test <" << test.testName << "> started:\n";
    }

    /**
     * @brief Сообщает об окончании выполнения теста.
     * @param test Детали теста.
     * @param secondsElapsed Время выполнения теста в секундах.
     */
    void ReportTestFinish(UnitTest::TestDetails const &test, float secondsElapsed) override
    {
        std::cout << "*passed("
                  << secondsElapsed << " seconds)\n"
                  << "================================"
                  << "================================\n";
    }

    /**
     * @brief Сообщает о сбое теста.
     * @param test Детали теста.
     * @param failure Описание сбоя.
     */
    void ReportFailure(UnitTest::TestDetails const &test, char const *failure) override
    {
        std::cout << "*failed: "
                  << " (" << failure << ")\n"
                  << "================================"
                  << "================================\n";
    }

    /**
     * @brief Сообщает об общей сводке выполнения тестов.
     * @param totalTestCount Общее количество тестов.
     * @param failedTestCount Количество неудачных тестов.
     * @param failureCount Общее количество сбоев.
     * @param secondsElapsed Общее время выполнения тестов в секундах.
     */
    void ReportSummary(int totalTestCount, int failedTestCount, int failureCount, float secondsElapsed) override
    {
        std::cout << "Summary: "
                  << totalTestCount << " tests, "
                  << failedTestCount << " failed, "
                  << failureCount << " failures, "
                  << secondsElapsed << " seconds\n";
    }
};
/**
 * @brief Тест для проверки инициализации атрибутов по умолчанию.
 */
TEST(Interface_DefaultAttributes)
{
    Interface interface;
    CHECK_EQUAL("/etc/vcalc.conf", interface.getPathToDB());
    CHECK_EQUAL("/var/log/vcalc.log", interface.getPathToLog());
    CHECK_EQUAL("127.0.0.1", interface.getAddress());
    CHECK_EQUAL(33333, interface.getPort());
}

/**
 * @brief Тест для проверки парсинга аргументов командной строки.
 */
TEST(Interface_ParseArgs)
{
    Interface interface;
    const char *argv[] = {
        "vserver",
        "--path_to_database", "custom_db.conf",
        "--path_to_log", "custom_log.log",
        "--address", "192.168.0.1",
        "--port", "8080"};
        
    int argc = sizeof(argv) / sizeof(argv[0]);
    interface.parseArgs(argc, (char **)argv);

    CHECK_EQUAL("custom_db.conf", interface.getPathToDB());
    CHECK_EQUAL("custom_log.log", interface.getPathToLog());
    CHECK_EQUAL("192.168.0.1", interface.getAddress());
    CHECK_EQUAL(8080, interface.getPort());
}

/**
 * @brief Тест для проверки выброса исключения при некорректных аргументах.
 */
TEST(Interface_InvalidArgs)
{
    Interface interface;
    const char *argv[] = {"program", "--invalid_arg"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    CHECK_THROW(interface.parseArgs(argc, (char **)argv), ArgsDecodeException);
}

/**
 * @brief Главная функция тестирования.
 * @details Инициализирует объект MyTestReporter и запускает тесты с использованием UnitTest++.
 * @param argc Количество аргументов командной строки.
 * @param argv Аргументы командной строки.
 * @return Код завершения программы. 0 - успешное завершение, 1 - ошибка.
 */
int main(int argc, char *argv[])
{
    MyTestReporter reporter;
    UnitTest::TestRunner runner(reporter);
    return runner.RunTestsIf(UnitTest::Test::GetTestList(), nullptr, UnitTest::True(), 0);
}
