#include "converter_json.h"
#include <fstream>
#include <iostream>

// Метод для загрузки конфигурации
void ConverterJSON::loadConfig(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Ошибка: не удалось открыть файл конфигурации: " + filename);
    }
    try {
        file >> config;
    } catch (const std::exception& e) {
        throw std::runtime_error("Ошибка при чтении файла конфигурации: " + filename + ". Детали: " + e.what());
    }
}

// Метод для обработки запросов
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
            // Добавьте сюда логику обработки запроса
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Ошибка при обработке запросов из файла: " + filename + ". Детали: " + e.what());
    }
}