#ifndef CONVERTER_JSON_H
#define CONVERTER_JSON_H

#include <string>
#include <vector>
#include <utility>    // std::pair
#include "../nlohmann/json.hpp"

/**
 * @class ConverterJSON
 * @brief Класс для работы с JSON-файлами (config.json, requests.json, answers.json).
 *
 * Осуществляет:
 * - Загрузку и проверку конфигурации из config.json (наличие полей config.name, config.version, config.max_responses).
 * - Загрузку и проверку списка запросов из requests.json (массив "requests").
 * - Запись результатов поиска в answers.json.
 */
class ConverterJSON {
public:
    /**
     * @brief Загружает конфигурацию из указанного JSON-файла.
     *        Проверяет наличие секции "config", а в ней полей "name", "version", "max_responses".
     * @param filename Путь к файлу конфигурации (обычно "config.json").
     * @throws std::runtime_error Если файл не найден или имеет некорректный формат.
     */
    void loadConfig(const std::string& filename);

    /**
     * @brief Обрабатывает (считывает) запросы из указанного JSON-файла.
     * @param filename Путь к файлу запросов (обычно "requests.json").
     * @throws std::runtime_error Если файл не найден или имеет некорректный формат.
     */
    void processRequests(const std::string& filename);

    /**
     * @brief Записывает результаты поиска в файл "answers.json".
     *
     * Формат итогового JSON-файла (пример):
     * \code{.json}
     * {
     *   "answers": {
     *     "request001": {
     *       "result": "true",
     *       "relevance": [
     *         { "docid": 2, "rank": 1.0 },
     *         { "docid": 0, "rank": 0.7 }
     *       ]
     *     },
     *     "request002": {
     *       "result": "false"
     *     }
     *   }
     * }
     * \endcode
     *
     * @param results Вектор, где каждый элемент соответствует запросу и содержит
     *                список пар (docid, rank).
     *                - results[0] -> результаты для первого запроса
     *                - results[1] -> результаты для второго запроса и т.д.
     */
    void putAnswers(const std::vector<std::vector<std::pair<int, float>>>& results);

    /**
     * @brief Возвращает максимальное количество ответов на один запрос, считанное из config.json.
     * @return max_responses (по умолчанию 5, если не указано в файле).
     */
    int getMaxResponses() const { return maxResponses_; }

private:
    nlohmann::json config;   ///< Объект, хранящий содержимое config.json
    nlohmann::json requests; ///< Объект, хранящий содержимое requests.json

    std::string name_;       ///< Поле name из config.json
    std::string version_;    ///< Поле version из config.json
    int maxResponses_ = 5;   ///< Поле max_responses из config.json (по умолчанию 5)
};

#endif // CONVERTER_JSON_H