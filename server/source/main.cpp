/**
 * @file main.cpp
 * @brief Главный файл программы.
 * @details Этот файл содержит функцию main, которая инициализирует интерфейс и запускает сервер.
 * @date 23.11.2024
 * @version 1.0
 * @author Бусыгин Г. С.
 */

#include "modules/interface.h"
#include "modules/network.h"
#include "modules/rw.h"
#include <iostream>

/**
 * @brief Функция для запуска сервера.
 * @param interface Объект интерфейса для получения настроек.
 * @param network Объект сети для управления соединениями.
 */
void run(Interface &interface, Network &network)
{
    while (true)
    {
        try
        {
            network.wait();
            network.auth();
            network.calc();
            network.close();
        }
        catch (const Exception &error)
        {
            if (!error.isCritical())
            {
                std::cerr << error.what();
                rw::writeLog(interface.getPathToLog(), error);
                network.close();
            }
            else if (error.isCritical())
            {
                network.quit();
                throw error;
            }
        }
    }
}

/**
 * @brief Главная функция программы.
 * @details Инициализирует объект Interface и запускает сервер. Обрабатывает все исключения, возникающие во время выполнения программы.
 * @param argc Количество аргументов командной строки.
 * @param argv Аргументы командной строки.
 * @return Код завершения программы. 0 - успешное завершение, 1 - ошибка.
 */
int main(int argc, char *argv[])
{
    Interface interface;
    try
    {
        // Парсинг аргументов командной строки
        interface.parseArgs(argc, argv);

        // Создание объекта Network
        Network network(
            interface.getAddress(),
            interface.getPort(),
            rw::readDataBase(interface.getPathToDB()));

        // Запуск сервера
        run(interface, network);
    }
    catch (const Exception &error)
    {
        rw::writeLog(interface.getPathToLog(), error);
        std::cerr << error.what() << std::endl;
        return 1;
    }
    catch (const std::exception &error)
    {
        std::cerr << "Standard exception: " << error.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unknown error occurred." << std::endl;
        return 1;
    }

    return 0;
}
