#include <iostream>
#include <string>
#include <filesystem> // Для работы с путями (C++17)
#include "src/converter_json.h"

// Определяем путь к файлам относительно исполняемого файла
std::filesystem::path getFilePath(const std::string& fileName) {
    std::filesystem::path executablePath = std::filesystem::current_path();
    std::filesystem::path filePath = executablePath / fileName;

    if (!std::filesystem::exists(filePath)) {
        throw std::runtime_error("Файл не найден: " + filePath.string());
    }
    return filePath;
}

int main() {
    try {
        // Определяем пути к файлам
        std::filesystem::path configPath = getFilePath("config.json");
        std::filesystem::path requestsPath = getFilePath("requests.json");

        // Создаём объект конвертера
        ConverterJSON converter;

        // Загружаем конфигурацию из config.json
        converter.loadConfig(configPath.string());

        // Обрабатываем запросы из requests.json
        converter.processRequests(requestsPath.string());

        std::cout << "Все запросы успешно обработаны!" << std::endl;
    } catch (const std::exception& e) {
        // Обработка ошибок
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
    return 0;
}