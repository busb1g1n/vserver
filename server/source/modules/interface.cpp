#include "interface.h"
#include <iostream>
#include <stdexcept>
#include <cstring>

std::string &Interface::getPathToDB()
{
    return this->pathToDB;
}

std::string &Interface::getPathToLog()
{
    return this->pathToLog;
}

std::string &Interface::getAddress()
{
    return this->address;
}

int Interface::getPort() const
{
    return this->port;
}

// Метод для парсинга аргументов командной строки
Interface::Interface()
    : pathToDB("/etc/vcalc.conf"),
      pathToLog("/var/log/vcalc.log"),
      address("127.0.0.1"),
      port(33333) {}

void Interface::parseArgs(int argc, char *argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            this->showHelp();
            exit(0);
        }
        else if ((strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--database") == 0) && (i + 1 < argc))
        {
            this->pathToDB = argv[i + 1];
            ++i; // Пропустить значение параметра
        }
        else if ((strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--log") == 0) && (i + 1 < argc))
        {
            this->pathToLog = argv[i + 1];
            ++i; // Пропустить значение параметра
        }
        else if ((strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--port") == 0) && (i + 1 < argc))
        {
            this->port = std::stoi(argv[i + 1]);
            ++i; // Пропустить значение параметра
        }
        else if ((strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--address") == 0) && (i + 1 < argc))
        {
            this->address = argv[i + 1];
            ++i; // Пропустить значение параметра
        }
        else
        {
            throw ArgsDecodeException("Invalid argument", "Interface::parseArgs", true);
        }
    }

    std::cout << "Parsed arguments: --pathToDB " << this->pathToDB
              << " --pathToLog " << this->pathToLog
              << " --address " << this->address
              << " --port " << this->port << std::endl;
}
// Метод для вывода справки
void Interface::showHelp() const
{
    std::cout << "Usage: program [options]\nOptions:\n"
              << "  -h, --help                 Show this help message and exit\n"
              << "  -p, --database             Path to the database file\n"
              << "  -l, --log                  Path to the log file\n"
              << "  -p, --port                 Port number (default: 33333)\n"
              << "  -a, --address              Address (default: 127.0.0.1)\n";
}
