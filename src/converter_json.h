#ifndef CONVERTER_JSON_H
#define CONVERTER_JSON_H

#include <string>
#include "../nlohmann/json.hpp"

// Класс для работы с JSON-конфигурацией и запросами
class ConverterJSON {
public:
    /**
     * @brief Загружает конфигурацию из указанного JSON-файла.
     * @param filename Путь к файлу конфигурации.
     * @throws std::runtime_error Если файл не найден или имеет некорректный формат.
     */
    void loadConfig(const std::string& filename);

    /**
     * @brief Обрабатывает запросы из указанного JSON-файла.
     * @param filename Путь к файлу запросов.
     * @throws std::runtime_error Если файл не найден или имеет некорректный формат.
     */
    void processRequests(const std::string& filename);

private:
    nlohmann::json config;   // Объект для хранения конфигурации
    nlohmann::json requests; // Объект для хранения запросов
};

#endif // CONVERTER_JSON_H