#include "converter_json.h"
#include <fstream>
#include <iostream>
#include <iomanip> // для setw, если нужно красиво форматировать вывод

void ConverterJSON::loadConfig(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Ошибка: не удалось открыть файл конфигурации: " + filename);
    }
    try {
        file >> config;  // Читаем JSON в объект `config`
    } catch (const std::exception& e) {
        throw std::runtime_error("Ошибка при чтении файла конфигурации: " + filename + ". Детали: " + std::string(e.what()));
    }

    if (!config.contains("config") || !config["config"].is_object()) {
        throw std::runtime_error("config file is empty (нет блока 'config' в JSON).");
    }

    auto cfg = config["config"];

    if (!cfg.contains("name") || !cfg["name"].is_string() || cfg["name"].get<std::string>().empty()) {
        throw std::runtime_error("config.json has no valid 'name' field");
    }
    name_ = cfg["name"].get<std::string>();

    if (!cfg.contains("version") || !cfg["version"].is_string() || cfg["version"].get<std::string>().empty()) {
        throw std::runtime_error("config.json has no valid 'version' field");
    }
    version_ = cfg["version"].get<std::string>();

    if (cfg.contains("max_responses")) {
        if (!cfg["max_responses"].is_number_integer()) {
            throw std::runtime_error("config.json has 'max_responses' but it's not an integer");
        }
        int val = cfg["max_responses"].get<int>();
        if (val < 1) {
            std::cerr << "warn: max_responses < 1, use 5 instead." << std::endl;
        } else {
            maxResponses_ = val;
        }
    }
}

void ConverterJSON::processRequests(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Ошибка: не удалось открыть файл запросов: " + filename);
    }

    try {
        file >> requests;

        if (!requests.contains("requests") || !requests["requests"].is_array()) {
            throw std::runtime_error("Некорректный формат файла запросов. Ожидается массив 'requests'.");
        }

        for (const auto& request : requests["requests"]) {
            if (!request.is_string()) {
                throw std::runtime_error("Некорректный формат запроса. Каждый запрос должен быть строкой.");
            }
            std::cout << "Обработка запроса: " << request.get<std::string>() << std::endl;
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Ошибка при обработке запросов из файла: " + filename + ". Детали: " + std::string(e.what()));
    }
}

void ConverterJSON::putAnswers(const std::vector<std::vector<std::pair<int, float>>>& results) {
    nlohmann::json root;
    nlohmann::json answersNode;

    for (size_t i = 0; i < results.size(); ++i) {
        // requestName
        std::string requestName;
        if      (i < 9)  requestName = "request00" + std::to_string(i + 1);
        else if (i < 99) requestName = "request0"  + std::to_string(i + 1);
        else             requestName = "request"   + std::to_string(i + 1);

        if (results[i].empty()) {
            // Ни одного документа
            nlohmann::json requestNode;
            requestNode["result"] = "false";
            answersNode[requestName] = requestNode;
        } else {
            // Нашлись документы
            nlohmann::json requestNode;
            requestNode["result"] = "true";

            // Допустим, если > 1 документа — массив "relevance"
            if (results[i].size() > 1) {
                nlohmann::json relevanceArray = nlohmann::json::array();
                for (auto const& [docid, rank] : results[i]) {
                    nlohmann::json item;
                    item["docid"] = docid;
                    item["rank"]  = rank;
                    relevanceArray.push_back(item);
                }
                requestNode["relevance"] = relevanceArray;
            } else {
                // Если ровно 1 документ
                requestNode["docid"] = results[i][0].first;
                requestNode["rank"]  = results[i][0].second;
            }
            answersNode[requestName] = requestNode;
        }
    }

    root["answers"] = answersNode;

    std::ofstream outFile("answers.json", std::ofstream::trunc);
    if (!outFile.is_open()) {
        std::cerr << "Не удалось создать файл answers.json для записи." << std::endl;
        return;
    }
    outFile << std::setw(4) << root << std::endl;
    outFile.close();
    std::cout << "Результаты поиска записаны в answers.json" << std::endl;
}